from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout

import os
import shutil
from pathlib import Path
import subprocess

from conan.tools.files import download, unzip, check_sha1

from src.JUCE.install_juce import execute as juce_installation


class VSTRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    JUCE_VERSION = "7.0.5"
    JUCE_INSTALLATION_REL_DIRECTORY = f"JUCE-{JUCE_VERSION}"
    JUCE_OPTIONS = {"JUCE_ENABLE_MODULE_SOURCE_GROUPS": "ON", "JUCE_BUILD_EXTRAS": "ON"}
    SRC_FOLDER = "src"

    def requirements(self):
        pass
        #self.requires("zlib/1.2.13")
        #self.requires("armadillo/11.4.3")

    def build_requirements(self):
        self.tool_requires("cmake/[>3.20.6]")

    def layout(self):
        cmake_layout(self, src_folder=self.SRC_FOLDER)

    def generate(self):

        juce_installation_dir = self.generators_folder + "/" + self.JUCE_INSTALLATION_REL_DIRECTORY
        juce_installation(self.JUCE_VERSION, juce_installation_dir)

        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

        # configure and install the JUCE package
        cmake = CMake(self)
        juce_options = self.JUCE_OPTIONS

        #juce_options |= {"CMAKE_PREFIX_PATH": juce_installation_dir}
        juce_options.update({"CMAKE_PREFIX_PATH": juce_installation_dir, "JUCE_MODULE_INCLUDE_DIR": juce_installation_dir + f"/include/JUCE-{self.JUCE_VERSION}/modules"})
        cmake.configure(variables=juce_options)

    def build(self):
        cmake = CMake(self)
        cmake.build()
