# [git](https://git-scm.com/downloads)

## 信息
```bash
git config --global user.name "你的用户名"
git config --global user.email "你的邮箱"
```

## 操作仓库
拉取远程仓库
```bash
git clone https://github.com/JackLau1222/ffmpeg-GUI
```
添加一个名为test的cpp文件
```bash
touch test.cpp 
```
将此更改提交到缓冲区
```bash
git add test.cpp 
```
将更改提交到本地仓库
```bash
git commit -a -m “add test.cpp” 
```
提交到目标仓库
```bash
git push
```
