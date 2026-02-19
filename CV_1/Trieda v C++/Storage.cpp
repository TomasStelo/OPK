//
// Created by tomas on 2/18/26.
//
#include "Storage.h"

Storage::Storage(double maxCapacity) {
    maxCapacity_ = maxCapacity;
}

bool Storage::add(double amount) {
    if (currentValue_ + amount > maxCapacity_) {
        return false;
    }
    else {
        currentValue_ += amount;
    }
}

double Storage::getPercentage() const {
    return currentValue_ / maxCapacity_;
}

double Storage::release() {

}
