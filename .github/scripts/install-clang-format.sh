#!/bin/bash

# install-clang-format - Installs clang-format and git-clang-format on Ubuntu
#
# Usage:
# install-clang-format.sh MAJOR_VERSION
#
#   MAJOR_VERSION: The LLVM major version from which you want to install the tools
#
# Example:
# install-clang-format.sh 14   Installs clang-format in the latest 14.x.y version.

set -euo pipefail

MAJOR_VERSION=$1

export DEBIAN_FRONTEND=noninteractive

# Set up LLVM repo
apt-get update
apt-get install -yq gnupg wget
wget -qO - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
cat >/etc/apt/sources.list.d/llvm.list <<EOF
deb http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-${MAJOR_VERSION} main
deb-src http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-${MAJOR_VERSION} main
EOF

# Actually install clang-format
apt-get update
apt-get install -yq "clang-format-${MAJOR_VERSION}" git

rm /usr/bin/clang-format || :
ln -s "$(which "clang-format-${MAJOR_VERSION}")" /usr/bin/clang-format

rm /usr/bin/git-clang-format || :
ln -s "/usr/lib/llvm-${MAJOR_VERSION}/bin/git-clang-format" /usr/bin/git-clang-format
