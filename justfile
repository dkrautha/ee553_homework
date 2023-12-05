build:
  meson compile -C build/

checks: build
  ninja -C build clang-tidy
  ninja -C build iwyu
  ninja -C build cppcheck
  ninja -C build cpplint

setup:
  CXX_LD=mold meson setup -Db_sanitize=address,undefined --reconfigure build/
