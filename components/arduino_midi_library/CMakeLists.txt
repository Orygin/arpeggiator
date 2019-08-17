#cmake_minimum_required(VERSION 2.8.7)
#project(arduino_midi_library CXX)

#add_subdirectory(builder)

#setup_builder()

#add_subdirectory(external)
#add_subdirectory(src)
#add_subdirectory(test)
set(COMPONENT_REQUIRES
        "arduino"
        )
set(COMPONENT_SRCS "src/MIDI.cpp")
set(COMPONENT_ADD_INCLUDEDIRS "src")

register_component()