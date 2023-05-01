<!--
Additive Synth - Experimental Synthesizer with some features to explore.
 
 Copyright (C) 2023 Christoph Wellm <christoph.wellm@creaflect.de>

 This program is free software: you can redistribute it and/or modify it under the terms of the 
 GNU General Public License version 3 as published by the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without 
 even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 General Public License for more details. 
 
 You should have received a copy of the GNU General Public License along with this program.  
 If not, see <http://www.gnu.org/licenses/>.
  
 SPDX-License-Identifier: GPL-3.0-only
-->

# Additive Synth

This project consist of an [additive synthesizer](https://en.wikipedia.org/wiki/Additive_synthesis) which is based on a simple sine generator. 
It serves mainly as a study project, but aims at supplying some interesting, experimental features.
As a basis, the [JUCE Framework](https://github.com/juce-framework/JUCE) is used heavily. 

## Table of Contents

* [Features](#features)
* [Getting Started](#getting-started)
    * [System Requirements](#system-requirements)
    * [Dependencies](#dependencies)
    * [Installation](#installation)
* [License](#license)
* [Development](#development)
    * [Building](#building)
    * [Code Structure](#code-structure)
    * [Extending](#extending)
* [Known Issues](#known-issues)
* [Contact](#contact)

## Features

* Additive Synth with up to 16 harmonics
* ADSR curve 
* experimental parameters

![Screenshot of the current version of the plugin.](/res/shotv_0_1.png)

## Getting Started

### System Requirements

This plugin has been tested to be working with the following setup:

#### Operating System

* Ubuntu-22.04
* Ubuntu-20.04
* Windows Server 2019
* Windows Server 2022
* Windows 10

#### DAW

* Reaper 

Feel free to let me know about your experiences with your setup, so I can extend these lists :slightly_smiling_face:	

### Dependencies

- [JUCE Framework](https://github.com/juce-framework/JUCE)

### Installation

#### Current Version

[Windows 2019](https://github.com/cwellm/VST_Plugin/suites/12591900072/artifacts/673521211)
[Windows 2022](https://github.com/cwellm/VST_Plugin/suites/12591900072/artifacts/673521212)
[Ubuntu 20.04](https://github.com/cwellm/VST_Plugin/suites/12591900072/artifacts/673521209)
[Ubuntu 22.04](https://github.com/cwellm/VST_Plugin/suites/12591900072/artifacts/673521210)

### Documentation

You can find the [Doxygen](https://www.doxygen.nl/index.html) code documentation on the [Github Pages](https://cwellm.github.io/VST_Plugin/) for this repository. You can also create the 
documentation for yourself locally, by running 

```python
pip install -r requirements.txt
python generate_documentation.py
```
This will download Doxygen and then create the docs in the _docs/generated_ folder. 

## License

This software is licensed under the GPL-3.0. See the [license text](LICENSE) for more information. Appropriate copyright notices are
added to all source files which originate from the contributors of this repository. Copyright information of the dependencies in use
within this software is given in the according links under the [Dependencies](#dependencies) section.

Licenses used throughout this project can be found in the [LICENSES](LICENSES) folder. 

Some files do not explicitly contain a license header. In this case, these are covered by the matching license as specified
in the [dep5](.reuse/dep5) file (for instance, for binary files). See [here](https://reuse.software/faq/#bulk-license) for more information about this.

To get a bill of materials of the files this repository (without dependencies), you can use [reuse spdx](https://reuse.readthedocs.io/en/latest/readme.html#usage).

## Development

### Building

### Code Structure

### Extending

## Known Issues

- When loading the plugin, it might be that the *Release* parameter has to be increased before any sound is produced.
Once the sound appears, one can set the Release as desired.

## Contribution

## Contact
