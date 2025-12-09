# robotique
Ce projet consiste à développer un système intelligent de planification de trajectoire pour un robot mobile. L’objectif principal est de permettre au robot de gérer ses mouvements tout en réagissant rapidement aux situations d’urgence, notamment en cas d’obstacle.
Structure des données:
 
Structure commande :

	mvt : Type de mouvement (1-4 : AVANCER/RECULER/GAUCHE/DROITE, 0 : URGENCE)

	vitesse : Vitesse en km/h (8-48 pour mouvements normaux)

	durée : Durée d'exécution en secondes

	urg : Indicateur booléen d'urgence

	File (FIFO) :

	File des commandes normales

	File des commandes urgentes

	Implémentation par liste chaînée simple

	Pile (LIFO) :

	Stocke les commandes de contournement générées lors d'urgences

Arbre Binaire de Recherche :

	assure un enchainement trié des vitesses des commandes normales
	
	Permet une extraction rapide de la vitesse maximale

Fonctionnalité principale:

*Gestion double-file : commandes normales + commandes urgentes

	Le système implémente deux files distinctes :

	une file pour les commandes normales

	une file pour les commandes urgentes

	Le robot traite toujours en priorité les commandes urgentes.

		Fonctions clés :

			ajouter_commande_normale()

			ajouter_commande_urgente()

			prendre_prochaine_commande()

			enfiler() / defiler()

*Détection d’obstacles via capteurs simulés

	Un capteur virtuel génère aléatoirement un événement d’obstacle.

	Si pas d’obstacle → ajout automatique d’une commande normale.

	Si obstacle détecté → ajout immédiat d’une commande ARRET_URGENCE.

		Fonctions clés :

			capteur()

			check()

*Génération automatique de commandes de contournement

	sauvegarde les commandes normales interrompues par une urgence, permettant leur reprise ultérieure

		Fonctions clés :

			commande_contournement()

			empiler() / depiler()

*Exécuteur intelligent de commandes

	Le module d’exécution :

	traite d’abord les urgences,

	puis les commandes normales,

	affiche chaque action avec délai simulé,

	met à jour les statistiques du robot.

		Fonction clé :

			executer()

*Journalisation complète des actions dans un fichier

	Toutes les commandes exécutées (urgentes + normales) sont enregistrées dans un fichier texte :

	journal_robot.txt

	Cela inclut :

	horodatage,

	type de commande,

	durée,

	vitesse,

	nature (URGENTE / NORMALE).

		Fonction clé :

			fprintf() dans executer()

*Représentation graphique des mouvements

	Le programme construit une traînée de symboles ASCII représentant les mouvements :

	> : avancer

	< : reculer

	^ : tourner gauche

	v : tourner droite

	x : arrêt d’urgence

	Affichée à la fin si l’utilisateur le demande.

*Statistiques de performance du robot

	À la fin de l’exécution, le système calcule :

	vitesse moyenne,

	vitesse maximale,

	distance totale parcourue,

	durée totale d’activité.

	Ces valeurs sont calculées à partir :

	de la durée cumulée,

	de la vitesse de chaque commande,

	d’un arbre binaire de recherche permettant de récupérer la vitesse maximale.

		Fonctions clés :

			insert()

			max_vitesse()

*Résumé très court (pour diapositives)

	Double file : commandes normales + urgentes

	Capteur virtuel → détection d’obstacle

	Génération automatique de contournement

	Exécuteur prioritaire (urgence > normal)

	Journalisation dans fichier texte

	Statistiques complètes (vitesse, distance, durée)

	Représentation graphique des mouvements

	Utilisation de piles, files, et arbres binaires
