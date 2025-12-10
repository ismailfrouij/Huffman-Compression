
#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

/*
   Usage :
      ./huffman c entree.txt sortie.huf   -> compresser
      ./huffman d entree.huf sortie.txt   -> décompresser
*/

static void usage(const char *prog) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s c input.txt output.huf   (compresser)\n", prog);
    fprintf(stderr, "  %s d input.huf output.txt   (decompresser)\n", prog);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    char mode = argv[1][0];
    const char *in = argv[2];
    const char *out = argv[3];

    if (mode == 'c') {
        if (!encoderFichier(in, out)) {
            fprintf(stderr, "La compression a échoué.\n");
            return EXIT_FAILURE;
        }
        printf("Compression terminee : %s -> %s\n", in, out);
    } else if (mode == 'd') {
        if (!decoderFichier(in, out)) {
            fprintf(stderr, "La décompression a échoué.\n");
            return EXIT_FAILURE;
        }
        printf("Decompression terminee : %s -> %s\n", in, out);
    } else {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
