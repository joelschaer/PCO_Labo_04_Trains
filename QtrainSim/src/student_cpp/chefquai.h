#ifndef CHEFQUAI_H
#define CHEFQUAI_H

#include "locomotive.h"
#include "ctrain_handler.h"
#include <vector>
#include <QThread>
#include <QSemaphore>

#define TRAIN_1 1
#define TRAIN_2 2

#define SET_DEV 1
#define SET_DEV_PAS 0

class ChefQuai{

private:
    std::vector<int> secCrit;
    QSemaphore mutex;
    QSemaphore inCrit1, inCrit2;


public:

    ChefQuai():mutex(1), inCrit1(1), inCrit2(1){
        //OnSegmentsTrain.resize(2);
        secCrit = {33,24,0,-1}; // début section critique, fin section critique, 0: section libre / 1: utilisé, train dans la section
    }

    void regler_aiguillage(int numeroTrain, int nextPoint, int dev){
        mutex.acquire();
        if(numeroTrain == TRAIN_1){
            switch(nextPoint){
            case 34:
            case 23:
                diriger_aiguillage(21,  DEVIE, 0);
                diriger_aiguillage(14,  DEVIE, 0);
                break;
            }
        }
        if(numeroTrain == TRAIN_2){
            if(dev == SET_DEV){
                switch(nextPoint){
                case 31:
                case 19:
                    diriger_aiguillage(22,  TOUT_DROIT, 0);
                    diriger_aiguillage(13,  TOUT_DROIT, 0);
                    break;
                }
            }
            else{
                switch(nextPoint){
                case 31:
                case 19:
                    diriger_aiguillage(21,  TOUT_DROIT, 0);
                    diriger_aiguillage(22,  DEVIE, 0);
                    diriger_aiguillage(13,  DEVIE, 0);
                    diriger_aiguillage(14,  TOUT_DROIT, 0);
                    break;
                }
            }
        }
        mutex.release();
    }

    bool isDispo(int numeroTrain , int nextPoint, int sens){
        bool ok = true;
        int posTab;
        // test la première ou la seconde position du tableau de section critique en fonction du sens de marche
        if(sens == 1){
            posTab = 0;
        }else{
            posTab = 1;
        }
        mutex.acquire();
        if(nextPoint == secCrit.at(posTab)){ // section critique 2
            if(secCrit.at(2) == 0 || secCrit.at(3) == numeroTrain){
                secCrit.at(2) = 1;
                secCrit.at(3) = numeroTrain;
                ok = true;
            }
            else{
                ok = false;
            }
        }
        mutex.release();
        return ok;
    }

    void changeSegment(int numeroTrain, int last, int sens){
        int posTab;
        // test la première ou la seconde position du tableau de section critique en fonction du sens de marche
        if(sens == 1){
            posTab = 1;
        }else{
            posTab = 0;
        }
        mutex.acquire();

        // si le contact passé est le dernier de la section critique et que le train était dans la section critique
        if(last == secCrit.at(posTab) && numeroTrain == secCrit.at(3)){
            secCrit.at(2) = 0;
            secCrit.at(3) = -1;
        }

        mutex.release();
    }

};

#endif // CHEFTRAIN_H
