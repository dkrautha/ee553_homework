build:
  meson compile -C build/

checks: build
  ninja -C build clang-tidy
  ninja -C build cppcheck
  ninja -C build cpplint
  ninja -C build iwyu

setup:
  CXX_LD=mold meson setup -Db_sanitize=address,undefined --reconfigure build/
