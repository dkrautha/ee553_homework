project('ee553', 'cpp',
  version : '0.1',
  default_options : [
             'warning_level=3', 
             'cpp_std=c++17',
             'werror=true'])

executable('hw_01',
           'main_hw01.cc',
           install : true)

executable('hw_02',
           'main_hw02.cc',
           install : true)

executable('hw_03',
           'main_hw03.cc',
           install : true)

executable('hw_04',
           'hw_04.cc',
           install : true)

executable('solar_homework',
           'solar_homework.cc',
           install : true)

executable('hw6', 
           'hw_06.cc',
           install : true)

executable('hw7',
           'hw7.cc',
           install : true)

run_target('iwyu',
           command : ['iwyu.sh', 'check'])

run_target('iwyu-fix',
           command : ['iwyu.sh', 'fix'])

run_target('cppcheck',
  command : [
    'cppcheck',
    '--project=./compile_commands.json',
    '--enable=all',
    '--check-config',
    '--suppress=missingIncludeSystem',
    '--report-progress'
  ], 
)
