set(COMPONENT_REQUIRES
        "encoder"
        "arduino_midi_library"
        )
set(COMPONENT_SRCS "rotaryEnc.cpp" "main.cpp" "arpEngine.cpp" "arpTask.cpp" )
set(COMPONENT_ADD_INCLUDEDIRS ".")

register_component()