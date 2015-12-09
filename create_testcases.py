#!/usr/bin/python2

import subprocess
import time
import os


steps = 2000
value = 2000
i = 0
# create test case dir
if not os.path.exists('test_cases'):
	os.makedirs('test_cases')

# open and delete the average linecount file's content
linecount_file = open('linecount', 'w+')
linecount_file.seek(0)
linecount_file.truncate()
while i < 400000: # create the test case files and write the linecount
	filename1 = 'test_cases/tree_' + str(i) + '_1.test'
	filename2 = 'test_cases/tree_' + str(i) + '_2.test'
	subprocess.call(['./ParallelGumtree', '-create', str(value)])
	
	# move the files
	os.rename("randomPrufer1", filename1)
	os.rename("randomPrufer2", filename2)
	
	# print average linecount to file tree_linecount
	average_linecount = ((sum(1 for line in open(filename1)) + sum(1 for line in open(filename2)))/2)
	linecount_file.write(str(average_linecount)+'\n')
	value+=steps
	steps+=1000
	i+=1
linecount_file.close
print "Number of test files: " + str(i)