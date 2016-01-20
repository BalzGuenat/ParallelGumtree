#!/usr/bin/python2

#This script runs the electron tests only with the C++ version.

import re
import pickle
import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np
import os
from os import listdir
import sys

max_threads_log = 3
runs = 5
runJava = True

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

sizes = len([f for f in listdir(dirName) if f.startswith("size_")])

# initialize the arrays for the time
parallelGumtreeTimes = np.empty([2,max_threads_log, runs, sizes])
parallelGumtreeTimes[:] = numpy.NAN
referenceGumtreeTimes = np.empty([2,runs, sizes])
referenceGumtreeTimes[:] = numpy.NAN
inputSizes = np.empty([files-1])
dirName = 'test_cases/electron/'

lastJavaTime = 0
	
sizes += 12	
for i in range(12,sizes):
	dirName = 'test_cases/size_' + str(i)
	actualSizes = np.empty([runs])
	test_files = [f for f in listdir(dirName) if f.endswith(".test")]
	if test_files > runs:
		print "more test_files in a size folder than runs(?)"
		sys.exit()
	inputSizes[i-12] = 2**i
	for r in range(0,len(test_files)-1):
		filename1 = dirName + test_files[r]
		filename2 = dirName + test_files[r+1]
			
		for num_threads in range(0,max_threads_log):
			# run and time the parallel Gumtree
			print('ParallelGumtree, files ' + filename1 + ' and ' + filename2 + ' with ' + str(2**num_threads) + ' threads')
			tic = time.time()
			output = subprocess.check_output(['./ParallelGumtree', '-num-threads', str(2**num_threads), filename1, filename2])
			toc = time.time()
			errno = 0 #for now
			if errno != 0:
				print('Error: ' + str(errno))
			
			output = output.splitlines()
			for line in output:
				m = timeRegex.match(line)
				if m:
					break
			match_t = int(m.group(1))
			total_t = (toc - tic) * 1000
			parallelGumtreeTimes[0][num_threads][r][i] = match_t
			parallelGumtreeTimes[1][num_threads][r][i] = total_t
			print('Match Time: ' + str(match_t) + 'ms, Total Time: ' + str(total_t) + 'ms')
			
		if runJava and lastJavaTime < 60000:
			# run and time the java reference algorithm
			print('JavaGumtree, run ' + str(r+1) + ' on file ' + str(i))
			tic = time.time()
			output = subprocess.check_output(['java', '-cp', './gumtree.jar', 'com.github.gumtreediff.client.Run', '-c', 'Clients.experimental', 'true', '-c', 'match.gt.minh', '2', '-c', 'match.bu.sim', '0.5', 'testdump', '-g', 'testgen', '-m', 'gumtree', filename1, filename2])
			toc = time.time()
			output = output.splitlines()
			tGenL = jReGen.match(output[1])
			tMatchL = jReMatch.match(output[2])
			t1 = int(tGenL.group(1))
			t2 = int(tMatchL.group(1))
			total_t = (toc - tic) * 1000
			referenceGumtreeTimes[0][r][i] = (t2 - t1)
			referenceGumtreeTimes[1][r][i] = total_t
			lastJavaTime = total_t
			print('Match Time: ' + str(t2 - t1) + 'ms, Total Time: ' + str(total_t) + 'ms')

with open('dump', 'w+') as dumpFile:
	pickle.dump((sizes, runs, inputSizes, parallelGumtreeTimes, referenceGumtreeTimes), dumpFile)