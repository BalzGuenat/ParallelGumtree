#!/usr/bin/python2

import pickle
import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np
import os
import filecmp

sizes = 5
runs = 5
max_threads_log = 3
runJava = True

plt.rcParams.update({'font.size': '18'})

average_linecount = np.empty([sizes])
# initialize the arrays for the time
parallelGumtreeTimes = np.empty([max_threads_log, runs, sizes])
referenceGumtreeTimes = np.empty([runs, sizes])
for i in range(0,sizes):
	dirName = 'test_cases/size_' + str(i)
	actualSizes = np.empty([runs])
	for r in range(0,runs):
		filename1 = dirName + '/tree_' + str(r) + '_1.test'
		filename2 = dirName + '/tree_' + str(r) + '_2.test'
		
		with open(dirName + '/tree_' + str(r) + '.size') as file:
			actualSizes[r] = int(str.strip(file.readlines()[0]))
			
		for num_threads in range(0,max_threads_log):
			# run and time the parallel Gumtree
			print('ParallelGumtree, run ' + str(r+1) + ' on file ' + str(i) + ' with ' + str(2**num_threads) + ' threads')
			tic = time.time()
			subprocess.call(['./ParallelGumtree', '-num-threads', str(2**num_threads), filename1, filename2])
			toc = time.time()
			parallelGumtreeTimes[num_threads][r][i] = (toc-tic)
			print('Time: ' + str(toc-tic))
			
		if runJava:
			# run and time the java reference algorithm
			print('JavaGumtree, run ' + str(r+1) + ' on file ' + str(i))
			tic = time.time()
			subprocess.call(['java', '-cp', './gumtree.jar', 'com.github.gumtreediff.client.Run', '-c', 'Clients.experimental', 'true', '-c', 'match.gt.minh', '1', '-c', 'match.bu.sim', '0.5', 'testdump', '-g', 'testgen', '-m', 'gumtree', filename1, filename2])
			toc = time.time()
			referenceGumtreeTimes[r][i] = (toc - tic)
			print('Time: ' + str(toc-tic))
	average_linecount[i] = actualSizes.mean()


with open('dump', 'w+') as dumpFile:
	pickle.dump((sizes, runs, average_linecount, parallelGumtreeTimes, referenceGumtreeTimes), dumpFile)

