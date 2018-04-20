#ifndef CHEFQUAI_H
#define CHEFQUAI_H

#include "locomotive.h"
#include "ctrain_handler.h"
#include <vector>
#include <QThread>
#include <QSemaphore>

class ChefQuai{

private:
    std::vector<std::vector<int>> OnSegmentsTrain;
    std::vector<std::vector<int>> secCrit;
    QSemaphore mutex;
    QSemaphore inCrit1, inCrit2;


public:

    ChefQuai():mutex(1), inCrit1(1), inCrit2(1){
        //OnSegmentsTrain.resize(2);
        OnSegmentsTrain = {{0,0},{0,0}};
        secCrit = {{15,6,0},{33,24,0}};
    }

    void regler_aiguillage(int numeroTrain, int nextPoint){
        if(numeroTrain == 0){
            switch(nextPoint){
            case 15:
                diriger_aiguillage(8, DEVIE,  0);
                break;
            case 6:
                diriger_aiguillage(2, DEVIE,  0);
                break;
            case 33:
                diriger_aiguillage(20, DEVIE,  0);
                break;
            case 24:
                diriger_aiguillage(14, DEVIE,  0);
                break;
            }
        } else if(numeroTrain == 1){
            switch(nextPoint){
            case 15:
                diriger_aiguillage(8, DEVIE,  0);
                break;
            case 6:
                diriger_aiguillage(2, TOUT_DROIT,  0);
                break;
            case 33:
                diriger_aiguillage(20, DEVIE,  0);
                break;
            case 24:
                diriger_aiguillage(14, TOUT_DROIT,  0);
                break;
            }
        }
    }

    bool isDispo(int numeroTrain , int nextPoint){
        bool ok = true;
        mutex.acquire();
        if(nextPoint == secCrit.at(0).at(0)){ // section critique 1
            if(secCrit.at(0).at(2) == 0){
                secCrit.at(0).at(2) = 1;
                ok = true;
            }
            else{
                ok = false;
            }
        }
        else if(nextPoint == secCrit.at(1).at(0)){ // section critique 2
            if(secCrit.at(1).at(2) == 0){
                secCrit.at(1).at(2) = 1;
                ok = true;
            }
            else{
                ok = false;
            }
        }
        mutex.release();
        return ok;
    }

    void changeSegment(int numeroTrain, int last){
        mutex.acquire();
        if(last == secCrit.at(0).at(1)){
            secCrit.at(0).at(2) = 0;
        }
        else if(last == secCrit.at(1).at(1)){
            secCrit.at(1).at(2) = 0;
        }
        mutex.release();
    }

};

#endif // CHEFTRAIN_H
