#!/usr/bin/python2

import subprocess
import time
import os

# create test case dir
if not os.path.exists('test_cases'):
	os.makedirs('test_cases')

# open and delete the average linecount file's content
linecount_file = open('test_cases/linecount', 'w+')
linecount_file.seek(0)
linecount_file.truncate()
for i in range(0, 10): # create the test case files and write the linecount
	totalSize = 0
	dirName = 'test_cases/size_' + str(i)
	if not os.path.exists(dirName):
		os.makedirs(dirName)
	for j in range(0,4):
		filename1 = dirName + '/tree_' + str(j) + '_1.test'
		filename2 = dirName + '/tree_' + str(j) + '_2.test'
		errcode = subprocess.call(['./ParallelGumtree', '-create', str(2 ** (10 + i))], stderr=subprocess.STDOUT)
		print(errcode)
		print "finished gen"
		
		# move the files
		wd = os.getcwd()
		os.rename(wd + "/randomPrufer1", filename1)
		os.rename(wd + "/randomPrufer2", filename2)
		
		# print average linecount to file tree_linecount
		totalSize += sum(1 for line in open(filename1))
		totalSize += sum(1 for line in open(filename2))
	linecount_file.write(str(totalSize / 8)+'\n')
linecount_file.close
print "Number of test files: " + str(i+1)