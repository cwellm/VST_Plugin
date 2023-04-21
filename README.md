
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

### Installation

### Documentation
You can find the [Doxygen](https://www.doxygen.nl/index.html) code documentation on the [Github Pages]() for this repository. You can also create the 
documentation for yourself locally, by running 

```python
pip install -r requirements.txt
python generate_documentation.py
```
This will download Doxygen and then create the docs in the _docs/generated_ folder. 

## License

## Development

### Building

### Code Structure

### Extending

## Known Issues
- When loading the plugin, it might be that the *Release* parameter has to be increased before any sound is produced.
Once the sound appears, one can set the Release as desired.

## Contribution

## Contact