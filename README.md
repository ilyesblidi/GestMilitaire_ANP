Dr. KERMI Adel, MCA & HDR - SFSD - 2ème annee ----- Annee universitaire : 2023/2024 

Travail Pratique – SFSD : « Ficher de donnees non ordonne TObarreF avec un
Index dense en MC et un fichier index1 TOF » - Programmation en C
On suppose qu’on souhaite construire un fichier binaire de donnees nomme « PERSONNEL-ANP_DZ.dat » contenant le personnel actif de l’Armee Nationale Populaire des six regions militaires. Ce fichier sera organise selon la methode « TObarreF » : fichier vu comme Tableau, non Ordonne, avec des enregistrements de taille Fixe. Un bloc peut contenir au maximum 1024 enregistrements.

Travail demande ref1ref2
Programmez et utilisez le modèle vu en cours. Le modèle comprend les fonctions : LireDir, EcrireDir, Entete, Aff_Entete, Ouvrir, Fermer, et Alloc_Bloc.
Programmez un module de creation du fichier PERSONNEL-ANP_DZ.dat où chaque enregistrement est compose de 9 champs : < Matricule, Nom, Prenom, Date_Naissance, Wilaya_Naissance, Groupe_Sanguin, Grade, Force_Armee, Region_Militaire >. La creation de ce fichier de donnees s’appuiera sur un module de chargement initial qui consiste à remplir le fichier avec N enregistrements (N etant un entier à lire, par exemple : 100 000, 200 000, etc.).
Pour chaque nouvel enregistrement insere, sa cle et ses coordonnees (n° de bloc et deplacement) sont inserees dans une table d'index1 (en MC) avec decalages pour garder l'ordre des cles.

Pour que le chargement initial de PERSONNEL-ANP_DZ.dat** soit rapide, vous devrez le realiser avec des valeurs aleatoires comme suit :

Matricule (Cle de l’enregistrement) : Vous generez un nombre aleatoire entre 111111 et 999999.
Nom/Prenom: Dans un premier temps, vous generez un nombre entre 4 et 30 (ce nombre etant le nombre de caractères du nom/prenom). Ensuite, pour chaque caractère du nom/prenom vous generez un nombre entre 1 et 26 puis vous mettez dans ce caractère la lettre qui correspond.
Date_Naissance: Vous generez un nombre aleatoire entre 1942 et 2005 pour l’annee, un nombre aleatoire entre 1 et 12 pour le mois, et un nombre aleatoire entre 1 et le nombre de jours du mois genere (soit 28 ou 29 ou 30 ou 31) pour le jour. Vous devrez tenir compte de l’ensemble des contrôles sur la date (par exemple : annee bissextile ou non, etc.).
Wilaya_Naissance : Vous generez un nombre aleatoire entre 1 et 58. Ensuite, vous recuperez le libelle de la wilaya à partir de ce nombre (par exemple : 16 : Alger, 31 : Oran, …, etc.).
Groupe_Sanguin : Vous generez un nombre entre 1 et 8. Ensuite, vous recuperez le groupe sanguin à partir de ce nombre (1 : O+, 2 : A+, 3 : B+, 4 : O-, 5 : A-, 6 : AB+, 7 : B-, 8 : AB-).
Grade : Vous generez un nombre entre 1 et 19. Ensuite, vous recuperez le grade à partir de ce nombre (1 : General d'Armee, 2 : General de corps d’armee, 3 : General-Major, 4 : General, 5 : Colonel, 6 : Lieutenant-colonel, 7 : Commandant, 8 : Capitaine, 9 : Lieutenant, 10 : Sous-lieutenant, 11 : Aspirant, 12 : Adjudant-Major, 13 : Adjudant-Chef, 14 : Adjudant, 15 : Sergent-Chef, 16 : Sergent, 17 : Caporal-Chef, 18 : Caporal et 19 : Djoundi).
Force_Armee : Vous generez un nombre entre 1 et 8. Ensuite, vous recuperez la force armee à partir de ce nombre (1 : Armee de terre, 2 : Armee de l’air, 3 : Marine nationale, 4 : Defense aerienne du territoire, 5 : Gendarmerie nationale, 6 : Garde republicaine, 7 : Departement du renseignement et de la securite, 8 : Sante militaire).
Region_militaire : Vous generez un nombre entre 1 et 6. Ensuite, vous recuperez la region à partir de ce nombre (1 : 1RM-Blida, 2 : 2RM-Oran, 3 : 3RM-Bechar, 4 : 4RM-Ouargla, 5 : 5RM-Constantine, 6 : 6RM-Tamanrasset).
Programmez les modules enumeres ci-dessous (avec affichage du cout de chaque operation) : 
Chargement de l'index1 en MC à partir du fichier index1 TOF nomme « MATRICULE_INDEX.idx ». 
Sauvegarde de l'index1 dans MATRICULE_INDEX.idx.
Recherche d’un militaire par son matricule.
Insertion d’un nouvel enregistrement au fichier PERSONNEL-ANP_DZ.dat
Suppression d’un enregistrement donne par le matricule.
Modification de la region militaire d’un enregistrement donne.
affichage ou consultation de tous militaires dont les matricules appartiennent à l’intervalle [M1, M2].
Programmez les modules facultatifs enumeres ci-dessous. Afin d’accelerer les traitements, vous pouvez utiliser d’autres tables d’Index adequates en MC et des fichiers de sauvegarde de ces tables.
Suppression de tous les enregistrements relatifs à une force armee donnee.
Consultation ou affichage de tous les enregistrements relatifs à region militaire donnee ayant un âge entre deux valeurs donnees (Requête à intervalle d’âges).
affichage de tous les enregistrements appartenant à une categorie de grades donnee (Officiers- generaux, officiers-superieurs, officiers, sous-officiers, et hommes de troupes).
Fragmentation de PERSONNEL-ANP_DZ.dat en six fichiers (F1, F2, F3, F4, F5 et F6) contenant chacun les enregistrements relatifs à une region militaire donne.
Points importants : ref1ref2
Tous les modules demandes doivent être executes via un menu principal.
Vous devez me remettre vos codes sources en les televersant sur mon Drive via un formulaire en ligne qui vous sera communique prochainement. Le dernier delai de remise du TP est fixe pour le Lundi 18 decembre 2023 avant 23h59. Aucun retard n’est tolere. Si vous depassiez ce delai, vous seriez sanctionnes.
Vous devez respecter le format suivant pour l’identificateur de vos fichiers : NOM1_NOM2_TP2_Gi.zip où i represente de numero de groupe (fichier compresse contenant vos codes sources, vos fichiers de donnees et fichiers d’index1 utilises). Si vous ne respectiez pas ce format, vous seriez sanctionnes.
Plusieurs aspects seront pris en consideration lors de l’evaluation de vos TP (la demonstration, les codes sources (les structures, les commentaires, etc.), l’interface homme-machine (IHM), etc.).**
Bon courage ! بالـتوفيـــق
2/2 
