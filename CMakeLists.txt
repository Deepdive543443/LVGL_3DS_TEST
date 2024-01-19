# cmake -DCMAKE_TOOLCHAIN_FILE=cmake/DevkitArm3DS.cmake ..
cmake_minimum_required(VERSION 2.8)
project(LVGL-demo.elf)
# include(FetchContent)
# project(LVGL-demo.elf LANGUAGES C)

# set(APP_TITLE Detkit_3DS)
# set(APP_DESCRIPTION "Light_weight object detection toolkits powered by NCNN")
# set(APP_AUTHOR Deepdive543443)
# set(APP_ICON 48_ncnn.png)
# set(ROMFS romfs)

# Note that you must copy the cmake folder and the DevkitArm3DS.cmake file in this directory
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake) # Add the cmake folder to the modules paths, so that we can use the tools and find_package for ctrulib
include(Tools3DS) # Include all the macros and tools needed for 3ds development.
find_package(CTRULIB REQUIRED) # Look for ctrulib


# Create executable 
file(GLOB_RECURSE SOURCES src/*)
add_executable(LVGL-demo.elf ${SOURCES}) # Create the elf file
target_include_directories(LVGL-demo.elf PRIVATE src)

# FetchContent_Declare(lvgl URL https://github.com/lvgl/lvgl.git)
# FetchContent_MakeAvailable(lvgl)
# target_include_directories(LVGL-demo.elf ${CMAKE_INCLUDE_CURRENT_DIR})

target_link_libraries(LVGL-demo.elf 3ds::ctrulib) # Link ctrulib, this will also setup the include directories !
# target_include_directories(LVGL-demo.elf PRIVATE ../../cpp_libs/rapidjson-1.1.0/include) # Link RapidJSON

set(ncnn_DIR ../../cpp_libs/ncnn_3ds/lib/cmake/ncnn) # Link ncnn
find_package(ncnn REQUIRED)
target_link_libraries(LVGL-demo.elf ncnn)

add_3dsx_target(LVGL-demo.elf 
    # ${APP_TITLE}
    # ${APP_DESCRIPTION}
    # ${APP_AUTHOR}
    # ${APP_ICON}
    # ${ROMFS}
) # Generate the .3dsx and .smdh from the hello_world target