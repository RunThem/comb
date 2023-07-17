set_project('comb')

set_xmakever('2.6.1')

set_version('0.0.1')

add_plugindirs('.plugins')

add_rules('mode.debug', 'mode.release')

add_defines('_GNU_SOURCE=1')

set_warnings('all', 'error')

add_cflags('-std=gnu2x')

if is_mode('debug') then
  add_cflags('-Wno-unused-function')
  add_cflags('-Wno-unused-variable')
end

--- clang block
-- add_cflags('-fblocks')
-- if is_plat('linux') then
--   add_ldflags('-lBlocksRuntime')
--   add_defines('__linux__=1')
-- end

add_repositories('RunThem https://github.com/RunThem/My-xmake-repo')
add_requires(
  -- RunThem repo
  'libu',
  'tiny-regex-c',

  -- xrepo
  'mimalloc'
)

add_includedirs('$(projectdir)/src')

target('comb', function()
  set_kind('binary')
  add_files('src/*.c')

  add_packages('mimalloc', 'libu', 'tiny-regex-c')
end)

target('fmt', function()
  set_kind('phony')
  set_default('false')

  add_files('src/**.c')
  set_pcheader('src/**.h')
end)
