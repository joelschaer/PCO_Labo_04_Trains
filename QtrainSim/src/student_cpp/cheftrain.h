#ifndef CHEFTRAIN_H
#define CHEFTRAIN_H

#include "locomotive.h"
#include "chefquai.h"
#include "ctrain_handler.h"
#include <QThread>

#define TRAIN_1 1
#define TRAIN_2 2

#define SET_DEV 1
#define SET_DEV_PAS 0

class ChefTrain : public QThread
{

private:
    Locomotive *loco;
    ChefQuai *chef;
    QList<int> *parcours;
    QList<int> *parcours_inv;
    int numeroTrain;

    int sens;
    int priorite; // 0: pas de prio / 1: prioritaire
    int newPriorite;
    QList<int> *deviation1;
    QList<int> *deviation1_inv;
    int deviationLength;

    QSemaphore* mutex;

public:
    ChefTrain(){}

    ChefTrain(Locomotive *loco, QList<int>* parcours, ChefQuai* chef, int numeroTrain, int priorite){
        this->loco = loco;
        this->parcours = parcours;
        this->chef = chef;
        this->priorite = priorite;
        this->newPriorite = priorite;
        this->numeroTrain = numeroTrain;
        this->parcours_inv = inverse_qlist(parcours);

        this->sens = 1;

        this->mutex = new QSemaphore(1);
    }

    void run()Q_DECL_OVERRIDE{
        //Attente du passage sur les contacts
        int nbTours = 0;

        while(true){
            if(nbTours == 2){
                loco->arreter();
                sleep(1);
                loco->inverserSens();
                sens *= -1;
                nbTours = 0;
            }

            if(sens == 1){
                runParcours(parcours, deviation1);
            }
            else{
                runParcours(parcours_inv, deviation1_inv);
            }
            nbTours++;
        }
        // lorsque contacte connu
        // demande au chef de quai si peut continuer ou pas
    }

    void runParcours(QList<int> *parcours, QList<int> *deviation1){
        bool libre;

        for (int i = 0; i < parcours->size(); i++) {

            // update de la prio avec newPrio si hors de la section critique
            switch(parcours->at(i)){
            case 33:
            case 32:
            case 25:
            case 24:
                break;
            default:
                priorite = newPriorite;
            }

            if(priorite == 1){

                // test si la prochaine et la surprochaine section est libre
                libre = chef->isDispo(numeroTrain, parcours->at(i), sens);
                if(i < parcours->size()-1){
                    libre &= chef->isDispo(numeroTrain, parcours->at(i+1), sens);
                }

                if(libre){

                    chef->regler_aiguillage(numeroTrain, parcours->at(i), SET_DEV_PAS);
                    loco->demarrer();

                    attendre_contact(parcours->at(i));
                    chef->changeSegment(numeroTrain, parcours->at(i), sens);

                    loco->afficherMessage(QString("I've reached contact no. %1.").arg(parcours->at(i)));
                }
                else{
                    // le train dispose qu'une voie d'évitement
                    if(deviation1 != NULL){
                        loco->afficherMessage(qPrintable(QString("train devié")));
                        loco->demarrer();

                        switch(parcours->at(i)){
                        case 33:
                            if(sens == 1){
                                runDeviation(deviation1);
                                i = parcours->length()-1;
                            }
                            break;
                        case 24:
                            if(sens == -1){
                                runDeviation(deviation1_inv);
                                i = 5;
                            }
                            break;
                        }
                    }
                    else{
                        loco->afficherMessage(qPrintable(QString("stop")));
                        loco->arreter();
                        sleep(1);

                        // Quesiton pour le prof : si le train repart d'ici, il y aura incrément de i et donc on saute l'attente d'un contact
                        while(true){
                            bool ok = chef->isDispo(numeroTrain,parcours->at(i), sens);
                            ok &= chef->isDispo(numeroTrain,parcours->at(i+1), sens);
                            if(ok){
                                sleep(1);
                                break;
                            }
                        }
                    }
                }
            }
            else {

                // le train dispose qu'une voie d'évitement
                if(deviation1 != NULL){
                    loco->afficherMessage(qPrintable(QString("train devié")));
                    loco->demarrer();

                    switch(parcours->at(i)){
                    case 33:
                        if(sens == 1){
                            runDeviation(deviation1);
                            i = parcours->length()-1;
                        }
                        break;
                    case 24:
                        if(sens == -1){
                            runDeviation(deviation1_inv);
                            i = 5;
                        }
                        break;
                    }
                }
                else{
                    loco->afficherMessage(qPrintable(QString("stop")));
                    loco->arreter();
                    sleep(1);

                    // Quesiton pour le prof : si le train repart d'ici, il y aura incrément de i et donc on saute l'attente d'un contact
                    while(true){
                        bool ok = chef->isDispo(numeroTrain,parcours->at(i), sens);
                        ok &= chef->isDispo(numeroTrain,parcours->at(i+1), sens);
                        mutex->acquire();
                        if(ok && priorite == 1){
                            sleep(1);
                            break;
                        }
                    }
                }

            }
        }// fin for
    } // fin runParcours

    void runDeviation(QList<int> *deviation){
        for(int i = 0; i < deviation->length(); ++i){
            chef->regler_aiguillage(numeroTrain, deviation->at(i), SET_DEV);
            attendre_contact(deviation->at(i));
            loco->afficherMessage(QString("I've reached contact no. %1.").arg(deviation->at(i)));
        }
    } // fin runDeviation

    void setDev(int deviationLength, QList<int>* deviation1){
        this->deviationLength = deviationLength;
        this->deviation1 = deviation1;
        this->deviation1_inv = inverse_qlist(deviation1);
    } // fin setDeviation

    QList<int>* inverse_qlist(QList<int>* list){
        QList<int>* qlist = new QList<int>();

        for(int i = list->length()-1; i >= 0; i--){
            qlist->append(list->at(i));
        }
        return qlist;
    }

    void setPriorite(int priorite){
        this->newPriorite = priorite;
    }

    void stopTrain(){

        loco->arreter();
    }

    ~ChefTrain(){
        delete parcours;
        delete deviation1_inv;
        delete mutex;
    }
}; // fin ChefTrain

#endif // CHEFTRAIN_H
