# kray

Kray built with C++.

Requires [clang++](https://clang.llvm.org/) (C++23 modules) and [vcpkg](https://github.com/microsoft/vcpkg) with `VCPKG_ROOT` exported.

## Development

Build:

```bash
cmake --preset default
cmake --build --preset default
```

Release:

```bash
cmake --preset release
cmake --build --preset release
```

Run:

```bash
./build/kray
# On Windows:
# .\build\kray.exe
```

## Using Only

If [Only](https://github.com/onlyjs/only) is installed, the same workflow is available through the project `Onlyfile`:

```bash
only build
only build-release
only run
only run-release
only fmt  # requires clang-format
```
