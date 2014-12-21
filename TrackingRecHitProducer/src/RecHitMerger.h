#ifndef FastSimulation_TrackingRecHitProducer_RecHitMerger_h
#define FastSimulation_TrackingRecHitProducer_RecHitMerger_h

//---------------------------------------------------------------------------
//! \class RecHitMerger
//!
//! \brief EDProducer to merge RecHits to model the effect of cluster merging
//!
//---------------------------------------------------------------------------

// Framework
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Data Formats
#include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSMatchedRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSMatchedRecHit2DCollection.h"
// #include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSMatchedRecHit2DCollection.h"
#include "DataFormats/GeometryVector/interface/Point3DBase.h"
#include "DataFormats/GeometrySurface/interface/LocalError.h"
// #include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
// #include "FastSimDataFormats/External/interface/FastTrackerClusterCollection.h"

//ROOT
#include <TH1D.h>

// STL
#include <vector>
#include <map>
#include <string>

namespace edm { 
  class ParameterSet;
  class Event;
  class EventSetup;
};

class RecHitMerger: public edm::EDProducer
{
public: 
	explicit RecHitMerger(edm::ParameterSet const& conf);
	virtual ~RecHitMerger();

	//merge-hit function
	bool passmerge(const SiTrackerGSMatchedRecHit2D& rechit1, const SiTrackerGSMatchedRecHit2D& rechit2);
	double mergedist;

	//process
	void beginRun(edm::Run const& run, const edm::EventSetup & es) override;
	void produce(edm::Event& e, const edm::EventSetup& c) override;
	void endRun(edm::Run const& run, const edm::EventSetup & es) override;

private:
	edm::EDGetTokenT<SiTrackerGSMatchedRecHit2DCollection> recHitToken;
	edm::InputTag hitProducer;
	bool printout;
	

};



#endif
