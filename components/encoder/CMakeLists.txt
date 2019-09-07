set(COMPONENT_REQUIRES
        "arduino"
        "Adafruit-MCP23017-Arduino-Library"
        )
set(COMPONENT_SRCS "ClickEncoder.cpp")
set(COMPONENT_ADD_INCLUDEDIRS "." "include")

register_component()