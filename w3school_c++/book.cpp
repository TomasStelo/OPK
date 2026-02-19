//
// Created by tomas on 2/19/26.
//

#include <iostream>
#include <string>
#include "book.h"

int main() {
    Book ObjBook1;
    ObjBook1.title = "Matilda";
    ObjBook1.author = "Roald Dahl";
    ObjBook1.year = 1988;

    Book ObjBook2;
    ObjBook2.title = "The Giving Tree";
    ObjBook2.author = "Shel Silverstein";
    ObjBook2.year = 1964;

    std::cout << ObjBook1.title << ", " << ObjBook1.author << ", " << ObjBook1.year << std::endl;
    std::cout << ObjBook2.title << ", " << ObjBook2.author << ", " << ObjBook2.year << std::endl;

    return 0;
}