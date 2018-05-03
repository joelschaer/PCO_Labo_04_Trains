/***
 *
 * Autheurs : Joel Schär, Yann Lederrey
 *
 * Description : Classe ayant le rôle de controlleur de locomotive.
 * Elle permet de faire la liaison entre une locomotive et un ChefQuai.
 *
 * runParcours : permet d'avancer sur le parcours prédéfinis auparavent.
 *
 * runDeviation : avancer sur le parcours de déviation dans le cas
 * ou une locomotive doit effectuer une déviation.
 *
 * setDev : permet de définir la déviation.
 *
 * stopTrain : permet d'arreter une locmotive.
 */


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
    QList<int> *deviation1;
    QList<int> *deviation1_inv;
    int deviationLength;


public:
    ChefTrain(){}

    // constructeur
    ChefTrain(Locomotive *loco, QList<int>* parcours, ChefQuai* chef, int numeroTrain){
        this->loco = loco;

        this->parcours = parcours;
        this->parcours_inv = inverse_qlist(parcours);

        this->chef = chef;

        this->numeroTrain = numeroTrain;
        this->sens = 1;
    }

    void run()Q_DECL_OVERRIDE{

        //Attente du passage sur les contacts
        int nbTours = 0;
        while(true){

            // changement de sens après deux tours
            if(nbTours == 2){
                loco->arreter();
                afficher_message(qPrintable(QString("changer de sens")));

                // nous devons attendre un moment pour attendre que la loco soit entièrement arrêtée avant de lui dire de changer de sens.
                sleep(5);
                loco->inverserSens();
                sleep(1);
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
    } // fin run


    void runParcours(QList<int> *parcours, QList<int> *deviation1){
        bool libre;

        loco->demarrer();

        for (int i = 0; i < parcours->size(); i++) {

                // demand si la prochaine et la surprochaine section sont libres
                libre = chef->isDispo(numeroTrain, parcours->at(i), parcours->at((i+1)%parcours->size()), sens);

                if(libre){
                    // la section est libre donc
                    // le chef de train demande au chef de quai de régler les aiguillages pour qu'il puisse avancer
                    chef->regler_aiguillage(numeroTrain, parcours->at(i), SET_DEV_PAS);

                    // on démare la loco au cas ou elle serait arrêtée.
                    loco->demarrer();
                    attendre_contact(parcours->at(i));

                    // on a passé le segement on indique donc qu'on à passé le contact ( sera testé si dernier de la section critique afin de la libéréer
                    chef->changeSegment(numeroTrain, parcours->at(i), sens);

                    loco->afficherMessage(QString("I've reached contact no. %1.").arg(parcours->at(i)));
                }
                else{
                    // le train dispose qu'une voie d'évitement donc il l'utilise si la section critique lui est reffusée.
                    if(numeroTrain == TRAIN_2){
                        loco->afficherMessage(qPrintable(QString("train devié")));

                        // on pour savoir dans quel sens il faut prendre la section critique en fonction des prochains contact
                        switch(parcours->at(i)){
                        case 31:
                        case 33:
                            if(sens == 1){
                                runDeviation(deviation1);
                                i = parcours->size()-1;
                            }
                            break;
                        case 19:
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
                        sleep(5);

                        while(!chef->isDispo(numeroTrain,parcours->at(i), parcours->at((i+1)%parcours->size()), sens)){
                            chef->attendreLaSection();
                        }

                        // il faut revenir d'un contact, car le train n'ayant pas avancer le contact suivant n'a pas encore été validé.
                        i--;
                        sleep(3);

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
    }

    void setDev(int deviationLength, QList<int>* deviation1){
        this->deviationLength = deviationLength;
        this->deviation1 = deviation1;
        this->deviation1_inv = inverse_qlist(deviation1);
    }

    QList<int>* inverse_qlist(QList<int>* list){
        QList<int>* qlist = new QList<int>();

        for(int i = list->length()-1; i >= 0; i--){
            qlist->append(list->at(i));
        }
        return qlist;
    }

    void stopTrain(){
        loco->arreter();
    }

    ~ChefTrain(){
        delete parcours;
        delete deviation1_inv;
    }
}; // fin ChefTrain

#endif // CHEFTRAIN_H
