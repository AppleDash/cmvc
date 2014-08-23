#!/bin/bash
echo -en "Content-Type: text/html\r\n\r\n"
rm -f test-c
res=$(make 2>&1)
if [ "$?" != "0" ]; then
	echo -en "<pre>An error has occured:\n"
	echo -en "$res</pre>"
fi
./test-c
