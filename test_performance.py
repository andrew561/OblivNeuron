
import subprocess

N = 16
iterations = 20

trials = []
for i in range(iterations):
    subprocess.call(["python3", "gen_data.py", str(N), "test_inputs.dat", \
                     "test_weights.dat"])
    party1 = subprocess.Popen(["make", "test1"], stdin=subprocess.PIPE,
                                stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                close_fds=True)
    party2 = subprocess.Popen(["make", "test2"], stdin=subprocess.PIPE,
                                stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                close_fds=True)
    output = party1.stdout.read().decode()
    target = output.find("elapsed: ") + len("elapsed: ")
    trial = float(output[target:target + 5])
    trials.append(trial)

print("{0:.4f}".format(sum(trials) / len(trials)))

