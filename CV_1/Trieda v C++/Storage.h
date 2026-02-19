//
// Created by tomas on 2/18/26.
//

#ifndef OPK_STORAGE_H
#define OPK_STORAGE_H

#pragma once

class Storage {
    public:
        Storage(double maxCapacity);
        bool add(double amount);
        double getPercentage() const;
        double release();
    private:
        double maxCapacity_;
        double currentValue_;
};


#endif //OPK_STORAGE_H