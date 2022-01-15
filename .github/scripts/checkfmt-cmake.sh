#!/bin/sh

# checkfmt-cmake - Checks the CMake formatting of commits using cmake-format
#
# Usage:
# checkfmt-cmake.sh FROM_REF TO_REF
#
#   FROM_REF: Reference to the base Git commit (this is the first git commit which will not be checked)
#   TO_REF:   Reference to the last Git commit to check
#
# Example:
# checkfmt-cmake.sh main HEAD    Checks all commits up unto head since the current main for correct
#                                formatting. Note that formatting cannot be fixed by adding additional
#                                commits; rather ill-formatted ones must be modified.

set -eu

FROM_REF=$1
TO_REF=$2

# Get commit hashes from ref names
from_sha=$(git rev-parse -q --verify "${FROM_REF}") || echo "Git revision \"${FROM_REF}\" was not found"
to_sha=$(git rev-parse -q --verify "${TO_REF}") || echo "Git revision \"${TO_REF}\" was not found"
head=$(git rev-parse -q --verify HEAD)

# git stash --include-untracked

for commit in $(git rev-list --reverse "${from_sha}..${to_sha}"); do
    # echo "${commit}"
    git checkout "${commit}" >/dev/null
    find . \( -name "*.cmake" -or -name "CMakeLists.txt" \) -print0 | xargs -0 cmake-format --check
done

# Recover previous state
# git stash pop
# git checkout "${head}"
