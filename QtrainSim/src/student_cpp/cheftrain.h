#ifndef CHEFTRAIN_H
#define CHEFTRAIN_H

#include "locomotive.h"
#include "chefquai.h"
#include "ctrain_handler.h"
#include <QThread>

class ChefTrain : public QThread
{

private:
    Locomotive *loco;
    ChefQuai *chef;
    QList<int> *parcours;
    QList<int> *parcours_inv;
    int sensDepart;
    int priorite; // 0: pas de prio / 1: prioritaire
    QList<int> *deviation1;
    QList<int> *deviation1_inv;
    QList<int> *deviation2;
    QList<int> *deviation2_inv;
    int deviationLength;


public:
    ChefTrain(){}

    ChefTrain(Locomotive *loco, QList<int>* parcours, ChefQuai* chef, int sensDepart, int priorite){
        this->loco = loco;
        this->parcours = parcours;
        this->chef = chef;
        this->sensDepart = sensDepart;
        this->priorite = priorite;

        this->parcours_inv = inverse_qlist(parcours);
    }

    void run()Q_DECL_OVERRIDE{
        //Attente du passage sur les contacts
        int nbTours = 0;
        int sens = 1;

        while(true){
            if(nbTours == 2){
                loco->arreter();
                sleep(1);
                loco->inverserSens();
                sens *= -1;
                nbTours = 0;
            }

        if(loco->numero() == 1)
            printf("hello");


            if(sens == 1){
                runParcours(parcours, deviation1, deviation2, sens);
            }
            //sens inverse (-1)
            else{
                runParcours(parcours_inv, deviation1_inv, deviation2_inv, sens);
            }
            nbTours++;
        }
        // lorsque contacte connu
        // demande au chef de quai si peut continuer ou pas
    }

    void runParcours(QList<int> *parcours, QList<int> *deviation1, QList<int> *deviation2, int sens){
        bool libre;

        for (int i = 0; i < parcours->size(); i++) {

            // test si la prochaine et la surprochaine section est libre
            libre = chef->isDispo(loco->numero(),parcours->at(i), sens);
            if(i < parcours->size()-1){
                libre &= chef->isDispo(loco->numero(),parcours->at(i+1), sens);
            }

            if(libre){
                loco->demarrer();
                chef->regler_aiguillage(loco->numero(), parcours->at(i), sens);
                attendre_contact(parcours->at(i));
                chef->changeSegment(loco->numero(), parcours->at(i), sens);

                afficher_message(qPrintable(QString("The engine no. ").append(loco->numero()).append(" has reached contact no. %2.")
                                            .arg(loco->numero()).arg(parcours->at(i))));
                loco->afficherMessage(QString("I've reached contact no. %1.").arg(parcours->at(i)));
            }
            else{
                // le train est prioritaire donc il prend la voie de déviation
                if(priorite == 1){
                    loco->afficherMessage(qPrintable(QString("train devié")));
                    loco->demarrer();

                    switch(parcours->at(i)){
                    case 15:
                        if(sens == 1){
                            runDeviation(deviation1, sens);
                            i = 5;
                        }
                        break;
                    case 6:
                        if(sens == -1){
                            runDeviation(deviation1, sens);
                            i = parcours->length()-1;
                        }
                        break;
                    case 33:
                        if(sens == 1){
                            runDeviation(deviation2, sens);
                            i = parcours->length()-1;
                        }
                        break;
                    case 24:
                        if(sens == -1){
                            runDeviation(deviation2, sens);
                            i = 5;
                        }
                        break;
                    }
                }

                // le train n'est pas prioritaire, donc il s'arrête
                else{
                    loco->afficherMessage(qPrintable(QString("stop")));
                    loco->arreter();
                    sleep(1);

                    // Quesiton pour le prof : si le train repart d'ici, il y aura incrément de i et donc on saute l'attente d'un contact
                    while(true){
                        bool ok = chef->isDispo(loco->numero(),parcours->at(i), sens);
                        ok &= chef->isDispo(loco->numero(),parcours->at(i+1), sens);
                        if(ok){
                            sleep(1);
                            break;
                        }
                    }
                }
            }
        }// fin for
    } // fin runParcours

    void runDeviation(QList<int> *deviation, int sens){
        for(int i = 0; i < deviation->length(); ++i){
            chef->regler_aiguillage(loco->numero(), deviation->at(i), sens);
            attendre_contact(deviation->at(i));
            afficher_message(qPrintable(QString("The engine no. ").append(loco->numero()).append(" has reached contact no. %2.")
                                        .arg(loco->numero()).arg(deviation->at(i))));
            loco->afficherMessage(QString("I've reached contact no. %1.").arg(deviation->at(i)));
        }
    } // fin runDeviation

    void setDev(int deviationLength, QList<int>* deviation1, QList<int>* deviation2){
        this->deviationLength = deviationLength;
        this->deviation1 = deviation1;
        this->deviation1_inv = inverse_qlist(deviation1);
        this->deviation2 = deviation2;
        this->deviation2_inv = inverse_qlist(deviation2);
    } // fin setDeviation

    QList<int>* inverse_qlist(QList<int>* list){
        QList<int>* qlist = new QList<int>();

        for(int i = list->length()-1; i >= 0; i--){
            qlist->append(list->at(i));
        }
        return qlist;
    }

    ~ChefTrain(){
        delete parcours;
        delete deviation1_inv;
        delete deviation2_inv;
    }
}; // fin ChefTrain

#endif // CHEFTRAIN_H
