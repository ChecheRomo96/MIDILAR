# MIDILAR Build Instructions

This guide explains how to build, install, and use the MIDILAR project, as well as how to enable optional features like testing, examples, and documentation.

---

## Quick Start: Default Build

To build MIDILAR with default settings:
1. Create a build directory:
    ```bash
    mkdir -p build
    ```

2. Run CMake to configure the build:
    ```bash
    cmake -S . -B build
    ```

3. Build the project:
    ```bash
    cmake --build build
    ```

---

## Advanced Build Options

### Enabling Optional Features

Use the following CMake options to enable or disable specific features:

| Option                | Description                          | Default |
|-----------------------|--------------------------------------|---------|
| `MIDILAR_EXAMPLES`    | Build example applications           | OFF     |
| `MIDILAR_TESTING`     | Enable unit tests                   | OFF     |
| `MIDILAR_DOCS`        | Generate API documentation with Doxygen | OFF     |

Example command to enable all features:
```bash
cmake -S . -B build -DMIDILAR_EXAMPLES=ON -DMIDILAR_TESTING=ON -DMIDILAR_DOCS=ON
```

---

### Building with Specific Generators

#### **Xcode (macOS)**
1. Configure the build:
    ```bash
    cmake -S . -B build/Xcode -DMIDILAR_EXAMPLES=ON -DMIDILAR_TESTING=ON -DMIDILAR_DOCS=ON -G"Xcode"
    ```

2. Build the project:
    ```bash
    cmake --build build/Xcode
    ```

3. Open the project in Xcode:
    ```bash
    open build/Xcode/MIDILAR.xcodeproj
    ```

#### **Unix Makefiles (Linux/macOS)**
1. Configure the build:
    ```bash
    cmake -S . -B build/makefiles -DMIDILAR_EXAMPLES=ON -DMIDILAR_TESTING=ON -DMIDILAR_DOCS=ON -G"Unix Makefiles"
    ```

2. Build the project:
    ```bash
    make -C build/makefiles
    ```

3. Run tests (if enabled):
    ```bash
    ctest --test-dir build/makefiles --output-on-failure
    ```

---

## Installation Instructions

To install MIDILAR to a specific directory:

1. Configure the build with an installation prefix:
    ```bash
    cmake -S . -B build/install -G"Unix Makefiles" -DCMAKE_INSTALL_PREFIX=install
    ```

2. Build and install:
    ```bash
    cmake --build build/install --target install
    ```

3. Verify installation:
   - Libraries: `install/lib/`
   - Headers: `install/include/`
   - CMake Config: `install/lib/cmake/MIDILAR/`

---

## Using MIDILAR in Another Project

### Using MIDILAR After Installation

After installation, add the following to your `CMakeLists.txt` to use MIDILAR in another project:

```cmake
find_package(MIDILAR REQUIRED)

add_executable(MyApp main.cpp)
target_link_libraries(MyApp MIDILAR::MIDILAR)
```

Set the `CMAKE_PREFIX_PATH` to the install directory if it's not in a default location:

```bash
cmake -DCMAKE_PREFIX_PATH=/path/to/install -S . -B build
```

### Using MIDILAR Without Installing

If you prefer to use MIDILAR without installing it, reference its source directory directly in your project:

1. Add MIDILAR as a subdirectory in your `CMakeLists.txt`:
    ```cmake
    add_subdirectory(path/to/MIDILAR)
    ```

2. Link your application with the MIDILAR library:
    ```cmake
    add_executable(MyApp main.cpp)
    target_link_libraries(MyApp MIDILAR::MIDILAR)  
    ```

This method is ideal for development workflows where MIDILAR is used as a dependency without a separate installation step.

---

## Building Documentation

### Prerequisites
Ensure Doxygen is installed on your system.

### Generating Documentation
1. Enable `MIDILAR_DOCS` during configuration:
    ```bash
    cmake -S . -B build/docs -DMIDILAR_DOCS=ON
    ```

2. Build the documentation:
    ```bash
    cmake --build build/docs --target docs
    ```  

3. Locate the generated documentation in `build/docs`.

---

## Additional Notes

- **Clean Build**: If you encounter issues, delete the build directory and reconfigure:
```bash
rm -rf build && mkdir build
```   
---
## Header Files

- **If Using MIDILAR with `add_subdirectory`**:
  - The required header paths are automatically propagated by CMake through `target_include_directories` in MIDILAR's `CMakeLists.txt`. No manual configuration is needed.

- **If Using MIDILAR After Installation**:
  - Include paths in your project should reference the installed headers, typically located in `install/include/MIDILAR-<version>/`.

- **If Using MIDILAR Without Installation**:
  - Include paths must point to the relevant directories in the source tree, such as `path/to/MIDILAR/src/`.
  - Add the source directory to your target's include paths in your `CMakeLists.txt`:
    ```cmake
    target_include_directories(MyApp PRIVATE path/to/MIDILAR/src)
    ```

- **Support**: If you encounter problems, verify the installation paths and ensure required dependencies are installed.

---