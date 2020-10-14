cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(CMAKE_CXX_STANDARD 98)
set(PROJECT_NAME airquality)
project(${PROJECT_NAME})

# set board type accordingly, for optiboot 328p 16Mhz set to uno
set(${PROJECT_NAME}_BOARD uno)

#for pro mini set to "pro" and set CPU
#set(${PROJECT_NAME}_BOARD pro)
#set(ARDUINO_CPU 16MHzatmega328)

print_board_list()

### Main sketch file
set(${PROJECT_NAME}_SKETCH airquality.ino)

set(${PROJECT_NAME}_PROGRAMMER avrispmkii)
set(${PROJECT_NAME}_PORT /dev/cu.usbserial-A4010ILD)

## Verbose build process
set(${PROJECT_NAME}_AFLAGS -v)

generate_arduino_firmware(${PROJECT_NAME})

print_programmer_list()
