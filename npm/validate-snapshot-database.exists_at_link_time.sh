#!/bin/bash
set -o errexit -o nounset -o pipefail
which shellcheck > /dev/null && shellcheck "$0"

# This script is a workaound for this yarn bug:
# https://github.com/yarnpkg/yarn/issues/3421
# This only works when called via the symbolic link
# in node_modules/.bin

NODE_MODULES_DIR="$(dirname "$0")/.."
"$NODE_MODULES_DIR/snapshot-validator/bin/validate-snapshot-database" "$@"
