//
// Created by tomas on 2/18/26.
//
#include "Storage.h"
#include <iostream>

Storage::Storage(double maxCapacity) {
    maxCapacity_ = maxCapacity;
    currentValue_ = 0.0;
}

bool Storage::add(double amount) {
    if (currentValue_ + amount > maxCapacity_) {
        std::cout << "Prekrocil si max capacity" << std::endl;
        return false;
    }
    currentValue_ += amount;
    return true;
}

double Storage::getPercentage() const {
    if (maxCapacity_ == 0) {
        return 0.0;
    }
    return currentValue_ / maxCapacity_;
}

double Storage::release() {
    double temp = currentValue_;
    currentValue_ = 0.0;
    return temp;
}

int main() {
    Storage myObj(100);

    myObj.add(30);
    std::cout << "Percentage: " << myObj.getPercentage() << std::endl;

    myObj.add(50);
    std::cout << "Percentage: " << myObj.getPercentage() << std::endl; // nedostatok na max

    myObj.add(60);
    std::cout << "Percentage: " << myObj.getPercentage() << std::endl; //prekrocenie cez max

    std::cout << "Released: " << myObj.release() << std::endl;
    std::cout << "Percentage po release: " << myObj.getPercentage() << std::endl;

    return 0;
}