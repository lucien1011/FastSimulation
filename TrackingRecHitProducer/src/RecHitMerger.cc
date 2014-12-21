/** RecHitMerger.cc
 * --------------------------------------------------------------
 * Description:  see RecHitMerger.h
 * Authors:  Lucien Lo (UofR)
 * History: 28th Oct, 2014 -  initial version
 * --------------------------------------------------------------
 */

//Header file
#include "RecHitMerger.h"

// Framework
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

// ROOT
#include <TFile.h>

// Data Formats
#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "DataFormats/GeometryVector/interface/Basic3DVector.h"

 //Other
 #include <cmath>

RecHitMerger::RecHitMerger(const edm::ParameterSet& conf)
{
	produces<SiTrackerGSMatchedRecHit2DCollection>("MergeTrackerGSMatchedRecHits");
	mergedist = conf.getParameter<double>("mergedist");
	hitProducer = conf.getParameter<edm::InputTag>("HitProducer");
	recHitToken = consumes<SiTrackerGSMatchedRecHit2DCollection>(hitProducer);
	printout = conf.getParameter<bool>("printout");
};

RecHitMerger::~RecHitMerger(){

};


bool RecHitMerger::passmerge(const SiTrackerGSMatchedRecHit2D& rechit1, const SiTrackerGSMatchedRecHit2D& rechit2){

	if (rechit1.det()==rechit2.det()){
		Basic3DVector<float> v1(rechit1.localPosition().x(),rechit1.localPosition().y(),rechit1.localPosition().z());
		Basic3DVector<float> v2(rechit2.localPosition().x(),rechit2.localPosition().y(),rechit2.localPosition().z());
		Basic3DVector<float> v(v1-v2);
		if ((v.mag()<mergedist) and (v.mag() != 0)) {
			return true;
		} else {
			return false;
		};
	} else {
		return false;
	};
};


void RecHitMerger::beginRun(edm::Run const& run, const edm::EventSetup & es){
	std::cout << "Merging Hits" << std::endl;
};

void RecHitMerger::endRun(edm::Run const& run, const edm::EventSetup & es){


};

void RecHitMerger::produce(edm::Event& e, const edm::EventSetup& c){

	edm::Handle<SiTrackerGSMatchedRecHit2DCollection> RecHitCollection;
	e.getByToken(recHitToken, RecHitCollection);
	
	std::map<unsigned, edm::OwnVector<SiTrackerGSMatchedRecHit2D>> MergeRecHits;

	//Clone new collection
	for (SiTrackerGSMatchedRecHit2DCollection::const_iterator ihit=RecHitCollection->begin();ihit!=RecHitCollection->end();++ihit){
		bool shouldmerge = false;
		for (SiTrackerGSMatchedRecHit2DCollection::const_iterator jhit=RecHitCollection->begin();jhit!=RecHitCollection->end();++jhit){
			if (printout){
				std::cout << "--------" << std::endl;
				std::cout << ihit->localPosition().x()+jhit->localPosition().x() << std::endl;
				std::cout << ihit->localPosition().y()+jhit->localPosition().y() << std::endl;
				std::cout << ihit->localPosition().z()+jhit->localPosition().z() << std::endl;
			};
			if (passmerge(*ihit,*jhit) and ihit!=jhit) {
				shouldmerge = true;

				//averaged position
				float newx = 0.5*(ihit->localPosition().x()+jhit->localPosition().x());
				float newy = 0.5*(ihit->localPosition().y()+jhit->localPosition().y());
				float newz = 0.5*(ihit->localPosition().z()+jhit->localPosition().z());

				LocalPoint newpos(newx,newy,newz);

				//add error by quadratrue
				float xx1 = ihit->localPositionError().xx();
				float yy1 = ihit->localPositionError().yy();
				float xx2 = jhit->localPositionError().xx();
				float yy2 = jhit->localPositionError().yy();
				float newxx = xx1+xx2;
				float newyy = yy1+yy2;
				LocalError newerr(newxx,0.0,newyy);

				SiTrackerGSMatchedRecHit2D newhit(newpos, newerr, *(ihit->det()), 
					ihit->simhitId(),ihit->simtrackId(),ihit->eeId(),ihit->cluster(),ihit->simMultX(),ihit->simMultY());
				MergeRecHits[newhit.simtrackId()].push_back(newhit);

			};
		};
		
		if (!shouldmerge){
			MergeRecHits[ihit->simtrackId()].push_back(*(ihit->clone()));
		};

	};
	std::auto_ptr<SiTrackerGSMatchedRecHit2DCollection> MergeRecHitCollection(new SiTrackerGSMatchedRecHit2DCollection);
	std::map<unsigned, edm::OwnVector<SiTrackerGSMatchedRecHit2D>>::const_iterator it = MergeRecHits.begin();
	std::map<unsigned, edm::OwnVector<SiTrackerGSMatchedRecHit2D>>::const_iterator lasthit = MergeRecHits.end();
	for ( ; it != lasthit; ++it){
		MergeRecHitCollection->put(it->first,(it->second).begin(),(it->second).end());
	};

	// output to file
	e.put(MergeRecHitCollection,"MergeTrackerGSMatchedRecHits");


};

