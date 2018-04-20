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
            switch(nextPoint){
            case 1:
                diriger_aiguillage(2, TOUT_DROIT,  0);
                break;
            case 2:
                diriger_aiguillage(1, DEVIE,  0);
                break;
            case 3:
                diriger_aiguillage(4, TOUT_DROIT,  0);
                break;
            case 4:
                diriger_aiguillage(3, DEVIE,  0);
                break;
            case 5:
                diriger_aiguillage(2, DEVIE,  0);
                break;
            case 6:
                diriger_aiguillage(1, TOUT_DROIT,  0);
                diriger_aiguillage(4, DEVIE,  0);
                break;
            case 7:
                diriger_aiguillage(3, DEVIE,  0);
                diriger_aiguillage(5, TOUT_DROIT,  0);
                break;
            case 8:
                diriger_aiguillage(3, TOUT_DROIT,  0);
                diriger_aiguillage(5, DEVIE,  0);
                break;
            case 9:
                diriger_aiguillage(18, DEVIE,  0);
                diriger_aiguillage(24, TOUT_DROIT,  0);
                break;
            case 10:
                diriger_aiguillage(8, TOUT_DROIT,  0);
                break;
            case 11:
                diriger_aiguillage(7, TOUT_DROIT,  0);
                break;
            case 12:
                diriger_aiguillage(10, TOUT_DROIT,  0);
                break;
            case 13:
                diriger_aiguillage(9, TOUT_DROIT,  0);
                break;
            case 14:
                diriger_aiguillage(8, DEVIE,  0);
                diriger_aiguillage(11, TOUT_DROIT,  0);
                break;
            case 15:
                diriger_aiguillage(7, DEVIE,  0);
                diriger_aiguillage(10, DEVIE,  0);
                break;
            case 16:
                diriger_aiguillage(9, DEVIE,  0);
                break;
            case 17:
                diriger_aiguillage(18, TOUT_DROIT,  0);
                diriger_aiguillage(24, DEVIE,  0);
                break;
            case 18:
                diriger_aiguillage(8, DEVIE,  0);
                diriger_aiguillage(11, DEVIE,  0);
                break;
            case 19:
                diriger_aiguillage(14, TOUT_DROIT,  0);
                break;
            case 20:
                diriger_aiguillage(13, TOUT_DROIT,  0);
                break;
            case 21:
                diriger_aiguillage(16, TOUT_DROIT,  0);
                break;
            case 22:
                diriger_aiguillage(15, TOUT_DROIT,  0);
                break;
            case 23:
                diriger_aiguillage(14, DEVIE,  0);
                break;
            case 24:
                diriger_aiguillage(13, DEVIE,  0);
                diriger_aiguillage(16, DEVIE,  0);
                break;
            case 25:
                diriger_aiguillage(15, DEVIE,  0);
                diriger_aiguillage(17, TOUT_DROIT,  0);
                break;
            case 26:
                diriger_aiguillage(15, DEVIE,  0);
                diriger_aiguillage(17, DEVIE,  0);
                break;
            case 27:
                diriger_aiguillage(6, DEVIE, 0);
                diriger_aiguillage(12, TOUT_DROIT,  0);
                break;
            case 28:
                diriger_aiguillage(20, TOUT_DROIT,  0);
                break;
            case 29:
                diriger_aiguillage(19, TOUT_DROIT,  0);
                break;
            case 30:
                diriger_aiguillage(22, TOUT_DROIT,  0);
                break;
            case 31:
                diriger_aiguillage(21, TOUT_DROIT,  0);
                break;
            case 32:
                diriger_aiguillage(20, DEVIE,  0);
                diriger_aiguillage(23, TOUT_DROIT,  0);
                break;
            case 33:
                diriger_aiguillage(19, DEVIE,  0);
                diriger_aiguillage(22, DEVIE,  0);
                break;
            case 34:
                diriger_aiguillage(21, DEVIE,  0);
                break;
            case 35:
                diriger_aiguillage(6, TOUT_DROIT,  0);
                diriger_aiguillage(12, DEVIE,  0);
                break;
            case 36:
                diriger_aiguillage(20, DEVIE,  0);
                diriger_aiguillage(23, DEVIE,  0);
                break;
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
