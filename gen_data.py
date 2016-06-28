
import argparse
import random

parser = argparse.ArgumentParser(description="Generate random weights/inputs for testing neuron functionality.")
parser.add_argument("N", help="number of weights/inputs", type=int)
parser.add_argument("weights_file", help="filename for file containing weights")
parser.add_argument("inputs_file", help="filename for file containing input values")

args = parser.parse_args()

weights_file = open(args.weights_file, 'w')
inputs_file = open(args.inputs_file, 'w')

weights_file.write(str(args.N) + '\n')
for _ in range(args.N):
    rand_weight = (random.random() * 2) - 1
    weights_file.write("{0:.7f}\n".format(rand_weight))
    
inputs_file.write(str(args.N) + '\n')
for _ in range(args.N):
    rand_input = (random.random() * 2) - 1
    inputs_file.write("{0:.7f}\n".format(rand_input))

