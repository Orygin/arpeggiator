set(COMPONENT_REQUIRES
        "encoder"
        "arduino_midi_library"
        "Adafruit-MCP23017-Arduino-Library"
        "Adafruit_SSD1306"
        )
set(COMPONENT_SRCS "inputTask.cpp" "main.cpp" "menuController.cpp" "displayController.cpp" "TempoTimer.cpp" "Sequencor.cpp" )
set(COMPONENT_ADD_INCLUDEDIRS ".")

register_component()