#!/bin/bash

set -e

if [ -d bin ]; then
  cd bin
else
  echo "Please run this script from the source root folder"
  exit
fi

PWD=`pwd`

if [ -f $PWD/../libs/lv2_ttl_generator.exe ]; then
  GEN=$PWD/../libs/lv2_ttl_generator.exe
  EXT=dll
elif [ -d /Developer ]; then
  GEN=$PWD/../libs/lv2_ttl_generator
  EXT=dylib
else
  GEN=$PWD/../libs/lv2_ttl_generator
  EXT=so
fi

FOLDERS=`find . -type d -name \*.lv2`

for i in $FOLDERS; do
  cd $i
  FILE=`ls *.$EXT | sort | head -n 1`
  $GEN ./$FILE
  cd ../..
done
