#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>

#define TAILLE_GRILLE 10
#define NB_PAVION 1
#define NB_CROISEUR 1
#define NB_CTORPILLEUR 2
#define NB_TORPILLEUR 1
#define TAILLE_PAVION 5
#define TAILLE_CROISEUR 4
#define TAILLE_CTORPILLEUR 3
#define TAILLE_TORPILLEUR 2

typedef struct CASE {
    int xCase;
    int yCase;
    int etatCase;
}CASE;

typedef struct PAVION {
    CASE casesPAvion[TAILLE_PAVION];
    int etatPAvion;
}PAVION;

typedef struct CROISEUR {
    CASE casesCroiseur[TAILLE_CROISEUR];
    int etatCroiseur
}CROISEUR;

typedef struct CTORPILLEUR {
    CASE casesCTorpilleur[TAILLE_CTORPILLEUR];
    int etatCTorpilleur
}CTORPILLEUR;

typedef struct TORPILLEUR {
    CASE casesTorpilleur[TAILLE_TORPILLEUR];
    int etatTorpilleur
}TORPILLEUR;

typedef struct JOUEUR {
    CASE Plateau[TAILLE_GRILLE][TAILLE_GRILLE];
    PAVION PAvions[NB_PAVION];
    CROISEUR Croiseurs[NB_CROISEUR];
    CTORPILLEUR CTorpilleurs[NB_CTORPILLEUR];
    TORPILLEUR Torpilleurs[NB_TORPILLEUR];
    int nbBateaux;
}JOUEUR;

typedef struct TABLESCORE {
    char nomJoueur[50];
    int score;
}TABLESCORE;


void ecrireChaine(char chaineLue[50], char chaineEcrite[50]);
void lireChaineBinaire(char chaineEcrite[50], FILE* Fichier);
int tailleInt(int nombre);
void afficheTour(int joueurActif);
void afficheTableau(JOUEUR Players[2],int espace, int joueurActif);
JOUEUR placeBateau(JOUEUR Player, int typeBateau, int numeroBateau);
void tir(JOUEUR Players[2], int idDefenseur, int espace, int joueurActif);
void victoire(JOUEUR Players[2], int gagnant, int nbTours, TABLESCORE ScoresActu[10]);
void Jeu(TABLESCORE ScoresActu[10]);
void TableDesScores(TABLESCORE ScoresActu[10]);


void main() {
    TABLESCORE TopDix[10];
    FILE* Scores = NULL;
    int i;
    int choix;

    //On cree le fichier s'il n'a pas ete cree
    Scores = fopen("scores.bin","ab");
    for(i=0; i<10; i++) {
        ecrireChaine("Personne",TopDix[i].nomJoueur);
        TopDix[i].score = -1;
    }

    fclose(Scores);

    Scores = fopen("scores.bin","rb");

    for(i=0; i<10; i++) {
        lireChaineBinaire(TopDix[i].nomJoueur,Scores);
        fread(&TopDix[i].score,sizeof(int),1,Scores);
        //Si aucun score n'est ecrit, on cree un nouveau slot vide
        if(TopDix[i].score == -1) {
            TopDix[i].score = 0;
            ecrireChaine("_________________________________________________",TopDix[i].nomJoueur);
        }
    }

    fclose(Scores);

    do {
        choix = 0;
        printf("Bataille Navale, par Gaetan PIOU\nChoisissez une option\n  1:Jouer\n  2:Table des scores\n  3:Sauvegarder les scores et quitter\n");
        scanf("%d",&choix);
        while(choix<1 || choix>3) {
            scanf("%d",&choix);
        }

        switch(choix) {
            case 1:
                Jeu(TopDix);
                break;
            case 2:
                TableDesScores(TopDix);
                break;
        }
    } while(choix != 3);

    Scores = fopen("scores.bin","wb");

    for(i=0; i<10; i++) {
        ecrireChaineBinaire(TopDix[i].nomJoueur,Scores);
        fwrite(&TopDix[i].score,sizeof(int),1,Scores);
    }

    fclose(Scores);

}


void lireChaineBinaire(char chaineEcrite[50], FILE* Fichier) {
    int i;

    for(i=0; i<50; i++) {
        fread(&chaineEcrite[i],sizeof(char),1,Fichier);
    }
}


void ecrireChaineBinaire(char chaineAEcrire[50], FILE* Fichier) {
    int i=0;

    while(i<50 && chaineAEcrire[i]!="\n") {
        fwrite(&chaineAEcrire[i],sizeof(char),1,Fichier);
        i++;
    }
}



void ecrireChaine(char chaineLue[50], char chaineEcrite[50]) {
    int i;
    for(i=0; i<50.; i++) {
        chaineEcrite[i] = chaineLue[i];
    }
}


int tailleInt(int nombre) {
    int quotient = nombre;
    int taille = 0;
    do {
        quotient = quotient / 10;
        taille++;
    } while(quotient > 0);
    return taille;
}


void afficheTour(int joueurActif) {
    system("cls");
    printf("Joueur %d, a ton tour !\n",joueurActif+1);
    getchar();
}


void afficheTableau(JOUEUR Players[2], int espace, int joueurActif) {
    system("cls");
    int i,j,k;
    int espaceRestant;
    int joueurActu = joueurActif;
    int repetition;

    for(repetition=0; repetition<2; repetition++) {
        if(joueurActu == joueurActif) {
            printf("Vous\n");
        } else {
            printf("Adversaire\n");
        }

        //Ecriture de la premiere colonne de la premiere ligne
        for(k=0; k<espace; k++) {
            printf(" ");
        }

        //Ecriture du reste de la premiere ligne
        for(i=0; i<TAILLE_GRILLE; i++) {
            espaceRestant = espace - tailleInt(i+1);
            printf("|");
            for(k=0; k<espaceRestant; k++) {
                printf(" ");
            }
            printf("%d",i+1);
        }
        printf("\n");

        //Ecriture des autres lignes
        for(j=0; j<TAILLE_GRILLE; j++) {
            espaceRestant = espace - tailleInt(j+1);
            for(k=0; k<espaceRestant; k++) {
                printf(" ");
            }
            printf("%d",j+1);
            for(i=0; i<TAILLE_GRILLE; i++) {
                printf("|");
                espaceRestant = espace - 1;
                for(k=0; k<espaceRestant; k++) {
                    printf(" ");
                }
                switch (Players[joueurActu].Plateau[i][j].etatCase) {
                    case 0:
                        printf(" ");
                        break;
                    case 1:
                        if(joueurActu == joueurActif) {
                            printf("B");
                        } else {
                            printf(" ");
                        }
                        break;
                    case 2:
                        printf("O");
                        break;
                    case 3:
                        printf("X");
                        break;
                }
            }
            printf("\n");
        }

        for(k=0; k<((TAILLE_GRILLE + 1) * (espace+1)); k++) {
            printf("_");
        }
        printf("\n");

        if(joueurActu == 1) {
            joueurActu = 0;
        } else {
            joueurActu = 1;
        }
    }
}


JOUEUR placeBateau(JOUEUR Player, int typeBateau, int numeroBateau) {
    int tailleBateau;
    char nomBateau[50];
    int possible;
    int xbase,ybase;
    int x,y;
    int sens;
    int i;
    JOUEUR NouveauPlayer = Player;

    switch(typeBateau) {
        case 1:
            tailleBateau = 5;
            ecrireChaine("Porte-Avion",nomBateau);
            break;
        case 2:
            tailleBateau = 4;
            ecrireChaine("Croiseur",nomBateau);
            break;
        case 3:
            tailleBateau = 3;
            ecrireChaine("Contre-Torpilleur",nomBateau);
            break;
        case 4:
            tailleBateau = 2;
            ecrireChaine("Torpilleur",nomBateau);
            break;
    }

    do {
        possible = 1;

        //Saisie de la position x du bateau
        printf("Veuillez entrer la position horizontale de votre %s numero %d\n",nomBateau,numeroBateau);
        scanf("%d",&xbase);
        xbase = xbase - 1;
        while(xbase<0 || xbase>9) {
            printf("ERREUR ! Veuillez entrer une position entre 1 et 10\n");
            printf("Veuillez entrer la position horizontale de votre %s numero %d\n",nomBateau,numeroBateau);
            scanf("%d",&xbase);
            xbase = xbase - 1;
        }

        //Saisie de la position y du bateau
        printf("Veuillez entrer la position verticale de votre %s numero %d\n",nomBateau,numeroBateau);
        scanf("%d",&ybase);
        ybase = ybase-1;
        while(ybase<0 || ybase>9) {
            printf("ERREUR ! Veuillez entrer une position entre 1 et 10\n");
            printf("Veuillez entrer la position verticale de votre %s numero %d\n",nomBateau,numeroBateau);
            scanf("%d",&ybase);
            ybase = ybase - 1;
        }

        //Saisie de l'orientation du bateau
        printf("Dans quel sens voulez vous placer votre bateau ?\n  1:Oriente vers le base\n  2:Oriente vers la droite\n  3:Oriente vers le haut\n  4:Oriente vers la gauche\n");
        scanf("%d",&sens);
        while(sens<1 || sens>4) {
            printf("ERREUR ! Veuillez entrer un nombre entre 1 et 4\n");
            printf("Dans quel sens voulez vous placer votre bateau ?\n  1:Oriente vers le base\n  2:Oriente vers la droite\n  3:Oriente vers le haut\n  4:Oriente vers la gauche\n");
            scanf("%d",&sens);
        }

        i=0;
        while(possible==1 && i<tailleBateau) {
            switch(sens) {
                case 1:
                    x = xbase;
                    y = ybase + i;
                    break;
                case 2:
                    x = xbase + i;
                    y = ybase;
                    break;
                case 3:
                    x = xbase;
                    y = ybase - i;
                    break;
                case 4:
                    x = xbase - i;
                    y = ybase;
                    break;
            }
            if(x>=0 && x<=9 && y>=0 && y<=9) {
                if(NouveauPlayer.Plateau[x][y].etatCase != 0) {
                    possible = 0;
                }
            } else {
                possible = 0;
            }
            i++;
        }
        if(possible==0) {
            printf("ERREUR ! Impossible de placer le bateau a cette position et dans ce sens\n");
        }
    } while(possible==0);

    for(i=0; i<tailleBateau; i++) {
        switch(sens) {
            case 1:
                x = xbase;
                y = ybase + i;
                break;
            case 2:
                x = xbase + i;
                y = ybase;
                break;
            case 3:
                x = xbase;
                y = ybase - i;
                break;
            case 4:
                x = xbase - i;
                y = ybase;
                break;
        }
        NouveauPlayer.Plateau[x][y].etatCase = 1;
        switch(typeBateau) {
            case 1:
                NouveauPlayer.PAvions[numeroBateau].casesPAvion[i] = NouveauPlayer.Plateau[x][y];
                break;
            case 2:
                NouveauPlayer.Croiseurs[numeroBateau].casesCroiseur[i] = NouveauPlayer.Plateau[x][y];
                break;
            case 3:
                NouveauPlayer.CTorpilleurs[numeroBateau].casesCTorpilleur[i] = NouveauPlayer.Plateau[x][y];
                break;
            case 4:
                NouveauPlayer.Torpilleurs[numeroBateau].casesTorpilleur[i] = NouveauPlayer.Plateau[x][y];
                break;
        }
    }
    fflush(stdin);
    NouveauPlayer.nbBateaux++;
    return NouveauPlayer;
}


void tir(JOUEUR Players[2], int idDefenseur, int espace, int joueurActif) {
    int xTir, yTir;
    int touche = 0;
    int coule = 1;
    int i,j;


    //Saisie des coordonnees du tir
    printf("Veuillez entrer la position horizontale de votre tir\n");
    scanf("%d",&xTir);
    xTir = xTir - 1;
    while(xTir < 0 || xTir > 9) {
        printf("ERREUR ! Veuillez entrer une position comprise entre 1 et 10 !\n");
        printf("Veuillez entrer la position horizontale de votre tir\n");
        scanf("%d",&xTir);
        xTir = xTir - 1;
    }
    printf("Veuillez entrer la position verticale de votre tir\n");
    scanf("%d",&yTir);
    yTir = yTir - 1;
    while(xTir < 0 || xTir > 9) {
        printf("ERREUR ! Veuillez entrer une position comprise entre 1 et 10 !\n");
        printf("Veuillez entrer la position verticale de votre tir\n");
        scanf("%d",&yTir);
        yTir = yTir - 1;
    }

    while(Players[idDefenseur].Plateau[xTir][yTir].etatCase >= 2) {
        printf("ERREUR ! Vous avez deja tire ici auparavant !\n");
        printf("Veuillez entrer la position horizontale de votre tir\n");
        scanf("%d",&xTir);
        xTir = xTir - 1;
        while(xTir < 0 || xTir > 9) {
            printf("ERREUR ! Veuillez entrer une position comprise entre 1 et 10 !\n");
            printf("Veuillez entrer la position horizontale de votre tir\n");
            scanf("%d",&xTir);
            xTir = xTir - 1;
        }
        printf("Veuillez entrer la position verticale de votre tir\n");
        scanf("%d",&yTir);
        yTir = yTir - 1;
        while(xTir < 0 || xTir > 9) {
            printf("ERREUR ! Veuillez entrer une position comprise entre 1 et 10 !\n");
            printf("Veuillez entrer la position verticale de votre tir\n");
            scanf("%d",&yTir);
            yTir = yTir - 1;
        }
    }

    Players[idDefenseur].Plateau[xTir][yTir].etatCase += 2;
    afficheTableau(Players, espace, joueurActif);


    //Verification

    //Verification des Portes-Avion
    i = 0;
    while(touche==0 && i<NB_PAVION) {
        j = 0;
        while(touche==0 && j<TAILLE_PAVION) {
            if(Players[idDefenseur].Plateau[xTir][yTir].xCase == Players[idDefenseur].PAvions[i].casesPAvion[j].xCase && Players[idDefenseur].Plateau[xTir][yTir].yCase == Players[idDefenseur].PAvions[i].casesPAvion[j].yCase) {
                touche = 1;
                Players[idDefenseur].PAvions[i].casesPAvion[j].etatCase = Players[idDefenseur].Plateau[xTir][yTir].etatCase;
                printf("Touche !\n");
            }
            j++;
        }
        if(touche == 1) {
            j = 0;
            while(coule==1 && j<TAILLE_PAVION) {
                if(Players[idDefenseur].PAvions[i].casesPAvion[j].etatCase!=3) {
                    coule = 0;
                }
                j++;
            }
            if(coule == 1) {
                printf("Coule !\n");
                Players[idDefenseur].nbBateaux--;
            }
        }
        i++;
    }

    //Verification des Croiseurs
    i = 0;
    while(touche==0 && i<NB_CROISEUR) {
        j = 0;
        while(touche==0 && j<TAILLE_CROISEUR) {
            if(Players[idDefenseur].Plateau[xTir][yTir].xCase == Players[idDefenseur].Croiseurs[i].casesCroiseur[j].xCase && Players[idDefenseur].Plateau[xTir][yTir].yCase == Players[idDefenseur].Croiseurs[i].casesCroiseur[j].yCase) {
                touche = 1;
                Players[idDefenseur].Croiseurs[i].casesCroiseur[j].etatCase = Players[idDefenseur].Plateau[xTir][yTir].etatCase;
                printf("Touche !\n");
            }
            j++;
        }
        if(touche == 1) {
            j = 0;
            while(coule==1 && j<TAILLE_CROISEUR) {
                if(Players[idDefenseur].Croiseurs[i].casesCroiseur[j].etatCase!=3) {
                    coule = 0;
                }
                j++;
            }
            if(coule == 1) {
                printf("Coule !\n");
                Players[idDefenseur].nbBateaux--;
            }
        }
        i++;
    }

    //Verification des Contre-Torpilleurs
    i = 0;
    while(touche==0 && i<NB_CTORPILLEUR) {
        j = 0;
        while(touche==0 && j<TAILLE_CTORPILLEUR) {
            if(Players[idDefenseur].Plateau[xTir][yTir].xCase == Players[idDefenseur].CTorpilleurs[i].casesCTorpilleur[j].xCase && Players[idDefenseur].Plateau[xTir][yTir].yCase == Players[idDefenseur].CTorpilleurs[i].casesCTorpilleur[j].yCase) {
                touche = 1;
                Players[idDefenseur].CTorpilleurs[i].casesCTorpilleur[j].etatCase = Players[idDefenseur].Plateau[xTir][yTir].etatCase;
                printf("Touche !\n");
            }
            j++;
        }
        if(touche == 1) {
            j = 0;
            while(coule==1 && j<TAILLE_CTORPILLEUR) {
                if(Players[idDefenseur].CTorpilleurs[i].casesCTorpilleur[j].etatCase!=3) {
                    coule = 0;
                }
                j++;
            }
            if(coule == 1) {
                printf("Coule !\n");
                Players[idDefenseur].nbBateaux--;
            }
        }
        i++;
    }

    //Verification des Torpilleurs
    i = 0;
    while(touche==0 && i<NB_TORPILLEUR) {
        j = 0;
        while(touche==0 && j<TAILLE_TORPILLEUR) {
            if(Players[idDefenseur].Plateau[xTir][yTir].xCase == Players[idDefenseur].Torpilleurs[i].casesTorpilleur[j].xCase && Players[idDefenseur].Plateau[xTir][yTir].yCase == Players[idDefenseur].Torpilleurs[i].casesTorpilleur[j].yCase) {
                touche = 1;
                Players[idDefenseur].Torpilleurs[i].casesTorpilleur[j].etatCase = Players[idDefenseur].Plateau[xTir][yTir].etatCase;
                printf("Touche !\n");
            }
            j++;
        }
        if(touche == 1) {
            j = 0;
            while(coule==1 && j<TAILLE_TORPILLEUR) {
                if(Players[idDefenseur].Torpilleurs[i].casesTorpilleur[j].etatCase!=3) {
                    coule = 0;
                }
                j++;
            }
            if(coule == 1) {
                printf("Coule !\n");
                Players[idDefenseur].nbBateaux--;
            }
        }
        i++;
    }

    fflush(stdin);
    getchar();
    fflush(stdin);
}

void victoire(JOUEUR Players[2], int gagnant, int nbTours, TABLESCORE ScoresActu[10]) {
    int i,j;
    int dommages = 0;
    int nom;
    int score;
    char nomGagnant[50];
    int trouve = 0;

    system("cls");

    for(j=0; j<TAILLE_GRILLE; j++) {
        for (i=0; i<TAILLE_GRILLE; i++) {
            if(Players[gagnant].Plateau[i][j].etatCase == 3) {
                dommages++;
            }
        }
    }

    score = Players[gagnant].nbBateaux * 1000 - (dommages * 100 + nbTours * 10);
    printf("Le joueur %d gagne en %d tours avec encore %d bateaux a flot !\nScore : %d\n",gagnant+1,nbTours,Players[gagnant].nbBateaux,score);

    printf("Joueur %d+1, entrez votre nom\n");
    scanf("%s",&nomGagnant);

    i = 0;

    while(trouve==0 && i<10) {
        if(score > ScoresActu[i].score) {
            for(j=9; j>i; j--) {
                ScoresActu[j] = ScoresActu[j-1];
            }
            ScoresActu[i].score = score;
            ecrireChaine(nomGagnant,ScoresActu[i].nomJoueur);
            trouve = 1;
        }
    }
    i++;
}



void Jeu(TABLESCORE ScoresActu[10]) {
    JOUEUR Player[2];
    int JActif=0;
    int nbEspace = tailleInt(TAILLE_GRILLE) + 1;
    int i,j;
    int gagnant = -1;
    int defenseur;
    int nbTours = 0;

    //Initialisation des deux grilles de joueurs
    for(JActif=0; JActif<2; JActif++) {
        for(j=0; j<TAILLE_GRILLE; j++) {
            for(i=0; i<TAILLE_GRILLE; i++) {
                Player[JActif].Plateau[i][j].xCase = i;
                Player[JActif].Plateau[i][j].yCase = j;
                Player[JActif].Plateau[i][j].etatCase = 0;
                Player[JActif].nbBateaux = 0;
            }
        }
    }


    //Placement des bateaux par les joueurs
    for(JActif=0; JActif<2; JActif++) {
        afficheTour(JActif);

        //Placement des Porte-Avions
        for(i=0; i<NB_PAVION; i++) {
            afficheTableau(Player,nbEspace,JActif);
            Player[JActif] = placeBateau(Player[JActif], 1, i);
        }

        //Placement des Croiseurs
        for(i=0; i<NB_CROISEUR; i++) {
            afficheTableau(Player,nbEspace,JActif);
            Player[JActif] = placeBateau(Player[JActif], 2, i);
        }

        //Placement des Contre-Torpilleurs
        for(i=0; i<NB_CTORPILLEUR; i++) {
            afficheTableau(Player,nbEspace,JActif);
            Player[JActif] = placeBateau(Player[JActif], 3, i);
        }

        //Placement des Torpilleurs
        for(i=0; i<NB_TORPILLEUR; i++) {
            afficheTableau(Player,nbEspace,JActif);
            Player[JActif] = placeBateau(Player[JActif], 4, i);
        }
    }


    JActif = 0;
    defenseur = 1;

    //Phase de jeu
    while(gagnant == -1) {
        afficheTour(JActif);
        afficheTableau(Player,nbEspace,JActif);
        tir(Player, defenseur, nbEspace, JActif);

        if(Player[defenseur].nbBateaux == 0) {
            gagnant = JActif;
        } else {
            defenseur = JActif;
            if(JActif == 1) {
                JActif = 0;
                nbTours++;
            } else {
                JActif = 1;
            }
        }
    }

    victoire(Player, gagnant, nbTours, ScoresActu);


    printf("Fin\n");
}



void TableDesScores(TABLESCORE ScoresActu[10]) {
    int i;
    system("cls");

    for(i=0; i<10; i++) {
        printf("%s %d\n", ScoresActu[i].nomJoueur, ScoresActu[i].score);
    }
}
