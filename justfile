build:
  meson compile -C build/

solar: build
  ./build/solar_homework

iwyu: build
  cd build && ninja iwyu

setup:
  CXX_LD=mold meson setup --reconfigure build/
