#include <gtest/gtest.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "../common/include/encode_parameter.h"
#include "../engine/include/converter.h"
#include <string>
#include <memory>
#include <filesystem>
#include <fstream>
#include <iostream>

// Test fixture for transcoder tests
class TranscoderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test directory
        test_dir_ = std::filesystem::temp_directory_path() / "transcoder_test";
        std::filesystem::create_directories(test_dir_);
        
        // Copy test media file to test directory
        std::string test_file = std::string(TEST_MEDIA_PATH) + "/test.mp4";
        if (std::filesystem::exists(test_file)) {
            std::filesystem::copy_file(test_file, test_dir_ / "test.mp4");
        } else {
            FAIL() << "Test media file not found at: " << test_file;
        }
    }

    void TearDown() override {
        // Clean up test directory
        std::filesystem::remove_all(test_dir_);
    }

    std::filesystem::path test_dir_;
};

// Fuzzing test for remuxing
TEST_F(TranscoderTest, RemuxFuzz) {
    const uint8_t* data = nullptr;
    size_t size = 0;
    FuzzedDataProvider provider(data, size);
    
    std::string inputFile = (test_dir_ / "test.mp4").string();
    std::string outputFile = (test_dir_ / ("output_" + std::to_string(provider.ConsumeIntegral<uint32_t>()) + ".mp4")).string();
    
    // For remuxing, don't set any codec parameters
    EncodeParameter encodeParams;
    ProcessParameter processParams;
    
    auto converter = std::make_unique<Converter>(&processParams, &encodeParams);
    converter->set_Transcoder("FFMPEG");
    bool result = converter->convert_Format(inputFile, outputFile);
    
    if (result) {
        EXPECT_TRUE(std::filesystem::exists(outputFile));
        EXPECT_GT(std::filesystem::file_size(outputFile), 0);
    }
}

// Fuzzing test for video transcoding
TEST_F(TranscoderTest, VideoTranscodeFuzz) {
    const uint8_t* data = nullptr;
    size_t size = 0;
    FuzzedDataProvider provider(data, size);
    
    std::string inputFile = (test_dir_ / "test.mp4").string();
    std::string outputFile = (test_dir_ / ("output_" + std::to_string(provider.ConsumeIntegral<uint32_t>()) + ".mp4")).string();
    
    EncodeParameter encodeParams;
    ProcessParameter processParams;
    
    // Set video encoding parameters
    encodeParams.set_Video_Codec_Name("libx264");
    encodeParams.set_Video_Bit_Rate(provider.ConsumeIntegralInRange<int>(1000000, 8000000));
    
    auto converter = std::make_unique<Converter>(&processParams, &encodeParams);
    converter->set_Transcoder("FFMPEG");
    bool result = converter->convert_Format(inputFile, outputFile);
    
    if (result) {
        EXPECT_TRUE(std::filesystem::exists(outputFile));
        EXPECT_GT(std::filesystem::file_size(outputFile), 0);
    }
}

// Fuzzing test for audio transcoding
TEST_F(TranscoderTest, AudioTranscodeFuzz) {
    const uint8_t* data = nullptr;
    size_t size = 0;
    FuzzedDataProvider provider(data, size);
    
    std::string inputFile = (test_dir_ / "test.mp4").string();
    std::string outputFile = (test_dir_ / ("output_" + std::to_string(provider.ConsumeIntegral<uint32_t>()) + ".aac")).string();
    
    EncodeParameter encodeParams;
    ProcessParameter processParams;
    
    // Set audio encoding parameters
    encodeParams.set_Video_Codec_Name("");  // Disable video
    encodeParams.set_Audio_Codec_Name("aac");
    encodeParams.set_Audio_Bit_Rate(provider.ConsumeIntegralInRange<int>(64000, 320000));
    
    auto converter = std::make_unique<Converter>(&processParams, &encodeParams);
    converter->set_Transcoder("FFMPEG");
    bool result = converter->convert_Format(inputFile, outputFile);
    
    if (result) {
        EXPECT_TRUE(std::filesystem::exists(outputFile));
        EXPECT_GT(std::filesystem::file_size(outputFile), 0);
    }
}

// Main fuzzing entry point
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 1) return 0;
    
    FuzzedDataProvider provider(data, size);
    
    // Test remuxing
    {
        EncodeParameter encodeParams;
        ProcessParameter processParams;
        
        std::string input_file = std::string(TEST_MEDIA_PATH) + "/test.mp4";
        std::string output_file = "test_output.mp4";
        
        auto converter = std::make_unique<Converter>(&processParams, &encodeParams);
        converter->set_Transcoder("FFMPEG");
        bool result = converter->convert_Format(input_file, output_file);
        EXPECT_TRUE(result);
    }
    
    // Test video encoding
    {
        EncodeParameter encodeParams;
        ProcessParameter processParams;
        
        encodeParams.set_Video_Codec_Name("libx264");
        encodeParams.set_Video_Bit_Rate(provider.ConsumeIntegralInRange<int>(1000000, 8000000));
        
        std::string input_file = std::string(TEST_MEDIA_PATH) + "/test.mp4";
        std::string output_file = "test_output.mp4";
        
        auto converter = std::make_unique<Converter>(&processParams, &encodeParams);
        converter->set_Transcoder("FFMPEG");
        bool result = converter->convert_Format(input_file, output_file);
        EXPECT_TRUE(result);
    }
    
    // Test audio encoding
    {
        EncodeParameter encodeParams;
        ProcessParameter processParams;
        
        encodeParams.set_Video_Codec_Name("");  // Disable video
        encodeParams.set_Audio_Codec_Name("aac");
        encodeParams.set_Audio_Bit_Rate(provider.ConsumeIntegralInRange<int>(64000, 320000));
        
        std::string input_file = std::string(TEST_MEDIA_PATH) + "/test.mp4";
        std::string output_file = "test_output.mp4";
        
        auto converter = std::make_unique<Converter>(&processParams, &encodeParams);
        converter->set_Transcoder("FFMPEG");
        bool result = converter->convert_Format(input_file, output_file);
        EXPECT_TRUE(result);
    }
    
    return 0;
} 