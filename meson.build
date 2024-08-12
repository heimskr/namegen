project('namegen', 'cpp',
	default_options: ['warning_level=2', 'werror=false', 'cpp_std=c++23'])

namegen_sources = ['NameGen.cpp']

inc_dirs = include_directories('.')

namegen_lib = static_library('namegen', namegen_sources,
	override_options: 'unity=off',
	include_directories: inc_dirs)

namegen = declare_dependency(
	include_directories: inc_dirs,
	link_with: namegen_lib
)
