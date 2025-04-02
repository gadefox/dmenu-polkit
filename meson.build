project('dmenu-polkit', 'c',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c11'])

polkit_dep = dependency('polkit-agent-1')
glib_dep = dependency('glib-2.0')

srcs = [
  'src/main.c',
  'src/util.c',
  'src/listener.c',
]

exe = executable(
  'dmenu-polkit', 
  srcs,
  dependencies: [
    polkit_dep,
    glib_dep,
  ],
  install : true)
