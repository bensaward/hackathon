#!/bin/bash

###################################################
#           	 GLOBAL PATHS                     #
###################################################
FILEMODE=$1
FILENAME=$2
CURRENTPATH=$(pwd)
export PATH=$HOMEDIR/nanopolish:$PATH  #add nanopolish
export PATH=$HOMEDIR/canu/Linux-amd64/bin/canu:$PATH #add canu
export PATH=$HOMEDIR/


if [-z $1 |-z $2] #if either command line argument 1 or argument 2 are empty
	then
		echo "genome_assemble.bash [fasta|gz] [fasta file|tar.gz folder]"
fi
	


###################################################
#             Assembly instructions		  #
###################################################
if [FILEMODE == "fasta"]
	then
		tar -xvf $CURRENTPATH/$FILENAME
		$FILENAME=${FILENAME%%.tar.gz} #cut the extension from FILENAME
fi

poretools fasta $CURRENTPATH/$FILENAME > temp.fasta

########## DONE GETTING FASTA FILES ##############

