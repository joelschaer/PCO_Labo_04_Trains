#ifndef CHEFQUAI_H
#define CHEFQUAI_H

#include "locomotive.h"
#include <vector>
#include <QThread>

class ChefQuai{

private:
    vector<vector<int>> OnSegmentsTrain;

public:
    ChefQuai(){

    }

    bool isDispo(int numeroTrain, int posA, int posB){
        bool isOnA = OnSegmentsTrain.at(numeroTrain).at(0) == posA;
        bool isOnB = OnSegmentsTrain.at(numeroTrain).at(1) == posB;
        if(isOnA && isOnB){
            return false;
        }
        else return true;
    }

    void changeSegment(int numeroTrain, int posA, int posB){
        OnSegmentsTrain.at(numeroTrain).at(0) = posA;
        OnSegmentsTrain.at(numeroTrain).at(1) = posB;
    }

};

#endif // CHEFTRAIN_H
