cmake_minimum_required(VERSION 3.5)

project(OpenConverterCmake VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Add include directories for external libraries
include_directories(
    /usr/local/include
    /opt/homebrew/Cellar/x264/r3108/include
    /opt/homebrew/Cellar/x265/4.0_1/include
    /Users/jacklau/Documents/Programs/Git/Github/bmf/output/bmf/include
)

link_directories(
    /usr/local/lib
    /opt/homebrew/Cellar/x264/r3108/lib
    /opt/homebrew/Cellar/x265/4.0_1/lib
    /opt/homebrew/Cellar/libx11/1.8.9/lib
    /Users/jacklau/Documents/Programs/Git/Github/bmf/output/bmf/lib
)

# # Add source files
# set(SOURCES
#     converter.cpp
#     encode_parameter.cpp
#     encode_setting.cpp
#     info.cpp
#     main.cpp
#     open_converter.cpp
#     process_parameter.cpp
#     stream_context.cpp
#     transcoder.cpp
# )

# # Add header files
# set(HEADERS
#     converter.h
#     encode_parameter.h
#     encode_setting.h
#     info.h
#     open_converter.h
#     process_parameter.h
#     stream_context.h
#     transcoder.h
# )

set(PROJECT_SOURCES
        converter.h
        encode_parameter.h
        encode_setting.h
        info.h
        open_converter.h
        process_parameter.h
        stream_context.h
        transcoder.h
        transcoder_bmf.h
        converter.cpp
        encode_parameter.cpp
        encode_setting.cpp
        info.cpp
        main.cpp
        open_converter.cpp
        process_parameter.cpp
        stream_context.cpp
        transcoder.cpp
        transcoder_bmf.cpp
        open_converter.ui
        encode_setting.ui
        lang.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(OpenConverterCmake
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET OpenConverterCmake APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(OpenConverterCmake SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(OpenConverterCmake
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# target_link_libraries(OpenConverterCmake PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(OpenConverterCmake
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    avformat
    avcodec
    avutil
#    bz2
#    z
#    iconv
    x264
    x265
    pthread
    swresample
#    VideoToolbox
#    CoreFoundation
#    Security
#    AudioToolbox
#    CoreMedia
#    CoreVideo
#    CoreServices
    engine
    bmf_module_sdk 
    hmp 
    builtin_modules
)

# Set translation files
set(TRANSLATIONS lang_chinese.ts)

set_target_properties(OpenConverterCmake PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS OpenConverterCmake
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(OpenConverterCmake)
endif()
