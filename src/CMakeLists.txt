cmake_minimum_required(VERSION 3.5)

project(OpenConverter VERSION 1.2 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BMF_TRANSCODER "enable BMF Transcoder" ON)
option(FFTOOL_TRANSCODER "enable FFmpeg Command Tool Transcoder" ON)
option(FFMPEG_TRANSCODER "enable FFmpeg Core Transcoder" ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

function(find_ffmpeg_version FFMPEG_VERSION_INCLUDE_DIR FFMPEG_LIB_DIR VERSION_VAR)
    # First, attempt to find the version from ffversion.h
    find_file(FFMPEG_VERSION_FILE
            NAMES ffversion.h
            PATHS ${FFMPEG_VERSION_INCLUDE_DIR}
            PATH_SUFFIXES ffmpeg
            NO_DEFAULT_PATH)

    if(NOT FFMPEG_VERSION_FILE)
        message(WARNING "Unable to find FFmpeg version file in ${FFMPEG_VERSION_INCLUDE_DIR}. Attempting to get version from library files.")
    endif()

    if(FFMPEG_VERSION_FILE)
        # If we find ffversion.h, extract the version from it
        file(STRINGS ${FFMPEG_VERSION_FILE} FFMPEG_VERSION_DEFINE
            REGEX "#define FFMPEG_VERSION ")

        string(REGEX MATCH "FFMPEG_VERSION \"([^\"]+)\"" FFMPEG_VERSION_STRING ${FFMPEG_VERSION_DEFINE})
        set(FFMPEG_VERSION_MATCHED ${CMAKE_MATCH_1})

        if(FFMPEG_VERSION_MATCHED AND NOT FFMPEG_VERSION_MATCHED MATCHES "N-")  
            string(REGEX MATCHALL "([0-9]+)" VERSION_PARTS ${FFMPEG_VERSION_MATCHED})
            list(GET VERSION_PARTS 0 VERSION_MAJOR)
            list(GET VERSION_PARTS 1 VERSION_MINOR)
            set(FFMPEG_VERSION_CLEAN "${VERSION_MAJOR}${VERSION_MINOR}")
            set(${VERSION_VAR} ${FFMPEG_VERSION_CLEAN} PARENT_SCOPE)
            message(STATUS "Detected FFmpeg version from ffversion.h: ${FFMPEG_VERSION_CLEAN}")
            return()
        else()
            message(WARNING "Unable to parse FFmpeg version from ${FFMPEG_VERSION_FILE}. Try the next step...")
        endif()
    endif()

    # If the version from ffversion.h is not found or is a dev version, fallback to libraries
    # Search for FFmpeg libraries to detect version
    file(GLOB FFMPEG_LIBRARIES "${FFMPEG_LIB_DIR}/libavcodec.*.*")

    if(FFMPEG_LIBRARIES)
        foreach(lib ${FFMPEG_LIBRARIES})
            string(REGEX MATCH "libavcodec\\.([0-9]+)" FFMPEG_VERSION_MATCH ${lib})
            if(FFMPEG_VERSION_MATCH)
                string(REGEX REPLACE "libavcodec\\.([0-9]+)" "\\1" FFMPEG_VERSION ${FFMPEG_VERSION_MATCH})
                
                # Map library version to FFmpeg version
                if(FFMPEG_VERSION STREQUAL "58")
                    set(FFMPEG_VERSION_CLEAN "40")  # FFmpeg v4.x
                elseif(FFMPEG_VERSION STREQUAL "59")
                    set(FFMPEG_VERSION_CLEAN "50")  # FFmpeg v5.x
                elseif(FFMPEG_VERSION STREQUAL "60")
                    set(FFMPEG_VERSION_CLEAN "60")  # FFmpeg v6.x
                elseif(FFMPEG_VERSION STREQUAL "61")
                    set(FFMPEG_VERSION_CLEAN "70")  # FFmpeg v7.x (dev)
                else()
                    message(FATAL_ERROR "Unsupported FFmpeg library version: ${FFMPEG_VERSION}")
                endif()

                set(${VERSION_VAR} ${FFMPEG_VERSION_CLEAN} PARENT_SCOPE)
                message(STATUS "Detected FFmpeg version from libraries: ${FFMPEG_VERSION_CLEAN}")
                return()
            endif()
        endforeach()
    endif()

    message(FATAL_ERROR "Unable to determine FFmpeg version from both ffversion.h and libraries.")
endfunction()

if(DEFINED ENV{FFMPEG_ROOT_PATH})
    set(FFMPEG_ROOT_PATH $ENV{FFMPEG_ROOT_PATH})
    message(STATUS "Using FFMPEG_ROOT_PATH: ${FFMPEG_ROOT_PATH}")
    
    set(FFMPEG_INCLUDE_DIRS ${FFMPEG_ROOT_PATH}/include)
    set(FFMPEG_LIBRARY_DIRS ${FFMPEG_ROOT_PATH}/lib)

elseif(WIN32)
    message(STATUS "This is a Windows platform.")
    message(STATUS "FFMPEG_ROOT_PATH environment variable is not set, using default path.")
    set(FFMPEG_ROOT_PATH "D:/ffmpeg/ffmpeg-n5.1.6-16-g6e63e49496-win64-gpl-shared-5.1")
    
    set(FFMPEG_INCLUDE_DIRS ${FFMPEG_ROOT_PATH}/include)
    set(FFMPEG_LIBRARY_DIRS ${FFMPEG_ROOT_PATH}/lib)

else()
    message(STATUS "This is a non-Windows platform, using pkg-config.")
    find_package(PkgConfig REQUIRED)
    
    pkg_check_modules(FFMPEG REQUIRED libavcodec libavformat libavfilter libavutil libswresample libswscale)
    
    set(FFMPEG_INCLUDE_DIRS ${FFMPEG_INCLUDE_DIRS})
    set(FFMPEG_LIBRARY_DIRS ${FFMPEG_LIBRARY_DIRS})
endif()

message(STATUS "FFMPEG_INCLUDE_DIRS: ${FFMPEG_INCLUDE_DIRS}")
message(STATUS "FFMPEG_LIBRARY_DIRS: ${FFMPEG_LIBRARY_DIRS}")

list(GET FFMPEG_LIBRARY_DIRS 0 FFMPEG_LIBRARY__DIRS_FIRST)
find_ffmpeg_version(${FFMPEG_INCLUDE_DIRS}/libavutil ${FFMPEG_LIBRARY__DIRS_FIRST} OC_FFMPEG_VERSION)
add_definitions(-DOC_FFMPEG_VERSION=${OC_FFMPEG_VERSION})

if (BMF_TRANSCODER)
    # Check if the BMF_ROOT_PATH environment variable is set
    if (DEFINED ENV{BMF_ROOT_PATH})
        set(BMF_ROOT_PATH $ENV{BMF_ROOT_PATH})
        message(STATUS "BMF_ROOT_PATH environment variable is set to ${BMF_ROOT_PATH}.")
    else()
        message(STATUS "BMF_ROOT_PATH environment variable is not set, using default path.")
        # You can set a default path here if you want
        set(BMF_ROOT_PATH "/Users/jacklau/Documents/Programs/Git/Github/bmf/output/bmf")
    endif()
    # Add BMF include and library directories
    include_directories(${BMF_ROOT_PATH}/include)
    link_directories(${BMF_ROOT_PATH}/lib)

    add_definitions(-DENABLE_BMF)  # Define a macro for BMF transcoder
#    list(APPEND HEADERS
#        ${CMAKE_SOURCE_DIR}/transcoder/include/transcoder_bmf.h
#    )
    list(APPEND SOURCES
        ${CMAKE_SOURCE_DIR}/transcoder/src/transcoder_bmf.cpp
    )
endif()

if (FFTOOL_TRANSCODER)
    # Check if ffmpeg is available in the environment
    find_program(FFMPEG_EXECUTABLE NAMES ffmpeg)
    if(FFMPEG_EXECUTABLE)
        message(STATUS "FFmpeg found at: ${FFMPEG_EXECUTABLE}")
#        list(APPEND HEADERS
#            ${CMAKE_SOURCE_DIR}/transcoder/include/transcoder_fftool.h
#        )
        list(APPEND SOURCES
            ${CMAKE_SOURCE_DIR}/transcoder/src/transcoder_fftool.cpp
        )
        add_definitions(-DENABLE_FFTOOL)  # Define a macro for FFmpeg Tool transcoder
        add_definitions(-DFFTOOL_PATH="${FFMPEG_EXECUTABLE}")
    else()
        message(FATAL_ERROR "FFmpeg not found in the system PATH!")
    endif()
endif()

if (FFMPEG_TRANSCODER)
    add_definitions(-DENABLE_FFMPEG)  # Define a macro for FFmpeg transcoder
#    list(APPEND HEADERS
#        ${CMAKE_SOURCE_DIR}/transcoder/include/transcoder_ffmpeg.h
#    )
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
    ${CMAKE_SOURCE_DIR}/transcoder/include/transcoder_fftool.h
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
    avformat
    avcodec
    avutil
    pthread
    swresample
    swscale
    avfilter
    avdevice
    postproc
)

if(BMF_TRANSCODER)
    target_link_libraries(OpenConverter PRIVATE
        engine
        bmf_module_sdk
        hmp
    )
endif()

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
