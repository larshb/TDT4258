#!/bin/bash

make #2> /dev/null

if [ $? -eq 0 ]
then
  echo "Successfully compiled, flashing binary..."
  make upload #eACommander.sh -r --address 0x00000000 -f "ex1.bin" -r
else
  echo "Compile error!" >&2
fi