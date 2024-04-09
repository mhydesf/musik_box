from setuptools import setup, find_packages, Extension
from setuptools.command.build_ext import build_ext
import os
import sys
import subprocess

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)

class CMakeBuild(build_ext):
    def run(self):
        try:
            subprocess.check_output(["cmake", "--version"])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ["-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=" + extdir,
                      "-DPYTHON_EXECUTABLE=" + sys.executable]

        cfg = "Debug" if self.debug else "Release"
        build_args = ["--config", cfg]

        build_temp = os.path.join(os.getcwd(), "build", ext.name)

        if not os.path.exists(build_temp):
            os.makedirs(build_temp)

        subprocess.check_call(["cmake", ext.sourcedir] + cmake_args, cwd=build_temp)
        subprocess.check_call(["cmake", "--build", ".", "--target", ext.name] + build_args, cwd=build_temp)

setup(
    name="musik_box",
    version="0.0.1",
    author="Your Name",
    description="A Python package with C++ extensions",
    long_description="",
    packages=find_packages(),
    ext_modules=[CMakeExtension("musik_box", "audio")],
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
)
