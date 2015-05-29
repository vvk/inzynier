#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "structures.h"
#include "functions.h"
#include "algorithm.h"

extern int verbosity_level;
extern char output_filename[80];

int main(int argc, char **argv) {

    verbosity_level = RELEASE;
    char **filenames = NULL;
    unsigned num_files = 0;
    bool read_from_stdin;
    bool piezo_verify = false;
    bool compute_positions = false;

    //parse args
    static struct option long_options[] = {
            {"debug",     no_argument,       NULL, 'd'},
            {"all",       no_argument,       NULL, 'a'},
            {"quiet",     no_argument,       NULL, 'q'},
            {"verbose",   required_argument, NULL, 's'},
            {"verify",    no_argument,       NULL, 'v'},
            {"piezo",     no_argument,       NULL, 'v'},
            {"positions", no_argument,       NULL, 'p'},
            {"help",      no_argument,       NULL, 'h'},
            {"output",    required_argument, NULL, 'o'}
    };
    int c;
    while ((c = getopt_long(argc, argv, "dqavphs:o:", long_options, NULL)) !=
           -1) {
        switch (c) {
            case 'd':
                verbosity_level = DEBUG;
                break;
            case 'q':
                verbosity_level = QUIET;
                break;
            case 'a':
                verbosity_level = ALL;
                break;
            case 's':
                if (strcmp(optarg, "debug") == 0 ||
                    strcmp(optarg, "DEBUG") == 0) {
                    verbosity_level = DEBUG;
                }
                else if (strcmp(optarg, "release") == 0 ||
                         strcmp(optarg, "RELEASE") == 0) {
                    verbosity_level = RELEASE;
                }
                else if (strcmp(optarg, "quiet") == 0 ||
                         strcmp(optarg, "QUIET") == 0) {
                    verbosity_level = QUIET;
                }
                else if (strcmp(optarg, "all") == 0 ||
                         strcmp(optarg, "ALL") == 0) {
                    verbosity_level = ALL;
                }
                else {
                    printf("Błędna flaga --verbose.\n");
                    usage(EXIT_FAILURE);
                }
                break;
            case 'v':
                piezo_verify = true;
                break;
            case 'p':
                compute_positions = true;
                break;
            case 'o':
                strcpy(output_filename, optarg);
                break;
            case 'h':
                usage(EXIT_SUCCESS);
                break;
            case '?':
            default:
                usage(EXIT_FAILURE);
                break;
        }
    }

    char *filename = NULL;
    if (optind == argc) {
        //nie podano żadnego pliku jako argument, odczyt z stdin
        read_from_stdin = true;
    }
    else {
        read_from_stdin = false;
        filenames = &argv[optind];
        num_files = argc - optind;
    }

    data_vector_t *data = init_data_vector();
    vehicle_data_t vehicle;

    if (read_from_stdin) { //odczyt z stdin
        if (is_verbosity_at_least(DEBUG)) {
            fputs("Odczyt z stdin.\n", stderr);
        }
        while (read_stdin(data) && data->length > 0) {
            if (is_verbosity_at_least(DEBUG)) {
                printf("Załadowano dane. Ilość próbek: %d\n", data->length);
            }

            vehicle = algorithm2(data, piezo_verify, compute_positions);
            handle_output(vehicle, piezo_verify, compute_positions, NULL);

        }
//        else if (verbosity_level != QUIET) {
//            fputs("Błąd w czasie odczytu.\n", stderr);
//        }
    }
    else { //odczyt z plików
        for (unsigned i = 0; i < num_files; i++) {
            filename = filenames[i];

            if (is_verbosity_at_least(DEBUG)) {
                fputs("Odczyt z pliku: ", stderr);
                fputs(filename, stderr);
                fputs("\n", stderr);
            }

            if (read_file(filename, data)) {
                if (is_verbosity_at_least(DEBUG)) {
                    printf("Załadowano dane. Ilość próbek: %d\n", data->length);
                }
                vehicle = algorithm2(data, piezo_verify, compute_positions);
                handle_output(vehicle, piezo_verify, compute_positions,
                              filename);
            }
        }
    }

    clear_data_vector(data);

    return EXIT_SUCCESS;
}