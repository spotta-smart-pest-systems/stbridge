from distutils.core import setup
from distutils.extension import Extension

ext = Extension(
    'stbridge_api',
    sources=['stbridge.cpp', 'libs/src/bridge/bridge.cpp', 'libs/src/common/criticalsectionlock.cpp',
    	'libs/src/common/stlink_device.cpp', 'libs/src/common/stlink_interface.cpp', 'libs/src/error/ErrLog.cpp'],
    libraries=['boost_python312'],
    extra_compile_args=['-std=c++17', '-Ilibs/src/bridge', '-Ilibs/src/common', '-Ilibs/src/error'],
    extra_link_args=['-L.', '-lusb-1.0', '-lfmt']
)

setup(
    name='stbridge',
    version='0.2',
    ext_modules=[ext],
    packages=['stbridge'],
    package_dir={'stbridge': 'stbridge'}
    )
