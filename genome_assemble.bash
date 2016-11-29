#!/bin/bash

###################################################
#           	 GLOBAL PATHS                     #
###################################################
#FILEMODE=$1
FILENAME=$1
HOMEDIR=/home/ubuntu
CURRENTPATH=$(pwd)
export PATH=$HOMEDIR/nanopolish:$PATH
export PATH=$HOMEDIR/canu/Linux-amd64/bin/:$PATH
export PATH=$HOMEDIR/transrate/:$PATH


if [-z $1 |-z $2] #if either command line argument 1 or argument 2 are empty
	then
		echo "genome_assemble.bash [fasta|gz] [fasta file|tar.gz folder]"
fi
	
###################################################
#             Assembly instructions		  #
###################################################

poretools fasta $CURRENTPATH/$FILENAME > temp.fasta
FILENAME=${FILENAME%%.}

############ Could use soley canu? ###############
########## DONE GETTING FASTA FILES ##############

canu -p $FILENAME -d $FILENAME-nanopore genomeSize='13.8m' gnuplotTested=true -nanopore-raw temp.fasta
### bodge gnuplotTested=true for server errors ###
############ This should assemble ################