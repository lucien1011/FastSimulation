# Config file to run hit merging on TTBar samples

# Configurables
runMergeHit = True

import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

process.load('Configuration.StandardSequences.Services_cff')
process.load('FastSimulation.Configuration.EventContent_cff')
process.load('FastSimulation.Configuration.FamosSequences_cff')
process.load('FastSimulation.Configuration.Geometries_MC_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

inputFileName = 'TTbar_Tauola_13TeV_cfi_GEN_SIM.root'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source ("PoolSource",
    fileNames=cms.untracked.vstring(inputFileName)
)

# from Configuration.AlCa.GlobalTag import GlobalTag
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

from FastSimulation.Tracking.IterativeTracking_cff import *

process.myTracking = cms.Sequence(
    pixelTracking+pixelVertexing #step0: for fast vertex finding (only needed for HLT later)
    +MeasurementTrackerEvent #just a dummy module, no clue why it is needed
    
    #see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideIterativeTracking
    +iterativeInitialStep #step0
    +iterativeLowPtTripletStep #step1
    +iterativePixelPairStep #step2
    +iterativeDetachedTripletStep #step3
    +iterativeMixedTripletStep #step4
    +iterativePixelLessStep #step5
    +iterativeTobTecStep #step6
    +lastTrackingSteps
)

## Modify input to merged hits
process.load('FastSimulation.TrackingRecHitProducer.RecHitMerger_cfi')
process.siTrackerMergeRecHit.mergedist = cms.double(2.0)

process.p0=cms.Path(
    process.siTrackerGaussianSmearingRecHits*
    process.siTrackerMergeRecHit*
    #either use the 'process.myTracking' sequence below or 'process.iterativeTracking' from the FastSim standard
    process.myTracking*
    process.vertexreco
)

#Output filename
outFileName = 'Merge.root'

process.output=cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(1048576),
    fileName = cms.untracked.string(outFileName),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('')
        #dataTier = cms.untracked.string('RECO')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p0')
    )
)
process.end=cms.EndPath(process.output)
