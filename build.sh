#!/bin/bash
# 重构代码语法编译验证 (不链接, 不依赖平台)
# 用法: ./build.sh [文件...]   默认全部 (core_*.c + crt_shims.c)
set -e
CC=${CC:-gcc}
CFLAGS="-std=c99 -Wall -Wextra -Wno-unused-parameter -Wno-implicit-function-declaration -fsyntax-only"
ROOT="$(cd "$(dirname "$0")" && pwd)"
FAIL=0
if [ $# -eq 0 ]; then
  set -- core_*.c crt_shims.c
fi
for f in "$@"; do
  if [ -f "$f" ]; then
    if $CC $CFLAGS -I"$ROOT/include" "$f" 2>&1; then
      echo "OK  : $f"
    else
      echo "FAIL: $f"; FAIL=1
    fi
  fi
done
exit $FAIL
