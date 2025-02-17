#include <iostream>
using namespace std;
 
void foo(){
    int a=1;
    if(a>0){cout<<"Positive"<<endl;}
    else{cout<<"Non-positive"<<endl;}
}
 
int main(){
  foo();
  return 0;
}


// Example.cpp
// 非 LLVM 风格的代码示例

int main() { int i; return;}


// 1. 缩进不一致
void exampleFunction1()
{
     int a = 0; // 额外的空格
 int b= 1;  // 缺少空格
  int c =2;  // 缺少空格
   int d =  3; // 多余的空格

 // 括号风格不一致
 if (a == 0) {b = 1;}
 else {c = 2;}

 // 缺少空格和括号
 for(int i=0; i<10; i++){ printf("i: %d\n", i); }

 // 函数体末尾缺少空行
 return;
}

// 2. 变量命名不规范（使用下划线命名法）
void example_function_2() {
    int my_variable = 0; // 使用下划线，不符合 LLVM 风格
    // 函数体内有多个连续空行

    // 不一致的缩进
        int x = 0;
    int y = 1;

    // 不一致的括号风格
    if (x == 0) { y = 1; }
    else { z = 2; }

    // 缺少空格
    int a=0,b=1,c=2;

    // 多余的空格
    int d =  3;

    // 不一致的空行
    int e = 4;

    int f = 5;

    // 文件末尾有空行
}



// 4. 函数参数风格不一致
void exampleFunction3(int a, int  b, int   c) {
    // 函数体内有多个连续空行

    // 不一致的缩进
        int x = 0;
    int y = 1;

    // 不一致的括号风格
    if (x == 0) { y = 1; }
    else { z = 2; }

    // 缺少空格
    int a=0,b=1,c=2;

    // 多余的空格
    int d =  3;

    // 不一致的空行
    int e =4;
    int f= 5;

    // 文件末尾有空行
}

// 3. 宏定义不符合 LLVM 风格
#define MAX_SIZE 1024

// 5. 注释风格不一致
// This is a comment
// Another comment


#include <iostream>
using namespace std;
 
void foo(){
    int a=1;
    if(a>0){cout<<"Positive"<<endl;}
    else{cout<<"Non-positive"<<endl;}
}
 
int main(){
  foo();
  return 0;
}


// test.cpp
int main(){ 
    printf("hello");return 0;
}


int main() { int i; return;}