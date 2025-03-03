/*
 * matrix_rotate.c - A program to rotate PGM images of playing cards.
 * Author: William Lyons
 * Course: CSCI 551
 *
 * Rotates a 690x920 PGM image into a 920x920 output, supporting multiple
 * clockwise and counterclockwise rotations using OpenMP for parallelization.
 *
 * Usage: ./matrix_rotate <inputfile> <outputfile> <thread_count> <rotations_right> <rotations_left>
 *
 * Credits:
 * - Starter code by Dr. Sam Siewert
 * - Rotation algorithms adapted from GeeksforGeeks
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <omp.h>

/* Playing card dimensions (3:4 aspect ratio, upright) */
#define DIMX 690
#define DIMY 920
#define SQDIM ((DIMX > DIMY) ? DIMX : DIMY) /* Square dimension for output */

/* Global variables for runtime configuration */
static int thread_count = 1; /* Number of OpenMP threads */
static int rotations_right = 0; /* Number of clockwise rotations */
static int rotations_left = 0; /* Number of counterclockwise rotations */

/* Pixel array for image data (920x920) */
static unsigned char P[SQDIM][SQDIM];

/* Function prototypes */
void zero_matrix(unsigned char mat[][SQDIM]);
void rotate(unsigned char mat[][SQDIM]);
void rotate_right(unsigned char mat[][SQDIM]);
void rotate_left(unsigned char mat[][SQDIM]);
void read_pgm_header(int fd, char *header);
void read_pgm_data(int fd, unsigned char mat[][SQDIM]);
void write_pgm(int fd, char *header, unsigned char mat[][SQDIM]);

int main(int argc, char *argv[]) {
    int fdin, fdout;
    char header[80];

    /* Validate command-line arguments */
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <inputfile> <outputfile> <threads> <rotations_right> <rotations_left>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Parse runtime parameters */
    sscanf(argv[3], "%d", &thread_count);
    sscanf(argv[4], "%d", &rotations_right);
    sscanf(argv[5], "%d", &rotations_left);

    /* Open input file */
    fdin = open(argv[1], O_RDONLY, 0644);
    if (fdin < 0) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    /* Open output file */
    fdout = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fdout < 0) {
        perror("Error opening output file");
        close(fdin);
        exit(EXIT_FAILURE);
    }

    /* Initialize pixel array */
    zero_matrix(P);

    /* Read PGM data */
    read_pgm_header(fdin, header);
    read_pgm_data(fdin, P);
    close(fdin);

    /* Perform rotations */
    rotate(P);

    /* Update header for 920x920 output */
    header[26] = '9';
    header[27] = '2';
    header[28] = '0';
    header[30] = '9';
    header[31] = '2';
    header[32] = '0';

    /* Write rotated PGM */
    write_pgm(fdout, header, P);
    close(fdout);

    return EXIT_SUCCESS;
}

/*
 * Zero out a 920x920 matrix.
 */
void zero_matrix(unsigned char mat[][SQDIM]) {
    for (int i = 0; i < SQDIM; i++) {
        for (int j = 0; j < SQDIM; j++) {
            mat[i][j] = 0;
        }
    }
}

/*
 * Apply the specified number of right and left rotations.
 */
void rotate(unsigned char mat[][SQDIM]) {
    for (int i = 0; i < rotations_left; i++) {
        rotate_left(mat);
    }
    for (int i = 0; i < rotations_right; i++) {
        rotate_right(mat);
    }
}

/*
 * Rotate the matrix 90 degrees clockwise using OpenMP.
 */
void rotate_right(unsigned char mat[][SQDIM]) {
    struct timespec start, stop;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

    #pragma omp parallel for num_threads(thread_count)
    for (int i = 0; i < SQDIM / 2; i++) {
        for (int j = i; j < SQDIM - i - 1; j++) {
            unsigned char temp = mat[i][j];
            mat[i][j] = mat[SQDIM - 1 - j][i];
            mat[SQDIM - 1 - j][i] = mat[SQDIM - 1 - i][SQDIM - 1 - j];
            mat[SQDIM - 1 - i][SQDIM - 1 - j] = mat[j][SQDIM - 1 - i];
            mat[j][SQDIM - 1 - i] = temp;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &stop);
    elapsed = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;
    printf("Clockwise rotation took %.6f seconds with %d threads.\n", elapsed, thread_count);
}

/*
 * Rotate the matrix 90 degrees counterclockwise using OpenMP.
 */
void rotate_left(unsigned char mat[][SQDIM]) {
    struct timespec start, stop;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

    #pragma omp parallel for num_threads(thread_count)
    for (int i = 0; i < SQDIM / 2; i++) {
        for (int j = i; j < SQDIM - i - 1; j++) {
            unsigned char temp = mat[i][j];
            mat[i][j] = mat[j][SQDIM - 1 - i];
            mat[j][SQDIM - 1 - i] = mat[SQDIM - 1 - i][SQDIM - 1 - j];
            mat[SQDIM - 1 - i][SQDIM - 1 - j] = mat[SQDIM - 1 - j][i];
            mat[SQDIM - 1 - j][i] = temp;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &stop);
    elapsed = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;
    printf("Counterclockwise rotation took %.6f seconds with %d threads.\n", elapsed, thread_count);
}

/*
 * Read the PGM header (assumes 38-byte header).
 */
void read_pgm_header(int fd, char *header) {
    if (read(fd, header, 38) != 38) {
        fprintf(stderr, "Error reading PGM header\n");
        exit(EXIT_FAILURE);
    }
}

/*
 * Read PGM image data into the matrix (690x920).
 */
void read_pgm_data(int fd, unsigned char mat[][SQDIM]) {
    for (int i = 0; i < DIMY; i++) {
        for (int j = 0; j < DIMX; j++) {
            if (read(fd, &mat[i][j], 1) != 1) {
                fprintf(stderr, "Error reading PGM data\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

/*
 * Write the rotated 920x920 PGM image.
 */
void write_pgm(int fd, char *header, unsigned char mat[][SQDIM]) {
    if (write(fd, header, 38) != 38) {
        fprintf(stderr, "Error writing PGM header\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < SQDIM; i++) {
        for (int j = 0; j < SQDIM; j++) {
            if (write(fd, &mat[i][j], 1) != 1) {
                fprintf(stderr, "Error writing PGM data\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}
