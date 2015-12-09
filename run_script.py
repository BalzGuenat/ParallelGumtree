#!/usr/bin/python2

import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np
import os
import filecmp

input_range = range(0, 27)

# empty the average linecount file
with open('test_cases/linecount') as file:
	average_linecount = f.readlines()
average_linecount = map(str.strip, average_linecount)
average_linecount = map(int, average_linecount)

runs = (0,3)

# initialize the arrays for the time
parallelGumtreeTimes = np.empty([len(runs),len(input_range)])
referenceGumtreeTimes = np.empty([len(runs),len(input_range)])
for j in runs:
	for i in input_range:
		filename1 = 'test_cases/tree_' + str(i) + '_1.test'
		filename2 = 'test_cases/tree_' + str(i) + '_2.test'

		# run and time the parallel Gumtree
		tic = time.time()
		subprocess.call(['./ParallelGumtree',filename1, filename2])
		toc = time.time()
		parallelGumtreeTimes[j][i] = (toc-tic)
		
		# run and time the java reference algorithm
		tic = time.time()
		subprocess.call(['java', '-cp', './gumtree.jar', 'com.github.gumtreediff.client.Run', '-c', 'Clients.experimental', 'true', '-c', 'match.gt.minh', '1', '-c', 'match.bu.sim', '0.5', 'testdump', '-g', 'testgen', '-m', 'gumtree', filename1, filename2])
		toc = time.time()
		referenceGumtreeTimes[j][i] = (toc - tic)
		
		# compare if each solution is the same:
		if not filecmp.cmp(filename2 + '.mtch', filename2 + '_parallel.mtch'):
			print "Matches of run " + str(i) + " are not equal!"
# calculate average for the times
parallel_average_times = parallelGumtreeTimes.mean(axis=0)
reference_average_times = referenceGumtreeTimes.mean(axis=0)
# plot everything
plt.plot(average_linecount, parallel_average_times, marker='o')
plt.plot(average_linecount, reference_average_times, marker='o')
plt.ylabel('elapsed time')
plt.xlabel('input size')
plt.legend(['parallel Gumtree', 'java reference algorithm'], loc='upper left')
plt.savefig('aaaplot.png')
plt.show()
