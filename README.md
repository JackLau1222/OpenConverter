# OpenConverter

## 📢 Project Introduction

OpenConverter is software built on the **FFmpeg** source code, **Qt** open-source framework, and **BMF framework**. It provides simple and user-friendly tools to convert, edit, and process audio and video files.

The converter includes the following main features:

1. Supports changing video codecs for encoding (e.g., libx264, libx265).
2. Supports lossless multimedia conversion.
3. Displays information about video and audio streams in multimedia files.
4. Shows progress and remaining time while converting files.
5. Allows runtime switching of transcoding cores (FFmpeg, FFTool, BMF).

This project is developed using the [Qt framework](./doc/Qt.md), the FFmpeg library, and the [BMF framework](https://github.com/BabitMF/bmf).

For FFmpeg development tutorials, refer to [Learn FFmpeg the Hard Way](https://github.com/TSGU-OSC/Learn_FFmpeg_the_Hard_Way).

## 🌟 Feature Details

After running OpenConverter, you can see and use the following features:

<div align="center">

<img src="./img/open_converter.png" style="width:100%">

<img src="./img/encode_setting.png" style="width:50%">

</div>

### 1. Support for Changing Audio and Video Codecs for Encoding (e.g., libx264, libx265, aac, ac3)

This converter allows users to easily change audio and video codecs. For example, you can choose to use the libx264 or libx265 encoder for higher compression rates and better video quality.

### 2. Support for Lossless Multimedia Conversion

The converter also supports lossless multimedia conversion. This means you can directly convert files from one format to another without re-encoding the original video and audio streams. This is very useful for quick file conversions.

### 3. Display Information About Video and Audio Streams

This converter can display detailed information about video and audio streams, including resolution, frame rate, bitrate, etc. This helps you understand the properties of multimedia files for making appropriate adjustments during conversion.

### 4. Show Progress and Remaining Time During File Conversion

During file conversion, the player shows real-time progress to help you estimate the time required for the process. This allows you to better plan your time and avoid unnecessary waiting.

### 5. Runtime Switching of Transcoding Cores (FFmpeg, FFTool, BMF)

The software offers three different transcoding cores to choose from: one written using the FFmpeg API, one that calls the FFmpeg command-line tool, and one written based on the BMF framework. You can also selectively compile these cores.

## User Guide

If you encounter any issues during operation, feel free to check our [User Guide Documentation](./doc/OpenConverter_Usage.md).

## 📖 Contribution Guide

If you are interested in contributing to the project or have found any bugs, please refer to our [Contribution Guide](./CONTRIBUTING.md).

## ☘️ License

OpenConverter is open-source software licensed under the Apache 2.0 License. Please read the [LICENSE](./LICENSE) file before use.
