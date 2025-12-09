#include <stdio.h>
#include <stdbool.h>
#include<stdlib.h>
#include <time.h>
#include <stddef.h>
#include <string.h>
struct commande {
    int mvt; //1=AVANCER,2=RECULER,3=TOURNER GAUCHE,4=TOURNER DROITE,0=ARRET URGENCE
    int vitesse;// en km/h
    int duree;// en secondes
    bool urg;// vrai si la commande est urgente
};

char* nom_mouvement(int i){
    switch(i){
        case 0: return "ARRET URGENCE"; 
        case 1: return "AVANCER";
        case 2: return "RECULER";
        case 3: return "TOURNER GAUCHE";
        case 4: return "TOURNER DROITE";
        default: return "INVALIDE";
    }
}
char* symbole_mouvement(int i){
    switch(i){
        case 0: return "x ";//arret d'urgence
        case 1: return "> ";//avancer
        case 2: return "< ";//reculer
        case 3: return "^ ";//gauche
        case 4: return "v ";//droite
        default: return "?";
    }
}

typedef struct Noeud {
    struct commande val;
    struct Noeud *suivant;
} Noeud;

typedef struct file {
    Noeud *tete;
    Noeud *queue;
    int taille;
} file;

void initialiser(file *f){
    f->tete = NULL;                            
    f->queue = NULL;
    f->taille = 0;
}
bool estVide(file *f){
    return (f->taille == 0);
}

void enfiler(file *f, struct commande c){
    if(f->taille == 100){
        printf("Erreur : la file est pleine !\n");
        return;
    }
    Noeud *nouveau = (Noeud*) malloc(sizeof(Noeud));
    nouveau->val = c;
    nouveau->suivant = NULL;

    if(f->tete == NULL){
        f->tete = f->queue = nouveau;
    } else {
        f->queue->suivant = nouveau;
        f->queue = nouveau;
    }
    f->taille++;
}

struct commande defiler(file *f){
    struct commande c;
    if(estVide(f)){
        c = (struct commande){ .mvt=-1, .vitesse=-1, .duree=-1, .urg=false };
        return c;
    }
    Noeud *p = f->tete;
    c = p->val;
    f->tete = f->tete->suivant;
    if(f->tete == NULL) f->queue = NULL;
    free(p);
    f->taille--;
    return c;
}

typedef struct pile {
    Noeud *sommet;
} pile;
void init_pile(pile *p){
    p->sommet = NULL;
}

void empiler(pile *p, struct commande c){
    Noeud *nouveau = malloc(sizeof(Noeud));
    nouveau->val = c;
    nouveau->suivant = p->sommet;
    p->sommet = nouveau;
}

struct commande depiler(pile *p){
    struct commande c;
    if(p->sommet == NULL){
        c = (struct commande){ .mvt=-1, .vitesse=-1, .duree=-1, .urg=false };
        return c;
    }
    Noeud *q = p->sommet;
    c = q->val;
    p->sommet = p->sommet->suivant;
    free(q);
    return c;
}

struct commande sommet(pile *p){
    if(p->sommet == NULL){
        struct commande c = { .mvt=-1, .vitesse=-1, .duree=-1, .urg=false };
        return c;
    }
    return p->sommet->val;
}

typedef struct Node {
    int val;
    struct Node *left;
    struct Node *right;
} Node;

Node* createNode(int x){
    Node *n = (Node*) malloc(sizeof(Node));
    n->val = x;
    n->left = n->right = NULL;
    return n;
}

Node* insert(Node *root, int x){
    if(root == NULL) return createNode(x);
    if(x < root->val) {
        root->left = insert(root->left, x);
    }
    else {
        root->right = insert(root->right, x);
    }
    return root;
}


int max_vitesse(Node *root){
    if(root == NULL) return -1;
    Node *cur = root;
    while(cur->right) {cur = cur->right;}
    return cur->val;
}

void free_arbre(Node *root){
    if(!root) return;
    free_arbre(root->left);
    free_arbre(root->right);
    free(root);
}

void ajouter_commande_normale(file *f){
    struct commande c;
    c.vitesse=(rand()%40)+8;
    c.mvt=(rand()%4)+1;
    c.urg=false;
    c.duree=(rand()%7)+1;
    enfiler(f,c);
}
void ajouter_commande_urgente(file *f){
    struct commande c;
    c.vitesse = 0;
    c.mvt = 0;
    c.urg = true;
    c.duree = (rand()%5)+1;
    enfiler(f, c);
}

struct commande commande_contournement(file *P ){
    Noeud* c = P->queue;
    struct commande con;
    if(c==NULL){
    con.vitesse=(rand()%4)+3;
    con.mvt=(rand()%4)+1;
    con.urg=false;
    con.duree=(rand()%7)+1;
    return con;
        }
    int m = c->val.mvt; 
    if (m%2==0){
        con.vitesse=(rand()%5)+3;
        con.mvt=m-1;
        con.urg=false;
        con.duree=(rand()%2)+1;
    }
    else{
        con.vitesse=(rand()%5)+3;
        con.mvt=m+1;
        con.urg=false;
        con.duree=(rand()%2)+1;
    }
    return con;
}
struct commande prendre_prochaine_commande(file *f, file *p){
    if(estVide(f)) return defiler(p);
    else return defiler(f);
}

bool capteur(){
    int k = rand() % 4;
    return (k == 0);
}

void check(file *f, file *p,char ch[250],pile *P1){
    ch[0] = '\0';   
    int i=0;
    int k;
    char *ch1;
    while(i < 4 ){
        if(!capteur()){
         ajouter_commande_normale(p);
          k=p->queue->val.mvt;   
          ch1 = symbole_mouvement(k);
          strcat(ch, ch1);
        }
        else{
         ajouter_commande_urgente(f);
          i++;
           k=f->queue->val.mvt;
           ch1 = symbole_mouvement(k);
            strcat(ch, ch1); 
           struct commande con = commande_contournement(p); 
           enfiler(p,con);
           empiler(P1,con);
           k =p->queue->val.mvt; 
           ch1 = symbole_mouvement(k);
           strcat(ch, ch1);
        
    }

    }
}
void executer(file *f, file *p,pile *p1){
    time_t current_time = time(NULL);
    int choix;
    printf("Entrez le type de commande a afficher (0 = normale, 1 = urgente, 2 = les deux) : ");
    scanf("%d", &choix);
    while(choix!=1 && choix!=2 && choix!=0){
    printf("Entrez le type de commande a afficher (0 = normale, 1 = urgente, 2 = les deux) : ");
    scanf("%d", &choix);
    }
    double vmoy_sum = 0.0;
    int dur = 0;
    int count_normales = 0;
    Node *arbre_vitesse = NULL;
    FILE *fp = fopen("journal_robot.txt", "a");
    while(!estVide(f) || !estVide(p)){
        struct commande c = prendre_prochaine_commande(f, p);
        if(c.mvt == -1) break;
        struct tm *tm_info = localtime(&current_time);
        if(c.urg){
            if(choix == 1 || choix == 2){
                printf("\033[1;31mCOMMANDE URGENTE : ARRET D'URGENCE.\033[0m\n");
                if(fp) fprintf(fp, "****[%02d:%02d:%02d] COMMANDE URGENTE : ARRET D'URGENCE****\n",
                        tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
            }
            current_time += c.duree;
        } 
        else {
            if(choix == 0 || choix == 2){
                printf("Commande normale : %s %ds A %dKM/H.\n",
                       nom_mouvement(c.mvt), c.duree, c.vitesse);
                       fprintf(fp, "[%02d:%02d:%02d] Commande normale : %s %ds A %dKM/H.\n",
                       tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec,
                       nom_mouvement(c.mvt), c.duree, c.vitesse);
            }
            current_time += c.duree;
            vmoy_sum += (double)c.vitesse * (double)c.duree;
            dur += c.duree;
            count_normales++;
            arbre_vitesse = insert(arbre_vitesse, c.vitesse);
        }
    }

    fclose(fp);
    if (choix!=1){
    printf("\nDernier Contournement effectue :\n");
       struct commande c=depiler(p1);
        printf("Commande de contournement : %s %ds A %dKM/H.\n",nom_mouvement(c.mvt), c.duree, c.vitesse);
    }

    if(dur > 0 && (choix == 0 || choix == 2)){
        double vmoy = vmoy_sum / dur;
        double distance = (vmoy * dur) / 3.6;
        int vmax = max_vitesse(arbre_vitesse);

        printf("\nSTATISTIQUES :\n");
        printf("Vitesse moyenne : %.2f KM/H.\n", vmoy);
        printf("Vitesse maximale : %d KM/H.\n", vmax);
        printf("Distance parcourue : %.2f metres.\n", distance);
        printf("Duree de fonctionnement : %ds.\n", dur);
    } 
    

    free_arbre(arbre_vitesse);
}

int main(){
    printf("=============================RoboGesto=========================\n");
    printf("Bienvenue dans la simulation de gestion des commandes d'un robot.\n");
    printf("\n");
    srand(time(NULL));
    file comnormal, comurgent;
    pile comcontournement;
    init_pile(&comcontournement);
    initialiser(&comnormal);
    initialiser(&comurgent);
    char ch[250];
    check(&comurgent, &comnormal,ch,&comcontournement);
    executer(&comurgent, &comnormal,&comcontournement);
    int choix2;
    printf("\nvoulez vous voir la representation graphique de mouvement? (0= non ,1 = oui):");
    scanf("%d",&choix2);
    if (choix2==1){
        printf("%s",ch);
    }
    printf("\n\n=============================================================\n");
     printf("Fin de la simulation.\n");
    return 0;
}
