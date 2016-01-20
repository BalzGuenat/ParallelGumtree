#!/usr/bin/python2
# note that parallelGumtreeTimes[0] respectively ref...[0] contains the match time only,
# and with 1 offset the total time
import sys
import pickle
import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np
import os
import filecmp

max_threads_log = 3
show_speedup = True

with open('dump') as dumpFile:
	(sizes, runs, average_linecount, parallelGumtreeTimes, referenceGumtreeTimes) = pickle.load(dumpFile)
runJava = referenceGumtreeTimes.any()

# sort the files by size:
p = average_linecount.argsort() # get the permutation
average_linecount = average_linecount[p] # reorder the average linecount itself
for i in range(0,2):
	for r in range(0,runs):
		if runJava:
			referenceGumtreeTimes[i][r] = referenceGumtreeTimes[i][r][p]
		for num_threads in range(0,max_threads_log):
			parallelGumtreeTimes[i][num_threads][r] = parallelGumtreeTimes[i][num_threads][r][p]
# calculate average for the times and plot everything
fig, ax = plt.subplots()

ax.set_ylabel('elapsed time [seconds]')
ax.set_xlabel('input size [average number of nodes]')
ax.set_title("Input tree size vs execution time")
mark = ['o', 's']
col = ['r','b']
for i in range(0,2):
	parallel_average_times = np.empty([max_threads_log, sizes])
	parallel_standard_deviation = np.empty([max_threads_log, sizes])
	reference_standard_deviation = np.empty([max_threads_log, sizes])
	label_extra = ", total" if i==1 else ""
	for t in range(0,max_threads_log):
		parallel_average_times[t] = np.nanmean(parallelGumtreeTimes[i][t],axis=0)
		parallel_standard_deviation[t] = np.nanstd(parallelGumtreeTimes[i][t],axis=0)
		ax.errorbar(average_linecount, parallel_average_times[t], parallel_standard_deviation[t], marker=mark[i], color=col[0], label=('C++, ' + str(2**t) + ' thread(s)' + label_extra))
	if runJava:
		reference_average_times = np.nanmean(referenceGumtreeTimes[i],axis=0)
		reference_standard_deviation = np.nanstd(referenceGumtreeTimes[i],axis=0)
		ax.errorbar(average_linecount, reference_average_times, reference_standard_deviation, marker=mark[i], color=col[1], label='Reference implementation' + label_extra)
ax.legend(loc='upper left', prop={'size':16}).draggable()
ax.set_xscale('log')
plt.draw()
plt.savefig('timePlot.png', bbox_inches='tight')
plt.savefig('timePlot.eps', bbox_inches='tight')
if show_speedup:
	# speedup plots vs the single thread c++ solution
	if max_threads_log>1:
		print "\nSpeedup vs single c++ thread"
		fig, ax = plt.subplots()
		for i in range(0,2):
			speedup_parallel_standard_deviation = np.empty([max_threads_log, sizes])
			average_speedup_parallel = np.empty([max_threads_log, sizes])
			label_extra = ", total" if i==1 else ""
			for t in range(1,max_threads_log):
				speedup_parallel = parallelGumtreeTimes[i][0]/parallelGumtreeTimes[i][t]
				average_speedup_parallel[t] = np.nanmean(speedup_parallel,axis=0)
				speedup_parallel_standard_deviation[t] = np.nanstd(speedup_parallel,axis=0)
				ax.errorbar(average_linecount, average_speedup_parallel[t], speedup_parallel_standard_deviation[t], marker=mark[i], color=col[i], label=('C++, ' + str(2**t) + ' thread(s)'+label_extra))
				#ax.plot(average_linecount, speedup_parallel, marker='o')
				if i == 0:
					print "average speedup with " + str(2**t) + " threads for the match time: " + str(np.nanmean(speedup_parallel))
				else:
					print "average speedup with " + str(2**t) + " threads for the total time: " + str(np.nanmean(speedup_parallel))
		ax.set_ylabel('speedup')
		ax.set_xlabel('input size [average number of nodes]')
		ax.set_title("speedup with multiple threads")
		ax.legend(loc='lower right').draggable()
		ax.set_xscale('log')
		plt.draw()
		plt.savefig('speedupPlot_c.png', bbox_inches='tight')
		plt.savefig('speedupPlot_c.eps', bbox_inches='tight')

# show plots
plt.show()
