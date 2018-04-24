#include "ctrain_handler.h"
#include "locomotive.h"
#include "chefquai.h"
#include "cheftrain.h"

#include <QList>

//Creation d'une locomotive
static Locomotive locomotive;
static Locomotive locomotive2;

static ChefTrain* chefTrain1;
static ChefTrain* chefTrain2;



#define TRAIN_1 1
#define TRAIN_2 2

#define PRIO 1
#define PAS_PRIO 0

//Arret d'urgence
void emergency_stop()
{
    chefTrain1->terminate();
    chefTrain2->terminate();

    locomotive.arreter();
    locomotive2.arreter();
    afficher_message("\nSTOP!");
}


//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_A);

    //Initialisation d'un parcours
    QList<int> parcours;
    parcours << 16 << 15 << 14 << 7 << 6 << 5 << 34 << 33 << 32 << 25 << 24 << 23;

    //Initialisation d'un parcours Loco 2
    QList<int> parcours2;
    parcours2 << 13 << 12 << 11 << 10 << 4 << 3 << 2 << 1 << 31 << 33 <<  32 << 25 << 24 << 19;

    QList<int> deviation;
    deviation << 31 << 30 << 29 << 28 << 22 << 21 << 20 << 19;

    //Initialisation des aiguillages
    diriger_aiguillage(1,  TOUT_DROIT, 0);
    diriger_aiguillage(2,  DEVIE, 0);
    diriger_aiguillage(3,  DEVIE, 0);
    diriger_aiguillage(4,  TOUT_DROIT, 0);
    diriger_aiguillage(5,  TOUT_DROIT, 0);
    diriger_aiguillage(6,  TOUT_DROIT, 0);
    diriger_aiguillage(7,  TOUT_DROIT, 0);
    diriger_aiguillage(8,  DEVIE, 0);
    diriger_aiguillage(9,  DEVIE, 0);
    diriger_aiguillage(10,  TOUT_DROIT, 0);
    diriger_aiguillage(11,  TOUT_DROIT, 0);
    diriger_aiguillage(15,  DEVIE, 0);
    diriger_aiguillage(16,  TOUT_DROIT, 0);
    diriger_aiguillage(17,  TOUT_DROIT, 0);
    diriger_aiguillage(19,  TOUT_DROIT, 0);
    diriger_aiguillage(20,  DEVIE, 0);
    diriger_aiguillage(23,  TOUT_DROIT, 0);

    //Initialisation de la locomotive
    locomotive.fixerNumero(0);
    locomotive.fixerVitesse(17);
    locomotive.fixerPosition(16, 23);
    locomotive.allumerPhares();
    locomotive.afficherMessage("Ready!");

    //Initialisation de la locomotive 2
    locomotive2.fixerNumero(1);
    locomotive2.fixerVitesse(14);
    locomotive2.fixerPosition(13, 19);
    locomotive2.allumerPhares();
    locomotive2.afficherMessage("Ready!");


    // il suffit de mettre la priorité des deux locomotives à 0 pour supprimer l'utilisation de la voie d'évitement.
    ChefQuai* chefQuai = new ChefQuai();

    chefTrain1 = new ChefTrain(&locomotive, &parcours, chefQuai, TRAIN_1, PRIO);
    chefTrain2 = new ChefTrain(&locomotive2, &parcours2, chefQuai, TRAIN_2, PRIO);
    chefQuai->setPrioTrain(PRIO,PRIO);

    chefTrain2->setDev(8, &deviation);
    chefTrain1->start();
    chefTrain2->start();

    afficher_message("commands are : 1 = train 1 prio , 2 = train 2 prio, 3 = no priority");

    while(true){

        QString command = getCommand();
        afficher_message(qPrintable(QString("Your command is: ") + command));

        if(command.compare("stop") == 0){
            emergency_stop();
            break;
        }
        if(command.compare("1") == 0){
            chefQuai->setPrioTrain(PRIO, PAS_PRIO);
            chefTrain1->setPriorite(PRIO);
            chefTrain2->setPriorite(PAS_PRIO);

        }
        if(command.compare("2") == 0){
            chefQuai->setPrioTrain(PAS_PRIO, PRIO);
            chefTrain1->setPriorite(PAS_PRIO);
            chefTrain2->setPriorite(PRIO);
        }
        if(command.compare("3") == 0){
            chefQuai->setPrioTrain(PRIO, PRIO);
            chefTrain1->setPriorite(PRIO);
            chefTrain2->setPriorite(PRIO);
        }
    }

    chefTrain1->wait();
    chefTrain2->wait();

    //Fin de la simulation
    mettre_maquette_hors_service();

    return EXIT_SUCCESS;
}
