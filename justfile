build:
  meson compile -C build/

iwyu: build
  cd build && ninja iwyu

setup:
  CXX_LD=mold meson setup -Db_sanitize=address,undefined --reconfigure build/
