# 常用Windows终端命令


列出当前目录中的文件和文件夹。
```
dir
```

切换当前目录。
```
cd C:\Users\user_name\Desktop
```

新建一个目录。
```
md new_folder
```

删除一个目录。
```
rd old_folder
```

显示指定文件的内容。
```
type file.txt
```

复制文件或文件夹。
```
copy file.txt backup_file.txt
```

复制文件夹及其内容。
```
xcopy source_folder destination_folder /E
```
* 将source_folder替换为要复制的源文件夹路径，将destination_folder替换为目标文件夹路径。



移动文件或文件夹。
```
move file.txt C:\Users\user_name\Documents
```

删除一个或多个文件。
```
del file.txt
```

测试网络连接状态。
```
ping www.google.com
```

显示计算机的网络配置信息，如IP地址、子网掩码等。
```
ipconfig
```

追踪数据包从本地计算机到目标计算机的路由路径。
```
tracert www.google.com
```

显示当前计算机的网络连接状态、协议等信息。
```
netstat -a
```

显示当前正在运行的进程列表。
```
tasklist
```

结束一个进程。
```
taskkill /im process.exe /f
```
* 将process.exe替换为要结束的进程名称。


显示计算机的硬件和软件配置信息。
```
systeminfo
```
这些是一些常用的Windows终端命令及其示例，它们可以帮助您在命令行中完成许多任务。
