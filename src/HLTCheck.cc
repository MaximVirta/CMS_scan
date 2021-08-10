// system include files
#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <limits>
#include <cmath>
#include <algorithm>
#include <sys/time.h>
#include <sys/stat.h>
#include <string>

// user include files, general
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

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
#include "HLTrigger/HLTcore/interface/HLTConfigData.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"



using namespace std;
using namespace edm;
using namespace trigger;
using namespace reco;

// class declaration


class HLTCheck: public edm::EDAnalyzer {
public:
        explicit HLTCheck(const edm::ParameterSet&);
        ~HLTCheck();
          
        // for trigger config
        string processName_;
        string datasetName_;
        string outputMulti80_;
        string outputMulti100_;

private:
        virtual void beginJob();
        virtual void beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup);
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob();
        bool triggerFired(const std::string& triggerWildCard, const edm::TriggerResults& triggerResults);
        unsigned int findTrigger(const std::string& triggerWildCard);
        bool providesGoodLumisection(const edm::Event& iEvent);
        ofstream multi100File;
        ofstream multi80File;
        int evtN = 1;
        int evtNmult100 = 1;
        int evtNmult80 = 1;
        int w=4;

        bool multi100fired = false;
        bool multi80fired = false;

        HLTConfigProvider hltConfig_;
        InputTag hltInputTag_;


};

HLTCheck::HLTCheck(const edm::ParameterSet& iConfig):
processName_(iConfig.getParameter<std::string>("processName")),
datasetName_(iConfig.getParameter<std::string>("datasetName")),
outputMulti80_(iConfig.getParameter<std::string>("outputMulti80")),
outputMulti100_(iConfig.getParameter<std::string>("outputMulti100")),
hltConfig_(),
hltInputTag_("TriggerResults","","HLT")
{

multi100File.open(outputMulti100_);
multi100File << "# pt," << "\t\teta," << "\t\tphi" << "\t\tevtN\n";
multi80File.open(outputMulti80_);
multi80File << "# pt," << "\t\teta," << "\t\tphi" << "\t\tevtN\n";

}


HLTCheck::~HLTCheck() {
        // do anything here that needs to be done at destruction time
        // (e.g. close files, deallocate resources etc.)
        multi100File.close();
        multi80File.close();
        cout << "Number of Multiplicity100 events: " << evtNmult100 << endl;
        cout << "Number of Multiplicity80 events: " << evtNmult80 << endl;
}

// ------------ method called for each event  ------------
void HLTCheck::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {


#ifdef THIS_IS_AN_EVENT_EXAMPLE
        Handle<ExampleData> pIn;
        iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
        ESHandle<SetupData> pSetup;
        iSetup.get<SetupRecord>().get(pSetup);
#endif


Handle<TriggerResults> trigResults;
iEvent.getByLabel(hltInputTag_, trigResults);


const vector<string> HLTNames = hltConfig_.datasetContent(datasetName_);

for (unsigned i = 0; i < HLTNames.size(); i++) {
    string name = HLTNames[i];
    string Multi100 = "Multiplicity100";
    string Multi80 = "Multiplicity80";

    // Print a name  of a trigger when it is fired
    if (name.find(Multi100) != string::npos) {
        multi100fired = triggerFired(name, ( * trigResults));
        if (multi100fired) cout << name << "\t" << multi100fired << endl;
    }
    if (name.find(Multi80) != string::npos) {
        multi80fired = triggerFired(name, ( * trigResults));
        if (multi80fired) cout << name << "\t" << multi80fired << endl;
    }

}


  Handle<reco::TrackCollection> tracksc;
  iEvent.getByLabel("generalTracks", tracksc);

  for (reco::TrackCollection::const_iterator it = tracksc->begin();
    it != tracksc->end(); it++) {

        // Write the track data onto a file if an event fired a corresponding HLT 
        if (multi100fired) multi100File << it->pt() << setw(w) << "\t" << it->eta() << setw(w) << "\t" << it->phi() << setw(w) << "\t" << evtNmult100 << endl;
        if (multi80fired) multi80File << it->pt() << setw(w) << "\t" << it->eta() << setw(w) << "\t" << it->phi() << setw(w) << "\t" << evtNmult80 << endl;
  
  }   //end of reco ::TrackCollection loop

evtN+=1;

if (multi100fired) evtNmult100+=1;
if (multi80fired) evtNmult80+=1;

} //HLTCheck: analyze ends


// ------------ method called once each job just before starting event loop  ------------
void HLTCheck::beginJob() {

}

// ------------ method called once each job just after ending the event loop  ------------
void HLTCheck::endJob() {
}

void HLTCheck::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
    // Print out different HLT triggers used in the run

   bool changed = true;
   if ( hltConfig_.init(iRun, iSetup, hltInputTag_.process(), changed) ) {
      // if init returns TRUE, initialisation has succeeded!
      edm::LogInfo("TopPairElectronPlusJetsSelectionFilter") << "HLT config with process name "
        << hltInputTag_.process() << " successfully extracted";
   }
   else {
      edm::LogError("TopPairElectronPlusJetsSelectionFilter_Error")
      << "Error! HLT config extraction with process name " << hltInputTag_.process() << " failed";
   }

}

// Check whether a trigger was fired
bool HLTCheck::triggerFired(const std::string& triggerWildCard, const edm::TriggerResults& triggerResults) {
   bool fired = false;
   unsigned int index = findTrigger(triggerWildCard);

   if (index < triggerResults.size()) {
      if (triggerResults.accept(index)) {
         fired = true;
      }
   }

   return fired;

}


unsigned int HLTCheck::findTrigger(const std::string& triggerWildCard) {
   const std::vector<std::string>& triggers = hltConfig_.triggerNames();
   unsigned int found = 9999;

   size_t length = triggerWildCard.size();
   for (unsigned int index = 0; index < triggers.size(); ++index) {
      if (length <= triggers[index].size() && triggerWildCard == triggers[index].substr(0, length)) {
         found = index;
         break;
      }
   }

   return found;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HLTCheck);                                                                            
