@echo off
clang ^
  --target=wasm32 ^
  -O3 ^
  -flto ^
  -nostdlib ^
  -fvisibility=hidden ^
  -Wl,--no-entry ^
  -Wl,--export-all ^
  -Wl,--lto-O3 ^
  -o test.wasm ^
  c/main.c

  REM -Wl,--initial-memory=1376256 ^
  REM -Wl,-z,stack-size=1024000 ^
