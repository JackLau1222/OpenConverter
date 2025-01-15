cmake_minimum_required(VERSION 3.5)

project(OpenConverter VERSION 1.2 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_BMF_TRANSCODER "Use BMF Transcoder" ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Ensure pkg-config is found
find_package(PkgConfig REQUIRED)

# Find the FFmpeg libraries (avcodec, avformat, avutil)
pkg_check_modules(FFMPEG REQUIRED libavcodec libavformat libavfilter libavutil libswresample libswscale)

## Use pkg-config to find x264
#pkg_check_modules(X264 REQUIRED x264)

## Use pkg-config to find x265 (if necessary)
#pkg_check_modules(X265 REQUIRED x265)

# Add source files conditionally based on whether to use BMF or FFmpeg transcoder
if (USE_BMF_TRANSCODER)
    # Check if the BMF_ROOT_PATH environment variable is set
    if (DEFINED ENV{BMF_ROOT_PATH})
        set(BMF_ROOT_PATH $ENV{BMF_ROOT_PATH})
    else()
        message(STATUS "BMF_ROOT_PATH environment variable is not set, using default path.")
        # You can set a default path here if you want
        set(BMF_ROOT_PATH "/Users/jacklau/Documents/Programs/Git/Github/bmf/output/bmf")
    endif()
    # Add BMF include and library directories
    include_directories(${BMF_ROOT_PATH}/include)
    link_directories(${BMF_ROOT_PATH}/lib)

    add_definitions(-DUSE_BMF)  # Define a macro for BMF transcoder
    list(APPEND SOURCES
        ${CMAKE_SOURCE_DIR}/transcoder/src/transcoder_bmf.cpp
    )
else()
    add_definitions(-DUSE_FFMPEG)  # Define a macro for FFmpeg transcoder
    list(APPEND SOURCES
        ${CMAKE_SOURCE_DIR}/transcoder/src/transcoder_ffmpeg.cpp
    )
endif()

# Add include directories for FFmpeg
include_directories(
    ${FFMPEG_INCLUDE_DIRS}
)

link_directories(
    ${FFMPEG_LIBRARY_DIRS}
)

# Add common source files
list(APPEND SOURCES
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/builder/src/encode_setting.cpp
    ${CMAKE_SOURCE_DIR}/builder/src/open_converter.cpp
    ${CMAKE_SOURCE_DIR}/common/src/encode_parameter.cpp
    ${CMAKE_SOURCE_DIR}/common/src/info.cpp
    ${CMAKE_SOURCE_DIR}/common/src/process_parameter.cpp
    ${CMAKE_SOURCE_DIR}/common/src/stream_context.cpp
    ${CMAKE_SOURCE_DIR}/engine/src/converter.cpp
)

# Add source files
# set(SOURCES
#     ${CMAKE_SOURCE_DIR}/main.cpp
#     ${CMAKE_SOURCE_DIR}/builder/src/encode_setting.cpp
#     ${CMAKE_SOURCE_DIR}/builder/src/open_converter.cpp
#     ${CMAKE_SOURCE_DIR}/common/src/encode_parameter.cpp
#     ${CMAKE_SOURCE_DIR}/common/src/info.cpp
#     ${CMAKE_SOURCE_DIR}/common/src/process_parameter.cpp
#     ${CMAKE_SOURCE_DIR}/common/src/stream_context.cpp
#     ${CMAKE_SOURCE_DIR}/engine/src/converter.cpp
#     ${CMAKE_SOURCE_DIR}/transcoder/src/transcoder.cpp
#     ${CMAKE_SOURCE_DIR}/transcoder/src/transcoder_bmf.cpp
# )

# Add header files
set(HEADERS
    ${CMAKE_SOURCE_DIR}/builder/include/encode_setting.h
    ${CMAKE_SOURCE_DIR}/builder/include/open_converter.h
    ${CMAKE_SOURCE_DIR}/common/include/encode_parameter.h
    ${CMAKE_SOURCE_DIR}/common/include/info.h
    ${CMAKE_SOURCE_DIR}/common/include/process_parameter.h
    ${CMAKE_SOURCE_DIR}/common/include/stream_context.h
    ${CMAKE_SOURCE_DIR}/engine/include/converter.h
    ${CMAKE_SOURCE_DIR}/transcoder/include/transcoder.h
    ${CMAKE_SOURCE_DIR}/transcoder/include/transcoder_bmf.h
    ${CMAKE_SOURCE_DIR}/transcoder/include/transcoder_ffmpeg.h
)

# Add UI files
set(UI_FILES
    ${CMAKE_SOURCE_DIR}/builder/src/encode_setting.ui
    ${CMAKE_SOURCE_DIR}/builder/src/open_converter.ui
)

# Add resource files
set(RESOURCE_FILES
    ${CMAKE_SOURCE_DIR}/resources/lang.qrc
)

# Handle MOC, UIC, and RCC (Qt build steps)
# Automatically handle UI files using UIC
#qt_wrap_ui(UI_HEADERS ${UI_FILES})
# qt_add_resources(RESOURCE_OBJECTS ${RESOURCE_FILES})
# qt_wrap_cpp(MOC_HEADERS ${HEADERS})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(OpenConverter
        MANUAL_FINALIZATION
        ${SOURCES} ${UI_FILES} ${RESOURCE_FILES} ${HEADERS}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET OpenConverter APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(OpenConverter SHARED
            ${SOURCES} ${UI_FILES} ${RESOURCE_FILES} ${HEADERS}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(OpenConverter
            ${SOURCES} ${UI_FILES} ${RESOURCE_FILES} ${HEADERS}
        )
    endif()
endif()

target_link_libraries(OpenConverter
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    ${FFMPEG_LDFLAGS}
#    avformat
#    avcodec
#    avutil
#    pthread
#    swresample

    engine
    bmf_module_sdk 
    hmp 
    builtin_modules
)

# Set translation files
set(TRANSLATIONS ${CMAKE_SOURCE_DIR}/resources/lang_chinese.ts)

set_target_properties(OpenConverter PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS OpenConverter
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(OpenConverter)
endif()
