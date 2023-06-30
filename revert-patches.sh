#!/bin/bash

# The script reverts patches applied to the 3rd party libraries in the libs folder.

cd libs

for lib in */; do
    pushd "$lib/src" || continue
    git checkout -- .
    git clean -df
    popd
done
