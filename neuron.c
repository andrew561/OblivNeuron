
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <obliv.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("usage: %s, <hostname:port> <1|2> <filename>\n", argv[0]);
        exit(0);
    }

    const char *remote_host = strtok(argv[1], ":");
    const char *port = strtok(NULL, ":");

    ProtocolDesc pd;
    protocolIO io;

    printf("Connecting to %s on port %s...\n", remote_host, port);
    if (argv[2][0] == '1') {
        if (protocolAcceptTcp2P(&pd, port) != 0) {
            printf("TCP accept from %s failed\n", remote_host);
            exit(1);
        }
    } else {
        if (protocolConnectTcp2P(&pd, remote_host, port) != 0) {
            printf("TCP connect to %s failed\n", remote_host);
            exit(1);
        }
    }

    current_party = (argv[2][0] == '1' ? 1 : 2);
    setCurrent(&pd, current_party);

    // argv[3] is the filename of the data file
    io.src = argv[3];

    // execute the protocol, computing the run time using the wall clock
    lap = wallClock();
    execYaoProtocol(&pd, neuron, &io);
    cleanupProtocol(&pd);
    double runtime = wallClock() - lap;

    printf("%s total time: %lf seconds\n",
           current_party == 1 ? "Generator" : "Evaluator", runtime);

    int num_gates = yaoGateCount();
    printf("Yao gate count: %u\n", num_gates);

    printf("\n");
    printf("1/(1 + e^-<x, y>) = %15.6e\n", (double) DESCALE(io.output);
    
    return 0;
}

