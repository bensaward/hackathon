#!/bin/bash

###################################################
#           	 GLOBAL PATHS                     #
###################################################

FILENAME=$1
HOMEDIR=#puthomedirhere
export PATH=$HOMEDIR/nanopolish:$PATH
if [-z $1]
	then
		echo "genome_assemble.bash [fasta file]"
fi
	


###################################################
#             Assembly instructions		  #
###################################################


