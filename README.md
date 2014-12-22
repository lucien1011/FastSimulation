##Setup Hit Merging Package

```shell
cmsrel CMSSW_7_2_0_pre1
cd CMSSW_7_2_0_pre1/src/
cmsenv
git clone git@github.com:lucien1011/FastSimulation.git
scram b -j 8
```

##Run Hit Merging

As an example, look at FastSimulation/Test/:
First, run TTbar_Tauola_13TeV_cfi_GEN_SIM.py to produce a GEN-SIM file:

```shell
cmsRun FastSimulation/Test/TTbar_Tauola_13TeV_cfi_GEN_SIM.py
```

Then, run SingleTracking.py over the output of TTbar_Tauola_13TeV_cfi_GEN_SIM.py

```shell
cmsRun FastSimulation/Test/SingleTracking.py
```

##Changing the merging distance
change the parameter mergedist in here:
```shell
FastSimulation/TrackingRecHitProducer/python/RecHitMerger_cfi.py
```
The default distance is 1.5cm, as shown in this file. This number is just for testing purpose and has to be further investigated.

##Some information

The EDProducer for the hit merger is added to FastSimulation/TrackingRecHitProducer/src/, as RecHitMerger.cc. The function passmerge reads two rechits and compare their distance. If the distance is larger than the mergedist in RecHitMerger_cfi.py, then the two rechits will be merged by the function mergehit, which produces a newhit from two input rechits by averaging their positions and adding their errors by quadrature.

Notice that to run the hit merging I have modified: 
```shell
FastSimulation/Tracking/python/TrackCandidateProducer_cfi.py
FastSimulation/Tracking/python/TrajectorySeedProducer_cfi.py
```

in which the line
```shell
HitProducer = cms.InputTag("siTrackerGaussianSmearingRecHits","TrackerGSMatchedRecHits"),
```
is changed to
```shell
HitProducer = cms.InputTag("siTrackerMergeRecHit","MergeTrackerGSMatchedRecHits"),
```
This is to make sure only the merged hit collection is used for the whole fastsim tracking sequence. If you want to run the standard fastim you have to change it back.
