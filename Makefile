
exe = neuron
libs = -lm

port = 1234
remote_host = localhost
connection = $(remote_host):$(port)
test_weights = test_weights.dat
test_inputs = test_inputs.dat

cilpath = ~/obliv-c

CFLAGS = -O3 -g -Wall

./a.out: $(exe).oc $(exe).c $(cilpath)/obj/x86_LINUX/libobliv.a
	$(cilpath)/bin/oblivcc $(CFLAGS) -I . $(exe).oc $(exe).c $(libs)

clean:
	rm -f a.out $(exe).oc.cil.i $(exe).oc.i $(exe).oc.cil.c runtime.dat

# Run tests quickly
test1:
	$(./a.out)
	./a.out $(connection) 1 $(test_weights)

test2:
	$(./a.out) 
	./a.out $(connection) 2 $(test_inputs)

