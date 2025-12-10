#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

Node* newNode(char c, int freq) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "Erreur d'allocation pour un noeud Huffman\n");
        exit(EXIT_FAILURE);
    }
    node->c = c;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

MinHeap* createMinHeap(int capacity) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!heap) {
        fprintf(stderr, "Erreur d'allocation pour le tas-min\n");
        exit(EXIT_FAILURE);
    }
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (Node**)malloc(capacity * sizeof(Node*));
    if (!heap->array) {
        fprintf(stderr, "Erreur d'allocation pour le tableau du tas-min\n");
        exit(EXIT_FAILURE);
    }
    return heap;
}

void freeHuffmanTree(Node *root) {
    if (!root) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}

void freeMinHeap(MinHeap *heap) {
    if (!heap) return;
    free(heap->array);
    free(heap);
}

void swapNode(Node **a, Node **b) {
    Node *tmp = *a;
    *a = *b;
    *b = tmp;
}

/* entasserMin : restaure la propriété de tas-min à partir de l'indice i */
void entasserMin(MinHeap *heap, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap->size && heap->array[left]->freq < heap->array[smallest]->freq)
        smallest = left;
    if (right < heap->size && heap->array[right]->freq < heap->array[smallest]->freq)
        smallest = right;

    if (smallest != i) {
        swapNode(&heap->array[i], &heap->array[smallest]);
        entasserMin(heap, smallest);
    }
}

/* extraireTasMin : enlève et retourne l'élément de fréquence minimale */
Node* extraireTasMin(MinHeap *heap) {
    if (heap->size == 0) return NULL;
    Node *root = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    entasserMin(heap, 0);
    return root;
}

/* insererTasMin : insère un nouveau noeud dans le tas-min */
void insererTasMin(MinHeap *heap, Node *node) {
    if (heap->size == heap->capacity) {
        /* réallocation si nécessaire */
        heap->capacity *= 2;
        heap->array = (Node**)realloc(heap->array, heap->capacity * sizeof(Node*));
        if (!heap->array) {
            fprintf(stderr, "Erreur de réallocation du tas-min\n");
            exit(EXIT_FAILURE);
        }
    }
    int i = heap->size;
    heap->size++;
    heap->array[i] = node;

    /* remontée pour restaurer la propriété min */
    while (i != 0) {
        int parent = (i - 1) / 2;
        if (heap->array[parent]->freq <= heap->array[i]->freq)
            break;
        swapNode(&heap->array[parent], &heap->array[i]);
        i = parent;
    }
}

/* construireTasMin : construit un tas-min à partir du contenu actuel du tableau */
void construireTasMin(MinHeap *heap) {
    int n = heap->size;
    for (int i = (n - 2) / 2; i >= 0; --i) {
        entasserMin(heap, i);
    }
}
