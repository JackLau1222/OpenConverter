# 项目开发规范

## 括号

- 正确示范：
```
if(true)
{
    printf("true\n");
}
```
- 错误示范：
```
if(true){
    printf("true\n");
}
```

##  函数/方法

- 使用驼峰命名法，单词与单词之间用下划线"_"分隔，且第二个单词首字母需要大写，比如：
```
int apply_Pushed(int x)
{
    return x;
}
```

## 变量

- 使用驼峰命名法,比如：
```
    int numberCount;
```

## 类

- 每个单词的首字母需要大写，比如：
```
class CoreAudio
{
public:
    int number;
};
```

## 文件命名

- 文件名称中，单词与单词之间使用下划线"_"进行分隔，比如：
```
    core_audio.c
    core_audio.h
```

## 开发环境版本

- Qt：5.14.2
- FFmpeg：5.1.4