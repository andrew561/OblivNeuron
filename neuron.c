
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <obliv.h>

#include "neuron.h"
#include "dbg.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        log_info("usage: %s, <hostname:port> <1|2> <filename>\n", argv[0]);
        exit(0);
    }

    const char *remote_host = strtok(argv[1], ":");
    const char *port = strtok(NULL, ":");

    ProtocolDesc pd;
    protocolIO io;

    log_info("Connecting to %s on port %s...\n", remote_host, port);
    if (argv[2][0] == '1') {
        if (protocolAcceptTcp2P(&pd, port) != 0) {
            log_info("TCP accept from %s failed\n", remote_host);
            exit(1);
        }
    } else {
        if (protocolConnectTcp2P(&pd, remote_host, port) != 0) {
            log_info("TCP connect to %s failed\n", remote_host);
            exit(1);
        }
    }

    int current_party = (argv[2][0] == '1' ? 1 : 2);
    setCurrentParty(&pd, current_party);

    // argv[3] is the filename of the data file
    io.src = argv[3];

    // execute the protocol, computing the run time using the wall clock
    double start_time = wall_clock();
    execYaoProtocol(&pd, neuron, &io);
    cleanupProtocol(&pd);
    double time_elapsed = wall_clock() - start_time;

    int num_gates = yaoGateCount();
    log_info("Yao gate count: %u gates\n", num_gates);
    log_info("Time elapsed: %6.4lf seconds\n", time_elapsed);
    log_info("1/(1 + e^<weights, inputs>) = %15.6e\n", (double) DESCALE(io.output));

    return 0;
}

void load_data(protocolIO *io, int **weights, int **inputs, int party) {
    FILE *input_file = fopen(io->src, "r");
    if (input_file == NULL) {
        log_info("File '%s' not found\n", io->src);
        clean_errno();
        exit(1);
    }

    // the first line of the input file should contain the number of weights/inputs
    if (fscanf(input_file, "%d", &(io->n)) <= 0) {
        log_err("First line of file does match file format.  Should contain integer"
                "corresponding to the number of weights/inputs.");
        clean_errno();
        exit(1);
    }

    // allocate space in memory for weights or inputs, depending on the party
    *weights = malloc(io->n * sizeof(int));
    *inputs = malloc(io->n * sizeof(int));

    if ((party == 1 && weights == NULL) || (party == 2 && inputs == NULL)) {
        log_err("Memory allocation failed for party %d", party);
        clean_errno();
        exit(1);
    }

    double a;
    int n = 0; // index of value being scanned
    while (!feof(input_file)) {
        int scan_result = fscanf(input_file, "%lf", &a);

        if (scan_result != 1) {
            if (scan_result < 0 && feof(input_file)) {
                break;
            } else {
                log_err("Input from '%s' does not match file format.  Check input file.\n\t"
                        "File format exception found at line %d or %d in file.\n", io->src,
                        io->n + 1, io->n + 2);
                clean_errno();
                exit(1);
            }
        }

        // convert the float input to a fixed-point representation
        int aint = a * SCALE;

        if (party == 1) {
            (*weights)[n] = aint;
        } else if (party == 2) {
            (*inputs)[n] = aint;
        }

        n += 1;
    }

    log_info("Loading %d data points...\n", io->n);
    fclose(input_file);
}

double wall_clock(void) {
    struct timespec t;
    // clock_gettime is supported by all platforms
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec + (1e-9 * t.tv_nsec);
}


