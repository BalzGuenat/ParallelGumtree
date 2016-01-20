#!/usr/bin/python2
from os import listdir
from shutil import copyfile
import os
dirName = 'test_cases/electron/'
test_files = [f for f in listdir(dirName) if f.endswith(".test")]
# log sizes
for size in range(12,25):
	# create size folder
	dir = dirName + "/size_" + str(size)
	if not os.path.exists(dir):
		os.makedirs(dir)
	for i in range(0,len(test_files)):
		filename = dirName+"/size_" + str(size) + "/" + test_files[i]
		# copy original file
		copyfile(dirName+test_files[i],filename)
		# truncate file
		with open(filename,"r+") as f:
			eof = False
			for _ in xrange(2**size):
				l = f.readline()
				if not l.strip():
					eof = True
					break
			if eof:
				os.remove(filename)
			else:
				f.truncate()
folders = [f for f in listdir(dirName) if f.startswith("size")]
for f in folders:
	try:
		os.rmdir(dirName + f)
	except OSError as ex:
		pass
