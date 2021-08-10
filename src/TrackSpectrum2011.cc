// system include files
#include <memory>
#include <iostream>
#include <fstream>

// user include files, general
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//------ EXTRA HEADER FILES--------------------//
#include "math.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Common/interface/Ref.h"

// for tracking information
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"

#include <TRandom3.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <TStopwatch.h>

//for trigger configuration
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

// class declaration

using namespace edm;
using namespace reco;
using namespace std;


class TrackSpectrum2011: public edm::EDAnalyzer {
public:
        explicit TrackSpectrum2011(const edm::ParameterSet&);
        ~TrackSpectrum2011();
          
        // for trigger config
        std::string   processName_;
        std::string   datasetName_;
        std::string   outputFile_;

        //HLT config provider object
        HLTConfigProvider hltConfig_;

private:
        virtual void beginJob();
        virtual void beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup);
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob();
        bool providesGoodLumisection(const edm::Event& iEvent);
        ofstream myfile;
        int evtN = 1;
        int w=4;


};


TrackSpectrum2011::TrackSpectrum2011(const edm::ParameterSet& iConfig):
processName_(iConfig.getParameter<std::string>("processName")),
datasetName_(iConfig.getParameter<std::string>("datasetName")),
outputFile_(iConfig.getParameter<std::string>("outputFile"))
{
  //Print the configuration just to check
  cout << "Here is the information passed to the constructor:" <<endl;
  cout << "Configuration: " << endl
       << "   ProcessName = " << processName_ << endl
       << "   DataSetName = " << datasetName_ << endl;

myfile.open(outputFile_);
myfile << "# pt," << "\t\teta," << "\t\tphi" << "\t\tevtN\n";
}


TrackSpectrum2011::~TrackSpectrum2011() {
        // do anything here that needs to be done at destruction time
        // (e.g. close files, deallocate resources etc.)
        myfile.close();
    
}

// ------------ method called for each event  ------------
void TrackSpectrum2011::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {




#ifdef THIS_IS_AN_EVENT_EXAMPLE
        Handle<ExampleData> pIn;
        iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
        ESHandle<SetupData> pSetup;
        iSetup.get<SetupRecord>().get(pSetup);
#endif


// Event is to be analyzed

  LogInfo("Analyze")
  << "Starting to analyze \n"
  << "Event number: " << (iEvent.id()).event()
  << ", Run number: " << iEvent.run()
  << ", Lumisection: " << iEvent.luminosityBlock();

  // Read the track information and then write it to the output file.

  Handle<reco::TrackCollection> tracksc;
  iEvent.getByLabel("generalTracks", tracksc);

  for (reco::TrackCollection::const_iterator it = tracksc->begin();
    it != tracksc->end(); it++) {
        myfile << it->pt() << setw(w) << "\t" << it->eta() << setw(w) << "\t" << it->phi() << setw(w) << "\t" << evtN << endl;
  }   //end of reco ::TrackCollection loop

evtN+=1;

} //TrackSpectrum2011: analyze ends


// ------------ method called once each job just before starting event loop  ------------
void TrackSpectrum2011::beginJob() {

}

// ------------ method called once each job just after ending the event loop  ------------
void TrackSpectrum2011::endJob() {
}

void TrackSpectrum2011::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
  // Print out different HLT triggers used in the run

  bool changed(true);
  if (hltConfig_.init(iRun,iSetup,processName_,changed)) {
      if (changed) {
        const vector<string> triggerNamesInDS = hltConfig_.datasetContent(datasetName_);
         for (unsigned i = 0; i < triggerNamesInDS.size(); i++) {
            if (i==0) cout << "HLT triggers used in this run: " << endl;
            cout << triggerNamesInDS[i] << endl;
          }
            }
  }

}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackSpectrum2011);                                                                            
