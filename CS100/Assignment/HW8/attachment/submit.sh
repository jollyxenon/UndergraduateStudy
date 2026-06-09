#!/usr/bin/env bash
set -euo pipefail

output="submission.zip"

if [ ! -d src ] || [ ! -d include ] || [ ! -f CMakeLists.txt ]; then
    echo "Please run this script from the project root containing src/, include/, and CMakeLists.txt."
    exit 1
fi

rm -f "$output"

if command -v zip >/dev/null 2>&1; then
    zip -qr "$output" src include CMakeLists.txt \
        -x '*.DS_Store' '__MACOSX/*' '._*'
elif command -v python3 >/dev/null 2>&1; then
    python3 -m zipfile -c "$output" src include CMakeLists.txt
else
    echo "Cannot find zip or python3. Please install one of them, Otherwise, zip the src/, include/, and CMakeLists.txt manually."
    exit 1
fi

echo "Created $output"
echo "Please submit $output to OJ."
