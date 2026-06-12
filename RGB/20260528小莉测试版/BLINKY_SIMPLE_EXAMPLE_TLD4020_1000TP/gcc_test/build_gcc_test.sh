#!/usr/bin/env bash
set -euo pipefail

export PATH="/usr/bin:/ucrt64/bin:${PATH:-}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJ_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
INCLUDE_DIR="$SCRIPT_DIR/include"
OUT_DIR="E:/AUDI_COLOR/gcc_test_out_xiaoli"

mkdir -p "$OUT_DIR"

CFLAGS=(
  -std=c99
  -Wall
  -Wextra
  -I"$INCLUDE_DIR"
  -I"$PROJ_DIR/Algorithm"
  -I"$PROJ_DIR/Algorithm/AlgoRatioK_localfit"
)

echo "== GCC syntax check =="
gcc "${CFLAGS[@]}" -fsyntax-only "$PROJ_DIR/Algorithm/Algorithm.c"
gcc "${CFLAGS[@]}" -fsyntax-only "$PROJ_DIR/Algorithm/AlgoRatioK_localfit/AlgoRatioK_localfit.c"

echo "== GCC object compile =="
gcc "${CFLAGS[@]}" -c "$PROJ_DIR/Algorithm/FloatLib.c" -o "$OUT_DIR/FloatLib.o"
gcc "${CFLAGS[@]}" -c "$PROJ_DIR/Algorithm/Algorithm.c" -o "$OUT_DIR/Algorithm.o"
gcc "${CFLAGS[@]}" -c "$PROJ_DIR/Algorithm/AlgoRatioK_localfit/AlgoRatioK_localfit.c" -o "$OUT_DIR/AlgoRatioK_localfit.o"
gcc "${CFLAGS[@]}" -c "$SCRIPT_DIR/gcc_stub.c" -o "$OUT_DIR/gcc_stub.o"
gcc "${CFLAGS[@]}" -c "$SCRIPT_DIR/main.c" -o "$OUT_DIR/main.o"

echo "== GCC link test =="
gcc "$OUT_DIR/main.o" "$OUT_DIR/Algorithm.o" "$OUT_DIR/AlgoRatioK_localfit.o" "$OUT_DIR/FloatLib.o" "$OUT_DIR/gcc_stub.o" -o "$OUT_DIR/gcc_test.exe"

echo
echo "GCC test build succeeded:"
echo "  $OUT_DIR/gcc_test.exe"

echo
echo "== Run gcc_test.exe =="
"$OUT_DIR/gcc_test.exe"
