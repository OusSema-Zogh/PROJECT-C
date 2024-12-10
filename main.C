#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_nom 50
#define max_actions 100
#define max_plantes 100            
#define ficher_utilisateurs "utilisateurs.txt"

typedef struct {
    char nom[max_nom];
    int quantite;            
} Plantation;

typedef struct {
    char nom[max_nom];
    int prix;
} Graine;

typedef enum {
    GRAINE,
    PLANTE
} TypeItem;

typedef struct Item {
    char nom[max_nom];
    int quantite;
    TypeItem type;  
    struct Item* suivant;
} Item;

typedef struct {
    int argent;
    Item* inventaire;
} Joueur;

typedef struct {
    char type_action[max_nom]; 
    char details[max_nom];     
    int quantite;              
    int montant;
} Action;

typedef struct {
    Action actions[max_actions];
    int top;
} Pile;

void init_pile(Pile* pile) {
    pile->top = -1;
}

int est_vide(Pile* pile) {
    return pile->top == -1;
}

int est_pleine(Pile* pile) {
    return pile->top == max_actions - 1;
}

void empiler(Pile* pile, Action action) {
    if (est_pleine(pile)) {
        printf("La pile est pleine.\n");
        return;
    }
    pile->actions[++(pile->top)] = action;
}

Action depiler(Pile* pile) {
    if (est_vide(pile)) {
        printf("La pile est vide.\n");
        Action vide = {0};
        return vide;
    }
    return pile->actions[(pile->top)--];
}

void copier_chaine(char* ch1, const char* ch2) {
    int i = 0;
    while (ch2[i] != '\0' && i < max_nom - 1) {
        ch1[i] = ch2[i];
        i++;
    }
    ch1[i] = '\0';
}

int comparer_chaines(const char* ch1, const char* ch2) {
    int i = 0;
    while (ch1[i] != '\0' && ch2[i] != '\0') {
        if (ch1[i] != ch2[i]) {
            return 0; 
        }
        i++;
    }
    return ch1[i] == ch2[i];
}

void enregistrer_utilisateur(const char* nom_utilisateur, const char* mdpass) {
    FILE* fichier = fopen(ficher_utilisateurs, "a");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier des utilisateurs.\n");
        return;
    }
    fprintf(fichier, "%s %s\n", nom_utilisateur, mdpass);
    fclose(fichier);
}

int verifier_login(const char* nom_utilisateur, const char* mdpass) {
    FILE* fichier = fopen(ficher_utilisateurs, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier des utilisateurs.\n");
        return 0;
    }

    char nom_utilisateur_fichier[max_nom];
    char mdpass_fichier[max_nom];

    while (fscanf(fichier, "%s %s", nom_utilisateur_fichier, mdpass_fichier) != EOF) {
        if (comparer_chaines(nom_utilisateur, nom_utilisateur_fichier) && comparer_chaines(mdpass, mdpass_fichier)) {
            fclose(fichier);
            return 1; 
        }
    }
    fclose(fichier);
    return 0; 
}

int verifier_utilisateur_existe(const char* nom_utilisateur) {
    FILE* fichier = fopen(ficher_utilisateurs, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier des utilisateurs.\n");
        return 0;
    }

    char nom_utilisateur_fichier[max_nom];

    while (fscanf(fichier, "%s", nom_utilisateur_fichier) != EOF) {
        if (comparer_chaines(nom_utilisateur, nom_utilisateur_fichier)) {
            fclose(fichier);
            return 1; 
        }
    }
    fclose(fichier);
    return 0; 
}

int inscrire_utilisateur() {
    char nom_utilisateur[max_nom], mdpass[max_nom];

    printf("Entrez un nom d'utilisateur: ");
    scanf("%s", nom_utilisateur);

    if (verifier_utilisateur_existe(nom_utilisateur)) {
        printf("Nom d'utilisateur déjà pris. Veuillez en choisir un autre.\n");
        return 0; 
    }
    printf("Entrez un mot de passe: ");
    scanf("%s", mdpass);

    enregistrer_utilisateur(nom_utilisateur, mdpass);
    printf("Inscription réussie ! Vous pouvez maintenant vous connecter.\n");
    return 1; 
}

void afficher_argent(Joueur joueur) {
    printf("Vous avez %d unités d'argent.\n", joueur.argent);
}

void afficher_plantations(Plantation plantations[], int nb_plantes) {
    printf("\nPlantations:\n");
    for (int i = 0; i < nb_plantes; i++) {
        printf("%d. %s - %d graines plantées\n",
               i + 1, plantations[i].nom, plantations[i].quantite);
    }
}

void afficher_graines(Graine graines[]) {
    printf("\nGraines disponibles à l'achat :\n");
    for (int i = 0; i < 8; i++) {
        printf("%d. %s - %d DT\n", i + 1, graines[i].nom, graines[i].prix);
    }
}

void afficher_inventaire(Joueur* joueur) {
    Item* item = joueur->inventaire;
    printf("\nInventaire:\n");
    
    printf("Graines:\n");
    while (item != NULL) {
        if (item->type == GRAINE) {
            printf("%s: %d\n", item->nom, item->quantite);
        }
        item = item->suivant;
    }
    
    item = joueur->inventaire;  
    printf("Plantes:\n");
    while (item != NULL) {
        if (item->type == PLANTE) {
            printf("%s: %d\n", item->nom, item->quantite);
        }
        item = item->suivant;
    }
}

int gestion_connexion_inscription() {
    int choix;
    char nom_utilisateur[max_nom], mdpass[max_nom];

    while (1) {
        printf("1. S'inscrire\n2. Se connecter\n3. Quitter\nVotre choix: ");
        scanf("%d", &choix);

        if (choix == 1) {
            if (inscrire_utilisateur() == 0) {
                continue;  
            }
            return 1;  
        } else if (choix == 2) {
            printf("Entrez votre nom d'utilisateur: ");
            scanf("%s", nom_utilisateur);
            printf("Entrez votre mot de passe: ");
            scanf("%s", mdpass);

            if (verifier_login(nom_utilisateur, mdpass)) {
                printf("Connexion réussie !\n");
                return 1;  
            } else {
                printf("Nom d'utilisateur ou mot de passe incorrect.\n");
                printf("Voulez-vous réessayer ou vous inscrire ?\n");
            }
        } else if (choix == 3) {
            return 0;  
        } else {
            printf("Choix invalide, essayez à nouveau.\n");
        }
    }
}

void acheter_graines(Pile* pile, Joueur* joueur, Graine graines[], Item** inventaire) {
    int choix_graines, quantite;
    afficher_graines(graines);
    printf("Choisissez une graine à acheter (1-%d): ", 8);
    scanf("%d", &choix_graines);
    printf("Combien de graines voulez-vous acheter ? ");
    scanf("%d", &quantite);

    if (choix_graines < 1 || choix_graines > 8) {
        printf("Choix invalide.\n");
        return;
    }

    int total_achat = graines[choix_graines - 1].prix * quantite;

    if (joueur->argent >= total_achat) {
        joueur->argent -= total_achat;
        Action achat;
        strcpy(achat.type_action, "acheter");  
        strcpy(achat.details, graines[choix_graines - 1].nom);   
        achat.quantite = quantite;
        achat.montant = total_achat;
        empiler(pile, achat);

        Item* item = *inventaire;
        while (item != NULL) {
            if (item->type == GRAINE && comparer_chaines(item->nom, graines[choix_graines - 1].nom)) {
                item->quantite += quantite;  
                printf("Quantité mise à jour pour %s. Nouvelle quantité : %d.\n", item->nom, item->quantite);
                return;
            }
            item = item->suivant;
        }

        Item* nouveau_item = (Item*)malloc(sizeof(Item));
        copier_chaine(nouveau_item->nom, graines[choix_graines - 1].nom);
        nouveau_item->quantite = quantite;
        nouveau_item->type = GRAINE;
        nouveau_item->suivant = *inventaire;
        *inventaire = nouveau_item;

        printf("Achat réussi !\n");
    } else {
        printf("Vous n'avez pas assez d'argent.\n");
    }
}

void supprimer_item(Item** inventaire, const char* nom) {
    Item* item = *inventaire;
    Item* precedent = NULL;

    while (item != NULL) {
        if (comparer_chaines(item->nom, nom) && item->quantite == 0) {
            if (precedent == NULL) {
                *inventaire = item->suivant;  
            } else {
                precedent->suivant = item->suivant; 
            }
            free(item);  
            return;
        }
        precedent = item;
        item = item->suivant;
    }
}

void supprimer_plantation(Plantation plantations[], int* nb_plantes, const char* nom) {
    for (int i = 0; i < *nb_plantes; i++) {
        if (comparer_chaines(plantations[i].nom, nom) && plantations[i].quantite == 0) {
            for (int j = i; j < *nb_plantes - 1; j++) {
                plantations[j] = plantations[j + 1];
            }
            (*nb_plantes)--;  
            return;
        }
    }
}

void planter_graines(Pile* pile, Joueur* joueur, Graine graines[], Plantation plantations[], int* nb_plantes) {
    int choix_graine, quantite;
    Item* item_graine = joueur->inventaire;
    int index = 1;
    int trouve = 0;
    
    while (item_graine != NULL) {
        if (item_graine->type == GRAINE && item_graine->quantite > 0) {
            printf("-%s (Quantité: %d)\n", item_graine->nom, item_graine->quantite);
            trouve = 1;
        }
        item_graine = item_graine->suivant;
        index++;
    }

    if (!trouve) {
        printf("Vous n'avez aucune graine disponible à planter.\n");
        return;
    }
    printf("Entrez le numéro de la graine que vous souhaitez planter : ");
    scanf("%d", &choix_graine);

    item_graine = joueur->inventaire;
    index = 1;
    Item* graine_choisie = NULL;
    
    while (item_graine != NULL) {
        if (item_graine->type == GRAINE && item_graine->quantite > 0) {
            if (choix_graine == index) {
                graine_choisie = item_graine;
                break;
            }
            index++;
        }
        item_graine = item_graine->suivant;
    }

    if (graine_choisie == NULL) {
        printf("Choix invalide.\n");
        return;
    }

    printf("Combien de graines voulez-vous planter ? ");
    scanf("%d", &quantite);

    if (graine_choisie->quantite >= quantite) {
        depiler(pile);
        graine_choisie->quantite -= quantite;

        int plantation_existante = 0;
        for (int i = 0; i < *nb_plantes; i++) {
            if (comparer_chaines(plantations[i].nom, graine_choisie->nom)) {
                plantations[i].quantite += quantite;
                
                plantation_existante = 1;
                break;
            }
        }

        if (!plantation_existante) {
            plantations[*nb_plantes].quantite = quantite;
            
            copier_chaine(plantations[*nb_plantes].nom, graine_choisie->nom);
            (*nb_plantes)++;
        }

        if (graine_choisie->quantite == 0) {
            supprimer_item(&joueur->inventaire, graine_choisie->nom);
        }
    } else {
        printf("Pas assez de graines dans l'inventaire.\n");
    }
}

void recolter_plantes(Joueur* joueur, Plantation plantations[], int* nb_plantes, Item** inventaire) {
    int choix_plante, quantite;
    afficher_plantations(plantations, *nb_plantes);

    printf("Choisissez la plante à récolter (1-%d): ", *nb_plantes);
    scanf("%d", &choix_plante);
    printf("Combien de plantes voulez-vous récolter ? ");
    scanf("%d", &quantite);

    if (choix_plante >= 1 && choix_plante <= *nb_plantes) {
        Plantation* plantation = &plantations[choix_plante - 1];

        if (plantation->quantite >= quantite) {
            
            plantation->quantite -= quantite;

            Item* item = *inventaire;
            while (item != NULL) {
                if (item->type == PLANTE && comparer_chaines(item->nom, plantation->nom)) {
                    item->quantite += quantite;
                    return;
                }
                item = item->suivant;
            }

            Item* nouvelle_plante = (Item*)malloc(sizeof(Item));
            copier_chaine(nouvelle_plante->nom, plantation->nom);
            nouvelle_plante->quantite = quantite;
            nouvelle_plante->type = PLANTE;
            nouvelle_plante->suivant = *inventaire;
            *inventaire = nouvelle_plante;

            if (plantation->quantite == 0) {
                supprimer_plantation(plantations, nb_plantes, plantation->nom);
            }
        } else {
            printf("Pas assez de plantes à récolter.\n");
        }
    } else {
        printf("Choix invalide.\n");
    }
}

void vendre_plantes(Joueur* joueur, Item** inventaire) {
    int choix_plante, quantite;
    Item* item_plante = *inventaire;
    int index = 1;
    int trouve = 0;
    
    while (item_plante != NULL) {
        if (item_plante->type == PLANTE && item_plante->quantite > 0) {
            printf("- %s (Quantité: %d)\n", item_plante->nom, item_plante->quantite);
            trouve = 1;
        }
        item_plante = item_plante->suivant;
        index++;
    }

    if (!trouve) {
        printf("Vous n'avez aucune plante disponible à vendre.\n");
        return;
    }

    printf("Entrez le numéro de la plante que vous souhaitez vendre : ");
    scanf("%d", &choix_plante);

    item_plante = *inventaire;
    index = 1;
    Item* plante_choisie = NULL;

    while (item_plante != NULL) {
        if (item_plante->type == PLANTE && item_plante->quantite > 0) {
            if (choix_plante == index) {
                plante_choisie = item_plante;
                break;
            }
            index++;
        }
        item_plante = item_plante->suivant;
    }

    if (plante_choisie == NULL) {
        printf("Choix invalide.\n");
        return;
    }

    printf("Combien de plantes voulez-vous vendre ? ");
    scanf("%d", &quantite);

    if (plante_choisie->quantite >= quantite) {
        
        int prix_vente = 0;

        if (comparer_chaines(plante_choisie->nom, "Carotte")) {
            prix_vente = 70;
        } else if (comparer_chaines(plante_choisie->nom, "Tomate")) {
            prix_vente = 100;
        } else if (comparer_chaines(plante_choisie->nom, "Salade")) {
            prix_vente = 50;
        } else if (comparer_chaines(plante_choisie->nom, "Courgette")) {
            prix_vente = 90;
        } else if (comparer_chaines(plante_choisie->nom, "Poivron")) {
            prix_vente = 120;
        } else if (comparer_chaines(plante_choisie->nom, "Aubergine")) {
            prix_vente = 100;
        } else if (comparer_chaines(plante_choisie->nom, "Haricot")) {
            prix_vente = 80;
        } else if (comparer_chaines(plante_choisie->nom, "Citrouille")) {
            prix_vente = 150;
        } else {
            printf("Plante inconnue.\n");
            return;
        }

        joueur->argent += prix_vente * quantite;

        plante_choisie->quantite -= quantite;

        printf("Vous avez vendu %d %s(s) pour %d dt.\n", quantite, plante_choisie->nom, prix_vente * quantite);

        if (plante_choisie->quantite == 0) {
            supprimer_item(inventaire, plante_choisie->nom);  
        }
    } else {
        printf("Pas assez de plantes à vendre.\n");
    }
}


void annuler_action(Pile* pile, Joueur* joueur, Plantation* plantation, int* nb_plantes, Item** inventaire) {
    if (est_vide(pile)) {
        printf("Aucune action à annuler.\n");
        return;
    }

    Action derniere_action = depiler(pile);

    if (strcmp(derniere_action.type_action, "acheter") == 0) {

        joueur->argent += derniere_action.montant;  

        Item* item = joueur->inventaire;
        while (item != NULL) {
            if (strcmp(item->nom, derniere_action.details) == 0 && item->type == GRAINE) {
                item->quantite -= derniere_action.quantite;
                if (item->quantite == 0) {
                    supprimer_item(&joueur->inventaire, item->nom);  
                }
                return;
            }
            item = item->suivant;
        }
    }
}

int main() {
    Pile pile;
    init_pile(&pile);

    Joueur joueur = {1000, NULL};  
    Graine graines[8] = {
        {"Carotte", 50},
        {"Tomate", 70},
        {"Salade", 30},
        {"Courgette", 60},
        {"Poivron", 90},
        {"Aubergine", 80},
        {"Haricot", 40},
        {"Citrouille", 100}
    };

    Plantation plantations[max_plantes] = {0};
    int nb_plantes = 0;  

    if (gestion_connexion_inscription() == 0) {
        printf("Sortie du jeu.\n");
        return 0;
    }

    int choix;
    while (1) {
        printf("\n      ---MENU---\n1. Afficher l'argent\n2. Acheter des graines\n3. Planter des graines\n4. Récolter des plantes\n5. Afficher inventaire\n6. Vendre des plantes\n7. Annuler l'achat des graines\n8. Quitter\nVotre choix: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                afficher_argent(joueur);
                break;
            case 2:
                acheter_graines(&pile, &joueur, graines, &joueur.inventaire);
                break;
            case 3:
                planter_graines(&pile, &joueur, graines, plantations, &nb_plantes);
                break;
            case 4:
                recolter_plantes(&joueur, plantations, &nb_plantes, &joueur.inventaire);
                break;
            case 5:
                afficher_inventaire(&joueur);
                break;
            case 6:
                vendre_plantes(&joueur, &joueur.inventaire);
                break;
            case 7:
                annuler_action(&pile, &joueur, plantations, &nb_plantes, &joueur.inventaire);
                break;
            case 8:
                printf("Au revoir !\n");
                return 0;
            default:
                printf("Choix invalide, essayez à nouveau.\n");
        }
    }
    return 0;
}