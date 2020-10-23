cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(CMAKE_CXX_STANDARD 98)
set(PROJECT_NAME airquality)
project(${PROJECT_NAME})

# set board type accordingly, for optiboot 328p 16Mhz set to uno
#set(${PROJECT_NAME}_BOARD uno)

#for pro mini 16MHz 5V
#set(${PROJECT_NAME}_BOARD mypro16)

#for pro mini 8MHz 3V3
set(${PROJECT_NAME}_BOARD mypro8)

print_board_list()

### Main sketch file
set(${PROJECT_NAME}_SKETCH airquality.ino)


file(GLOB SRC_TOP ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB HDR_TOP ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp  )
file(GLOB_RECURSE HDR_FILES ${PROJECT_SOURCE_DIR}/*.h )

set(${PROJECT_NAME}_SRCS ${SRC_TOP} ${SRC_FILES})
set(${PROJECT_NAME}_HDRS ${HDR_TOP} ${HDR_FILES})



set(${PROJECT_NAME}_PROGRAMMER avrispmkii)
set(${PROJECT_NAME}_PORT /dev/cu.usbserial-A4010ILD)

## Verbose build process
set(${PROJECT_NAME}_AFLAGS -v)

generate_arduino_firmware(${PROJECT_NAME})

print_programmer_list()
