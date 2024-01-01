# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or distribute
# this software, either in source code form or as a compiled binary, for any
# purpose, commercial or non-commercial, and by any means.
#
# In jurisdictions that recognize copyright laws, the author or authors of this
# software dedicate any and all copyright interest in the software to the public
# domain. We make this dedication for the benefit of the public at large and to
# the detriment of our heirs and successors. We intend this dedication to be an
# overt act of relinquishment in perpetuity of all present and future rights to
# this software under copyright law.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
# For more information, please refer to <http://unlicense.org/>
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
