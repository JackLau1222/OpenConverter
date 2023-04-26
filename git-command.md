# [git](https://git-scm.com/downloads)

## 介绍

Git是一种分布式版本控制系统，它可以记录文件的变更历史并协调多人在同一个代码库上的开发。使用Git，开发人员可以轻松地跟踪代码的修改、回退到以前的版本、合并不同的代码分支和解决代码冲突等任务。Git最初由Linus Torvalds创建，现在是开源社区中最流行的版本控制系统之一。

### 在Git中，有四个重要的概念：工作区、缓冲区、本地仓库和远程仓库。

* 工作区：工作区（Working Directory）是指开发者正在进行开发工作的目录。它包含了项目的源码文件、资源文件等内容。在这个目录下，开发者可以自由修改、添加、删除文件等操作。

* 缓冲区：缓冲区（Staging Area）也称为索引（Index），它是一个临时存储区域，用于存放即将提交到本地仓库的文件变更。开发者对工作区所做的修改，需要先通过git add命令将修改加入到缓冲区，才能提交到本地仓库。

* 本地仓库：本地仓库（Local Repository）是指保存在本地计算机上的Git仓库。它包含了代码库的完整历史记录、分支、标签等信息。在开发过程中，每次提交到缓冲区的变更，都可以通过git commit命令保存到本地仓库中。

* 远程仓库：远程仓库（Remote Repository）是指保存在远程服务器上的Git仓库。它包含了与本地仓库相同的内容，但它是由多个开发者共享的中央代码库。开发者可以通过git push命令将本地仓库中的变更推送到远程仓库，也可以通过git pull命令将远程仓库的变更更新至本地仓库。

## 信息配置
```bash
git config --global user.name "你的用户名"
git config --global user.email "你的邮箱"
```

## 操作仓库
拉取远程仓库
```bash
git clone https://github.com/JackLau1222/ffmpeg-GUI
```
更新本地仓库(自动同步)
```bash
git pull 
````
更新本地仓库(手动同步)
```bash
git fetch 
git merge 
````

添加一个名为test的cpp文件（Unix/Linux）
```bash
touch test.cpp 
```
添加一个名为test的cpp文件（Windows）
```bash
type nul > filename.txt
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
