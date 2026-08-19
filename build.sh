#!/bin/bash
# 重构代码语法编译验证 (不链接, 不依赖平台)
# 用法: ./build.sh [组件目录]   默认全部
set -e
CC=${CC:-gcc}
CFLAGS="-std=c99 -Wall -Wextra -Wno-unused-parameter -Wno-implicit-function-declaration -fsyntax-only"
ROOT="$(cd "$(dirname "$0")" && pwd)"
FAIL=0
# third_party/giflib 需要 gnu99 (fdopen 等 POSIX 声明); 其余用 c99
for f in "$@"; do
  if [ -f "$f" ]; then
    case "$f" in
      third_party/*) EXTRA="-std=gnu99" ;;
      *) EXTRA="" ;;
    esac
    if $CC $CFLAGS $EXTRA -I"$ROOT/include" -I"$ROOT/third_party/giflib" "$f" 2>&1; then
      echo "OK  : $f"
    else
      echo "FAIL: $f"; FAIL=1
    fi
  fi
done
exit $FAIL
