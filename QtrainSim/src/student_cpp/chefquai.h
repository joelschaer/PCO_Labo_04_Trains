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
        secCrit = {{15,6,0,-1},{33,24,0,-1}}; // début section critique, fin section critique, 0: section libre / 1: utilisé, train dans la section
    }

    void regler_aiguillage(int numeroTrain, int nextPoint ,int sens){
        mutex.acquire();
        if(sens == 1){
            switch(nextPoint){
            case 1:
                diriger_aiguillage(2, TOUT_DROIT,  0);
                break;
            case 2:
                break;
            case 3:
                diriger_aiguillage(4, TOUT_DROIT,  0);
                break;
            case 4:
                break;
            case 5:
                diriger_aiguillage(2, DEVIE,  0);
                break;
            case 6:
                diriger_aiguillage(4, DEVIE,  0);
                break;
            case 7:
                break;
            case 8:
                diriger_aiguillage(3, DEVIE,  0);
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
                break;
            case 12:
                diriger_aiguillage(10, TOUT_DROIT,  0);
                break;
            case 13:
                break;
            case 14:
                diriger_aiguillage(8, DEVIE,  0);
                diriger_aiguillage(11, TOUT_DROIT,  0);
                break;
            case 15:
                diriger_aiguillage(10, DEVIE, 0);
                break;
            case 16:
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
                break;
            case 21:
                diriger_aiguillage(16, TOUT_DROIT,  0);
                break;
            case 22:
                break;
            case 23:
                diriger_aiguillage(14, DEVIE,  0);
                break;
            case 24:
                diriger_aiguillage(16, DEVIE,  0);
                break;
            case 25:
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
                break;
            case 30:
                diriger_aiguillage(22, TOUT_DROIT,  0);
                break;
            case 31:
                break;
            case 32:
                diriger_aiguillage(20, DEVIE,  0);
                diriger_aiguillage(23, TOUT_DROIT,  0);
                break;
            case 33:
                diriger_aiguillage(22, DEVIE,  0);
                break;
            case 34:
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
        else{
            switch(nextPoint){
            case 1:
                break;
            case 2:
                diriger_aiguillage(1, TOUT_DROIT,  0);
                break;
            case 3:
                break;
            case 4:
                diriger_aiguillage(3, TOUT_DROIT,  0);
                break;
            case 5:
                break;
            case 6:
                diriger_aiguillage(1, DEVIE,  0);
                break;
            case 7:
                diriger_aiguillage(3, DEVIE,  0);
                diriger_aiguillage(5, TOUT_DROIT,  0);
                break;
            case 8:
                diriger_aiguillage(3, DEVIE,  0);
                diriger_aiguillage(5, DEVIE,  0);
                break;
            case 9:
                diriger_aiguillage(18, DEVIE,  0);
                diriger_aiguillage(24, TOUT_DROIT,  0);
                break;
            case 10:
                break;
            case 11:
                diriger_aiguillage(7, TOUT_DROIT,  0);
                break;
            case 12:
                break;
            case 13:
                diriger_aiguillage(9, TOUT_DROIT,  0);
                break;
            case 14:
                break;
            case 15:
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
                break;
            case 20:
                diriger_aiguillage(13, TOUT_DROIT,  0);
                break;
            case 21:
                break;
            case 22:
                diriger_aiguillage(15, TOUT_DROIT,  0);
                break;
            case 23:
                break;
            case 24:
                diriger_aiguillage(13, DEVIE,  0);
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
                break;
            case 29:
                diriger_aiguillage(19, TOUT_DROIT,  0);
                break;
            case 30:
                break;
            case 31:
                diriger_aiguillage(21, TOUT_DROIT,  0);
                break;
            case 32:
                break;
            case 33:
                diriger_aiguillage(19, DEVIE,  0);
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
        if(nextPoint == secCrit.at(0).at(posTab)){ // section critique 1
            if(secCrit.at(0).at(2) == 0 || secCrit.at(0).at(3) == numeroTrain){ // si la section critique est libre ou si lui même est dans la section
                secCrit.at(0).at(2) = 1;
                secCrit.at(0).at(3) = numeroTrain;
                ok = true;
            }
            else{
                ok = false;
            }
        }
        else if(nextPoint == secCrit.at(1).at(posTab)){ // section critique 2
            if(secCrit.at(1).at(2) == 0 || secCrit.at(1).at(3) == numeroTrain){
                secCrit.at(1).at(2) = 1;
                secCrit.at(1).at(3) = numeroTrain;
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
        if(last == secCrit.at(0).at(posTab) && numeroTrain == secCrit.at(0).at(3)){
            secCrit.at(0).at(2) = 0;
            secCrit.at(0).at(3) = -1;
        }
        else if(last == secCrit.at(1).at(posTab) && numeroTrain == secCrit.at(1).at(3)){
            secCrit.at(1).at(2) = 0;
            secCrit.at(1).at(3) = -1;
        }

        mutex.release();
    }

};

#endif // CHEFTRAIN_H
