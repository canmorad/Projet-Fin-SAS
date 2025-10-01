#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#define MAX_CLIENTS 100
#define MAX_PRODUITS 400

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

typedef struct{
    char *idProduit ;
    char *nom ;
    char *categorie ;
    float prix ;
    int stock ;
    char *description ;
}Produit;

void menuPrincipal(Client *clients , int *nbClients);
void menuProfil(Client *clients , int *nbClients);
void afficherCatalogue(Produit *produits, int *nbProduits);

char *idClient = NULL;

void genererIdClient(char *idClient , int compteur){

    sprintf(idClient , "C%03d" , compteur);
}

void genererIdProduit(char *idProduit , int compteur){

    sprintf(idProduit , "P%03d" , compteur);
}

void genererEmail(char *nom , char *prenom , char *email){

    sprintf(email , "%s.%s@email.com" , prenom,nom);
}

void genererDate(char *date){
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    sprintf(date , "%02d/%02d/%04d" , tm->tm_mday,tm->tm_mon+1,tm->tm_year+1900);
}

int estNomUtilisateurUnique(Client *clients , int nbClients , char *nomUtilisateur){
    int i;
    for(i=0 ; i<nbClients ; i++){
        if(strcmp((clients+i)->nomUtilisateur , nomUtilisateur) == 0)
            return 0;
    }
    return 1;
}

int estPasswordUnique(Client *clients , int nbClients , char *password){
    int i;
    for(i=0 ; i<nbClients ; i++){
        if(strcmp((clients+i)->password , password) == 0)
            return 0;
    }
    return 1;
}

void alloueProduit(Produit *produit){
    produit->idProduit = malloc(50);
    produit->nom  = malloc(50);
    produit->categorie  = malloc(50);
    produit->description  = malloc(100);
}

char *idProduit ;
    char *nom ;
    char *categorie ;
    float prix ;
    int stock ;
    char *description ;

void copieProduit(Produit *p1 , Produit *p2){
    strcpy(p1->idProduit , p2->idProduit);
    strcpy(p1->nom , p2->nom);
    p1->prix = p2->prix;
    p1->stock = p2->stock;
    strcpy(p1->description , p2->description);
}

void creerProfil(Client *clients , int *nbClients){
    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          Creation de profil                            ");
    gotoxy(25,5); cprintf("                                                                        ");

    gotoxy(25,27); cprintf("                                                                        ");
    gotoxy(25,28); cprintf("                                                                        ");
    gotoxy(25,29); cprintf("                                                                        ");

    gotoxy(35,7); cprintf("Creer profil");
    gotoxy(35,10);  printf("Nom                  :");
    gotoxy(35,12);  printf("Prenom               :");
    gotoxy(35,14);  printf("Nom d'utilisateur    :");
    gotoxy(35,16);  printf("Mot de passe         :");

    if(*nbClients >= MAX_CLIENTS){
        textbackground(4);
        gotoxy(35,25);   cprintf("La table est pleine !");

        getch();
        menuProfil(clients , nbClients);
    }

    (clients+*nbClients)->idClient = malloc(10);
    (clients+*nbClients)->nom = malloc(50);
    (clients+*nbClients)->prenom = malloc(50);
    (clients+*nbClients)->email = malloc(50);
    (clients+*nbClients)->dateInscription = malloc(20);
    (clients+*nbClients)->nomUtilisateur  = malloc(50);
    (clients+*nbClients)->password = malloc(50);

    gotoxy(60,10); gets((clients+*nbClients)->nom);
    gotoxy(60,12); gets((clients+*nbClients)->prenom);

    do{
        gotoxy(60,14);  printf("                                        ");
        gotoxy(60,14); gets((clients+*nbClients)->nomUtilisateur);
        if(!estNomUtilisateurUnique(clients , *nbClients , (clients+*nbClients)->nomUtilisateur)){
           gotoxy(60,14);  printf("Ce username est deja pris!");
        }
    }while(!estNomUtilisateurUnique(clients , *nbClients , (clients+*nbClients)->nomUtilisateur));

    do{
        gotoxy(60,16);  printf("                                        ");
        gotoxy(60,16); gets((clients+*nbClients)->password);
        if(!estPasswordUnique(clients , *nbClients , (clients+*nbClients)->password)){
           gotoxy(60,16);  printf("Ce password est deja pris!");
        }
    }while(!estPasswordUnique(clients , *nbClients , (clients+*nbClients)->password));

    (clients+*nbClients)->solde = 0.0;

    genererEmail((clients+*nbClients)->nom , (clients+*nbClients)->prenom
                  , (clients+*nbClients)->email);

    genererDate((clients+*nbClients)->dateInscription);
    genererIdClient((clients+*nbClients)->idClient, *nbClients+1);

    //idClient = (clients+*nbClients)->idClient;

    (*nbClients)++;

    textbackground(4);
    gotoxy(35,25);   cprintf("Le compte creer avec succes !");

    getch();
    menuProfil(clients , nbClients);
}

 char *verifierConnexion(Client *clients , int nbClients , char *nomUtilisateur , char *password){
    int i;
    for(i=0 ; i<nbClients ; i++){
        if(strcmp((clients+i)->nomUtilisateur , nomUtilisateur ) == 0 &&
           strcmp((clients+i)->password , password ) ==0 )

            return (clients+i)->idClient;

    }
    return NULL;
}

void login(Client *clients , int *nbClients){
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

    idClient = verifierConnexion(clients , *nbClients , nomUtilisateur , password);

    if(idClient != NULL){
        textbackground(4);
        gotoxy(45,17);   cprintf("Bienvenue !");

        getch();
        menuProfil(clients , nbClients);
    }else{
        textbackground(4);
        gotoxy(45,17);   cprintf("Aucun client connecte !");

        getch();
        menuProfil(clients , nbClients);
    }

}

void logout(Client *clients , int *nbClients){
    idClient = NULL;

    getch();
    menuProfil(clients , nbClients);
}

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

void consulterProfil(Client *clients , int *nbClients){
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

    indice = rechercherParID(clients , *nbClients , idClient);
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
        gotoxy(35,25);   cprintf("Aucun client connecte !");

    }

    getch();
    menuProfil(clients , nbClients);
}

void modifierProfil(Client *clients , int *nbClients){
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

    if(idClient != NULL){
        indice = rechercherParID(clients , *nbClients , idClient);
        if(indice != -1){
            gotoxy(60,10);  gets((clients+indice)->nom);
            gotoxy(60,12);  gets((clients+indice)->prenom);
        }
    }else{
        textbackground(4);
        gotoxy(35,25);   cprintf("Aucun client connecte !");

    }

    getch();
    menuProfil(clients , nbClients);
}

void afficherProduit(Produit *produits, int indice){
    gotoxy(10,11+indice); printf("%s",(produits+indice)->idProduit);
    gotoxy(25,11+indice); printf("%s",(produits+indice)->nom);
    gotoxy(55,11+indice); printf("%s",(produits+indice)->categorie);
    gotoxy(80,11+indice); printf("%.2f",(produits+indice)->prix);
    gotoxy(95,11+indice); printf("%d",(produits+indice)->stock);
}

void rechercherParNom(Produit *produits, int *nbProduits){
    int i;
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

    gotoxy(60,7); gets(nom);

    for(i=0 ; i<*nbProduits ; i++){
        if(strcmp((produits+i)->nom , nom) == 0){
            afficherProduit(produits, i);
            existe = 1;
        }
    }

    if(existe == 0){
        textbackground(4);
        gotoxy(35,15);   cprintf("Accun produit enregistre !");
    }

    getch();
}

void trierParPrix(Produit *produits, int *nbProduits){
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

    afficherCatalogue(produits, nbProduits);
}

void rechercherParCategorie(Produit *produits, int *nbProduits){
    int i;
    int existe=0;
    char categorie[50];

    system("cls"); system("color f0");
    textbackground(7); textcolor(0);

    gotoxy(25,3); cprintf("                                                                        ");
    gotoxy(25,4); cprintf("                          Recherche produits                            ");
    gotoxy(25,5); cprintf("                                                                        ");

    gotoxy(35,7); printf("Categorie                : ");

    textbackground(4);
    gotoxy(10,9); cprintf("ID");
    gotoxy(25,9); cprintf("Nom");
    gotoxy(55,9); cprintf("Categorie");
    gotoxy(80,9); cprintf("Prix");
    gotoxy(95,9); cprintf("Stock");

    gotoxy(60,7); gets(categorie);

    for(i=0 ; i<*nbProduits ; i++){
        if(strcmp((produits+i)->categorie , categorie) == 0){
            afficherProduit(produits, i);
            existe = 1;
        }
    }

    if(existe == 0){
        textbackground(4);
        gotoxy(35,15);   cprintf("Accun produit enregistre !");
    }

    getch();
}

void afficherCatalogue(Produit *produits, int *nbProduits){
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
        gotoxy(35,15);   cprintf("Accun produit enregistre !");

        getch();
        return ;
    }

    for(i=0 ; i<*nbProduits ; i++){
        afficherProduit(produits, i);
    }

    getch();
}

void menuProfil(Client *clients , int *nbClients){
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

        case '1' : creerProfil(clients , nbClients); break;
        case '2' : modifierProfil(clients , nbClients); break;
        case '3' : consulterProfil(clients , nbClients); break;
        case '4' : login(clients , nbClients); break;
        case '5' : logout(clients , nbClients); break;
        case '0' : menuPrincipal(clients , nbClients); break;
    }
}

void menuProduit(Client *clients , int *nbClients){
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
    gotoxy(35,10);   cprintf("             2- Recherche produits                   ");
    gotoxy(35,11);   cprintf("                                                     ");
    gotoxy(35,12);   cprintf("             3- Tri des produits                     ");
    gotoxy(35,13);   cprintf("                                                     ");
    gotoxy(35,14);   cprintf("             4- Détails produit                      ");
    gotoxy(35,15);   cprintf("                                                     ");
    gotoxy(35,16);   cprintf("             5- Produits prédéfinis                  ");
    gotoxy(35,17);   cprintf("                                                     ");
    gotoxy(35,18);   cprintf("             0- Retour menu Principal                ");
    gotoxy(35,19);   cprintf("                                                     ");



    do{
        gotoxy(35,22);   printf("Entrez votre choix : ");
        choix = getch();
    }while(choix!='1' && choix!='2' && choix!='3' && choix!='4' && choix!='5' && choix!='0');

    switch(choix){

        case '1' : menuProfil(clients , nbClients); break;
        case '2' : break;
        case '3' : break;
        case '4' : break;
        case '5' : break;
        case '0' :  exit(0); break;
    }

}


void menuPrincipal(Client *clients , int *nbClients){
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

        case '1' : menuProfil(clients , nbClients); break;
        case '2' : break;
        case '3' : break;
        case '4' : break;
        case '5' : break;
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
    Client *clients = malloc(MAX_CLIENTS*sizeof(Client));
    int nbClients = 0;

    Produit *produits = malloc(MAX_PRODUITS*sizeof(Produit));
    int nbProduits = 0;

    //creerProfil(clients , &nbClients);
    //login(clients , &nbClients);
    //consulterProfil(clients , &nbClients);
    //modifierProfil(clients , &nbClients);
   // consulterProfil(clients , &nbClients );
   // menuPrincipal(clients , &nbClients);
    initialiserProduits(produits, &nbProduits);
    //printf(" le nombre est : %d",nbProduits);
    //afficherCatalogue(produits , &nbProduits);

    //rechercherParNom(produits , &nbProduits);

    trierParPrix(produits, &nbProduits);
    return 0;
}
