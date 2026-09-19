# kray

Kray built with C++.

## Build

Configure:

```bash
cmake --preset default
```

Build:

```bash
cmake --build --preset default
```

Run:

```bash
./build/kray
```

Format:

```bash
cmake --build --preset default --target format
```

## Using Only

If [Only](https://github.com/onlyjs/only) is installed, the same workflow is available through the project `Onlyfile`:

```bash
only config
only build
only run
only fmt
only clean
```

`config` prepares the build directory; run it before `build`, `run`, or `fmt`.
