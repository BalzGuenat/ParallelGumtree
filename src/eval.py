import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np

input_range = range(1, 30)
times = []

for i in input_range:
	input_size = str(2**i)
	tic = time.clock()
	subprocess.call(['./exec',input_size])
	toc = time.clock()
	times.append(toc - tic)


plt.plot(times, np.power(2, input_range))
plt.ylabel('input size')
plt.xlabel('elapsed time')
plt.show()
