# Additive Synth - Experimental Synthesizer with some features to explore.
#  
#  Copyright (C) 2023 Christoph Wellm <christoph.wellm@creaflect.de>
#
#  This program is free software: you can redistribute it and/or modify it under the terms of the 
#  GNU General Public License version 3 as published by the Free Software Foundation.
#  
#  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without 
#  even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
#  General Public License for more details. 
#  
#  You should have received a copy of the GNU General Public License along with this program.  
#  If not, see <http://www.gnu.org/licenses/>.
#   
#  SPDX-License-Identifier: GPL-3.0-only

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.microsoft import MSBuildToolchain

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

    DOXYGEN_VERSION = "1.9.6"

    SRC_FOLDER = "src"

    def requirements(self):
        pass
        #self.requires("zlib/1.2.13")
        #self.requires("armadillo/11.4.3")
        #self.requires("doxygen/1.9.4")

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

        juce_options.update({"CMAKE_PREFIX_PATH": juce_installation_dir, "JUCE_MODULE_INCLUDE_DIR": 
                             juce_installation_dir + f"/include/JUCE-{self.JUCE_VERSION}/modules"})
        cmake.configure(variables=juce_options)

    def build(self):
        cmake = CMake(self)
        cmake.build()
