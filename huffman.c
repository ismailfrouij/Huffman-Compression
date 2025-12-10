#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

static char *dupliquer_chaine(const char *s) {
    size_t len = strlen(s) + 1;
    char *cpy = (char*)malloc(len);
    if (!cpy) {
        fprintf(stderr, "Erreur d'allocation dans dupliquer_chaine\n");
        exit(EXIT_FAILURE);
    }
    memcpy(cpy, s, len);
    return cpy;
}

Node* construireArbreHuffman(int freq[]) {
    /* compter le nombre de caractères distincts */
    int nbDistincts = 0;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (freq[i] > 0)
            nbDistincts++;
    }
    if (nbDistincts == 0) return NULL;
    MinHeap *heap = createMinHeap(nbDistincts);

    /* créer un noeud par caractère et l'insérer dans le tas */
    for (int i = 0, idx = 0; i < ALPHABET_SIZE; ++i) {
        if (freq[i] > 0) {
            heap->array[idx++] = newNode((char)i, freq[i]);
            heap->size++;
        }
    }
    construireTasMin(heap);

    /* combiner les deux noeuds les moins fréquents jusqu'à obtenir un seul noeud */
    while (heap->size > 1) {
        Node *gauche = extraireTasMin(heap);
        Node *droite = extraireTasMin(heap);
        Node *parent = newNode('\0', gauche->freq + droite->freq);
        parent->left = gauche;
        parent->right = droite;
        insererTasMin(heap, parent);
    }
    Node *racine = extraireTasMin(heap);
    freeMinHeap(heap);
    return racine;
}

/* fonction récursive utilitaire pour construire les codes */
static void genererCodesRec(Node *root, char *codes[ALPHABET_SIZE],
                            char *buffer, int profondeur) {
    if (!root) return;

    /* feuille : on enregistre le code terminé */
    if (!root->left && !root->right) {
        buffer[profondeur] = '\0';
        codes[(unsigned char)root->c] = dupliquer_chaine(buffer);
        return;
    }
    /* gauche -> ajouter '0' */
    buffer[profondeur] = '0';
    genererCodesRec(root->left, codes, buffer, profondeur + 1);
    /* droite -> ajouter '1' */
    buffer[profondeur] = '1';
    genererCodesRec(root->right, codes, buffer, profondeur + 1);
}

void genererCodesHuffman(Node *root, char *codes[ALPHABET_SIZE]) {
    char buffer[ALPHABET_SIZE]; /* suffisant pour les profondeurs réalistes */
    genererCodesRec(root, codes, buffer, 0);
}

void libererCodes(char *codes[ALPHABET_SIZE]) {
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (codes[i]) {
            free(codes[i]);
            codes[i] = NULL;
        }
    }
}

int calculerFrequences(const char *nomFichier, int freq[ALPHABET_SIZE]) {
    for (int i = 0; i < ALPHABET_SIZE; ++i) freq[i] = 0;
    FILE *f = fopen(nomFichier, "rb");
    if (!f) {
        perror("Impossible d'ouvrir le fichier source");
        return 0;
    }
    int c;
    int total = 0;
    while ((c = fgetc(f)) != EOF) {
        freq[(unsigned char)c]++;
        total++;
    }
    fclose(f);
    return total;
}

/* Écrit les fréquences sous forme texte : 256 entiers séparés par des espaces + '\n' */
static void ecrireFrequences(FILE *out, int freq[ALPHABET_SIZE]) {
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        fprintf(out, "%d ", freq[i]);
    }
    fprintf(out, "\n");
}

/* Lit les 256 fréquences au début du fichier compressé. */
static int lireFrequences(FILE *in, int freq[ALPHABET_SIZE]) {
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (fscanf(in, "%d", &freq[i]) != 1) {
            return 0;
        }
    }
    /* consommer le '\n' éventuel */
    int c = fgetc(in);
    (void)c;
    return 1;
}

int encoderFichier(const char *fichierEntree, const char *fichierSortie) {
    int freq[ALPHABET_SIZE];
    if (!calculerFrequences(fichierEntree, freq)) {
        return 0;
    }
    Node *racine = construireArbreHuffman(freq);
    if (!racine) {
        fprintf(stderr, "Fichier vide, rien à encoder.\n");
        return 0;
    }
    char *codes[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; ++i) codes[i] = NULL;
    genererCodesHuffman(racine, codes);

    FILE *in = fopen(fichierEntree, "rb");
    if (!in) {
        perror("Impossible d'ouvrir le fichier source");
        libererCodes(codes);
        freeHuffmanTree(racine);
        return 0;
    }
    FILE *out = fopen(fichierSortie, "wb");
    if (!out) {
        perror("Impossible de créer le fichier de sortie");
        fclose(in);
        libererCodes(codes);
        freeHuffmanTree(racine);
        return 0;
    }

    /* écrire les fréquences en en-tête */
    ecrireFrequences(out, freq);

    /* encoder caractère par caractère en 0/1 */
    int c;
    while ((c = fgetc(in)) != EOF) {
        char *code = codes[(unsigned char)c];
        if (!code) continue; /* ne devrait pas arriver */
        fputs(code, out);
    }

    fclose(in);
    fclose(out);
    libererCodes(codes);
    freeHuffmanTree(racine);
    return 1;
}

int decoderFichier(const char *fichierEntree, const char *fichierSortie) {
    int freq[ALPHABET_SIZE];
    FILE *in = fopen(fichierEntree, "rb");
    if (!in) {
        perror("Impossible d'ouvrir le fichier compressé");
        return 0;
    }
    if (!lireFrequences(in, freq)) {
        fprintf(stderr, "Erreur lors de la lecture des fréquences\n");
        fclose(in);
        return 0;
    }
    Node *racine = construireArbreHuffman(freq);
    if (!racine) {
        fprintf(stderr, "Arbre Huffman vide\n");
        fclose(in);
        return 0;
    }
    FILE *out = fopen(fichierSortie, "wb");
    if (!out) {
        perror("Impossible de créer le fichier de sortie décompressé");
        fclose(in);
        freeHuffmanTree(racine);
        return 0;
    }

    /* décodage : on lit les '0' et '1' un par un */
    Node *courant = racine;
    int c;
    while ((c = fgetc(in)) != EOF) {
        if (c == '0') {
            courant = courant->left;
        } else if (c == '1') {
            courant = courant->right;
        } else {
            /* ignorer tout autre caractère (ex: fin de ligne) */
            continue;
        }

        /* si on est sur une feuille, on émet le caractère et on revient à la racine */
        if (courant && !courant->left && !courant->right) {
            fputc(courant->c, out);
            courant = racine;
        }
    }

    fclose(in);
    fclose(out);
    freeHuffmanTree(racine);
    return 1;
}
