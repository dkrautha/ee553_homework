# EE553_HW

This is the repository I used while taking EE553 at Stevens. I had assumed we'd be using GitHub, so I created this repo privately to have, but we ended up using Canvas for submissions instead.

I'm releasing this as a showcase of my C++ style and the tools I use to be as effective as possible (build system, linting tools, etc.). Be warned, my style is distinct and tries to adhere to many modern C++ principles (that don't get taught in the class), so yoink and submit this code at your own peril.

Here's a quick rundown of the tools I use:

- Formatter: clang-format
- Build system: Meson
- Linting:
  - clang-tidy
  - cppcheck
  - cpplint
  - Include what you use
- Linker: mold
- GCC Sanitizers: address, undefined
- Debugger: rr and codelldb
