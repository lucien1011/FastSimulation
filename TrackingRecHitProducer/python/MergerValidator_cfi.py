import FWCore.ParameterSet.Config as cms

siTrackerMergerValidator = cms.EDAnalyzer("MergerValidator",
	HitProducer =  cms.InputTag("siTrackerGaussianSmearingRecHits","TrackerGSMatchedRecHits"),
	histoRechitDistlowx = cms.double(0.0),
	histoRechitDisthighx = cms.double(2.0),
	outputdir = cms.string("plots/"),
	printout = cms.bool(False),
	)