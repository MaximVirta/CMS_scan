import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes
process = cms.Process("analysis")

# **********************************************************************
# set the maximum number of events to be processed                     *
#    this number (argument of int32) is to be modified by the user     *
#    according to need and wish                                        *
#    default is preset to 10000 events                                 *
# **********************************************************************

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )


# define JSON file for 2011 (replace by 2012 version for 2012 data)

goodJSON = 'datasets/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt'

myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',')

# ****************************************************************************
# define the input data set here by inserting the appropriate .txt file list *
# then load the data set                                                     *
# ****************************************************************************

files2011data = FileUtils.loadListFromFile (
       'datasets/CMS_Run2011B_MinimumBias_AOD_12Oct2013-v1_00000_file_index.txt', # Uncomment to use this file
       # 'datasets/CMS_Run2011B_MinimumBias_AOD_12Oct2013-v1_010000_file_index.txt', # Uncomment to use this file
       # 'datasets/CMS_Run2011B_MinimumBias_AOD_12Oct2013-v1_20000_file_index.txt' # Uncomment to use this file
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(*files2011data
    )
)

# apply JSON file
#   (needs to be placed *after* the process.source input file definition!)

process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
process.source.lumisToProcess.extend(myLumis)

# *************************************************
# number of events to be skipped (0 by default)   *
# *************************************************
process.source.skipEvents = cms.untracked.uint32(0)

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'FT_53_LV5_AN1::All'

# give a name of a macro to be used
# then pass process name, dataset name and output filename to the macro
# uncomment and comment the proper parameters to use one of the given macros

process.analysis = cms.EDAnalyzer(
    # 'HLTCheck',
    'TrackSpectrum2011',

    # ********************************************
    # Uncomment one of these when using HLTCheck *
    # ********************************************

    # outputMulti100 = cms.string("outputs/mult100_v1_00000.dat"), 
    # outputMulti80 = cms.string("outputs/mult80_v1_00000.dat"),
    # outputMulti100 = cms.string("outputs/mult100_v1_010000.dat"),
    # outputMulti80 = cms.string("outputs/mult80_v1_010000.dat"),
    # outputMulti100 = cms.string("outputs/mult100_v1_20000.dat"),
    # outputMulti80 = cms.string("outputs/mult80_v1_20000.dat"),

    # *****************************************************
    # Uncomment one of these when using TrackSpectrum2011 *
    # *****************************************************

    outputFile = cms.string("outputs/output_v1_00000.dat"),
    # outputFile = cms.string("outputs/output_v1_010000.dat"),
    # outputFile = cms.string("outputs/output_v1_20000.dat"),

    processName = cms.string("HLT"),
    datasetName = cms.string("MinimumBias")
)

process.p = cms.Path(process.analysis)
