#ifndef CHEFQUAI_H
#define CHEFQUAI_H

#include "locomotive.h"
#include <vector>
#include <QThread>

class ChefQuai{

private:
    std::vector<std::vector<int>> OnSegmentsTrain;

public:

    ChefQuai(){
        OnSegmentsTrain.resize(2);
        OnSegmentsTrain.at(0).resize(2);
        OnSegmentsTrain.at(1).resize(2);
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
