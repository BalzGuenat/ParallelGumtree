#!/usr/bin/python2

import subprocess
import time
import os

minSize = 13
sizes = 6
casesPerSize = 5

# create test case dir
if not os.path.exists('test_cases'):
	os.makedirs('test_cases')

for size in range(0, sizes): # create the test case files and write the linecount
	dirName = 'test_cases/size_' + str(size)
	if not os.path.exists(dirName):
		os.makedirs(dirName)
	for case in range(0, casesPerSize):
		filename1 = dirName + '/tree_' + str(case) + '_1.test'
		filename2 = dirName + '/tree_' + str(case) + '_2.test'
		errorcode = -1
		while errorcode != 0:
			errorcode = subprocess.call(['./ParallelGumtree', '-create', str(2 ** (minSize + size))], stderr=subprocess.STDOUT)
			print(errorcode)
		print "finished gen"
		
		# move the files
		wd = os.getcwd()
		os.rename(wd + "/randomPrufer1", filename1)
		os.rename(wd + "/randomPrufer2", filename2)
		
		# print average linecount to file tree_linecount
		totalSize = 0
		totalSize += sum(1 for line in open(filename1))
		totalSize += sum(1 for line in open(filename2))
		# open and delete the average linecount file's content
		linecount_file = open(dirName + '/tree_' + str(case) + '.size', 'w+')
		linecount_file.seek(0)
		linecount_file.truncate()
		linecount_file.write(str(totalSize / 2))
		linecount_file.close
		
print "Number of test files: " + str(sizes*casesPerSize)