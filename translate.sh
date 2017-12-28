#!/bin/sh

sed "/^\/\// d;  /^#/ d;   s/\"/[[/; s/\"/]]/ "  "$CMAKE_CURRENT_SOURCE_DIR/src/TokenKinds.def" | cat "$CMAKE_CURRENT_SOURCE_DIR/transform.m4" - | m4 > ragel_fragment.rl
