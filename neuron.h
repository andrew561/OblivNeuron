
// for fixed-point arithmetic
#define SCALE (1 << 16) // 2^16, used for conversion
#define DESCALE(x) x / SCALE // for use in multiplication

typedef struct {
    char *src; // filename for weights/input values
    int n; // number of inputs to neuron
    int output;
} protocolIO;

void load_data(protocolIO *io, int **weights, int **inputs, int party);

