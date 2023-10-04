build:
  meson compile -C build/

setup:
  CXX_LD=mold meson setup --reconfigure build/
