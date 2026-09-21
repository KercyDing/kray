# kray

Kray built with C++.

Requires `clang++` on `PATH`.

## Development

Build:

```bash
cmake --preset default
cmake --build --preset default
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
only run
only fmt  # requires clang-format
```
