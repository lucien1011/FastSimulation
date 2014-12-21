/** MergerValidator.cc
 * --------------------------------------------------------------
 * Description:  see MergerValidator.h
 * Authors:  Lucien Lo (UofR)
 * History: 28th Oct, 2014 -  initial version
 * --------------------------------------------------------------
 */

//Header file
#include "MergerValidator.h"

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

 //Other
 #include <cmath>

//User specific
 #include "DataFormats/GeometryVector/interface/Basic3DVector.h"


MergerValidator::MergerValidator(const edm::ParameterSet& conf)
{
	hitProducer = conf.getParameter<edm::InputTag>("HitProducer");
	recHitToken = consumes<SiTrackerGSMatchedRecHit2DCollection>(hitProducer);
	histoRechitDistlowx = conf.getParameter<double>("histoRechitDistlowx");
	histoRechitDisthighx = conf.getParameter<double>("histoRechitDisthighx");
	printout = conf.getParameter<bool>("printout");
	outdir = conf.getParameter<std::string>("outputdir");

};

MergerValidator::~MergerValidator(){

};


float MergerValidator::rechitDist(const SiTrackerGSMatchedRecHit2D& rechit1, const SiTrackerGSMatchedRecHit2D& rechit2){
	if (rechit1.det()==rechit2.det()){
		Basic3DVector<float> v1(rechit1.localPosition().x(),rechit1.localPosition().y(),rechit1.localPosition().z());
		Basic3DVector<float> v2(rechit2.localPosition().x(),rechit2.localPosition().y(),rechit2.localPosition().z());
		Basic3DVector<float> v(v1-v2);
		if (printout){
			std::cout << "-------" << std::endl;
			std::cout << rechit1.localPosition().x() << std::endl;
			std::cout << rechit1.localPosition().y() << std::endl;
			std::cout << rechit1.localPosition().z() << std::endl;
			std::cout << rechit2.localPosition().x() << std::endl;
			std::cout << rechit2.localPosition().y() << std::endl;
			std::cout << rechit2.localPosition().z() << std::endl;
			std::cout << v.mag() << std::endl;
		};
		// result = v.mag();
		return v.mag();
	} else {
		return -1;
	};
};


void MergerValidator::beginRun(edm::Run const& run, const edm::EventSetup & es){
	std::cout << "Validating Merger" << std::endl;
	h_RechitDist =  new TH1F("h_RechitDist","Rechit Distance",50,histoRechitDistlowx,histoRechitDisthighx);
	MergeHisto = new TFile(outdir+"MergeHisto.root","RECREATE");
};

void MergerValidator::endRun(edm::Run const& run, const edm::EventSetup & es){
	h_RechitDist->Write();
	MergeHisto->Close();
};

void MergerValidator::analyze(edm::Event const& e, const edm::EventSetup& c){

	edm::Handle<SiTrackerGSMatchedRecHit2DCollection> RecHitCollection;
	e.getByToken(recHitToken, RecHitCollection);
	
	for (SiTrackerGSMatchedRecHit2DCollection::const_iterator ihit=RecHitCollection->begin();ihit!=RecHitCollection->end();++ihit){
		for (SiTrackerGSMatchedRecHit2DCollection::const_iterator jhit=ihit+1;jhit!=RecHitCollection->end();++jhit){
			if (ihit->det()==jhit->det()){
				float temp;
				temp = rechitDist(*ihit,*jhit);
				if (temp != -1.0){
					h_RechitDist->Fill(temp);
				};
			};
		};

	};


};

