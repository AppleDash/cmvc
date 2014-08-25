#!/bin/bash
#echo -en "Content-Type: text/html\r\n\r\n"
#rm -f test-c
makeres=$(make 2>&1)
if [ "$?" != "0" ]; then
	echo -en "Status: 500\r\n"
        echo -en "Content-Type: text/plain\r\n"
        echo -en "\r\n"
	echo -en "A compilation error has occured:\n"
	echo -en "$makeres"
	exit 0
fi
output="$(./test-c)"
res="$?"
if [ "$res" != "0" ]; then
	echo -en "Status: 500\r\n"
	echo -en "Content-Type: text/plain\r\n"
	echo -en "\r\n"
	echo -en "Error happened: C program exited with code $res"
	exit 0
fi
echo -en "Content-Type: text/html\r\n"
echo -en "\r\n"
echo -n "$output"
