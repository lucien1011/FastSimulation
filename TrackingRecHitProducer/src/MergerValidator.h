#ifndef FastSimulation_TrackingRecHitProducer_MergerValidator_h
#define FastSimulation_TrackingRecHitProducer_MergerValidator_h

//---------------------------------------------------------------------------
//! \class MergerValidator
//!
//! \brief EDProducer to merge RecHits to model the effect of cluster merging
//!
//---------------------------------------------------------------------------

// Framework
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Data Formats
// #include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSMatchedRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSMatchedRecHit2DCollection.h"
// #include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSMatchedRecHit2DCollection.h"
// #include "DataFormats/GeometryVector/interface/Point3DBase.h"
// #include "DataFormats/GeometrySurface/interface/LocalError.h"
// #include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
// #include "FastSimDataFormats/External/interface/FastTrackerClusterCollection.h"

//ROOT
#include <TH1D.h>
#include <TString.h>

// STL
#include <vector>
#include <map>
#include <string>

namespace edm { 
  class ParameteSrSet;
  class Event;
  class EventSetup;
};

class MergerValidator: public edm::EDAnalyzer
{
public: 
	explicit MergerValidator(edm::ParameterSet const& conf);
	virtual ~MergerValidator();

	float rechitDist(const SiTrackerGSMatchedRecHit2D& rechit1, const SiTrackerGSMatchedRecHit2D& rechit2);

	//process
	void beginRun(edm::Run const& run, const edm::EventSetup & es) override;
	void analyze(edm::Event const& e, const edm::EventSetup& c) override;
	void endRun(edm::Run const& run, const edm::EventSetup & es) override;

private:
	edm::EDGetTokenT<SiTrackerGSMatchedRecHit2DCollection> recHitToken;
	edm::InputTag hitProducer;


	//Histogram variable
	TFile * MergeHisto;
	TH1F * h_RechitDist;
	double histoRechitDistlowx;
	double histoRechitDisthighx;
	bool printout;
	TString outdir;
	

};



#endif
