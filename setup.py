from setuptools import setup
from setuptools.extension import Extension
from Cython.Build import cythonize
import numpy

# cc_flags = ['-O3', '-march=native', '-mtune=native']
lib_dirs = ['/usr/local/lib']
include_dirs = ['/usr/local/include', 'src', 'src/include', '/usr/include/opencv4', numpy.get_include()]
libs = ['opencv_core', 'opencv_calib3d', 'opencv_features2d',
        'opencv_imgproc', 'opencv_optflow', 'opencv_video', 'opencv_imgcodecs']

cc_flags = ['-O2']
cc_flags += ['-I' + i for i in include_dirs]
cc_flags += ['-L' + i for i in lib_dirs]
cc_flags += ['-l' + i for i in libs]
print(cc_flags)

modules = cythonize([
    Extension(
        "optical_flow",
        [
            "python/optical_flow.pyx",
            "src/cwrapper.cpp",
            "src/flow_opencv.cpp",
            "src/optical_flow.cpp",
            "src/trackFeatures.cpp"
        ],
        extra_compile_args=cc_flags,
        extra_link_args=cc_flags
    ),
])

setup(
    name='optical-flow-px4',
    version='0.0.1',
    description='Optical flow for PX4 using OpenCV, extracted from gazebo_optical_flow_plugin',
    author='Tolga Demirdal',
    url='https://github.com/shadymeowy/optical-flow-px4',
    setup_requires=['cython', 'numpy'],
    install_requires=['numpy'],
    ext_modules=modules
)
