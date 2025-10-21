project('Aether Engine', ['cpp', 'c'],
          version: '2025.10.20',
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
dependency('sdl3-mixer', required : true),
dependency('nlohmann_json', required : true)
]

if host_machine.system() == 'windows'
    # Resource file
    rc_file = 'icon.rc'
    res_obj = rc_file.replace('.rc', '.res')

    # Compile .rc -> .res using windres
    rc_compile = custom_target(
        'compile_rc',
        input: rc_file,
        output: res_obj,
        command: ['x86_64-w64-mingw32-windres', '@INPUT@', '-O', 'coff', '-o', '@OUTPUT@']
    )
    sources += rc_compile
endif

executable('Aether Engine', sources,
  dependencies: deps,
  install: true,
)
