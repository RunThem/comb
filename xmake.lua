--- xmake configure
set_xmakever('2.6.1')
add_plugindirs('.plugins')

--- Project name
set_project('comb')

--- Project version
set_version('0.0.1')

--- Build mode
add_rules('mode.debug', 'mode.release')

--- Macro definition
add_defines('_GNU_SOURCE=1')

--- No warning
set_warnings('all', 'error')

--- Build C flags
add_cflags('-std=gnu2x')

--- Unused variables and functions are allowed in debug mode
if is_mode('debug') then
  add_cflags('-Wno-unused-function')
  add_cflags('-Wno-unused-variable')
end

--- Lambda expressions
local lambda = false
if lambda then
  add_cflags('-fblocks')
  if is_plat('linux') then
    add_ldflags('-lBlocksRuntime')
    add_defines('__linux__=1')
  end
end

--- Private repositories
add_repositories('RunThem https://github.com/RunThem/My-xmake-repo')
add_requires('libu')
add_requires('mimalloc')

--- Project common header file path
add_includedirs('$(projectdir)/src')

target('comb', function()
  set_kind('binary')
  add_files('src/*.c')

  add_packages('mimalloc', 'libu')
end)
