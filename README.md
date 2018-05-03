# PCO_Labo_04_Trains

## Objectifs
Réaliser un prgramme concurrent permettant de faire circuler deux trains sur des parcours prédéfinis. Les trains doivent se partager un bout du parcours, Cette zone est alors une zone critique.

Ajouter au système une gestion de priorité permettant de définir un train plus prioritaire qu'un autre ou les deux au même niveau.

## description de l’implémentation

Nous avons choisi d'utiliser la maquette A.

Nous utilisons trois niveau de priorités:
- la première définis la locomotive interne en tant que prioritaire. La locomotive externe restera alors sur ça voie d'évitement.
- la deuxième définis la locomitve externe comme prioritaire. La locomotive interne d'arrête donc en entrée de section critique et reste stoppé indéfiniment.
- la troisième définis les deux locomotive au même niveau de priorité. La première faisant la demande d'accès a la section critique (tronçon en commun) peut y accéder. Si la locomitive externe y accède, la locomotive interne s'arrête. Si la locomotive interne y accède, la locomotive externe utilise la voie d'évitement.

Afin de gérer la concurrence des trains nous avons ajouté deux classes.
- ChefQuai : cette classe joue le rôle de controlleur pour une locomotive.
- ChefTrain : cette classe joue le rôle de controlleur pour notre section critique et la "discution" entre nos chef de train pour savoir la disponibilité des parcours.

##### cppmain
Dans notre fichier `main` nous crééons nos deux locomotives, fixons leurs valeurs ainsi que leurs parcours.
Nous créons un ChefQuai.
Nous créons deux `ChefTrain` et nous leurs assignons une locomotive, un parcours, leur `ChefQuai` puis une valeurs correspondant a un id.
```
chefTrain1 = new ChefTrain(&locomotive, &parcours, chefQuai, TRAIN_1);

chefTrain2 = new ChefTrain(&locomotive2, &parcours2, chefQuai, TRAIN_2);
```
On définis ensuite la priorité des train, Deux valeurs possible `PRIO` ou `PAS_PRIO` correspondant a respectivement 1 ou 0. Pour définir cela on utilise la méthode du `ChefQuai` `setPrioTrain` prenant en paramètre la priorité du premier train puis celle du deuxième.

On définis le parcours de déviation du train qui en a besoin, cela grâce à la méthode `setDev` de notre `ChefTrain`.

Le reste du code permet l'utilisation et l'envois des commande.
- stop : permet d'effectuer un arret des locomotives.
- 1 : définis la locomotive 1 (ID) en tant que prioritaire.
- 2 : définis la locomotive 2 (ID) en tant que prioritaire.
- 3 : définis les deux locomotive au même niveau de priorité.

##### ChefQuai

##### ChefTrain

### Choix d'implémentation principaux

### Etapes

Les étapes de développement on été les suivantes :
- Reflexion sur le modèle a utiliser (classes, communication entre les locomotives, comment gérer les sections critiques etc...)
- Implémentation des classes nécéssaires à notre implémentation.
- Première phase de test sur simulation du fonctionnement des deux locomotives selon la (les) section(s) critique(s).
- Ajout de la gestion des priorités.
- Deuxième phase de test sur simulation de la gestion des priorités.
- Première phase de test sur maquette.
- Simplification des sections critiques (au départ deux sections, réduit à une), ajout de la gestion du temps de changement des aigullage ainsi que le temps d'arrêt.
- Deuxième phase de test sur maquette.

## Tests en simulation

Scénarios de tests :
- Test du fonctionnement d'une seul locomotive. Cela nous a permis de nous familiariser avec l'application.
- Test du fonctionnement de deux locomotive se partagants une ou plusieurs sections critiques. Cela nous a permis de vérifier que nous avons correctement géré les sections critiques afin d'éviter des accidents.
- Test du fonctionnement des priorités, Cela nous a permis de vérifier notre implémentation des priorités et de partage de la section critiques selon les droits d'accès, ainsi que l'utilisation de la voie d'évitement.
- Tests multiples de fiabilité, pour cela nous avons "joué" avec la vitesse des locomotives afin de s'assurer que le fonctionnement n'est pas du a un coup de chance.
- Tests de changements de priorité durant le fonctionnement des locomotives alors que celle-ci se trouve en dedans ou en dehors d'une section critique. Cela nous a permis de vérifier que le changement de priorité et "indépendant" du fonctionnement des locomotives et cela n'impacte pas leur comportement.
- Test sur la durée, nous avons lancé la simulation et l'avons laissé tourné plusieurs longues minutes avec des vitesses de locomotives assez différentes et plusieurs priorité afin de s'assurer que aucun cas "exceptionnel" pose encore problème.

Problèmes rencontrés :
- Pour la partie simulation nous n'avons pas rencontrés beaucoup de problèmes, les locomotive ont rapidement fonctionné correctement. Nous avons tout de même découvert certains cas problèmatique ou une locomotive peut déclanger un aiguillage alors que l'autre se trouve dessus, cela faisait alors dérailler cette dernière. Problème qui a été résolus.

## Tests sur maquette

La plupart des tests effectués sur simulation on été refait tel-quel sur la maquette.
Scénarios de tests supplémentaires :
- Test de changement de vitesse sur les locomotive afin de connaitre les vitesses maximales laissant encore le temps à notre implémentation de gérer le changement d'aiguillage et de temps de freinage.

Problèmes rencontrés :
- Difficulté a anticiper le temps de changement des aiguillages (nous avons du ajouter un capteur d'avance pour la detection des changements d'aiguillage).
- Difficulté a faire fonctionner les locomotive. Certaines locomotives ne voulais pas changer de sens comme désiré et continuait dans leur sens initial (problème inexistant sur simulation). Nous avons essayé de gérer cela avec des "sleep" permettant a la locomotive de s'arrêter entièrement avant de changer de sens.
- Difficulté a corriger les erreurs innatendues. Plusieurs fois durant les tests sur maquettes nous avons rencontré des erreurs dû au maquettes disfonctionnels, problème du circuit ou tout simplement problème dans notre code que nous n'arrivions pas a reproduire dans notre simulation.