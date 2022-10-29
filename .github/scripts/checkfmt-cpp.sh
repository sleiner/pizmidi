#!/bin/sh

# checkfmt-cpp - Checks the C/C++ formatting of commits using clang-format
#
# Usage:
# checkfmt-cpp.sh FROM_REF TO_REF
#
#   FROM_REF: Reference to the base Git commit (this is the first git commit which will not be checked)
#   TO_REF:   Reference to the last Git commit to check
#
# Example:
# checkfmt-cpp.sh main HEAD    Checks all commits up unto head since the current main for correct
#                              formatting. Note that formatting cannot be fixed by adding additional
#                              commits; rather ill-formatted ones must be modified.

set -eu

FROM_REF=$1
TO_REF=$2

# Get commit hashes from ref names
from_sha=$(git rev-parse -q --verify "${FROM_REF}") || echo "Git revision \"${FROM_REF}\" was not found"
to_sha=$(git rev-parse -q --verify "${TO_REF}") || echo "Git revision \"${TO_REF}\" was not found"

# Ensure that currently no .clang-format files were modified, since we might discard those changes...
if git diff --name-only HEAD | grep -q ".clang-format$"; then
    echo "You have unstaged changes to .clang-format files." >/dev/stderr
    echo "Please commit them before running this script." >/dev/stderr
    exit 1
fi

get_clang_format_files() {
    commit=$1
    git ls-tree -r "${commit}" --format "./%(path)" | grep "/.clang-format$" | xargs git checkout "${commit}" --
}

remove_clang_format_files() {
    commit=$1
    git ls-tree -r "${commit}" --format "./%(path)" | grep "/.clang-format$" | xargs rm
}

remove_clang_format_files HEAD

for commit in $(git rev-list --reverse "${from_sha}..${to_sha}"); do
    predecessor=$(git rev-parse -q --verify "${commit}~1") || echo "Predecessor to \"${commit}\" was not found"

    echo "Checking diff between ${predecessor} and ${commit}..."
    get_clang_format_files "${commit}"
    diff=$(git-clang-format --quiet --diff "${predecessor}" "${commit}" --extensions "h,hh,hpp,c,cc,cpp" -- .) || :
    if [ -n "${diff}" ]; then
        echo "=> ${commit} was not formatted correctly:"
        echo "${diff}"
        exit 1
    fi
    remove_clang_format_files "${commit}"
done

get_clang_format_files HEAD
