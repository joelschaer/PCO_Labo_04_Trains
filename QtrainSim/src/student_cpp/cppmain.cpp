#include "ctrain_handler.h"
#include "locomotive.h"
#include "chefquai.h"
#include "cheftrain.h"

#include <QList>

//Creation d'une locomotive
static Locomotive locomotive;
static Locomotive locomotive2;

//Arret d'urgence
void emergency_stop()
{
    locomotive.arreter();
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
    parcours2 << 13 << 15 << 14 << 7 << 6 << 1 << 31 << 33 <<  32 << 25 << 24 << 19;

    //parcours de deviations
    QList<int> deviation1;
    deviation1 << 13 << 12 << 11 << 10 << 4 << 3 << 2 << 1;

    QList<int> deviation2;
    deviation2 << 31 << 30 << 29 << 28 << 22 << 21 << 20 << 19;

    //Initialisation des aiguillages
    diriger_aiguillage(8,  DEVIE,       0);
    diriger_aiguillage(2,  DEVIE,       0);
    diriger_aiguillage(20, DEVIE,       0);
    diriger_aiguillage(14, DEVIE,       0);
    diriger_aiguillage(11, TOUT_DROIT,  0);
    diriger_aiguillage(17, TOUT_DROIT,  0);
    diriger_aiguillage(23, TOUT_DROIT,  0);

    //Initialisation de la locomotive
    locomotive.fixerNumero(0);
    locomotive.fixerVitesse(17);
    locomotive.fixerPosition(16, 23);
    locomotive.allumerPhares();
    //locomotive.demarrer();
    locomotive.afficherMessage("Ready!");

    //Initialisation de la locomotive 2
    locomotive2.fixerNumero(1);
    locomotive2.fixerVitesse(14);
    locomotive2.fixerPosition(13, 19);
    locomotive2.allumerPhares();
    //locomotive2.demarrer();
    locomotive2.afficherMessage("Ready!");


    // il suffit de mettre la priorité des deux locomotives à 0 pour supprimer l'utilisation de la voie d'évitement.
    ChefQuai* chefQuai = new ChefQuai();
    ChefTrain* chefTrain1 = new ChefTrain(&locomotive, &parcours, chefQuai, 1, 0);
    ChefTrain* chefTrain2 = new ChefTrain(&locomotive2, &parcours2, chefQuai, 1, 1);
    chefTrain2->setDev(8, &deviation1, &deviation2);
    chefTrain1->start();
    chefTrain2->start();

    chefTrain1->wait();
    chefTrain2->wait();
    return EXIT_SUCCESS;
}


/*
//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_B);

    //Initialisation d'un parcours
    QList<int> parcours;
    parcours << 24 << 21 << 16 << 15 << 10 << 11 << 6 << 5;

    //Initialisation des aiguillages
    diriger_aiguillage(16, TOUT_DROIT,  0);
    diriger_aiguillage(15, DEVIE,       0);
    diriger_aiguillage(12, DEVIE,       0);
    diriger_aiguillage(11, DEVIE,       0);
    diriger_aiguillage(9,  TOUT_DROIT,  0);
    diriger_aiguillage(5,  TOUT_DROIT,  0);
    diriger_aiguillage(8,  DEVIE,       0);
    diriger_aiguillage(7,  TOUT_DROIT,  0);
    diriger_aiguillage(4,  TOUT_DROIT,  0);
    diriger_aiguillage(3,  TOUT_DROIT,  0);

    //Initialisation de la locomotive
    locomotive.fixerNumero(1);
    locomotive.fixerVitesse(12);
    locomotive.fixerPosition(24, 5);
    locomotive.allumerPhares();
    locomotive.demarrer();
    locomotive.afficherMessage("Ready!");

    //Attente du passage sur les contacts
    for (int i = 0; i < parcours.size(); i++) {
        attendre_contact(parcours.at(i));
        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                    .arg(locomotive.numero()).arg(parcours.at(i))));
        locomotive.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));
    }

    //Arreter la locomotive
    locomotive.arreter();
    locomotive.afficherMessage("Yeah, piece of cake!");

    //Fin de la simulation
    mettre_maquette_hors_service();

    //Exemple de commande
    afficher_message("Enter a command in the input field at the top of the window.");
    QString commande = getCommand();
    afficher_message(qPrintable(QString("Your command is: ") + commande));

    return EXIT_SUCCESS;
}
*/
