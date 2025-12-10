#ifndef HUFFMAN_H_INCLUDED
#define HUFFMAN_H_INCLUDED

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "heap.h"

#define ALPHABET_SIZE 256

/* Construction de l'arbre de Huffman à partir du tableau de fréquences */
Node* construireArbreHuffman(int freq[]);

/* Génération des codes de Huffman pour chaque caractère.
   codes doit être un tableau de ALPHABET_SIZE pointeurs sur char initialisés à NULL.
*/
void genererCodesHuffman(Node *root, char *codes[ALPHABET_SIZE]);

/* Libération des codes */
void libererCodes(char *codes[ALPHABET_SIZE]);

/* Calcul des fréquences d'un fichier texte */
int calculerFrequences(const char *nomFichier, int freq[ALPHABET_SIZE]);

/* Encodage : écrit les fréquences puis la suite de bits '0'/'1' */
int encoderFichier(const char *fichierEntree, const char *fichierSortie);

/* Décodage */
int decoderFichier(const char *fichierEntree, const char *fichierSortie);

#endif /* HUFFMAN_H */


#endif // HUFFMAN_H_INCLUDED
