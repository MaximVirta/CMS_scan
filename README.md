#############################################################
HOW TO USE:

Copy all the provided files and folders into the CMSSW 
enviroment.Compile the build with command "scram b". 
(Note that it is important to have all the files in the 
corresponding folders for this to work.) 

After compiling one can start a program with command 
"cmsRun analyzer_cfg.py".

Both, the full minimum bias and HLT, scans use same python
configuration file: "analyzer_cfg.py". Change of the scan 
can be done inside the configuration file.

In the configuration file one must define
1. 	input file 
2. 	output file 
3. 	which plugin to use (HLT or MB)

1. 	The input file is expected to be entered as a index list
	which includes required root files. Give the path to the
	index list to variable "files2011data". You may comment/
	uncomment one of the given index files. (These files
	correspond to minimum bias runs from 2011.
	http://opendata.cern.ch/record/277)

2. 	The output files are given as cms.string arguments for
	cms.EDAnalyzer at the end of the file. 

3. 	A name of the plugin used is also given as cms.string 
	argument for the cms.EDAnalyzer. Here two possible
	plugin names are "TrackSpectrum2011" (Minimum Bias
	scan) and "HLTCheck" (HLT scan). 

The outputs are .dat files in the following csv-format:

"# pt,           eta,            phi             evtN".

In addition one can change the number of events scanned. 
This can be done through "process.maxEvents" variable in 
the configuration file. 
#############################################################