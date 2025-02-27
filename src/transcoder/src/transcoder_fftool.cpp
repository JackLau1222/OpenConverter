#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include "../include/transcoder_fftool.h"

TranscoderFFTool::TranscoderFFTool(ProcessParameter *processParameter,
                                   EncodeParameter *encodeParameter)
    : Transcoder(processParameter, encodeParameter),
      copyVideo(false),
      copyAudio(false),
      frameTotalNumber(0) {}

TranscoderFFTool::~TranscoderFFTool() {
    // Destructor implementation
}

bool TranscoderFFTool::prepared_opt() {

    if (encodeParameter->get_Video_Codec_Name() == "") {
        copyVideo = true;
    } else {
        copyVideo = false;
    }

    if (encodeParameter->get_Audio_Codec_Name() == "") {
        copyAudio = true;
    } else {
        copyAudio = false;
    }

    if (encodeParameter) {
        videoCodec = encodeParameter->get_Video_Codec_Name();
        videoBitRate = encodeParameter->get_Video_Bit_Rate();
        audioCodec = encodeParameter->get_Audio_Codec_Name();
        audioBitRate = encodeParameter->get_Audio_Bit_Rate();
    }

    return true;
}

bool TranscoderFFTool::transcode(std::string input_path, std::string output_path) {
    if (!prepared_opt()) {
        std::cerr << "Failed to prepare options for transcoding." << std::endl;
        return false;
    }

    // Unified Path Processing Function
    auto quotePath = [](const std::string& path) {
        return "\"" + path + "\"";
    };

    // Build the FFmpeg command
    std::stringstream cmd;

// Check if FFMPEG_PATH is defined (ensure it's set by CMake)
#ifdef FFTOOL_PATH
    cmd << quotePath(FFTOOL_PATH);
#else
    std::cerr << "FFmpeg path is not defined! Ensure CMake sets FFMPEG_PATH." << std::endl;
    return false;
#endif

    // Add the -y flag to overwrite output file without prompting
     cmd << " -i " << quotePath(input_path) << " -y";

    // Video codec options
    if (copyVideo) {
        cmd << " -c:v copy";  // Copy video stream without re-encoding
    } else {
        if (!videoCodec.empty()) {
            cmd << " -c:v " << videoCodec;  // Use specified video codec
        } else {
            std::cerr << "Video codec is not specified!" << std::endl;
            return false;
        }
        if (videoBitRate > 0) {
            cmd << " -b:v " << videoBitRate;  // Set video bitrate if specified
        }
    }

    // Audio codec options
    if (copyAudio) {
        cmd << " -c:a copy";  // Copy audio stream without re-encoding
    } else {
        if (!audioCodec.empty()) {
            cmd << " -c:a " << audioCodec;  // Use specified audio codec
        } else {
            std::cerr << "Audio codec is not specified!" << std::endl;
            return false;
        }
        if (audioBitRate > 0) {
            cmd << " -b:a " << audioBitRate;  // Set audio bitrate if specified
        }
    }

    // Output file path
    cmd << " " << quotePath(output_path);

    // Execute the command
    std::cout << "Executing: " << cmd.str() << std::endl;

    int ret = 0;

#ifdef _WIN32
    // Windows-specific command execution (use cmd /c for shell commands)
    std::string fullCmd = "cmd /c " + quotePath(cmd.str());
    ret = system(fullCmd.c_str());
#else
    // Unix-like systems (Linux/macOS) can directly use system()
    ret = system(cmd.str().c_str());
#endif

    if (ret != 0) {
        std::cerr << "FFmpeg transcoding failed with exit code: " << ret << std::endl;
        return false;
    }

    std::cout << "Transcoding completed successfully." << std::endl;
    return true;
}
