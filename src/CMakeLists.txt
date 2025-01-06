cmake_minimum_required(VERSION 3.5)

project(OpenConverter VERSION 1.2 LANGUAGES CXX)

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
    ${CMAKE_SOURCE_DIR}/builder/include
    ${CMAKE_SOURCE_DIR}/common/include
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/transcoder/include
)

link_directories(
    /usr/local/lib
    /opt/homebrew/Cellar/x264/r3108/lib
    /opt/homebrew/Cellar/x265/4.0_1/lib
    /opt/homebrew/Cellar/libx11/1.8.9/lib
    /Users/jacklau/Documents/Programs/Git/Github/bmf/output/bmf/lib
)

# Add source files
set(SOURCES
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/builder/src/encode_setting.cpp
    ${CMAKE_SOURCE_DIR}/builder/src/open_converter.cpp
    ${CMAKE_SOURCE_DIR}/common/src/encode_parameter.cpp
    ${CMAKE_SOURCE_DIR}/common/src/info.cpp
    ${CMAKE_SOURCE_DIR}/common/src/process_parameter.cpp
    ${CMAKE_SOURCE_DIR}/common/src/stream_context.cpp
    ${CMAKE_SOURCE_DIR}/engine/src/converter.cpp
    ${CMAKE_SOURCE_DIR}/transcoder/src/transcoder.cpp
    ${CMAKE_SOURCE_DIR}/transcoder/src/transcoder_bmf.cpp
)

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
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(OpenConverter
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# target_link_libraries(OpenConverter PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

#target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}_autogen/include)


target_link_libraries(OpenConverter
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
