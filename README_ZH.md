# OpenConverter

## 📢  项目介绍

OpenConverter 是一个基于 **FFmpeg**源代码、**Qt**开源框架、**BMF框架**构建的软件，它提供了一些简单易用的工具，可以方便地转换、编辑和处理音视频文件。

这款转换器具有以下主要功能：

1. 支持更改视频编解码器以进行编码（例如libx264，libx265）
2. 支持无编码转换多媒体。
3. 支持显示多媒体文件中视频和音频流的信息。
4. 支持在转换文件时显示进度和剩余时间。
5. 运行时切换转码内核（FFmpeg、FFTool、BMF）

本项目使用[Qt框架](./doc/Qt.md)、FFmpeg库、[BMF框架](https://github.com/BabitMF/bmf)完成开发。

FFmpeg 开发教程可参考 [Learn FFmpeg the Hard Way](https://github.com/TSGU-OSC/Learn_FFmpeg_the_Hard_Way)

## 🌟  功能详解

运行 OpenConverter 后，您可以看到并使用以下功能：

<div align="center">

<img src="./img/open_converter.png" style="width:100%">

<img src="./img/encode_setting.png" style="width:50%">

</div>

### 1. 支持更改音视频编解码器以进行编码（例如libx264，libx265，aac，ac3）

这款转换器允许用户轻松地更改音视频编解码器。例如，您可以选择使用libx264或libx265编码器，以获得更高的压缩率和更好的视频质量。

### 2. 支持无编码转换多媒体。

该转换器还支持无需编码即可转换多媒体文件。这意味着您可以在不改变原始视频和音频流的情况下，直接将文件从一种格式转换为另一种格式。这对于快速转换文件非常有用。

### 3. 支持显示多媒体文件中视频和音频流的信息

这款转换器可以显示有关视频和音频流的详细信息，包括分辨率、帧率、比特率等。这有助于您了解多媒体文件的属性，以便在转换过程中进行相应的调整。

### 4. 支持在转换文件时显示进度和剩余时间

在转换文件时，该播放器会实时显示转换进度，以便您了解转换过程所需的时间。这有助于您更好地安排时间，避免不必要的等待。

### 5. 运行时切换转码内核（FFmpeg、FFTool、BMF）

该软件拥有三个不同的转码内核可供选择，分别为由FFmpeg API编写的的内核，调用FFmpeg命令行工具的内核，以及基于BMF框架编写的内核，您还可以选择性编译这些内核。

## 使用指南
如果在运行过程遇到问题，欢迎查看我们的[使用指南文档](./doc/OpenConverter_Usage.md)

## 📖 贡献指南

如果你有兴趣贡献项目或发现了错误，请参考我们的 [贡献指南](./CONTRIBUTING.md)

## ☘️ 许可证

OpenConverter 是基于 Apache 2.0 许可证开源的。请在使用前阅读 [LICENSE](./LICENSE) 文件。
