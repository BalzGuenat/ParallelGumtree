#!/usr/bin/bash

java -cp ./gumtree.jar com.github.gumtreediff.client.Run \
	-c Clients.experimental true \
	-c match.gt.minh 1 \
	-c match.bu.sim 0.5 \
	testdump \
	-g testgen \
	-m gumtree \
	$1 $2
