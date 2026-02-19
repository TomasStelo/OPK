//
// Created by tomas on 2/19/26.
//

#include <iostream>
#include "outside_example.h"

void outside_example::myMethod() {
    std::cout << "Hello World!" << std::endl;
}

int main() {
    outside_example myObj;
    myObj.myMethod();
    return 0;
}
