build:
  meson compile -C build/

solar: build
  ./build/solar_homework

setup:
  CXX_LD=mold meson setup --reconfigure build/
