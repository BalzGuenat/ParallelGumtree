#!/usr/bin/python2

import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np
import os
import filecmp

sizes = 8 #go up to 8 for larger tests
runs = 1 # 4
max_threads_log = 3
runJava = False

# empty the average linecount file
with open('test_cases/linecount') as file:
	average_linecount = file.readlines()
average_linecount = map(str.strip, average_linecount)
average_linecount = map(int, average_linecount)
del average_linecount[sizes:]
average_linecount = np.array(average_linecount)

# initialize the arrays for the time
parallelGumtreeTimes = np.empty([max_threads_log, runs, sizes])
referenceGumtreeTimes = np.empty([runs, sizes])
for i in range(0,sizes):
	filename1 = 'test_cases/tree_' + str(i) + '_1.test'
	filename2 = 'test_cases/tree_' + str(i) + '_2.test'

	for r in range(0,runs):
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
	
# sort the files by size:
p = average_linecount.argsort() # get the permutation
print "\nPermutation for having the files ordered by size"
print p
average_linecount = average_linecount[p] # reorder the average linecount itself
for r in range(0,runs):
	if runJava:
		referenceGumtreeTimes[r] = referenceGumtreeTimes[r][p]
	for num_threads in range(0,max_threads_log):
		parallelGumtreeTimes[num_threads][r] = parallelGumtreeTimes[num_threads][r][p]
# calculate average for the times and plot everything
print "raw times, first single thread, then parallel"
print parallelGumtreeTimes
if runJava:
	print "raw times of the java reference solution"
	print referenceGumtreeTimes
fig, ax = plt.subplots()
parallel_average_times = np.empty([max_threads_log, sizes])
for t in range(0,max_threads_log):
	parallel_average_times[t] = parallelGumtreeTimes[t].mean(axis=0)
	ax.plot(average_linecount, parallel_average_times[t], marker='o')
if runJava:
	reference_average_times = referenceGumtreeTimes.mean(axis=0)
	ax.plot(average_linecount, reference_average_times, marker='o')
ax.set_ylabel('elapsed time')
ax.set_xlabel('input size')
ax.set_title("input size (in average number of tree lines) vs time")
legend = []
for t in range(0,max_threads_log):
	legend.append(str(2**t) + ' thread(s)')
if runJava:
	legend.append('java reference algorithm')
ax.legend(legend, loc='upper left').draggable()
ax.set_xscale('log')
plt.draw()
plt.savefig('timePlot.png')

if runJava:
	# speedup plots
	print "\nSpeedup vs Java reference"
	fig, ax = plt.subplots()
	for t in range(0,max_threads_log):
		speedup_parallel = reference_average_times/parallel_average_times[t]
		ax.plot(average_linecount, speedup_parallel, marker='o')
		print "average speedup with " + str(2**t) + " threads: " + str(np.mean(speedup_parallel))
	ax.set_ylabel('speedup')
	ax.set_xlabel('input size')
	ax.set_title("speedup compared to the java solution")
	ax.legend(legend[:-1], loc='upper right').draggable()
	plt.draw()
	plt.savefig('speedupPlot.png')

# speedup plots vs the single thread c++ solution
if max_threads_log>1:
	print "\nSpeedup vs single c++ thread"
	fig, ax = plt.subplots()
	for t in range(1,max_threads_log):
		speedup_parallel = parallel_average_times[0]/parallel_average_times[t]
		ax.plot(average_linecount, speedup_parallel, marker='o')
		print "average speedup with " + str(2**t) + " threads: " + str(np.mean(speedup_parallel))
	if runJava:
		speedup_reference = parallel_average_times[0]/reference_average_times
		print "average speedup of the java reference solution: " + str(np.mean(speedup_reference))
		ax.plot(average_linecount, speedup_reference, marker='o')
	ax.set_ylabel('speedup')
	ax.set_xlabel('input size')
	ax.set_title("speedup compared to the single thread c++ solution")
	ax.legend(legend[1:], loc='upper right').draggable()
	plt.draw()
	plt.savefig('speedupPlot_c.png')

# show plots
plt.show()

