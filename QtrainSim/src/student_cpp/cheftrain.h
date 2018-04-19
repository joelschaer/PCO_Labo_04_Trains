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
        for (int i = 0; i < parcours->size(); i++) {
            bool ok = chef->isDispo(loco->numero(),i, i+1);
            if(ok){
                attendre_contact(parcours->at(i));

                afficher_message(qPrintable(QString("The engine no. ").append(loco->numero()).append(" has reached contact no. %2.")
                                            .arg(loco->numero()).arg(parcours->at(i))));
                loco->afficherMessage(QString("I've reached contact no. %1.").arg(parcours->at(i)));
            }
            else{
                loco->arreter();
            }
        }
        // lorsque contacte connu
        // demande au chef de quai si peut continuer ou pas
    }

};

#endif // CHEFTRAIN_H
