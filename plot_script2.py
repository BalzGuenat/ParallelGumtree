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
	(testCount, inputSizes, parallelGumtreeTimes, referenceGumtreeTimes) = pickle.load(dumpFile)

# sort the files by size:
p = inputSizes.argsort()
#print p
inputSizes = inputSizes[p]
runJava = referenceGumtreeTimes[0] != 0
if runJava:
	referenceGumtreeTimes = referenceGumtreeTimes[p]
for thrds in range(0, max_threads_log):
		parallelGumtreeTimes[thrds] = parallelGumtreeTimes[thrds][p]


plt.rcParams.update({'font.size': '18'})
fig, ax = plt.subplots()
for t in range(0,max_threads_log):
	ax.plot(inputSizes, parallelGumtreeTimes[t], marker='o', ls='None')
if runJava:
	ax.plot(inputSizes, referenceGumtreeTimes, marker='o', ls='None')
ax.set_ylabel('elapsed time [seconds]')
ax.set_xlabel('input size [average number of nodes]')
ax.set_title("Input tree size vs execution time")
legend = []
for t in range(0,max_threads_log):
	legend.append('C++, ' + str(2**t) + ' thread(s)')
if runJava:
	legend.append('Reference implementation')
ax.legend(legend, loc='upper left', prop={'size':14}).draggable()
ax.set_xscale('log')
ax.set_yscale('log')
plt.draw()
plt.savefig('timePlot.png', bbox_inches='tight')
#plt.savefig('timePlot.eps', bbox_inches='tight')
