#ifndef CHEFQUAI_H
#define CHEFQUAI_H

#include "locomotive.h"
#include "ctrain_handler.h"
#include <vector>
#include <QThread>
#include <QSemaphore>
#include <QMutex>

#define TRAIN_1 1
#define TRAIN_2 2

#define SET_DEV 1
#define SET_DEV_PAS 0

class ChefQuai{

private:
    int debutSection;
    int finSection;
    bool sectionOccupee;
    int trainDansSection;
    bool trainEnTrainDeSortir;

    QMutex* trainEnAttente;
    QSemaphore* mutexSecCrit;
    QSemaphore* mutexAiguillage;


    int prioTrain1;
    int prioTrain2;

public:

    ChefQuai(){
        debutSection = 33;
        finSection = 24;
        sectionOccupee = false;
        trainEnTrainDeSortir = false;
        trainDansSection = -1;

        trainEnAttente = new QMutex();
        mutexSecCrit = new QSemaphore(1);
        mutexAiguillage = new QSemaphore(1);
    }

    // règle les aiguillage sur demande des chefs de train.
    void regler_aiguillage(int numeroTrain, int nextPoint, int dev){
        mutexAiguillage->acquire();
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
        mutexAiguillage->release();
    }

    // sur demande des chef de train indique si le tronçon demandé est libre d'accès.
    // pour prendre des dispositions suffisamment vite nous testons la prochaine section et la surprochaine section.
    bool isDispo(int numeroTrain , int nextPoint, int nextNextPoint, int sens){

        bool ok = true; // si le prochain ou le surprochain contact demandé n'est pas le premier de la section critique le train est de tt façon autorisé à avancé.

        // test en fonction du sens de march du train
        mutexSecCrit->acquire();
        if(sens == 1){

            if(nextPoint == debutSection || nextNextPoint == debutSection){ // section critique dans le sens normal

                //il faut que le train soit prioritaire pour qu'il soit autoriser a avancer
                if((numeroTrain == TRAIN_1 && prioTrain1 == 1) || (numeroTrain == TRAIN_2 && prioTrain2 == 1)){

                    // si la section n'est pas occupée ou que le train est déjà dans la section on lui donne de droit d'avancer et il réserve la section critique.
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
            if(nextPoint == finSection || nextNextPoint == finSection){ // section critique dans le sens inverse

                //il faut que le train soit prioritaire pour qu'il soit autoriser a avancer
                if((numeroTrain == TRAIN_1 && prioTrain1 == 1) || (numeroTrain == TRAIN_2 && prioTrain2 == 1)){

                    // si la section n'est pas occupée ou que le train est déjà dans la section on lui donne de droit d'avancer et il réserve la section critique.
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
        mutexSecCrit->release();
        return ok;
    }

    void changeSegment(int numeroTrain, int last, int sens){
        mutexSecCrit->acquire();
        // nous devons assurer que le train ai passé le contact suivant le dernier de la séction critique avant que la séction soit libérée.
        // Cela pour des raisons d'innercie et de vitesse de réaction des threads qui est relativement décalée.
        if(trainEnTrainDeSortir == true && numeroTrain == trainDansSection){
            sectionOccupee = false;
            trainEnTrainDeSortir = false;
            trainDansSection = -1;
            trainEnAttente->unlock();
        }
        // test la première ou la seconde position du tableau de section critique en fonction du sens de marche
        if(sens == 1){
            // si le contact passé est le dernier de la section critique et que le train était dans la section critique
            if(last == finSection && numeroTrain == trainDansSection){
                trainEnTrainDeSortir = true;
                afficher_message(qPrintable(QString("loco " + QString::number(numeroTrain) + " sort de section")));
            }
        }else{
            // si le contact passé est le dernier de la section critique et que le train était dans la section critique
            if(last == debutSection && numeroTrain == trainDansSection){
                trainEnTrainDeSortir = true;
                afficher_message(qPrintable(QString("loco " + QString::number(numeroTrain) + " sort de section")));
            }
        }
        mutexSecCrit->release();
    }

    void setPrioTrain (int prioTrain1, int prioTrain2){
        mutexSecCrit->acquire();
        this->prioTrain1 = prioTrain1;
        this->prioTrain2 = prioTrain2;
        trainEnAttente->unlock();
        mutexSecCrit->release();
    }

    void attendreLaSection(){
        trainEnAttente->lock();
    }

};



#endif // CHEFTRAIN_H
