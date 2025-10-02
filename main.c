#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#define MAX_CLIENTS 100
#define MAX_PRODUITS 400
#define MAX_LINES 400

// Structure représentant un client
typedef struct{
    char *idClient ;
    char *nom ;
    char *prenom ;
    char *email ;
    float solde ;
    char  *dateInscription ;
    char *nomUtilisateur ;
    char *password ;
}Client ;

// Structure représentant un produit
typedef struct{
    char *idProduit ;
    char *nom ;
    char *categorie ;
    float prix ;
    int stock ;
    char *description ;
}Produit;

// Structure représentant une ligne de commande pour un produit acheté par un client
typedef struct{
    char *noClient;
    char *noProduit;
    int qte;
    float prixUnitaire;
    char *dateAchat;
}LigneCommande;

// Pointeur global pour stocker l'ID du client connecté
char *clientConnecte = NULL;

//Prototypes des fonctions

void menuPrincipal(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                   LigneCommande *lignesCommande , int *nbLignesCommande);

void menuProfil(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                   LigneCommande *lignesCommande , int *nbLignesCommande);

void menuProduit(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                   LigneCommande *lignesCommande , int *nbLignesCommande);

void menuSolde(Produit *produits, int *nbProduits ,Client *clients , int *nbClients ,
                   LigneCommande *lignesCommande , int *nbLignesCommande);

void afficherCatalogue(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                   LigneCommande *lignesCommande , int *nbLignesCommande);

// Génère un identifiant unique pour un client

void genererIdClient(char *idClient , int compteur){

    sprintf(idClient , "C%03d" , compteur);
}

// Génère un identifiant unique pour un produit

void genererIdProduit(char *idProduit , int compteur){

    sprintf(idProduit , "P%03d" , compteur);
}

//Supprime tous les espaces d'une chaîne et retourne une nouvelle chaîne sans espaces

char *supprimerEspaces(char *text){
    int i , j=0;
    char *temp = malloc(strlen(text)+1);
    for(i=0 ; text[i]!='\0' ; i++){
        if(text[i] != ' '){
            temp[j++] = text[i];
        }
    }
    temp[j] = '\0';
    return temp;
}

//Vérifie si une chaine de caractère est composée uniquement de lettres alphabétiques

int estChaine(char *text){
    int i;
    for(i=0 ; text[i] != '\0' ; i++){
        if(!(text[i]>='a' && text[i]<='z') && !(text[i]>='A' && text[i]<='Z') && !(text[i]==' '))
            return 0;
    }
    return 1;
}

// Génère automatiquement l'email du client à partir du nom et prénom

void genererEmail(char *nom , char *prenom , char *email){
    char *p = supprimerEspaces(prenom);
    char *n = supprimerEspaces(nom);

    sprintf(email , "%s.%s@email.com" , p,n);

    free(p);
    free(n);
}

// Génère la date actuelle au format JJ/MM/AAAA

void genererDate(char *date){
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    sprintf(date , "%02d/%02d/%04d" , tm->tm_mday,tm->tm_mon+1,tm->tm_year+1900);
}

// Vérifie si le nom d'utilisateur est unique parmi tous les clients

int estNomUtilisateurUnique(Client *clients , int nbClients , char *nomUtilisateur){
    int i;
    for(i=0 ; i<nbClients ; i++){
        if(strcmp((clients+i)->nomUtilisateur , nomUtilisateur) == 0)
            return 0;
    }
    return 1;
}

// Vérifie si le mot de passe est unique parmi tous les clients

int estPasswordUnique(Client *clients , int nbClients , char *password){
    int i;
    for(i=0 ; i<nbClients ; i++){
        if(strcmp((clients+i)->password , password) == 0)
            return 0;
    }
    return 1;
}

// Alloue dynamiquement la mémoire nécessaire pour les champs d'un ligne commande

void alloueLigneCommande(LigneCommande *ligneCommande){
    ligneCommande->noClient = malloc(10);
    ligneCommande->noProduit = malloc(10);
    ligneCommande->dateAchat = malloc(15);
}

// Libère la mémoire allouée dynamiquement pour les champs d'un ligne commande

void viderLigneCommande(LigneCommande *ligneCommande){

    if(ligneCommande->noClient) free(ligneCommande->noClient);
    if(ligneCommande->noProduit) free(ligneCommande->noProduit);
    if(ligneCommande->dateAchat) free(ligneCommande->dateAchat);
}

// Alloue dynamiquement la mémoire nécessaire pour les champs d'un produit

void alloueProduit(Produit *produit){
    produit->idProduit = malloc(50);
    produit->nom  = malloc(50);
    produit->categorie  = malloc(50);
    produit->description  = malloc(100);
}

// Libère la mémoire allouée dynamiquement pour les champs d'un produit

void viderProduit(Produit *produit){

    if(produit->idProduit) free(produit->idProduit);
    if(produit->nom) free(produit->nom);
    if(produit->categorie  ) free(produit->categorie  );
    if(produit->description) free(produit->description);
}

// Alloue dynamiquement la mémoire nécessaire pour les champs d'un client

void alloueClient(Client *client){
    client->idClient = malloc(10);
    client->nom = malloc(50);
    client->prenom = malloc(50);
    client->email = malloc(50);
    client->dateInscription = malloc(20);
    client->nomUtilisateur  = malloc(50);
    client->password = malloc(50);
}

//Libère la mémoire allouée dynamiquement pour les champs d'un client

void viderClient(Client *client){
    if(client->idClient) free(client->idClient);
    if(client->nom) free(client->nom);
    if(client->prenom) free(client->prenom);
    if(client->email) free(client->email);
    if(client->dateInscription) free(client->dateInscription);
    if(client->nomUtilisateur) free(client->nomUtilisateur);
    if(client->password) free(client->password);
}

// Copie toutes les informations d'un produit p2 vers un produit p1

void copieProduit(Produit *p1 , Produit *p2){
    strcpy(p1->idProduit , p2->idProduit);
    strcpy(p1->nom , p2->nom);
    p1->prix = p2->prix;
    p1->stock = p2->stock;
    strcpy(p1->description , p2->description);
}

// Crée un nouveau profil client et l'ajoute à la liste

void creerProfil(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                 LigneCommande *lignesCommande , int *nbLignesCommande){

    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          Creation de profil                            ");
    gotoxy(25,5); cprintf("                                                                        ");

    gotoxy(25,27); cprintf("                                                                        ");
    gotoxy(25,28); cprintf("                                                                        ");
    gotoxy(25,29); cprintf("                                                                        ");

    gotoxy(35,7); cprintf("Creer profil");
    gotoxy(35,10);  printf("Prenom               :");
    gotoxy(35,12);  printf("Nom                  :");
    gotoxy(35,14);  printf("Nom d'utilisateur    :");
    gotoxy(35,16);  printf("Mot de passe         :");

    if(*nbClients >= MAX_CLIENTS){
        textbackground(4);
        gotoxy(35,25);   cprintf("La table est pleine !");

        getch();
        menuProfil(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
    }

    alloueClient(clients+*nbClients);

    do{
        gotoxy(60,10);  printf("                                       ");
        gotoxy(60,10); gets((clients+*nbClients)->prenom);

        if(!estChaine((clients+*nbClients)->prenom)){
           gotoxy(60,10);  printf("Prenom invalide !");
           getch();
        }
    }while(!estChaine((clients+*nbClients)->prenom));

    do{
        gotoxy(60,12);  printf("                                       ");
        gotoxy(60,12); gets((clients+*nbClients)->nom);

        if(!estChaine((clients+*nbClients)->nom)){
           gotoxy(60,12);  printf("Nom invalide !");
           getch();
        }
    }while(!estChaine((clients+*nbClients)->nom));

    do{
        gotoxy(60,14);  printf("                                       ");
        gotoxy(60,14); gets((clients+*nbClients)->nomUtilisateur);
        if(!estNomUtilisateurUnique(clients , *nbClients , (clients+*nbClients)->nomUtilisateur)){
           gotoxy(60,14);  printf("Ce username est deja pris!");
           getch();
        }
    }while(!estNomUtilisateurUnique(clients , *nbClients , (clients+*nbClients)->nomUtilisateur));

    do{
        gotoxy(60,16);  printf("                                        ");
        gotoxy(60,16); gets((clients+*nbClients)->password);
        if(!estPasswordUnique(clients , *nbClients , (clients+*nbClients)->password)){
           gotoxy(60,16);  printf("Ce password est deja pris!");
           getch();
        }
    }while(!estPasswordUnique(clients , *nbClients , (clients+*nbClients)->password));

    (clients+*nbClients)->solde = 0.0;

    genererEmail((clients+*nbClients)->nom , (clients+*nbClients)->prenom
                  , (clients+*nbClients)->email);

    genererDate((clients+*nbClients)->dateInscription);
    genererIdClient((clients+*nbClients)->idClient, *nbClients+1);

    (*nbClients)++;

    textbackground(2);
    gotoxy(40,20);   cprintf("Le compte creer avec succes !");

    getch();
    menuProfil(produits , nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
}

// Vérifie les informations de connexion et retourne l'ID du client si correct

char *verifierConnexion(Client *clients , int nbClients , char *nomUtilisateur , char *password){
    int i;
    for(i=0 ; i<nbClients ; i++){
        if(strcmp((clients+i)->nomUtilisateur , nomUtilisateur ) == 0 &&
           strcmp((clients+i)->password , password ) ==0 )

            return (clients+i)->idClient;

    }
    return NULL;
}

// Permet à un client de se connecter

void login(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
           LigneCommande *lignesCommande , int *nbLignesCommande){

    char nomUtilisateur[50] , password[50];
    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          Se connecter                                  ");
    gotoxy(25,5); cprintf("                                                                        ");

    gotoxy(25,27); cprintf("                                                                        ");
    gotoxy(25,28); cprintf("                                                                        ");
    gotoxy(25,29); cprintf("                                                                        ");

    gotoxy(35,7); cprintf("Se connecter");
    gotoxy(35,10);  printf("Nom d'utilisateur    :");
    gotoxy(35,13);  printf("Mot de passe         :");

    gotoxy(60,10);  gets(nomUtilisateur);
    gotoxy(60,13);  gets(password);

    clientConnecte = verifierConnexion(clients , *nbClients , nomUtilisateur , password);

    if(clientConnecte != NULL){
        textbackground(2);
        gotoxy(45,17);   cprintf("Bienvenue !");

    }else{

        textbackground(4);
        gotoxy(45,17);   cprintf("Aucun client connecte !");

    }
    getch();
    menuProfil(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
}

// Déconnecte le client actuellement connecté

void logout(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
            LigneCommande *lignesCommande , int *nbLignesCommande){

    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          se deconnecter                                ");
    gotoxy(25,5); cprintf("                                                                        ");

    gotoxy(25,27); cprintf("                                                                        ");
    gotoxy(25,28); cprintf("                                                                        ");
    gotoxy(25,29); cprintf("                                                                        ");

    if(clientConnecte != NULL){
        clientConnecte = NULL  ;

        textbackground(2);
        gotoxy(45,17);   cprintf("Deconnectez-vous ensuite avec succes !");
    }else{
        textbackground(4);
        gotoxy(45,17);   cprintf("Aucun client connecte !");
    }

    getch();
    menuProfil(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
}

// Recherche l'indice d'un client dans le tableau à partir de son ID

int rechercherParID(Client *clients , int nbClients , char *idClient){
    int i;

    if(idClient == NULL)
        return -1;

    for(i=0 ; i<nbClients ; i++){
        if(strcmp((clients+i)->idClient , idClient ) == 0)
           return i;
    }
    return -1;
}

// Affiche les informations du profil du client connecté

void consulterProfil(Produit *produits, int *nbProduits , Client *clients , int *nbClients  ,
                     LigneCommande *lignesCommande , int *nbLignesCommande){

    int indice;

    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          Consultation du profil                        ");
    gotoxy(25,5); cprintf("                                                                        ");

    gotoxy(25,27); cprintf("                                                                        ");
    gotoxy(25,28); cprintf("                                                                        ");
    gotoxy(25,29); cprintf("                                                                        ");

    gotoxy(35,7); cprintf("Consulter profil");
    gotoxy(35,10);  printf("Nom                 :");
    gotoxy(35,12);  printf("Prenom              :");
    gotoxy(35,14);  printf("Email               :");
    gotoxy(35,16);  printf("Nom d'utilisateur   :");
    gotoxy(35,18);  printf("Mot de passe        :");
    gotoxy(35,20);  printf("Solde               :");
    gotoxy(35,22);  printf("Date inscription    :");

    indice = rechercherParID(clients , *nbClients , clientConnecte);
    if(indice != -1){
        gotoxy(60,10);  printf("%s",(clients+indice)->nom);
        gotoxy(60,12);  printf("%s",(clients+indice)->prenom);
        gotoxy(60,14);  printf("%s",(clients+indice)->email);
        gotoxy(60,16);  printf("%s",(clients+indice)->nomUtilisateur);
        gotoxy(60,18);  printf("%s",(clients+indice)->password);
        gotoxy(60,20);  printf("%.2f",(clients+indice)->solde);
        gotoxy(60,22);  printf("%s",(clients+indice)->dateInscription);

    }else{
        textbackground(4);
        gotoxy(40,25);   cprintf("Aucun client connecte !");

    }

    getch();
    menuProfil(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
}

// Modifie les informations du client connecté (nom et prénom)

void modifierProfil(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                    LigneCommande *lignesCommande , int *nbLignesCommande){

    int indice;

    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          Modification du profil                        ");
    gotoxy(25,5); cprintf("                                                                        ");

    gotoxy(25,27); cprintf("                                                                        ");
    gotoxy(25,28); cprintf("                                                                        ");
    gotoxy(25,29); cprintf("                                                                        ");

    gotoxy(35,7); cprintf("modifier profil");
    gotoxy(35,10);  printf("Nom                 :");
    gotoxy(35,12);  printf("Prenom              :");

    if(clientConnecte != NULL){
        indice = rechercherParID(clients , *nbClients , clientConnecte);
        if(indice != -1){
            gotoxy(60,10);  gets((clients+indice)->nom);
            gotoxy(60,12);  gets((clients+indice)->prenom);

            genererEmail((clients+indice)->nom , (clients+indice)->prenom
                  , (clients+indice)->email);

            textbackground(2);
            gotoxy(35,25);   cprintf("Les informations ont ete mises a jour avec succes!");
        }
    }else{
        textbackground(4);
        gotoxy(35,25);   cprintf("Aucun client connecte !");

    }

    getch();
    menuProfil(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
}


// Affiche un produit dans le catalogue

void afficherProduit(Produit *produits, int indice){
    gotoxy(10,11+indice); printf("%s",(produits+indice)->idProduit);
    gotoxy(25,11+indice); printf("%s",(produits+indice)->nom);
    gotoxy(55,11+indice); printf("%s",(produits+indice)->categorie);
    gotoxy(80,11+indice); printf("%.2f",(produits+indice)->prix);
    gotoxy(95,11+indice); printf("%d",(produits+indice)->stock);
}

// Recherche un produit par son ID et retourne son indice

int rechercherIdProduit(Produit *produits, int nbProduits , char *idProduit){
    int i;
    for(i=0 ; i<nbProduits ; i++){
        if(strcmp( (produits+i)->idProduit , idProduit) == 0)
            return i;
    }
    return -1;
}

// Recherche un produit par son nom et affiche ses informations

void rechercherParNom(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                      LigneCommande *lignesCommande , int *nbLignesCommande){
    int i , j=0;
    int existe=0;
    char nom[50];

    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          Recherche produits                            ");
    gotoxy(25,5); cprintf("                                                                        ");

    gotoxy(35,7); printf("Nom                 : ");

    textbackground(4);
    gotoxy(10,9); cprintf("ID");
    gotoxy(25,9); cprintf("Nom");
    gotoxy(55,9); cprintf("Categorie");
    gotoxy(80,9); cprintf("Prix");
    gotoxy(95,9); cprintf("Stock");

    if(*nbProduits == 0){
        textbackground(4);
        gotoxy(35,15);   cprintf("Accun produit enregistre !");

        getch();
        menuProduit(produits, nbProduits, clients , nbClients , lignesCommande , nbLignesCommande);
    }

    gotoxy(60,7); gets(nom);

    for(i=0 ; i<*nbProduits ; i++){
        if(strcmp((produits+i)->nom , nom) == 0){
            gotoxy(10,11+j); printf("%s",(produits+i)->idProduit);
            gotoxy(25,11+j); printf("%s",(produits+i)->nom);
            gotoxy(55,11+j); printf("%s",(produits+i)->categorie);
            gotoxy(80,11+j); printf("%.2f",(produits+i)->prix);
            gotoxy(95,11+j); printf("%d",(produits+i)->stock);
            j++;
            existe = 1;
            break;
        }
    }

    if(existe == 0){
        textbackground(4);
        gotoxy(35,15);   cprintf("Accun produit enregistre !");

        getch();
        menuProduit(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
    }

    getch();
    menuProduit(produits, nbProduits , clients , nbClients, lignesCommande , nbLignesCommande);
}

// Trie les produits par prix croissant

void trierParPrixCroissant(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                  LigneCommande *lignesCommande , int *nbLignesCommande){

    int i , echange;
    Produit temp;
    alloueProduit(&temp);

    do{
        echange = 0;
        for(i=0 ; i<*nbProduits-1 ; i++){
            if((produits+i)->prix > (produits+i+1)->prix){
                copieProduit(&temp , produits+i);
                copieProduit(produits+i , produits+i+1);
                copieProduit(produits+i+1 , &temp);
                echange = 1;
            }
        }
    }while(echange == 1);

    viderProduit(&temp);

    afficherCatalogue(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
}

// Trie les produits par prix décroissant

void trierParPrixDecroissant(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                  LigneCommande *lignesCommande , int *nbLignesCommande){

    int i , echange;
    Produit temp;
    alloueProduit(&temp);

    do{
        echange = 0;
        for(i=0 ; i<*nbProduits-1 ; i++){
            if((produits+i)->prix < (produits+i+1)->prix){
                copieProduit(&temp , produits+i);
                copieProduit(produits+i , produits+i+1);
                copieProduit(produits+i+1 , &temp);
                echange = 1;
            }
        }
    }while(echange == 1);

    viderProduit(&temp);

    afficherCatalogue(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
}

// Trie les produits par nom par ordre alphabétique

void trierParNom(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                 LigneCommande *lignesCommande , int *nbLignesCommande){

    int i , echange;
    Produit temp;
    alloueProduit(&temp);

    do{
        echange = 0;
        for(i=0 ; i<*nbProduits-1 ; i++){
            if( strcmp( (produits+i)->nom , (produits+i+1)->nom ) >0 ){
                copieProduit(&temp , produits+i);
                copieProduit(produits+i , produits+i+1);
                copieProduit(produits+i+1 , &temp);
                echange = 1;
            }
        }
    }while(echange == 1);

    viderProduit(&temp);

    afficherCatalogue(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
}

// Recherche les produits par catégorie et les affiche

void rechercherParCategorie(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                            LigneCommande *lignesCommande , int *nbLignesCommande){
    int i , j=0;
    int existe=0;
    char categorie[50];

    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          Recherche produits                            ");
    gotoxy(25,5); cprintf("                                                                        ");

    gotoxy(35,7); printf("Categorie         : ");

    textbackground(4);
    gotoxy(10,9); cprintf("ID");
    gotoxy(25,9); cprintf("Nom");
    gotoxy(55,9); cprintf("Categorie");
    gotoxy(80,9); cprintf("Prix");
    gotoxy(95,9); cprintf("Stock");

    if(*nbProduits == 0){
        textbackground(4);
        gotoxy(40,15);   cprintf("Accun produit enregistre !");

        getch();
        menuProduit(produits, nbProduits, clients , nbClients , lignesCommande , nbLignesCommande);
    }

    gotoxy(60,7); gets(categorie);

    for(i=0 ; i<*nbProduits ; i++){
        if(strcmp((produits+i)->categorie , categorie) == 0){
            gotoxy(10,11+j); printf("%s",(produits+i)->idProduit);
            gotoxy(25,11+j); printf("%s",(produits+i)->nom);
            gotoxy(55,11+j); printf("%s",(produits+i)->categorie);
            gotoxy(80,11+j); printf("%.2f",(produits+i)->prix);
            gotoxy(95,11+j); printf("%d",(produits+i)->stock);
            j++;
            existe = 1;
        }
    }

    if(existe == 0){
        textbackground(4);
        gotoxy(40,15);   cprintf("Accun produit enregistre !");

    }
    getch();
    menuProduit(produits, nbProduits, clients , nbClients, lignesCommande , nbLignesCommande);
}

// Affiche tout le catalogue des produits

void afficherCatalogue(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                       LigneCommande *lignesCommande , int *nbLignesCommande){

    int i ;
    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          Affichage catalogue                           ");
    gotoxy(25,5); cprintf("                                                                        ");

    textbackground(4);
    gotoxy(10,8); cprintf("ID");
    gotoxy(25,8); cprintf("Nom");
    gotoxy(55,8); cprintf("Categorie");
    gotoxy(80,8); cprintf("Prix");
    gotoxy(95,8); cprintf("Stock");

    if(*nbProduits == 0){
        textbackground(4);
        gotoxy(40,15);   cprintf("Accun produit enregistre !");

        getch();
        menuProduit(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
    }

    for(i=0 ; i<*nbProduits ; i++){
        afficherProduit(produits, i);
    }

    getch();
    menuProduit(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
}

// Affiche les détails complets d'un produit spécifique

void afficherDetailsProduit(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                            LigneCommande *lignesCommande , int *nbLignesCommande){

    int  i , indice;
    int existe=0;
    char idProduit[10];

    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          Details produit                               ");
    gotoxy(25,5); cprintf("                                                                        ");

    gotoxy(35,7); printf("Numero produit    : ");

    textbackground(4);

    gotoxy(35,10); printf("Nom            :");
    gotoxy(35,12); printf("Categorie      :");
    gotoxy(35,14); printf("Prix           :");
    gotoxy(35,16); printf("Stock          :");
    gotoxy(35,18); printf("description    :");

    if(*nbProduits == 0){
        textbackground(4);
        gotoxy(40,25);   cprintf("Accun produit enregistre !");

        getch();
        menuProduit(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
    }

    gotoxy(60,7); gets(idProduit);

    indice = rechercherIdProduit(produits, *nbProduits , idProduit);

    if(indice != -1){

        gotoxy(55,10); puts((produits+indice)->nom);
        gotoxy(55,12); puts((produits+indice)->categorie);
        gotoxy(55,14); printf("%.2f",((produits+indice)->prix));
        gotoxy(55,16); printf("%d",((produits+indice)->stock));
        gotoxy(55,18); puts((produits+indice)->description );
    }else{
        textbackground(4);
        gotoxy(40,25);   cprintf("Accun produit enregistre !");
    }

    getch();
    menuProduit(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
}

// Affiche le solde du client connecté
void consulterSolde(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                    LigneCommande *lignesCommande , int *nbLignesCommande){

    int indice;

    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          Consultation du solde                         ");
    gotoxy(25,5); cprintf("                                                                        ");

    gotoxy(25,27); cprintf("                                                                        ");
    gotoxy(25,28); cprintf("                                                                        ");
    gotoxy(25,29); cprintf("                                                                        ");

    gotoxy(35,7); cprintf(" Consulter solde ");
    gotoxy(35,10);  printf("Nom               :");
    gotoxy(35,12);  printf("Solde             :");


    indice = rechercherParID(clients , *nbClients , clientConnecte);
    if(indice != -1){
        gotoxy(60,10);  printf("%s  %s",(clients+indice)->prenom,(clients+indice)->nom);
        gotoxy(60,12);  printf("%.2f",(clients+indice)->solde);

    }else{
        textbackground(4);
        gotoxy(35,25);   cprintf("Aucun client connecte !");

    }

    getch();
    menuSolde(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
}

// Vérifie si le client connecté a assez de solde pour un achat

int verifierSolde(Client *clients , int *nbClients , float prixTotale){
    int indice = rechercherParID(clients , *nbClients , clientConnecte);

    if((clients+indice)->solde >= prixTotale)
        return 1;
    else
        return 0;
}

// Vérifie si le stock d'un produit est suffisant pour l'achat

int verifierStock(Produit *produit , int nbProd){

    if(produit->stock >= nbProd)
        return 1;
    else
        return 0;
}

// Permet au client connecté d'acheter un produit

void achatProduit(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                   LigneCommande *lignesCommande , int *nbLignesCommande){

    int j;
    int existe=0 , indice;
    char id[50];
    int nbProd;
    float prixTotale ;

    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          Selection de produit                          ");
    gotoxy(25,5); cprintf("                                                                        ");

    gotoxy(35,7); printf("Numero Produit      : ");

    textbackground(4);
    gotoxy(10,9); cprintf("ID");
    gotoxy(23,9); cprintf("Nom");
    gotoxy(50,9); cprintf("Prix");
    gotoxy(80,9); cprintf("Stock");

    if(*nbProduits == 0){
        textbackground(4);
        gotoxy(35,15);   cprintf("Accun produit enregistre !");

        getch();
        menuPrincipal(produits, nbProduits, clients , nbClients , lignesCommande , nbLignesCommande);
    }

    gotoxy(60,7); scanf("%s",id);

    if(clientConnecte != NULL){
        indice = rechercherIdProduit(produits , *nbProduits , id);
        if(indice != -1){
            gotoxy(10,11); printf("%s",(produits+indice)->idProduit);
            gotoxy(23,11); printf("%s",(produits+indice)->nom);
            gotoxy(50,11); printf("%.2f",(produits+indice)->prix);
            gotoxy(80,11); printf("%d",(produits+indice)->stock);
            do{
                gotoxy(35 , 13); printf("Entrez le nombre de produit : ");
                gotoxy(65 , 13); scanf("%d",&nbProd);
            }while(nbProd <= 0);

            prixTotale = ((produits+indice)->prix) * nbProd;

            if(verifierSolde(clients , nbClients , prixTotale)){
                if(verifierStock(produits+indice , nbProd)){

                    int j = rechercherParID(clients , *nbClients , clientConnecte);

                    (clients+j)->solde -= prixTotale;
                    (produits+indice)->stock -= nbProd;

                    alloueLigneCommande(lignesCommande+*nbLignesCommande);

                    strcpy((lignesCommande+*nbLignesCommande)->noClient , clientConnecte);
                    strcpy((lignesCommande+*nbLignesCommande)->noProduit  , (produits+indice)->idProduit);
                    genererDate((lignesCommande+*nbLignesCommande)->dateAchat );

                    (lignesCommande+*nbLignesCommande)->prixUnitaire = (produits+indice)->prix;
                    (lignesCommande+*nbLignesCommande)->qte = nbProd ;

                    (*nbLignesCommande)++;

                    textbackground(2);
                    gotoxy(40,15);   cprintf("Achat effecte avec succes !");

                }else{
                    textbackground(4);
                    gotoxy(40,15);   cprintf("Ce produit n'est plus en stock !");
                }
            }else{
                textbackground(4);
                gotoxy(40,15);   cprintf("Votre solde est insuffisant pour acheter ce produit !");
            }

       }else{
            textbackground(4);
            gotoxy(40,15);   cprintf("Accun produit enregistre !");

            getch();
            menuPrincipal(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
        }
    }else{
        textbackground(4);
        gotoxy(40,25);   cprintf("Aucun client connecte !");

    }

    getch();
    menuPrincipal(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
}

// Permet au client connecté d'ajouter de l'argent à son solde

void depotArgent(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                 LigneCommande *lignesCommande , int *nbLignesCommande){

    int indice;
    float solde;

    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          Depot d'argent                                ");
    gotoxy(25,5); cprintf("                                                                        ");

    gotoxy(25,27); cprintf("                                                                        ");
    gotoxy(25,28); cprintf("                                                                        ");
    gotoxy(25,29); cprintf("                                                                        ");

    gotoxy(35,7); cprintf(" Ajouter solde ");
    gotoxy(35,10);  printf("Solde                :");


    indice = rechercherParID(clients , *nbClients , clientConnecte);
    if(indice != -1){
        gotoxy(60,10);  scanf("%f",&solde);
        (clients+indice)->solde += solde;

        textbackground(2);
        gotoxy(35,25);   cprintf("Ensuite, le depot de l'argent a ete effectue avec succes !");

    }else{
        textbackground(4);
        gotoxy(40,15);   cprintf("Aucun client connecte !");

    }

    getch();
    menuSolde(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
}

// Affiche les statistiques des achats du client connecté

void statistique(Produit *produits, int *nbProduits ,Client *clients , int *nbClients ,
                LigneCommande *lignesCommande , int *nbLignesCommande){
    int i ;
    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          Mes statistiques                              ");
    gotoxy(25,5); cprintf("                                                                        ");

    textbackground(4);
    gotoxy(10,8); cprintf("NoClient");
    gotoxy(25,8); cprintf("NoProduit");
    gotoxy(55,8); cprintf("Quantite");
    gotoxy(70,8); cprintf("PrixUnitaire");
    gotoxy(90,8); cprintf("Date d'achat");


    if(*nbLignesCommande == 0){
        textbackground(2);
        gotoxy(40,15);   cprintf("Accun storie enregistre !");

        getch();
        menuPrincipal(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
    }

    for(i=0 ; i<*nbLignesCommande ; i++){
        gotoxy(10,10+i); printf("%s",(lignesCommande+i)->noClient);
        gotoxy(25,10+i); printf("%s",(lignesCommande+i)->noProduit);
        gotoxy(55,10+i); printf("%d",(lignesCommande+i)->qte);
        gotoxy(75,10+i); printf("%.2f",(lignesCommande+i)->prixUnitaire);
        gotoxy(90,10+i); printf("%s",(lignesCommande+i)->dateAchat);
    }

    getch();
    menuProduit(produits, nbProduits , clients , nbClients , lignesCommande , nbLignesCommande);
}

// Menu de gestion du profil client

void menuProfil(Produit *produits, int *nbProduits ,Client *clients , int *nbClients ,
                LigneCommande *lignesCommande , int *nbLignesCommande){

    char choix;

    system("cls"); system("color f0");
    textbackground(4); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                           Gestion du profil client                     ");
    gotoxy(25,5); cprintf("                                                                        ");

    textbackground(7); textcolor(2);
    gotoxy(35,7);    cprintf("                                                     ");
    gotoxy(35,8);    cprintf("             1- Creation de profil                   ");
    gotoxy(35,9);    cprintf("                                                     ");
    gotoxy(35,10);   cprintf("             2- Modification du profil               ");
    gotoxy(35,11);   cprintf("                                                     ");
    gotoxy(35,12);   cprintf("             3- Consultation du profil               ");
    gotoxy(35,13);   cprintf("                                                     ");
    gotoxy(35,14);   cprintf("             4- Se connecter                         ");
    gotoxy(35,15);   cprintf("                                                     ");
    gotoxy(35,16);   cprintf("             5- se deconnecter                       ");
    gotoxy(35,17);   cprintf("                                                     ");
    gotoxy(35,18);   cprintf("             0- Retour menu Principal                ");
    gotoxy(35,19);   cprintf("                                                     ");



    do{
        gotoxy(35,22);   printf("Entrez votre choix : ");
        choix = getch();
    }while(choix!='1' && choix!='2' && choix!='3' && choix!='4' && choix!='5' && choix!='0');

    switch(choix){

        case '1' : creerProfil(produits, nbProduits , clients , nbClients ,
                               lignesCommande , nbLignesCommande); break;

        case '2' : modifierProfil(produits, nbProduits , clients , nbClients ,
                                  lignesCommande , nbLignesCommande); break;

        case '3' : consulterProfil(produits, nbProduits , clients , nbClients ,
                                   lignesCommande , nbLignesCommande); break;

        case '4' : login(produits, nbProduits , clients , nbClients,
                         lignesCommande , nbLignesCommande); break;

        case '5' : logout(produits, nbProduits , clients , nbClients ,
                          lignesCommande , nbLignesCommande); break;

        case '0' : menuPrincipal(produits, nbProduits , clients , nbClients ,
                                 lignesCommande , nbLignesCommande); break;
    }
}

//Affiche le menu de gestion du prix

void menuTrier(Produit *produits, int *nbProduits ,Client *clients , int *nbClients ,
            LigneCommande *lignesCommande , int *nbLignesCommande){

    char choix;

    system("cls"); system("color f0");
    textbackground(4); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                           Tri des produits                             ");
    gotoxy(25,5); cprintf("                                                                        ");

    textbackground(7); textcolor(2);
    gotoxy(35,7);    cprintf("                                                     ");
    gotoxy(35,8);    cprintf("             1- Produits par prix (croissant)        ");
    gotoxy(35,9);    cprintf("                                                     ");
    gotoxy(35,10);   cprintf("             2- Produits par prix (décroissant)      ");
    gotoxy(35,11);   cprintf("                                                     ");
    gotoxy(35,12);   cprintf("             0- Retour menu Principal                ");
    gotoxy(35,13);   cprintf("                                                     ");

    do{
        gotoxy(35,22);   printf("Entrez votre choix : ");
        choix = getch();
    }while(choix!='1' && choix!='2' && choix!='0');

    switch(choix){

        case '1' : trierParPrixCroissant(produits, nbProduits , clients , nbClients ,
                                  lignesCommande , nbLignesCommande); break;

        case '2' : trierParPrixDecroissant(produits, nbProduits , clients , nbClients ,
                               lignesCommande , nbLignesCommande); break;

        case '0' : menuProduit(produits, nbProduits , clients , nbClients ,
                                 lignesCommande , nbLignesCommande); break;
    }
}

//Affiche le menu de gestion du solde

void menuSolde(Produit *produits, int *nbProduits ,Client *clients , int *nbClients ,
            LigneCommande *lignesCommande , int *nbLignesCommande){

    char choix;

    system("cls"); system("color f0");
    textbackground(4); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                           Gestion du Solde Virtuel                     ");
    gotoxy(25,5); cprintf("                                                                        ");

    textbackground(7); textcolor(2);
    gotoxy(35,7);    cprintf("                                                     ");
    gotoxy(35,8);    cprintf("             1- Consultation du solde                 ");
    gotoxy(35,9);    cprintf("                                                     ");
    gotoxy(35,10);   cprintf("             2- Depot d'argent                       ");
    gotoxy(35,11);   cprintf("                                                     ");
    gotoxy(35,12);   cprintf("             0- Retour menu Principal                ");
    gotoxy(35,13);   cprintf("                                                     ");

    do{
        gotoxy(35,22);   printf("Entrez votre choix : ");
        choix = getch();
    }while(choix!='1' && choix!='2' && choix!='0');

    switch(choix){

        case '1' : consulterSolde(produits, nbProduits , clients , nbClients ,
                                  lignesCommande , nbLignesCommande); break;

        case '2' : depotArgent(produits, nbProduits , clients , nbClients ,
                               lignesCommande , nbLignesCommande); break;

        case '0' : menuPrincipal(produits, nbProduits , clients , nbClients ,
                                 lignesCommande , nbLignesCommande); break;
    }
}

//Affiche le menu des produits
void menuProduit(Produit *produits, int *nbProduits , Client *clients , int *nbClients ,
                 LigneCommande *lignesCommande , int *nbLignesCommande){
    char choix;

    system("cls"); system("color f0");
    textbackground(4); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                           Catalogue des Produits                       ");
    gotoxy(25,5); cprintf("                                                                        ");

    textbackground(7); textcolor(2);
    gotoxy(35,7);    cprintf("                                                     ");
    gotoxy(35,8);    cprintf("             1- Affichage catalogue                  ");
    gotoxy(35,9);    cprintf("                                                     ");
    gotoxy(35,10);   cprintf("             2- Recherche produits par nom           ");
    gotoxy(35,11);   cprintf("                                                     ");
    gotoxy(35,12);   cprintf("             3- Recherche produits par categorie     ");
    gotoxy(35,13);   cprintf("                                                     ");
    gotoxy(35,14);   cprintf("             4- Tri des produits par prix            ");
    gotoxy(35,15);   cprintf("                                                     ");
    gotoxy(35,16);   cprintf("             5- Tri des produits par nom             ");
    gotoxy(35,17);   cprintf("                                                     ");
    gotoxy(35,18);   cprintf("             6- Details produit                      ");
    gotoxy(35,19);   cprintf("                                                     ");
    gotoxy(35,20);   cprintf("             0- Retour menu Principal                ");
    gotoxy(35,21);   cprintf("                                                     ");

    do{
        gotoxy(35,23);   printf("Entrez votre choix : ");
        choix = getch();
    }while(choix!='1' && choix!='2' && choix!='3' && choix!='4' && choix!='5' && choix!='6' && choix!='0');

    switch(choix){

        case '1' : afficherCatalogue(produits, nbProduits , clients , nbClients,
                                     lignesCommande , nbLignesCommande); break;

        case '2' : rechercherParNom(produits, nbProduits , clients , nbClients ,
                                    lignesCommande , nbLignesCommande); break;

        case '3' : rechercherParCategorie(produits, nbProduits , clients , nbClients ,
                                          lignesCommande , nbLignesCommande); break;

        case '4' : menuTrier(produits, nbProduits, clients , nbClients ,
                                lignesCommande , nbLignesCommande); break;

        case '5' : trierParNom(produits, nbProduits, clients , nbClients ,
                               lignesCommande , nbLignesCommande); break;

        case '6' : afficherDetailsProduit(produits, nbProduits , clients , nbClients ,
                                          lignesCommande , nbLignesCommande); break;

        case '0' : menuPrincipal(produits, nbProduits , clients , nbClients ,
                                 lignesCommande , nbLignesCommande); break;
    }

}

//Menu principal de l'application
void menuPrincipal(Produit *produits, int *nbProduits ,Client *clients , int *nbClients
                   ,LigneCommande *lignesCommande , int *nbLignesCommande){
    char choix;

    system("cls"); system("color f0");
    textbackground(4); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                           Systeme d'achat                              ");
    gotoxy(25,5); cprintf("                                                                        ");

    textbackground(7); textcolor(2);
    gotoxy(35,7);    cprintf("                                                     ");
    gotoxy(35,8);    cprintf("             1- Gestion du profil client             ");
    gotoxy(35,9);    cprintf("                                                     ");
    gotoxy(35,10);   cprintf("             2- Gestion du solde virtuel             ");
    gotoxy(35,11);   cprintf("                                                     ");
    gotoxy(35,12);   cprintf("             3- Consultation des produits            ");
    gotoxy(35,13);   cprintf("                                                     ");
    gotoxy(35,14);   cprintf("             4- Effectuer un achat                   ");
    gotoxy(35,15);   cprintf("                                                     ");
    gotoxy(35,16);   cprintf("             5- Mes statistiques                     ");
    gotoxy(35,17);   cprintf("                                                     ");
    gotoxy(35,18);   cprintf("             0- Quitter l'application                ");
    gotoxy(35,19);   cprintf("                                                     ");



    do{
        gotoxy(35,22);   printf("Entrez votre choix : ");
        choix = getch();
    }while(choix!='1' && choix!='2' && choix!='3' && choix!='4' && choix!='5' && choix!='0');

    switch(choix){

        case '1' : menuProfil(produits, nbProduits , clients , nbClients ,
                              lignesCommande , nbLignesCommande); break;

        case '2' : menuSolde(produits, nbProduits , clients , nbClients ,
                             lignesCommande , nbLignesCommande); break;

        case '3' : menuProduit(produits, nbProduits , clients , nbClients ,
                               lignesCommande , nbLignesCommande); break;

        case '4' : achatProduit(produits, nbProduits , clients , nbClients ,
                                lignesCommande , nbLignesCommande); break;
        case '5' : statistique(produits, nbProduits , clients , nbClients ,
                                lignesCommande , nbLignesCommande); break;

        case '0' :  exit(0); break;
    }

}

void initialiserProduits(Produit *produits, int *nbProduits){

    // --- Produit 1
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Ordinateur Portable");
    strcpy((produits + *nbProduits)->categorie, "Electronique");
    (produits + *nbProduits)->prix = 7500.00;
    (produits + *nbProduits)->stock = 12;
    strcpy((produits + *nbProduits)->description, "Laptop performant avec processeur i7 et 16Go RAM.");
    (*nbProduits)++;

    // --- Produit 2
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Smartphone");
    strcpy((produits + *nbProduits)->categorie, "Electronique");
    (produits + *nbProduits)->prix = 3200.00;
    (produits + *nbProduits)->stock = 30;
    strcpy((produits + *nbProduits)->description, "Smartphone Android avec 128Go stockage et 6Go RAM.");
    (*nbProduits)++;

    // --- Produit 3
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Casque Audio");
    strcpy((produits + *nbProduits)->categorie, "Accessoires");
    (produits + *nbProduits)->prix = 450.00;
    (produits + *nbProduits)->stock = 50;
    strcpy((produits + *nbProduits)->description, "Casque sans fil Bluetooth avec reduction de bruit.");
    (*nbProduits)++;

    // --- Produit 4
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Souris Gamer");
    strcpy((produits + *nbProduits)->categorie, "Accessoires");
    (produits + *nbProduits)->prix = 250.00;
    (produits + *nbProduits)->stock = 40;
    strcpy((produits + *nbProduits)->description, "Souris gaming avec capteur optique haute precision.");
    (*nbProduits)++;

    // --- Produit 5
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Clavier Mecanique");
    strcpy((produits + *nbProduits)->categorie, "Accessoires");
    (produits + *nbProduits)->prix = 600.00;
    (produits + *nbProduits)->stock = 25;
    strcpy((produits + *nbProduits)->description, "Clavier mecanique retro-eclaire RGB.");
    (*nbProduits)++;

    // --- Produit 6
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Television LED 43");
    strcpy((produits + *nbProduits)->categorie, "Electromenager");
    (produits + *nbProduits)->prix = 2800.00;
    (produits + *nbProduits)->stock = 10;
    strcpy((produits + *nbProduits)->description, "TV Full HD avec port HDMI et USB.");
    (*nbProduits)++;

    // --- Produit 7
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Refrigerateur");
    strcpy((produits + *nbProduits)->categorie, "Electromenager");
    (produits + *nbProduits)->prix = 5400.00;
    (produits + *nbProduits)->stock = 8;
    strcpy((produits + *nbProduits)->description, "Frigo 2 portes avec congelateur integre.");
    (*nbProduits)++;

    // --- Produit 8
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Machine e laver");
    strcpy((produits + *nbProduits)->categorie, "Electromenager");
    (produits + *nbProduits)->prix = 3900.00;
    (produits + *nbProduits)->stock = 6;
    strcpy((produits + *nbProduits)->description, "Machine a laver 7Kg avec programme rapide.");
    (*nbProduits)++;

    // --- Produit 9
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Tablette 10");
    strcpy((produits + *nbProduits)->categorie, "Electronique");
    (produits + *nbProduits)->prix = 2100.00;
    (produits + *nbProduits)->stock = 15;
    strcpy((produits + *nbProduits)->description, "Tablette Android avec ecran tactile HD.");
    (*nbProduits)++;

    // --- Produit 10
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Imprimante");
    strcpy((produits + *nbProduits)->categorie, "Bureau");
    (produits + *nbProduits)->prix = 1200.00;
    (produits + *nbProduits)->stock = 20;
    strcpy((produits + *nbProduits)->description, "Imprimante multifonction jet d'encre WiFi.");
    (*nbProduits)++;

    // --- Produit 11
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Enceinte Bluetooth");
    strcpy((produits + *nbProduits)->categorie, "Accessoires");
    (produits + *nbProduits)->prix = 350.00;
    (produits + *nbProduits)->stock = 45;
    strcpy((produits + *nbProduits)->description, "Enceinte portable avec son stereo et batterie longue duree.");
    (*nbProduits)++;

    // --- Produit 12
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Camera de Surveillance");
    strcpy((produits + *nbProduits)->categorie, "Electronique");
    (produits + *nbProduits)->prix = 800.00;
    (produits + *nbProduits)->stock = 18;
    strcpy((produits + *nbProduits)->description, "Camera WiFi HD avec vision nocturne.");
    (*nbProduits)++;

    // --- Produit 13
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Aspirateur Robot");
    strcpy((produits + *nbProduits)->categorie, "Electromenager");
    (produits + *nbProduits)->prix = 2600.00;
    (produits + *nbProduits)->stock = 10;
    strcpy((produits + *nbProduits)->description, "Robot aspirateur intelligent avec nettoyage automatique.");
    (*nbProduits)++;

    // --- Produit 14
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Micro-ondes");
    strcpy((produits + *nbProduits)->categorie, "Electromenager");
    (produits + *nbProduits)->prix = 1200.00;
    (produits + *nbProduits)->stock = 14;
    strcpy((produits + *nbProduits)->description, "Micro-ondes avec grill et minuterie digitale.");
    (*nbProduits)++;

    // --- Produit 15
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Ventilateur");
    strcpy((produits + *nbProduits)->categorie, "Maison");
    (produits + *nbProduits)->prix = 300.00;
    (produits + *nbProduits)->stock = 35;
    strcpy((produits + *nbProduits)->description, "Ventilateur sur pied reglable en hauteur.");
    (*nbProduits)++;

    // --- Produit 16
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Chaise de Bureau");
    strcpy((produits + *nbProduits)->categorie, "Bureau");
    (produits + *nbProduits)->prix = 950.00;
    (produits + *nbProduits)->stock = 22;
    strcpy((produits + *nbProduits)->description, "Chaise ergonomique avec reglage de hauteur.");
    (*nbProduits)++;

    // --- Produit 17
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Table de Bureau");
    strcpy((produits + *nbProduits)->categorie, "Bureau");
    (produits + *nbProduits)->prix = 1500.00;
    (produits + *nbProduits)->stock = 10;
    strcpy((produits + *nbProduits)->description, "Table en bois avec tiroirs integres.");
    (*nbProduits)++;

    // --- Produit 18
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Four electrique");
    strcpy((produits + *nbProduits)->categorie, "Electromenager");
    (produits + *nbProduits)->prix = 2700.00;
    (produits + *nbProduits)->stock = 7;
    strcpy((produits + *nbProduits)->description, "Four electrique multifonction avec chaleur tournante.");
    (*nbProduits)++;

    // --- Produit 19
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Lampe LED");
    strcpy((produits + *nbProduits)->categorie, "Maison");
    (produits + *nbProduits)->prix = 150.00;
    (produits + *nbProduits)->stock = 60;
    strcpy((produits + *nbProduits)->description, "Lampe LED economique avec design moderne.");
    (*nbProduits)++;

    // --- Produit 20
    alloueProduit(produits + *nbProduits);
    genererIdProduit((produits + *nbProduits)->idProduit, *nbProduits + 1);
    strcpy((produits + *nbProduits)->nom, "Disque Dur Externe");
    strcpy((produits + *nbProduits)->categorie, "Electronique");
    (produits + *nbProduits)->prix = 850.00;
    (produits + *nbProduits)->stock = 28;
    strcpy((produits + *nbProduits)->description, "Disque dur externe 1To USB 3.0 rapide.");
    (*nbProduits)++;

}


int main(){
    int i;

    Client *clients = malloc(MAX_CLIENTS*sizeof(Client));
    int nbClients = 0;

    Produit *produits = malloc(MAX_PRODUITS*sizeof(Produit));
    int nbProduits = 0;

    LigneCommande *lignesCommande = malloc(MAX_LINES*sizeof(LigneCommande));
    int nblignesCommande = 0;

    initialiserProduits(produits, &nbProduits);

    menuPrincipal(produits, &nbProduits, clients , &nbClients , lignesCommande , &nblignesCommande);

    for(i=0 ; i<nbClients ; i++){
        viderClient(clients+i);
    }
    free(clients);

    for(i=0 ; i<nbProduits ; i++){
        viderProduit(produits+i);
    }
    free(produits);

    for(i=0 ; i<nblignesCommande ; i++){
        viderLigneCommande(lignesCommande+i);
    }
    free(lignesCommande);

    return 0;
}
