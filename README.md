# PCO_Labo_04_Trains

## Objectifs
Réaliser un prgramme concurrent permettant de faire circuler deux trains sur des parcours prédéfinis. Les trains doivent se partager un bout du parcours, Cette zone est alors une zone critique.

Ajouter au système une gestion de priorité permettant de définir un train plus prioritaire qu'un autre ou les deux au même niveau.

## description de l’implémentation

Nous avons choisi d'utiliser la maquette A.

Nous utilisons trois niveau de priorités:
- la première définis la locomotive interne en tant que prioritaire. La locomotive externe restera alors sur ça voie d'évitement.
- la deuxième définis la locomitve externe comme prioritaire. La locomotive interne d'arrête donc en entrée de section critique et reste stoppé indéfiniment.
- la troisième définis les deux locomotive au même niveau de priorité. Dans ce mode la première locomotife faisant la demande d'accès à la section critique (tronçon en commun) peut y accéder. Si la locomitive externe y accède, la locomotive interne s'arrête. Si la locomotive interne y accède, la locomotive externe utilise la voie d'évitement.

Afin de gérer la concurrence des trains nous avons ajouté deux classes.
- ChefTrain : cette classe joue le rôle de controlleur pour une locomotive.
- ChefQuai : cette classe joue le rôle de controlleur pour notre section critique et la "discution" entre nos chef de train pour savoir la disponibilité des parcours.

#### cppmain
Nous contrôlons le réglage de tous les aiguillages qui restent fixe durant l'exécution.
Dans notre fichier `main` nous crééons nos deux locomotives, fixons leurs valeurs ainsi que leurs parcours.
Nous créons un ChefQuai.
Nous créons deux `ChefTrain` et nous leurs assignons une locomotive, un parcours, le `ChefQuai` puis une valeurs correspondant a un id de train pour les distinguer dans notre code.
```
chefTrain1 = new ChefTrain(&locomotive, &parcours, chefQuai, TRAIN_1);

chefTrain2 = new ChefTrain(&locomotive2, &parcours2, chefQuai, TRAIN_2);
```
On définis ensuite la priorité des trains. Deux valeurs possible `PRIO` ou `PAS_PRIO` correspondant a respectivement 1 ou 0. Pour définir cela on utilise la méthode du `ChefQuai` `setPrioTrain` prenant en paramètre la priorité du premier train puis celle du deuxième.

On définis le parcours de déviation du train qui en a besoin, cela grâce à la méthode `setDev` de notre `ChefTrain`.

Le reste du code permet l'utilisation et l'envois des commande.
- stop : permet d'effectuer un arret des locomotives.
- 1 : définis la locomotive 1 (ID) en tant que prioritaire.
- 2 : définis la locomotive 2 (ID) en tant que prioritaire.
- 3 : définis les deux locomotive au même niveau de priorité.

#### ChefQuai
C'est dans cette méthode que nous gérons l'accès à la séction critique. Elle est responsable de gérer tout ce qui à trait à la maquette. Les aiguillages, les sections de railles.

Les chefs de trains demandent au chef de quai avant d'accéder à chaque nouveau tronçon de voie, si celui-ci est libre d'accès ou non sans se préocupé si celui-ci fait partie ou non de la section critique. Le chef de quai à connaissance de l'emplacement du tronçon commun et de ça disponnibilité. Il peut alors indiquer au chef de train s'il est autoriser à continuer sa route ou non.

Ces autorisations sont gérées par une fonction du chef de quai `isDispo` qui retourne true ou false selon plusieurs critère qui sont: la disponnibilité de la section, le train qui est dans la section (si un train est dans la section on le laisse continuer sa route), la priorité du train (un train non prioritaire ne peut pas continuer sa route.)

Il est résonsable de libérer la section critique quand un train en sort et d'indiquer au potentiel train en attente qu'il peut à nouveau continuer sa route.

#### ChefTrain

Le chef de train est en charge de demander au chef de quai s'il est autorisé à continuer sa route. Si ce n'est pas le cas à pour rôle de prendre de disposition afin que son train ne se retrouve pas sur la même voie qu'un autre.

Ses attributs sont :
- une locomotive
- un parcours a suivre
- un parcours de déviation (optionnel (uniquement le train 2))
- le sens actuel de marche

Le thread du chef de train est différent pour chaque loco. Il run une boucle qui avance sur le parcours qui lui est fourni et pour chaque nouveau tronçon appel la fonction `isDispo` du chef de quai.
Si celui-ci (`ChefQuai`):
- donne le feu vert (la section n'est pas une section critique, aucun train n'est dans la section et il dispose d'une priorité suffisante pour avancer), le chef de train demande au chef de quai de régler les aiguillages pour l'accès à ce tronçon et avance.
- sinon il indique que le tronçon n'est pas disponnible ( la section critique est déjà occupée ou le train n'a pas la priorité et donc je peux pas y accéder). Le chef de train prend alors des dispositions différentes:
	- S'il dispose d'un tronçon de déviation il engage la procédure pour empunter cet autre chemin. Cela consiste à demander au chef de quai de régler les aiguillages pour emprunter cet autre chemin et continuer sa route. Une fois l'évitement effectué il reprend sur le toncon normal.
	- S'il ne dipose pas d'un tronçon de déviation, il s'arrête et indique au chef de quai qu'il s'est arrêté. Celui-ci attend que l'autre sorte de la section critique pour lui indiquer qu'il peut continuer.

#### fonctionnement du system de priorité

La priorité est gérée par le chef de quai. Il donne le feu vert au train pour continuer la route s'il s'approche de la section critique, seulement s'il est en droit d'y accéder. Le cas contraire il interdit l'accès. Et le chef de train prend lui même les dipositions en cas de refus de la section critique.

Les threads qui gère les trains n'ont donc jamais besoin de se préocuper des priorités.

### Choix d'implémentation principaux
Nous avons choisi de séparer la gestion du circuit en deux entités. Les chefs de trains a raison d'un chef par train et le chef de quai qui fait office de point de contact unique. Les chefs de trains doivent donc se référer à cette entité commune pour chaqune des action qui n'est pas propore à la locomotive. De cette manière le point de contact entre les trains est partagé et il est possible de gérer les interractions sans que celle-ci n'interfèrent entre elles.
Pour faire une gestion de concurence une entité commune est indispensable et il nous parrait être le plus simple de faire cette gestion par une entité indépendante plutôt que de l'intégrer avec des ressources partages entre les objets de chef de trains.

Afin de prévoir le changement des aiguillages sur les maquettes nous avons anticipé la détéction à un contact d'avance, au lieu de tester le contact après l'aiguillage on test le contact avant l'aiguillage.

Afin de pouvoir accéder à la section critique (tronçon en commun) chaque Chef de train essaie de réserver celle-ci. Cette réservation se fait au contact `i` (prochain contact de la section critique) puis elle se fait au contact `i+1` (contact suivant dans la section critique). Lorsque le test est effectué sur un point i, la disponibilité de la section critique est alors donnée au Chef de train, on test une deuxième fois avec `i+1` afin de prévoir les cas (changement de sens etc...) ou le Chef de train n'aurait pas eu le temps de tester le contact à `i`. De ce fait la section critique est réservée assez tôt avant d'y entrer afin d'éviter que l'autre locomotive lui passe devant.

### Etapes

Les étapes de développement on été les suivantes :
- Reflexion sur le modèle a utiliser (classes, communication entre les locomotives, comment gérer les sections critiques etc...), cette réxion nous à menée au choix d'implémentation sur le modèle (`ChefTrain`, `ChefQuai`)
- Implémentation des classes nécéssaires à notre structure.
- Première phase de test sur simulation du fonctionnement des deux locomotives selon la (les) section(s) critique(s).
- Ajout de la gestion des priorités de manière dynamique.
- Deuxième phase de test sur simulation de la gestion des priorités. Nous avons alors constaté que lors du changement des priorité dans certains cas de figure le système était mis dans une situations critique si ne pouvais pas être géré sur les maquettes.
- Première phase de test sur maquette.Nous avons alors remarqué que tout notre système réagissait beaucoup trop tard par rapport à la vitesse de déplacement des trains et la réaction des aiguillages.
- Simplification des sections critiques (au départ deux sections, réduit à une), ajout de la gestion du temps de changement des aigullage ainsi que le temps d'arrêt. Pratiquement tout le code à été réadapté
- Deuxième phase de test sur maquette. Beaucoup moins de problèmes constaté, mais il reste certains problèmes qui surviennent dans certains cas de figure incompréhensible.
- Nous faisons alors plusieurs tests afin de simplifier encore notre code et découvrir certaines petites incohérence du code qui n'étaient jamais apparu comme erreur sur simulateur.
- Dernière phase de test sur maquette. Le code fonctionne correctement et nous n'avons pas constaté de problème quand à la réaction ou le fonctionnement. Il reste cependant le problème lié au maquette qui sont quelquefois capricieuses et un aiguillage qui ne réagit pas quand il devrait conduit le train à sortir du chemain prévu.

## Tests en simulation

Scénarios de tests :
- Test du fonctionnement d'une seul locomotive. Cela nous a permis de nous familiariser avec l'application.
- Test du fonctionnement de deux locomotive se partagants une ou plusieurs sections critiques. Cela nous a permis de vérifier que nous avons correctement géré les sections critiques afin d'éviter des accidents.
- Test du fonctionnement des priorités, Cela nous a permis de vérifier notre implémentation des priorités et de partage de la section critiques selon les droits d'accès, ainsi que l'utilisation de la voie d'évitement.
- Tests multiples de fiabilité, pour cela nous avons "joué" avec la vitesse des locomotives afin de s'assurer que le fonctionnement n'est pas du a un coup de chance.
- Tests de changements de priorité durant le fonctionnement des locomotives alors que celle-ci se trouve en dedans ou en dehors d'une section critique. Cela nous a permis de vérifier que le changement de priorité et "indépendant" du fonctionnement des locomotives et cela n'impacte pas leur comportement.
- Test sur la durée, nous avons lancé la simulation et l'avons laissé tourné plusieurs longues minutes avec des vitesses de locomotives assez différentes et plusieurs priorité afin de s'assurer que aucun cas "exceptionnel" pose encore problème.

Problèmes rencontrés :
- Pour la partie simulation nous n'avons pas rencontrés beaucoup de problèmes, les locomotive ont rapidement fonctionné correctement. Nous avons tout de même découvert certains cas problèmatique ou une locomotive peut déclanger un aiguillage alors que l'autre se trouve dessus, cela faisait alors dérailler cette dernière. Problème qui a été résolus en gérant différemment nos section critique afin d'assurer que ces cas de figures ne puissent pas survenir. Les locomotives doivent être largement sortie des parcours qui pourraient être commun avant d'actionner les aiguillages.

## Tests sur maquette

La plupart des tests effectués sur simulation on été refait tel-quel sur la maquette.
Scénarios de tests supplémentaires :
- Test de changement de vitesse sur les locomotive afin de connaitre les vitesses maximales laissant encore le temps à notre implémentation de gérer le changement d'aiguillage et de temps de freinage.

Problèmes rencontrés :
- Difficulté a anticiper le temps de changement des aiguillages (nous avons du ajouter un capteur d'avance pour la detection des changements d'aiguillage).
- Difficulté a faire fonctionner les locomotive. Certaines locomotives ne voulais pas changer de sens comme désiré et continuait dans leur sens initial (problème inexistant sur simulation). Nous avons essayé de gérer cela avec des "sleep" permettant a la locomotive de s'arrêter entièrement avant de changer de sens.
- Difficulté a corriger les erreurs innatendues. Plusieurs fois durant les tests sur maquettes nous avons rencontré des erreurs dû au maquettes disfonctionnels, problème du circuit ou tout simplement problème dans notre code que nous n'arrivions pas a reproduire dans notre simulation.