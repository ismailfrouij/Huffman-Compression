#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED
#ifndef HEAP_H
#define HEAP_H

typedef struct Node {
    char c;                 /* caractère (pour les feuilles) */
    int freq;               /* fréquence d'apparition */
    struct Node *left;      /* fils gauche */
    struct Node *right;     /* fils droit */
} Node;

typedef struct {
    int size;               /* nombre d'éléments actuellement dans le tas */
    int capacity;           /* capacité maximale du tas */
    Node **array;           /* tableau de pointeurs sur Node */
} MinHeap;

/* Création / destruction */
Node* newNode(char c, int freq);
MinHeap* createMinHeap(int capacity);
void freeHuffmanTree(Node *root);
void freeMinHeap(MinHeap *heap);

/* Fonctions utilitaires pour le tas-min */
void swapNode(Node **a, Node **b);
void entasserMin(MinHeap *heap, int i);
Node* extraireTasMin(MinHeap *heap);
void insererTasMin(MinHeap *heap, Node *node);
void construireTasMin(MinHeap *heap);

#endif /* HEAP_H */



#endif // HEAP_H_INCLUDED
