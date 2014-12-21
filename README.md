To setup the hit merger package:

cmsrel CMSSW_7_2_0_pre1
cd CMSSW_7_2_0_pre1/src/
cmsenv
git clone git@github.com:lucien1011/FastSimulation.git
scram b -j 8

----------------

To run hit merging, as an example, look at FastSimulation/Test/:
First, run TTbar_Tauola_13TeV_cfi_GEN_SIM.py to produce a GEN-SIM file:
cmsRun FastSimulation/Test/TTbar_Tauola_13TeV_cfi_GEN_SIM.py

Then, run SingleTracking.py over the output of TTbar_Tauola_13TeV_cfi_GEN_SIM.py
cmsRun FastSimulation/Test/SingleTracking.py

----------------

The EDProducer for the hit merger is added to FastSimulation/TrackingRecHitProducer/src/, as RecHitMerger.cc.

----------------

Notice that to run the hit merging I have modified: 
FastSimulation/Tracking/python/TrackCandidateProducer_cfi.py and
FastSimulation/Tracking/python/TrackCandidateProducer_cfi.py

in which the line
HitProducer = cms.InputTag("siTrackerGaussianSmearingRecHits","TrackerGSMatchedRecHits"),
is changed to
HitProducer = cms.InputTag("siTrackerMergeRecHit","MergeTrackerGSMatchedRecHits"),

This is to make sure only the merged hit collection is used for the whole fastsim tracking sequence. If you want to run the standard fastim you have to change it back.
