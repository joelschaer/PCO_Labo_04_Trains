#ifndef CHEFTRAIN_H
#define CHEFTRAIN_H

#include "locomotive.h"
#include "chefquai.h"
#include <QThread>

class ChefTrain : public QThread
{

private:
    Locomotive loco;
    ChefQuai chef;
    int parcours[];

public:
    ChefTrain(Locomotive loco, int parcours[], ChefQuai chef){
        this->loco = loco;
        this->chef = chef;
        this->parcours = parcours;
    }

    void run()Q_DECL_OVERRIDE{
        //Attente du passage sur les contacts
        for (int i = 0; i < parcours.size(); i++) {
            bool ok = chef.isDispo(loco.numero(),i, i+1);
            if(ok){
                attendre_contact(parcours.at(i));
                afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                            .arg(loco.numero()).arg(parcours.at(i))));
                loco.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));
            }
            else{
                loco.arreter();
            }
        }

        // lorsque contacte connu
        // demande au chef de quai si peut continuer ou pas
    }

};

#endif // CHEFTRAIN_H
