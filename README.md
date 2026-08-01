# DMirror

A fast, lightweight, cross-platform backup application written in modern C++ with **wxWidgets**. DMirror provides an intuitive graphical interface for creating backups while remaining portable across Windows, Linux, and macOS.

## Features
- Cross-platform GUI using wxWidgets
- Modern C++20 codebase

## Required Libraries

### Linux / Unix

This application requires the following runtime libraries:

- wxWidgets 3.2:
  - `libwx_gtk3u_core-3.2.so.0`
  - `libwx_baseu-3.2.so.0`

These libraries must be available on the target system at runtime.

On Debian/Ubuntu-based systems, install the required wxWidgets runtime libraries with:

```bash
sudo apt install libwxgtk3.2-1
```

## Building from Source
Building DMirror from source is straightforward thanks to its streamlined build system and platform-specific Makefiles. After installing the required dependencies for your operating system, the project can be built with a single `make` command. The build process is designed to minimize setup while remaining flexible for developers who wish to customize their build configuration.

### Requirements

- C++20 compatible compiler
- CMake
- GNU Make

### Linux (Debian/Ubuntu)

#### Install Dependencies

```bash
sudo apt-get update

sudo apt-get install -y \
    build-essential \
    cmake \
    libwxgtk3.2-dev
```

#### Build

```bash
make
```

### Windows

#### Install Dependencies

Install **CMake** using Chocolatey:

```powershell
choco install cmake --installargs "ADD_CMAKE_TO_PATH=System" -y
```

Install **wxWidgets** with vcpkg:

```powershell
vcpkg install wxwidgets:x64-windows-static
```

Configure CMake with the vcpkg toolchain directly:

```powershell
cmake -B build `
    -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake `
    -DVCPKG_TARGET_TRIPLET=x64-windows-static
```

> Adjust the `C:\vcpkg` path if your vcpkg installation is located elsewhere.

#### Build

```powershell
make
```

## License

This project is licensed under the **Apache License 2.0**. See the `LICENSE` file for details.