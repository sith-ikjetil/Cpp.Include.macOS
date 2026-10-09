#!/bin/bash
cppcheck  --enable=all \
          --std=c++23 \
          --check-level=exhaustive \
          --suppress=missingIncludeSystem \
          --suppress=unusedFunction \
          --suppress=checkersReport \
          --suppress=unreadVariable \
          --suppress=functionStatic \
          .
