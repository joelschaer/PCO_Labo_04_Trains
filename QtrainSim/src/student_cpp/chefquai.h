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
    //std::vector<int> secCrit;
    int debutSection;
    int finSection;
    bool sectionOccupee;
    int trainDansSection;

    QSemaphore mutex;

    int prioTrain1;
    int prioTrain2;

public:

    ChefQuai() : mutex(1){
        debutSection = 33;
        finSection = 24;
        sectionOccupee = false;
        trainDansSection;

        //secCrit = {33,24,0,-1}; // début section critique, fin section critique, 0: section libre / 1: utilisé, train dans la section
    }

    void regler_aiguillage(int numeroTrain, int nextPoint, int dev){
        mutex.acquire();
        if(numeroTrain == TRAIN_1){
            switch(nextPoint){
            case 33:   // 24 et 33 nécessaire dans le cas ou un contact est sauté lors d'un arrêt
            case 24:
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
                case 30:  // 20 et 30 nécessaire dans le cas ou un contact est sauté lors d'un arrêt
                case 20:
                case 31:
                case 19:
                    diriger_aiguillage(22,  TOUT_DROIT, 0);
                    diriger_aiguillage(13,  TOUT_DROIT, 0);
                    break;
                }
            }
            else{
                switch(nextPoint){
                case 24:  // 24 et 33 nécessaire dans le cas ou un contact est sauté lors d'un arrêt
                case 33:
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
        // test en fonction du sens de march du train
        mutex.acquire();
        if(sens == 1){
            if(nextPoint == debutSection){ // section critique
                if((numeroTrain == TRAIN_1 && prioTrain1 == 1) || (numeroTrain == TRAIN_2 && prioTrain2 == 1)){
                    if(sectionOccupee == false || trainDansSection == numeroTrain){
                        sectionOccupee = true;
                        trainDansSection = numeroTrain;
                        afficher_message(qPrintable(QString("loco " + QString::number(numeroTrain) + " entre en section")));
                        ok = true;
                    }
                    else{
                        ok = false;
                    }
                }
                else{
                    // si le train n'est pas prioritaire, mais qu'il est dans la section critique, on le laisse sortir.
                    if(trainDansSection == numeroTrain){
                        ok = true;
                    }
                    else{
                        ok = false;
                    }
                }
            }
        }else{
            if(nextPoint == finSection){ // section critique
                if((numeroTrain == TRAIN_1 && prioTrain1 == 1) || (numeroTrain == TRAIN_2 && prioTrain2 == 1)){
                    if(sectionOccupee == false || trainDansSection == numeroTrain){
                        sectionOccupee = true;
                        trainDansSection = numeroTrain;
                        afficher_message(qPrintable(QString("loco " + QString::number(numeroTrain) + " entre en section")));
                        ok = true;
                    }
                    else{
                        ok = false;
                    }
                }
                else{
                    // si le train n'est pas prioritaire, mais qu'il est dans la section critique, on le laisse sortir.
                    if(trainDansSection == numeroTrain){
                        ok = true;
                    }
                    else{
                        ok = false;
                    }
                }
            }
        }

        mutex.release();
        return ok;
    }

    void changeSegment(int numeroTrain, int last, int sens){
        mutex.acquire();
        // test la première ou la seconde position du tableau de section critique en fonction du sens de marche
        if(sens == 1){
            // si le contact passé est le dernier de la section critique et que le train était dans la section critique
            if(last == finSection && numeroTrain == trainDansSection){
                sectionOccupee = false;
                trainDansSection = -1;
                afficher_message(qPrintable(QString("loco " + QString::number(numeroTrain) + " sort de section")));
            }
        }else{
            // si le contact passé est le dernier de la section critique et que le train était dans la section critique
            if(last == debutSection && numeroTrain == trainDansSection){
                sectionOccupee = false;
                trainDansSection = -1;
                afficher_message(qPrintable(QString("loco " + QString::number(numeroTrain) + " sort de section")));
            }
        }



        mutex.release();
    }

    void setPrioTrain (int prioTrain1, int prioTrain2){
        mutex.acquire();
        this->prioTrain1 = prioTrain1;
        this->prioTrain2 = prioTrain2;
        mutex.release();
    }

};

#endif // CHEFTRAIN_H
