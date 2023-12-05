project(
  'ee553', 
  'cpp',
  version : '0.1.0',
  default_options : [
    'warning_level=3', 
    'cpp_std=c++17',
    'werror=true',
  ]
)

executable(
  'hw_01',
  'hw_01.cc',
  install : true,
)

executable(
  'hw_02',
  'hw_02.cc',
  install : true,
)

executable(
  'hw_03',
  'hw_03.cc',
  install : true
)

executable(
  'hw_04',
  'hw_04.cc',
  install : true,
)

executable(
  'solar_homework',
  'solar_homework.cc',
  install : true,
)

executable(
  'hw_06', 
  'hw_06.cc',
  install : true,
)

executable(
  'hw_07',
  'hw_07.cc',
  install : true
)

executable(
  'hw_08',
  'hw_08.cc',
  install : true
)

iwyupy_program = find_program('iwyu_tool.py', required : false)
iwyu_program = find_program('iwyu-tool', required : false)
iwyu_config = ['-p', './compile_commands.json']
if iwyupy_program.found()
  iwyupy_target = run_target('iwyu', command : [iwyupy_program, iwyu_config])
elif iwyu_program.found()
  iwyu_target = run_target('iwyu', command : [iwyu_program, iwyu_config])
endif

cppcheck_program = find_program('cppcheck', required : false)
if cppcheck_program.found()
  cppcheck_config = [
    '--project=./compile_commands.json',
    '--enable=all',
    '--inconclusive',
    '--suppress=missingIncludeSystem',
  ]
  run_target('cppcheck', command : [cppcheck_program, cppcheck_config])
endif

cpplint_program = find_program('cpplint', required : false)
if cpplint_program.found()
  cpplint_config = [
    '--verbose=5',
    '--recursive',
    join_paths(meson.current_source_dir(), '.')
  ]
  run_target('cpplint', command : [cpplint_program, cpplint_config])
endif

