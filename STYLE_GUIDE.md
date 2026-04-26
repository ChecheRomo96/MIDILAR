# MIDILAR Style Guide

## Naming Conventions

### Classes
- Use PascalCase
- Example: `LUT1D`, `VelocityShaper`

### Methods
- Use PascalCase
- Example: `SetBinValue()`, `GetValue()`

### Variables
- Use snake_case with leading underscore for private members
- Example: `_buffer`, `_capacity`

### Macros
- Use UPPER_CASE
- Example: `MIDILAR_ENABLE_LUT`

---

## Memory Management

- Use `malloc` / `free`
- Avoid `std` unless explicitly enabled