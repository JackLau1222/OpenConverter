/* This code intentionally does not follow the LLVM code style */
#include <iostream>
#include <string>

using namespace std;

class Example {  // Class name not in CamelCase
private:
    string data;

public:
    Example(string input) : data(input) {}
    
    void printData() { // Method name not in LowerCamelCase
        cout << "Data: " << data << endl;
    }
};

int main(/* argc, *argv */) {  // Unused parameters
    Example obj("Hello, World");
    obj.printData();  // Poorly named variable

    /* No space around control characters */
    for(int i=0;i<10;++i) {
        if(i>5) {
            cout << "i is " << i << endl;
        }
    }

    return 0; // Semicolon has no space before it
}