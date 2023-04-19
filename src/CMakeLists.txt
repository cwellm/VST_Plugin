cmake_minimum_required(VERSION 3.15)

project(Additive_Synth VERSION 1.0 LANGUAGES C CXX)

if (CMAKE_BUILD_TYPE)
	message(STATUS "Build type = ${CMAKE_BUILD_TYPE}")
endif()

message(STATUS "Trying to find package ZLIB...")
find_package(ZLIB)
message(STATUS "Package ZLIB found!")
message(STATUS "Trying to find package JUCE...")
find_package(JUCE CONFIG REQUIRED)
message(STATUS "Package JUCE found!")

#//  global compilation flag configuring windows sdk headers
#//  preventing inclusion of min and max macros clashing with <limits>
#define NOMINMAX 1

#//  override byte to prevent clashes with <cstddef>
#define byte win_byte_override

add_compile_definitions(NOMINMAX=1 byte=win_byte_override)
add_compile_definitions(_HAS_STD_BYTE=0)

set_property(GLOBAL PROPERTY USE_FOLDERS YES)

juce_add_plugin(Additive_Synth 
	VERSION 1.0
	FORMATS VST3 Standalone
	IS_SYNTH TRUE
	NEEDS_MIDI_INPUT TRUE
)

target_sources(Additive_Synth PRIVATE 
	PluginEditor.cpp 
	PluginEditor.h 
	PluginProcessor.cpp 
	PluginProcessor.h
)

target_compile_definitions(Additive_Synth
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(Additive_Synth
    PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_utils#
		juce::juce_audio_basics
		juce::juce_audio_formats
		juce::juce_audio_plugin_client
		juce::juce_audio_processors
		juce::juce_core
		juce::juce_data_structures
		juce::juce_events
		juce::juce_graphics
		juce::juce_gui_basics
		juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

juce_generate_juce_header(Additive_Synth)

# add sources from synth subdir
target_sources(Additive_Synth PRIVATE
	synth/SineGenerator.h
	synth/SineGenerator.cpp
	synth/AdditiveSynth.h
	synth/QuantumEffects.h
	synth/QuantumEffects.cpp
	util/SoundProcessor.h
	util/SoundProcessor.cpp
	components/AddSynthComponent.h
	components/AddSynthComponent.cpp
	components/QuantumComponent.h
	components/QuantumComponent.cpp 
	components/ADSRComponent.h 
	components/ADSRComponent.cpp 
)