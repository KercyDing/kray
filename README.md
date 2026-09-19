# kray

A C++ project built with CMake.

## Build

Configure:

```bash
cmake -B build -G Ninja
```

Build:

```bash
cmake --build build
```

Run:

```bash
./build/kray
```

## Using Only

If [Only](https://github.com/onlyjs/only) is installed, the same workflow is available through the project `Onlyfile`:

```bash
only config
only build
only run
only clean
```

`build` runs `config` first, and `run` builds the project first when needed.
