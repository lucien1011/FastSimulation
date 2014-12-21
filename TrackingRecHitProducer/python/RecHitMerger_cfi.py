import FWCore.ParameterSet.Config as cms

siTrackerMergeRecHit = cms.EDProducer("RecHitMerger",
	mergedist = cms.double(1.5),
	HitProducer =  cms.InputTag("siTrackerGaussianSmearingRecHits","TrackerGSMatchedRecHits"),
	printout = cms.bool(False),
	)