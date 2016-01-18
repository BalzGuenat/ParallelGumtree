#!/usr/bin/python2

#This script runs the electron tests only with the C++ version.

import re
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

filenames = ['electron-01-350c572a8c8b6f035d3cb583dc01fe159137ca63.test',
			 'electron-02-af02739c4e1aea0a115e1bb0c3610d71c0ddf5ab.test',
			 'electron-03-44b8343585ff321bd6f5d88ac82ba55d5d40736d.test',
			 'electron-04-884a8a753fcdea4294c6c5a7f8e6c9fcb47aefd8.test',
			 'electron-05-489539d62e4150c61d42600d847351d90cab7bae.test',
			 'electron-06-10e4698baa94ddb0c1a1537385279875c2cccbe8.test']

files = len(filenames)

timeRegex = re.compile("^Matching Time:\t(\d+)$")
jReGen = re.compile("^Tree generated:\t(\d+)$")
jReMatch = re.compile("^Trees matched:\t(\d+)$")

# initialize the arrays for the time
parallelGumtreeTimes = np.empty([max_threads_log, files, runs])
referenceGumtreeTimes = np.empty([runs, files])
inputSizes = np.empty([files-1])
dirName = 'test_cases/electron/'
for fidx in range(0,files-1):
	filename1 = dirName + filenames[fidx]
	filename2 = dirName + filenames[fidx+1]
	
	totalSize = 0
	totalSize += sum(1 for line in open(filename1))
	totalSize += sum(1 for line in open(filename2))
	inputSizes[fidx] = totalSize / 2
	
	for r in range(0,runs):
		for num_threads in range(0,max_threads_log):
			# run and time the parallel Gumtree
			print('ParallelGumtree with ' + str(2**num_threads) + ' threads')
			output = subprocess.check_output(['./ParallelGumtree', '-num-threads', str(2**num_threads), filename1, filename2])
			print output
			output = output.splitlines()
			for line in output:
				m = timeRegex.match(line)
				if m:
					break
			t = int(m.group(1))
			parallelGumtreeTimes[num_threads][fidx][r] = t
			print('Time:\t' + str(t) + 'ms')

with open('dump', 'w+') as dumpFile:
	pickle.dump((files*runs, inputSizes, parallelGumtreeTimes, referenceGumtreeTimes), dumpFile)
