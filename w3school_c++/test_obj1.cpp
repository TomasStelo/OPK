//
// Created by tomas on 2/19/26.
//
#include <iostream>
#include <string>
#include "test_obj1.h"

int main() {
    MyClass myObj;

    myObj.myNum = 15;
    myObj.myString = "Some text";

    std::cout << myObj.myNum << "\n";
    std::cout << myObj.myString << std::endl;

    return 0;
}