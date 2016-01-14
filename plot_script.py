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

with open('dump') as dumpFile:
	(sizes, runs, average_linecount, parallelGumtreeTimes, referenceGumtreeTimes) = pickle.load(dumpFile)
runJava = referenceGumtreeTimes.any()

# sort the files by size:
p = average_linecount.argsort() # get the permutation
average_linecount = average_linecount[p] # reorder the average linecount itself
for r in range(0,runs):
	if runJava:
		referenceGumtreeTimes[r] = referenceGumtreeTimes[r][p]
	for num_threads in range(0,max_threads_log):
		parallelGumtreeTimes[num_threads][r] = parallelGumtreeTimes[num_threads][r][p]
# calculate average for the times and plot everything
fig, ax = plt.subplots()
parallel_average_times = np.empty([max_threads_log, sizes])
parallel_standard_deviation = np.empty([max_threads_log, sizes])
reference_standard_deviation = np.empty([max_threads_log, sizes])
for t in range(0,max_threads_log):
	parallel_average_times[t] = parallelGumtreeTimes[t].mean(axis=0)
	parallel_standard_deviation[t] = parallelGumtreeTimes[t].std(axis=0)
	ax.errorbar(average_linecount, parallel_average_times[t], parallel_standard_deviation[t], marker='o')
if runJava:
	reference_average_times = referenceGumtreeTimes.mean(axis=0)
	reference_standard_deviation = referenceGumtreeTimes.std(axis=0)
	ax.errorbar(average_linecount, reference_average_times, reference_standard_deviation, marker='o')
ax.set_ylabel('elapsed time [seconds]')
ax.set_xlabel('input size [average number of nodes]')
ax.set_title("Input tree size vs execution time")
legend = []
for t in range(0,max_threads_log):
	legend.append('C++, ' + str(2**t) + ' thread(s)')
if runJava:
	legend.append('Reference implementation')
ax.legend(legend, loc='upper left', prop={'size':16}).draggable()
ax.set_xscale('log')
plt.draw()
plt.savefig('timePlot.png', bbox_inches='tight')
plt.savefig('timePlot.eps', bbox_inches='tight')

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
	ax.set_xscale('log')
	plt.draw()
	plt.savefig('speedupPlot.png', bbox_inches='tight')

# speedup plots vs the single thread c++ solution
if max_threads_log>1:
	print "\nSpeedup vs single c++ thread"
	speedup_parallel_standard_deviation = np.empty([max_threads_log, sizes])
	average_speedup_parallel = np.empty([max_threads_log, sizes])
	fig, ax = plt.subplots()
	for t in range(1,max_threads_log):
		speedup_parallel = parallelGumtreeTimes[0]/parallelGumtreeTimes[t]
		average_speedup_parallel[t] = speedup_parallel.mean(axis=0)
		speedup_parallel_standard_deviation[t] = speedup_parallel.std(axis=0)
		ax.errorbar(average_linecount, average_speedup_parallel[t], speedup_parallel_standard_deviation[t], marker='o')
		#ax.plot(average_linecount, speedup_parallel, marker='o')
		print "average speedup with " + str(2**t) + " threads: " + str(np.mean(speedup_parallel))
	if runJava:
		speedup_reference = parallel_average_times[0]/reference_average_times
		print "average speedup of the java reference solution: " + str(np.mean(speedup_reference))
		ax.plot(average_linecount, speedup_reference, marker='o')
	ax.set_ylabel('speedup')
	ax.set_xlabel('input size [average number of nodes]')
	ax.set_title("speedup with multiple threads")
	ax.legend(legend[1:], loc='lower right').draggable()
	ax.set_xscale('log')
	plt.draw()
	plt.savefig('speedupPlot_c.png', bbox_inches='tight')

# show plots
plt.show()
