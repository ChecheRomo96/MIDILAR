
# MIDILAR Build Instructions

This guide explains how to build, test, and use MIDILAR, including optional features like examples, testing, and documentation.

**Project Website:** [MIDILAR Documentation](https://checheromo96.github.io/MIDILAR/)

---

## Quick Start: Build, Test, and Generate Documentation

1. Create a build directory:
    ```bash
    mkdir -p build && cd build
    ```

2. Configure the project with optional features enabled:
    ```bash
    cmake .. -DMIDILAR_TESTING=ON -DMIDILAR_EXAMPLES=ON -DMIDILAR_DOCS=ON
    ```

3. Build the project:
    ```bash
    cmake --build .
    ```

4. Run tests:
    ```bash
    ctest --output-on-failure
    ```

5. Generate API documentation (requires Doxygen):
    ```bash
    cmake --build . --target docs
    ```

6. Access outputs:
   - **Examples**: Available in the build directory.
   - **Documentation**: Found in `build/docs`.

---

## Installation

To install MIDILAR:

1. Configure the build for installation:
    ```bash
    cmake -S . -B build -DCMAKE_INSTALL_PREFIX=<install_path>
    ```

2. Build and install:
    ```bash
    cmake --build build --target install
    ```

3. Installed components:
   - **Libraries**: Located in `<install_path>/lib/`
   - **Headers**: Located in `<install_path>/include/`
   - **CMake Configuration**: Found in `<install_path>/lib/cmake/MIDILAR/`

4. Verify installation:
   Ensure the necessary files are in their respective directories, and your build environment can find the installed MIDILAR.

---

## Advanced Configuration

### CMake Options

| Option              | Description                      | Default |
|---------------------|----------------------------------|---------|
| `MIDILAR_TESTING`   | Enable unit tests               | OFF     |
| `MIDILAR_EXAMPLES`  | Build example applications      | OFF     |
| `MIDILAR_DOCS`      | Generate API documentation      | OFF     |

Enable features by adding options to the `cmake` command:
```bash
cmake .. -DMIDILAR_TESTING=ON -DMIDILAR_EXAMPLES=ON -DMIDILAR_DOCS=ON
```

---

## Using MIDILAR in Your Project

### After Installation
Add the following to your `CMakeLists.txt`:
```cmake
find_package(MIDILAR REQUIRED)

add_executable(MyApp main.cpp)
target_link_libraries(MyApp MIDILAR::MIDILAR)
```

Set `CMAKE_PREFIX_PATH` if MIDILAR is installed in a non-standard location:
```bash
cmake -DCMAKE_PREFIX_PATH=/path/to/install ..
```

### Without Installation
Reference the MIDILAR source directory directly:
```cmake
add_subdirectory(path/to/MIDILAR)
add_executable(MyApp main.cpp)
target_link_libraries(MyApp MIDILAR::MIDILAR)
```

---

## Support

- **Clean Build:** If you encounter issues, clear the build directory and reconfigure:
    ```bash
    rm -rf build && mkdir build && cd build
    cmake ..
    ```

- For detailed documentation, visit [MIDILAR Documentation](https://checheromo96.github.io/MIDILAR/).


---
# Running Examples


## Run MIDILAR_BuildTest
./bin/MIDILAR_BuildTest

## Run MIDILAR_MidiProcessor_ClockGenerator
./bin/MIDILAR_MidiProcessor_ClockGenerator

## Run MIDILAR_MidiProcessor_Echo
./bin/MIDILAR_MidiProcessor_Echo

## Run MIDILAR_MidiProcessor_MidiFilter
./bin/MIDILAR_MidiProcessor_MidiFilter

## Run MIDILAR_SystemTools_Clock_Usage
./bin/MIDILAR_SystemTools_Clock_Usage


# Running Tests

## Run SystemTools_Clock_Test
./bin/SystemTools_Clock_Test

## Run MIDILAR_MidiProcessor
./bin/MIDILAR_MidiProcessor


---