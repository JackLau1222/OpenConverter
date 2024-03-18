# FFMPEG GUI
FFMPEG GUI 是一个基于 ffmpeg源代码、qt开源框架构建的软件，它提供了一些简单易用的工具，可以方便地转换、编辑和处理音视频文件。

本项目使用[Qt框架](/Qt.md)和[git](/git-command.md)仓库完成开发

## [Git介绍](/git-command.md)

## [Windows终端命令](/Windows-command.md)

## [Qt工程文件学习模版](/project_learn)

### ffmpeg介绍
FFmpeg是一套开源的跨平台音视频处理工具，可以用来录制、转换和流式传输音视频文件。FFmpeg由C语言编写而成，支持多种操作系统，包括Windows、Linux、macOS等。

[ffmpeg官网](https://www.ffmpeg.org/)

[ffmpeg下载](https://ffmpeg.org/download.html)

[ffmpeg官方文档](http://ffmpeg.org/ffmpeg-all.html)

[ffmpeg教程](https://www.wikiwand.com/en/FFmpeg)

#### FFmpeg提供了一些核心库和工具，使得开发者能够方便地处理视频和音频文件，支持的功能包括：

* 视频编解码：支持多种视频格式的编解码，如MPEG-4、H.264、VP8等。
* 音频编解码：支持多种音频格式的编解码，如MP3、AAC、FLAC等。
* 视频过滤器：支持对视频进行裁剪、缩放、旋转等操作，还支持添加水印、边框等特效。
* 音频过滤器：支持对音频进行降噪、混响、均衡器等处理。
* 流媒体处理：支持从网络摄像头、麦克风等设备抓取输入，并将处理后的数据输出到网络或本地文件。



## 使用
运行 FFMPEG GUI 后，您可以使用以下功能：

### 查看音视频信息

目前支持查看比特率、编码格式、色彩空间、采样精度、采样格式、帧速率、声道数等核心信息。

### 转封装

目前支持抽取音视频、字幕流转封装为新的封装格式（容器）

### 二次编码

选择您需要的参数进行重新编码

### 调整参数

可以调整一些常用的视频参数，例如帧率、码率、分辨率等等。您只需选择要设定的参数，并设置对应的值，即可开始调整参数。

## 贡献
如果您想为 FFMPEG GUI 做出贡献，请按照以下步骤操作：

### Fork 本项目
创建一个新的分支，进行修改或添加功能。
提交您的更改，创建一个 pull request。

## 许可证
FFMPEG GUI 是基于 Apache 2.0 许可证开源的。请在使用前阅读 LICENSE 文件。
