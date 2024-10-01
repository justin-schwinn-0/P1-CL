#!/bin/bash

# Change this to your netid
netid=jts200006

# Your main project name 
PROG=NAME_

CONFIGNAME="testConfig.txt"

# Root directory of your project on the remote location
PROJDIR=/home/010/j/jt/jts200006/${PROG}

# Directory where the config file is located on your local system
CONFIGLOCAL=/home/justin/Desktop/code/${PROG}/${CONFIGNAME}

CONFIGDOCKER=/home/010/j/jt/jts200006/${PROG}/${CONFIGNAME}

# Directory your java classes are in
BINDIR=$PROJDIR/build


n=0

cat $CONFIGLOCAL | sed -e "s/#.*//" | sed -e "/^\s*$/d" |
(
    read i
    echo $i
    while [[ $n -lt $i ]]
    do
    	read line
    	p=$( echo $line | awk '{ print $1 }' )
        host=$( echo $line | awk '{ print $2 }' )
	
	    gnome-terminal -- ssh $netid@$host "cd ${BINDIR}/..;/${BINDIR}/${PROG} ${PROJDIR}/${CONFIGNAME} ${p};" "exec bash" &

        n=$(( n + 1 ))
    done
)
