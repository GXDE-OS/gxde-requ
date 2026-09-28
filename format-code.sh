#!/bin/bash

set -e
cd "$(dirname "$0")"

CLANG_FORMAT=${CLANG_FORMAT:-clang-format}
if ! command -v "$CLANG_FORMAT" > /dev/null; then
    echo "err: $CLANG_FORMAT NOT FOUND!!" >&2
    exit 1
fi

check=false
if [[ "$1" == "--check" ]]; then
    check=true
    shift
fi

if [[ $# -gt 0 ]]; then
    files=("$@")
else
    mapfile -t files < <(git ls-files --cached --others --exclude-standard \
        '*.cpp' '*.cc' '*.cxx' '*.h' '*.hpp')
fi

if [[ ${#files[@]} -eq 0 ]]; then
    exit 0
fi

if $check; then
    "$CLANG_FORMAT" --dry-run --Werror "${files[@]}"
else
    "$CLANG_FORMAT" -i "${files[@]}"
fi
