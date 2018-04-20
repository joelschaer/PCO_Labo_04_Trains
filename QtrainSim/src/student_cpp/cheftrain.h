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

public:
    ChefTrain(){}

    ChefTrain(Locomotive *loco, QList<int>* parcours, ChefQuai* chef){
        this->loco = loco;
        this->parcours = parcours;
        this->chef = chef;
    }

    void run()Q_DECL_OVERRIDE{
        //Attente du passage sur les contacts
        int nbTours = 0;
        int sens = 1;
        bool ok;
        while(true){
            if(nbTours == 2){
                loco->inverserSens();
                sens *= -1;
                nbTours = 0;
            }

            if(sens == 1){
                for (int i = 0; i < parcours->size(); i++) {
                    if(i == parcours->size()-1)
                            printf("hello");

                    ok = chef->isDispo(loco->numero(),parcours->at(i), sens);
                    if(i < parcours->size()-1){
                        ok &= chef->isDispo(loco->numero(),parcours->at(i+1), sens);
                    }
                    if(ok){
                        loco->demarrer();
                        chef->regler_aiguillage(loco->numero(), parcours->at(i));
                        attendre_contact(parcours->at(i));
                        chef->changeSegment(loco->numero(), parcours->at(i), sens);

                        afficher_message(qPrintable(QString("The engine no. ").append(loco->numero()).append(" has reached contact no. %2.")
                                                    .arg(loco->numero()).arg(parcours->at(i))));
                        loco->afficherMessage(QString("I've reached contact no. %1.").arg(parcours->at(i)));
                    }
                    else{
                        afficher_message(qPrintable(QString("stop")));
                        loco->arreter();
                        while(true){
                            bool ok = chef->isDispo(loco->numero(),parcours->at(i), sens);
                            if(ok){
                                break;
                            }
                        }
                    }
                }
            }
            else{
                for (int i = parcours->size()-1 ; i >= 0; i--) {

                    ok = chef->isDispo(loco->numero(),parcours->at(i), sens);
                    if(i > 0){
                        ok &= chef->isDispo(loco->numero(),parcours->at(i-1), sens);
                    }
                    if(ok){
                        loco->demarrer();
                        chef->regler_aiguillage(loco->numero(), parcours->at(i));
                        attendre_contact(parcours->at(i));
                        chef->changeSegment(loco->numero(), parcours->at(i), sens);

                        afficher_message(qPrintable(QString("The engine no. ").append(loco->numero()).append(" has reached contact no. %2.")
                                                    .arg(loco->numero()).arg(parcours->at(i))));
                        loco->afficherMessage(QString("I've reached contact no. %1.").arg(parcours->at(i)));
                    }
                    else{
                        afficher_message(qPrintable(QString("stop")));
                        loco->arreter();
                        while(true){
                            bool ok = chef->isDispo(loco->numero(),parcours->at(i), sens);
                            if(ok){
                                break;
                            }
                        }
                    }
                }
            }

            nbTours++;
        }
        // lorsque contacte connu
        // demande au chef de quai si peut continuer ou pas
    }

};

#endif // CHEFTRAIN_H
