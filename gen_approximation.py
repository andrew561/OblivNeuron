
import math
import numpy as np
import pylab
from matplotlib import pyplot
from matplotlib import collections

SCALE = 2**16 # for double to fixed-point conversion

def logistic(x):
    return 1 / (1 + math.e**(-x))

def convert_to_fixed_point_hex(f):
    fp = np.int32(SCALE * f)
    if fp < 0:
        fp = 2**32 + 1 + fp
    output = hex(fp)[2:]
    if output[-1] == 'L':
        output = output[:-1]
    return "0x" + (8 - len(output)) * '0' + output

lower_bound, upper_bound = (-4, +4)
n = 16 # number of linear segments

# generate evenly-spaced points on the function
delta = (upper_bound - lower_bound) / float(n)
points = []
x = lower_bound
for i in range(n + 1):
    y = logistic(x)
    points.append((x, y))
    x += delta

# display the piecewise linear approximation
lines = []
for i in range(len(points) - 1):
    a = (points[i][0], points[i + 1][0])
    b = (points[i][1], points[i + 1][1])
    lines.append(a)
    lines.append(b)
    lines.append('r')
pyplot.plot(*lines)
pylab.show()

# convert list of points to list of lines in (x1, x2, slope, y-intercept) format
lines = []
for i in range(len(points) - 1):
    p1, p2 = points[i], points[i + 1]
    slope = (p2[1] - p1[1]) / (p2[0] - p1[0])
    intercept = p1[1] - (slope * p1[0])
    lines.append((p1[0], p2[0], slope, intercept))

# convert every value in the list on lines to strings of fixed-point hexadecimal
new_lines = []
for line in lines:
    new_lines.append(tuple(map(convert_to_fixed_point_hex, line)))

for line in new_lines:
    print("obliv if (oexponent >= {} && oexponent < {})\n    ooutput = {} * oexponent + {};".format(line[0], line[1], line[2], line[3]))

