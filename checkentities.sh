#!/bin/bash

cd $(dirname $(readlink -f "$0"))

DIR="${1:-./assets/entities}"
FAIL=0

for i in "$DIR/"*; do
    ./build/bin/checkentcfg "$i" > /dev/null || FAIL=1
done

exit $FAIL
