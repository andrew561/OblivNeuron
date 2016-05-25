
// for fixed-point arithmetic
#define SCALE (1 << 16) // 2^16
#define DESCALE(x) x / SCALE // for use in multiplication

typedef struct {
    char *src; // filename for weights/input values
    int n; // number of inputs to neuron
    int output;
} protocolIO;

// for timing execution of protocol
extern double lap;
double wallClock();
const char *mySide();

/* I think this should be in neuron.oh but I'm not entirely certain how
   the Obliv-C linker works...  uncomment this line and delete the
   corresponding line in neuron.oh if the linker complains. */
//void neuron(void *args);

void load_data(protocolIO *io, int **weights, int **inputs, int party);
void check_mem(int *weights, int *inputs, int party);

