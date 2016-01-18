#!/usr/bin/python2

import sys
import pickle
import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np
import os
import filecmp

max_threads_log = 3
runs = 1
runJava = False

filenames = ['electron-01-350c572a8c8b6f035d3cb583dc01fe159137ca63.test',
			 'electron-04-884a8a753fcdea4294c6c5a7f8e6c9fcb47aefd8.test',
			 'electron-02-af02739c4e1aea0a115e1bb0c3610d71c0ddf5ab.test',
			 'electron-05-489539d62e4150c61d42600d847351d90cab7bae.test',
			 'electron-03-44b8343585ff321bd6f5d88ac82ba55d5d40736d.test',
			 'electron-06-10e4698baa94ddb0c1a1537385279875c2cccbe8.test']

files = len(filenames)

# initialize the arrays for the time
parallelGumtreeTimes = np.empty([max_threads_log, files * runs])
referenceGumtreeTimes = np.empty([files * runs])
inputSizes = np.empty([files * runs])
dirName = 'test_cases/electron/'
testCase = 0
for fidx in range(0,files-1):
	for r in range(0,runs):
		filename1 = dirName + filenames[fidx]
		filename2 = dirName + filenames[fidx+1]
		
		
		totalSize = 0
		totalSize += sum(1 for line in open(filename1))
		totalSize += sum(1 for line in open(filename2))
		
		for num_threads in range(0,max_threads_log):
			# run and time the parallel Gumtree
			print('ParallelGumtree with ' + str(2**num_threads) + ' threads')
			tic = time.time()
			errno = subprocess.call(['./ParallelGumtree', '-num-threads', str(2**num_threads), filename1, filename2])
			if errno != 0:
				print('Error: ' + str(errno))
			toc = time.time()
			parallelGumtreeTimes[num_threads][testCase] = (toc-tic)
			print('Time: ' + str(toc-tic))
			
		if runJava:
			# run and time the java reference algorithm
			print('JavaGumtree')
			tic = time.time()
			subprocess.call(['java', '-cp', './gumtree.jar', 'com.github.gumtreediff.client.Run', '-c', 'Clients.experimental', 'true', '-c', 'match.gt.minh', '2', '-c', 'match.bu.sim', '0.5', 'testdump', '-g', 'testgen', '-m', 'gumtree', filename1, filename2])
			toc = time.time()
			referenceGumtreeTimes[testCase] = (toc - tic)
			print('Time: ' + str(toc-tic))
		testCase += 1

with open('dump', 'w+') as dumpFile:
	pickle.dump((sizes*runs, inputSizes, parallelGumtreeTimes, referenceGumtreeTimes), dumpFile)
