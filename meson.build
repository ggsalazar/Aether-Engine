project('Aether Engine', ['cpp', 'c'],
          version: '0.8.0',
    meson_version: '>= 0.62.0',
  default_options: [ 'warning_level=2', 'werror=false', 'cpp_std=gnu++2a', ],
)

# Add runtime bounds-checking only in Debug mode
if get_option('buildtype') == 'debug'
  #faster but less strict
  #add_project_arguments('-D_GLIBCXX_ASSERTIONS', language : 'cpp')
  #slower but more strict
  add_project_arguments('-D_GLIBCXX_DEBUG', language : 'cpp')
endif
#The following command can be run in bash to help debug build graphs
#meson introspect builddir --targets

sources = []

subdir('src')

deps = [
dependency('sdl3', required : true),
dependency('sdl3-image', required : true),
dependency('sdl3-ttf', required : true),
dependency('nlohmann_json', required : true)
]

executable('Aether Engine', sources,
  dependencies: deps,
  install: true,
)
