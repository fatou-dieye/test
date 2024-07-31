#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define TAILLE_MDP 6  // Taille suffisante pour le mot de passe (5 caractères + '\0')
#define FICHIER_UTILISATEURS "utilisateurs.txt"

//by dawg

typedef struct {
    int idCard;
    char mdp[TAILLE_MDP];
    double solde;
    bool etatCmpt;
} Utilisateur;

bool is_valid_id(int idCard) {
    return idCard >= 1000 && idCard <= 9999;
}

bool is_valid_mdp(const char* mdp) {
    if (strlen(mdp) != TAILLE_MDP - 1) return false;
    for (int i = 0; i < TAILLE_MDP - 1; i++) {
        if (!isdigit(mdp[i])) return false;
    }
    return true;
}

bool is_valid_montant(double montant) {
    return montant > 0 && ((int)montant % 1000 == 0);
}

void lire_utilisateurs(Utilisateur utilisateurs[], int* nombreUtilisateurs) {
    FILE* fichier = fopen(FICHIER_UTILISATEURS, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier des utilisateurs.\n");
        return;
    }

    *nombreUtilisateurs = 0;
    while (fscanf(fichier, "%d %s %lf %d", &utilisateurs[*nombreUtilisateurs].idCard,
                  utilisateurs[*nombreUtilisateurs].mdp,
                  &utilisateurs[*nombreUtilisateurs].solde,
                  (int*)&utilisateurs[*nombreUtilisateurs].etatCmpt) == 4) {
        (*nombreUtilisateurs)++;
    }
    fclose(fichier);
}

void ecrire_utilisateurs(Utilisateur utilisateurs[], int nombreUtilisateurs) {
    FILE* fichier = fopen(FICHIER_UTILISATEURS, "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier des utilisateurs.\n");
        return;
    }

    for (int i = 0; i < nombreUtilisateurs; i++) {
        fprintf(fichier, "%d %s %.2f %d\n", utilisateurs[i].idCard,
                utilisateurs[i].mdp,
                utilisateurs[i].solde,
                utilisateurs[i].etatCmpt);
    }
    fclose(fichier);
}

void G4GAB() {
    Utilisateur utilisateurs[100];
    int nombreUtilisateurs;
    lire_utilisateurs(utilisateurs, &nombreUtilisateurs);

    double solde_gab = 500000.0;
    int idCard;
    char mdp[TAILLE_MDP];
    int tentative;
    int choix;
    double montant;
    Utilisateur* utilisateur = NULL;
    bool exitProgram = false;

    do {
        utilisateur = NULL;
        printf("Entrez votre identifiant de carte (4 chiffres) :\n");
        while (scanf("%d", &idCard) != 1 || !is_valid_id(idCard)) {
            printf("Identifiant de carte invalide. Veuillez réessayer :\n");
            while (getchar() != '\n');  // Clear the input buffer
        }

        // Vérifier l'idCard
        for (int i = 0; i < nombreUtilisateurs; i++) {
            if (utilisateurs[i].idCard == idCard) {
                utilisateur = &utilisateurs[i];
                break;
            }
        }

        if (utilisateur == NULL) {
            printf("Identifiant de carte introuvable.\n");
        } else if (utilisateur->etatCmpt) {
            printf("Compte bloqué. Veuillez contacter votre banque.\n");
        } else {
            tentative = 1;
            printf("Entrez votre mot de passe :\n");
            scanf("%s", mdp);

            while (tentative <= 3 && strcmp(mdp, utilisateur->mdp) != 0) {
                printf("Mot de passe incorrect. Tentative %d sur 3.\n", tentative);
                tentative++;
                if (tentative <= 3) {
                    printf("Entrez votre mot de passe :\n");
                    scanf("%s", mdp);
                }
            }

            if (strcmp(mdp, utilisateur->mdp) != 0) {
                printf("Compte bloqué après trois tentatives échouées.\n");
                utilisateur->etatCmpt = true;  // Bloquer le compte
                ecrire_utilisateurs(utilisateurs, nombreUtilisateurs);
            } else {
                printf("Authentification réussie.\n");
                while (true) {
                    printf("1. Vérifier le solde\n");
                    printf("2. Retirer de l'argent\n");
                    printf("3. Annuler\n");
                    printf("4. Retourner au menu principal\n");
                    while (scanf("%d", &choix) != 1 || choix < 1 || choix > 4) {
                        printf("Choix invalide. Veuillez réessayer :\n");
                        while (getchar() != '\n');  // Clear the input buffer
                    }

                    if (choix == 1) {
                        printf("Votre solde est de : %.2f\n", utilisateur->solde);
                    } else if (choix == 2) {
                        while (true) {
                            printf("1. 10 000 F\n");
                            printf("2. 50 000 F\n");
                            printf("3. Autre montant\n");
                            printf("4. Retourner au menu principal\n");
                            while (scanf("%d", &choix) != 1 || choix < 1 || choix > 4) {
                                printf("Choix invalide. Veuillez réessayer :\n");
                                while (getchar() != '\n');  // Clear the input buffer
                            }

                            if (choix == 4) break;

                            if (choix == 1) {
                                montant = 10000.0;
                            } else if (choix == 2) {
                                montant = 50000.0;
                            } else if (choix == 3) {
                                printf("Entrez le montant multiple de 1000 :\n");
                                while (scanf("%lf", &montant) != 1 || !is_valid_montant(montant)) {
                                    printf("Montant invalide. Le montant doit être un multiple de 1000. Veuillez réessayer :\n");
                                    while (getchar() != '\n');  // Clear the input buffer
                                }
                            }

                            if (montant > 0) {
                                if (montant > utilisateur->solde) {
                                    printf("Fonds insuffisants sur le compte.\n");
                                } else {
                                    utilisateur->solde -= montant;
                                    solde_gab -= montant;
                                    printf("Retrait de %.2f effectué avec succès. Solde restant : %.2f\n", montant, utilisateur->solde);
                                    ecrire_utilisateurs(utilisateurs, nombreUtilisateurs);
                                }
                            }
                        }
                    } else if (choix == 3) {
                        printf("Transaction annulée.\n");
                        break;  // Sortir de la boucle du menu après annulation
                    } else if (choix == 4) {
                        break;  // Retourner au menu principal
                    }
                }
            }
        }
    } while (!exitProgram);
}

int main() {
    G4GAB();
    return 0;
}
