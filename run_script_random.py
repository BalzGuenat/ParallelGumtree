#!/usr/bin/python2

#This script runs the randomly generated tests and measures the time needed for the matching algorithm and the total execution time (including parsing and match dumping).
#The results are grouped by input size.

import re
import pickle
import subprocess
import time
import numpy as np

sizes = 5
runs = 1
max_threads_log = 3
runJava = True


timeRegex = re.compile("^Matching Time:\t(\d+)$")
jReGen = re.compile("^Tree generated:\t(\d+)$")
jReMatch = re.compile("^Trees matched:\t(\d+)$")



average_linecount = np.empty([sizes])
# initialize the arrays for the time
parallelGumtreeTimes = np.empty([2,max_threads_log, runs, sizes])
referenceGumtreeTimes = np.empty([2,runs, sizes])
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
			
		if runJava:
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
			print('Match Time: ' + str(t2 - t1) + 'ms, Total Time: ' + str(total_t) + 'ms')
	average_linecount[i] = actualSizes.mean()


with open('dump', 'w+') as dumpFile:
	pickle.dump((sizes, runs, average_linecount, parallelGumtreeTimes, referenceGumtreeTimes), dumpFile)

