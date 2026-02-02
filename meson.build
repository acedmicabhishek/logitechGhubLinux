project('logitech_ghub', ['cpp', 'c'],
  version: '0.1.0',
  default_options: ['cpp_std=c++23']
)

gtk_dep = dependency('gtk4')
libadwaita_dep = dependency('libadwaita-1')
hidapi_dep = dependency('hidapi-hidraw')

# Include path to plugin_interface.h (Local)
inc = include_directories('.')

shared_library('logitech_ghub',
  [
    'logitech_plugin.cpp',
    'backend/logitech_peripherals.cpp',
    'backend/hidpp_driver.cpp',
    'backend/hidpp20_device.cpp'
  ],
  dependencies: [gtk_dep, libadwaita_dep, hidapi_dep],
  include_directories: inc,
  install: true,
  install_dir: get_option('libdir') / 'kerneldrive/plugins',
  name_prefix: '' 
)
