#include <stdio.h>

#define BUFFER_SIZE 32768

#define LMIN 4
#define LMAX 255

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: compress [input file] [output file]\n");
        return 1;
    }

    FILE *infile = fopen(argv[1], "rb");
    if (!infile) {
        fprintf(stderr, "Could not open input file for reading.\n");
        return 2;
    }

    unsigned char buf[BUFFER_SIZE] = {0};
    size_t fsize = fread(buf, 1, BUFFER_SIZE, infile);

    if (fsize >= BUFFER_SIZE) {
        fprintf(stderr, "The input file is too large for compressing!\n");
        fclose(infile);
        return 3;
    } else if (fsize == 0) {
        fprintf(stderr, "The input file is empty.\n");
        fclose(infile);
        return 3;
    }

    fclose(infile);

    FILE *outfile = fopen(argv[2], "wb");
    if (!outfile) {
        fprintf(stderr, "Could not open output file for writing.\n");
        return 2;
    }

    unsigned char bytes[BUFFER_SIZE] = {0};
    int bi = 0;

    size_t pos = 0;

    while (pos < fsize) {
        unsigned char l = 0, ml = 0;
        int p = 0, mp = 0;

        for (; p < (int)pos; p++) {
            if (l >= LMAX)
                break;

            if (pos + l < fsize && buf[p] == buf[pos + l]) {
                l++;
            } else {
                if (l >= ml) {
                    ml = l;
                    mp = p;
                }

                p -= l;
                l = 0;
            }
        }

        if (l >= ml) {
            ml = l;
            mp = p;
        }

        if (ml >= LMIN) {
            int bs = 0;

            while (bi > 0) {
                int bx = bi;
                if (bx > 128)
                    bx = 128;

                unsigned char b = (1 << 7) | (bx - 1);

                fwrite(&b, 1, 1, outfile);
                fwrite(bytes + bs, 1, bx, outfile);

                bi -= bx;
                bs += bx;
            }

            mp -= ml;

            unsigned char offset[2];
            offset[0] = (mp >> 8) & 0xff;
            offset[1] = mp & 0xff;

            fwrite(offset, 1, 2, outfile);
            fwrite(&ml, 1, 1, outfile);

            pos += ml;
        } else {
            bytes[bi++] = buf[pos++];
        }

        printf("Position: %zu/%zu\n", pos, fsize);
    }

    int bs = 0;

    while (bi > 0) {
        int bx = bi;
        if (bx > 128)
            bx = 128;

        unsigned char b = (1 << 7) | (bx - 1);

        fwrite(&b, 1, 1, outfile);
        fwrite(bytes + bs, 1, bx, outfile);

        bi -= bx;
        bs += bx;
    }

    fclose(outfile);

    return 0;
}
