#!/bin/bash
#
#recurse from current dir and output name of any .a files
#that contain the desired symbol.
echo "You asked to search for:    $1"
if [[ -d $2 ]]; then
    echo "You asked to search in dir: $2"
else
    echo "Usage: findsym.sh <symbol> dirname"
fi
for filename in $2/*.a; do
  if nm --defined-only -C -l $filename 2>&1 | grep $1; then
    echo $filename
  fi
done

for filename in $2/*.so; do
  if nm --defined-only -D -C -l  $filename 2>&1 | grep $1; then
    echo $filename
  fi
done

