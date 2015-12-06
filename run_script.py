import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np
import os
import filecmp

input_range = range(1, 30)
parallelGumtreeTimes = []
referenceGumtreeTimes = []
average_linecount = []

for i in input_range:
	input_size = str(2**i)
	
	# create trees
	subprocess.call(['./ParallelGumtree', '-create', input_size])
	# rename trees
	os.rename("randomPrufer1", 'tree_' + str(i) + '_1')
	os.rename("randomPrufer2", 'tree_' + str(i) + '_2')
	# get the exact average line count of both trees
	average_linecount.append((sum(1 for line in open('tree_' + str(i) + '_1')) + sum(1 for line in open('tree_' + str(i) + '_2')))/2)
	
	# run and time the parallel Gumtree
	tic = time.clock()
	subprocess.call(['./ParallelGumtree','tree_' + str(i) + '_1', 'tree_' + str(i) + '_2'])
	toc = time.clock()
	parallelGumtreeTimes.append(toc-tic)
	
	# run and time the java reference algorithm
	tic = time.clock()
	subprocess.call(['java', '-cp', './gumtree.jar', 'com.github.gumtreediff.client.Run', '\t-c', 'Clients.experimental', 'true', '\t-c', 'match.gt.minh', '1', '\t-c', 'match.bu.sim', '0.5', '\ttestdump', '\t-g', 'testgen', '\t-m', 'gumtree', 'tree_' + str(i) + '_1', 'tree_' + str(i) + '_2'])
	toc = time.clock()
	referenceGumtreeTimes.append(toc - tic)
	
	# compare if each solution is the same:
	if not filecmp.cmp('tree_' + str(i) + '_2.mtch','tree_' + str(i) + '_2_parallel.mtch'):
		print "Matches of run " + str(i) + " are not equal!"

# plot everything
plt.plot(average_linecount, parallelGumtreeTimes, marker='o')
plt.plot(average_linecount, referenceGumtreeTimes, marker='o')
plt.ylabel('elapsed time')
plt.xlabel('input size')
plt.legend(['parallel Gumtree', 'java reference algorithm'], loc='upper left')
plt.show()
