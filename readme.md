# MIDILAR

MIDILAR is a modular, lightweight C++ framework for real-time MIDI processing and embedded digital signal processing (DSP). It is designed to support both desktop and embedded development environments, including Arduino-compatible systems.

---

## 🚀 Key Features

- **Real-Time MIDI Parsing & Routing**
- **Modular DSP Infrastructure**
- **Lookup Tables (LUTs)** for waveform generation and shaping
- **Callback-based MIDI Message Handling**
- **Built-in MIDI Processors** (e.g., filtering, velocity scaling, clock division)

---

## 📚 Documentation

Full documentation is available at:

👉 [MIDILAR API Documentation](https://checheromo96.github.io/MIDILAR/)

From the main **Topics** tab in the documentation, you can access:

- **[Example Programs](https://checheromo96.github.io/MIDILAR/topics.html#MIDILAR_Examples)**  
  A curated set of tutorials and minimal reference implementations showing how to use MIDILAR features in practice — ideal for learning and debugging.

- **[Build Instructions](https://checheromo96.github.io/MIDILAR/topics.html#BuildGuide)**  
  Guides for building MIDILAR using either CMake or Arduino/PlatformIO. Covers library configuration, optional modules, and usage examples.

---

## 🔧 Getting Started

You can build MIDILAR using standard CMake:

```bash
cmake -B build -S . -DMIDILAR_TESTING=ON -DMIDILAR_EXAMPLES=ON -DMIDILAR_DOCS=ON
cmake --build build
```

Or configure it for embedded development using the `System/UserSetup.h` file when working with Arduino.

---

## 📁 Example Usage

The `examples/` folder includes various standalone and Arduino-compatible sketches. See the documentation's **Example Programs** section for visual output and detailed explanations.

---

## 💡 Learn More

- [API Reference](https://checheromo96.github.io/MIDILAR/)
- [Example Programs](https://checheromo96.github.io/MIDILAR/topics.html#MIDILAR_Examples)
- [Build Instructions](https://checheromo96.github.io/MIDILAR/topics.html#BuildGuide)

---

## 🧪 Testing & Docs

Enable testing and documentation via CMake:

```bash
cmake -B build -S . -DMIDILAR_TESTING=ON -DMIDILAR_DOCS=ON
ctest --test-dir build
cmake --build build --target docs
```

Docs will be generated under `build/doxygen/html/index.html`.

---

## License

MIDILAR is licensed under the MIT License. See the LICENSE file for more details.

---

MIDILAR is open-source and actively maintained. Feedback, suggestions, and contributions are welcome!