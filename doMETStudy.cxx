#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TChain.h"
#include "TAttMarker.h"
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Following headers help decode L1T ntuples
#include "L1Trigger/L1TNtuples/interface/L1AnalysisEventDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoVertexDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisCaloTPDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1CaloTowerDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoJetDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoMetDataFormat.h"

#define N_IETA_BINS 80
#define N_IPHI_BINS 72
#define N_ETT_BINS 50
#define ETT_RANGE 1000
#define N_INDIV_EVENTS 20

// Number of events that pass any cuts, for normalising later
int nPassing = 0;

// 1d formatter
void formatPlot1D(TH1D* plot1d, int colour){
  plot1d->GetXaxis()->SetTitleOffset(1.2);
  plot1d->GetYaxis()->SetTitleOffset(1.4);
  plot1d->SetMinimum(0.);
  plot1d->SetLineColor(colour);
  plot1d->SetLineWidth(2);
  plot1d->Scale(1. / (double) nPassing);
  plot1d->Draw("HIST");
  plot1d->SetStats(false);
}

// 2d formatter
void formatPlot2D(TH2D* plot2d){
  plot2d->GetXaxis()->SetTitleOffset(1.2);
  plot2d->GetYaxis()->SetTitleOffset(1.4);
  plot2d->Draw("colz");
  plot2d->SetStats(false);
}

//main plotting function
void doMETStudy(){

  std::cout << "Running doMETStudy.cxx..." << std::endl;

  vector<string> towers;
  vector<string> hcal;
  vector<string> ecal;

  bool isTest = false;
  
  // Load files

  if (isTest == false){
  // Default: USE FOR TOWERS
  towers.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/treis/l1t-integration-v97p17v2-CMSSW-1000/ZeroBias/crab_l1t-integration-v97p17v2-CMSSW-1000__ZeroBias_Run2017F-v1/180302_133644/0000/L1Ntuple_*.root");
  towers.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/treis/l1t-integration-v97p17v2-CMSSW-1000/ZeroBias2/crab_l1t-integration-v97p17v2-CMSSW-1000__ZeroBias2_Run2017F-v1/180302_134600/0000/L1Ntuple_*.root");
  towers.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/treis/l1t-integration-v97p17v2-CMSSW-1000/ZeroBias4/crab_l1t-integration-v97p17v2-CMSSW-1000__ZeroBias4_Run2017F-v1/180302_134615/0000/L1Ntuple_*.root");
  towers.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/treis/l1t-integration-v97p17v2-CMSSW-1000/ZeroBias5/crab_l1t-integration-v97p17v2-CMSSW-1000__ZeroBias5_Run2017F-v1/180302_134744/0000/L1Ntuple_*.root");
  towers.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/treis/l1t-integration-v97p17v2-CMSSW-1000/ZeroBias6/crab_l1t-integration-v97p17v2-CMSSW-1000__ZeroBias6_Run2017F-v1/180302_134659/0000/L1Ntuple_*.root");
  towers.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/treis/l1t-integration-v97p17v2-CMSSW-1000/ZeroBias7/crab_l1t-integration-v97p17v2-CMSSW-1000__ZeroBias7_Run2017F-v1/180302_134644/0000/L1Ntuple_*.root");
  towers.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/treis/l1t-integration-v97p17v2-CMSSW-1000/ZeroBias8/crab_l1t-integration-v97p17v2-CMSSW-1000__ZeroBias8_Run2017F-v1/180302_134728/0000/L1Ntuple_*.root");
  //  towers.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_NoPUS/ZeroBias8b4e1/crab_noRECO-l1t-v96p27_NoPUS__8b4e1/170915_101410/0000/L1Ntuple_*.root");
  //  towers.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_NoPUS/ZeroBias8b4e2/crab_noRECO-l1t-v96p27_NoPUS__8b4e2/170915_101429/0000/L1Ntuple_*.root");
  //  towers.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_NoPUS/ZeroBias8b4e3/crab_noRECO-l1t-v96p27_NoPUS__8b4e3/170915_101449/0000/L1Ntuple_*.root");
  //  towers.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_NoPUS/ZeroBias8b4e4/crab_noRECO-l1t-v96p27_NoPUS__8b4e4/170915_101249/0000/L1Ntuple_*.root");
  //  towers.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_NoPUS/ZeroBias8b4e5/crab_noRECO-l1t-v96p27_NoPUS__8b4e5/170915_101308/0000/L1Ntuple_*.root");
  //  towers.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_NoPUS/ZeroBias8b4e6/crab_noRECO-l1t-v96p27_NoPUS__8b4e6/170915_101329/0000/L1Ntuple_*.root");
  //  towers.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_NoPUS/ZeroBias8b4e7/crab_noRECO-l1t-v96p27_NoPUS__8b4e7/170915_101349/0000/L1Ntuple_*.root");
  //  towers.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_NoPUS/ZeroBias8b4e8/crab_noRECO-l1t-v96p27_NoPUS__8b4e8/170915_101509/0000/L1Ntuple_*.root");
  //  towers.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_NoPUS/ZeroBias8b4e9/crab_noRECO-l1t-v96p27_NoPUS__8b4e9/170915_101529/0000/L1Ntuple_*.root");
  //  towers.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_NoPUS/ZeroBias8b4e10/crab_noRECO-l1t-v96p27_NoPUS__8b4e10/170915_101548/0000/L1Ntuple_*.root");
  
  // HCAL
  hcal.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_HCAL/ZeroBias8b4e1/crab_noRECO-l1t-v96p27_HCAL__8b4e1/170922_121415/0000/L1Ntuple_*.root");
  hcal.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_HCAL/ZeroBias8b4e2/crab_noRECO-l1t-v96p27_HCAL__8b4e2/170922_121439/0000/L1Ntuple_*.root");
  hcal.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_HCAL/ZeroBias8b4e3/crab_noRECO-l1t-v96p27_HCAL__8b4e3/170922_121504/0000/L1Ntuple_*.root");
  //  hcal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_HCAL/ZeroBias8b4e4/crab_noRECO-l1t-v96p27_HCAL__8b4e4/170922_121241/0000/L1Ntuple_*.root");
  //  hcal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_HCAL/ZeroBias8b4e5/crab_noRECO-l1t-v96p27_HCAL__8b4e5/170922_121302/0000/L1Ntuple_*.root");
  //  hcal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_HCAL/ZeroBias8b4e6/crab_noRECO-l1t-v96p27_HCAL__8b4e6/170922_121326/0000/L1Ntuple_*.root");
  //  hcal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_HCAL/ZeroBias8b4e7/crab_noRECO-l1t-v96p27_HCAL__8b4e7/170922_121350/0000/L1Ntuple_*.root");
  //  hcal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_HCAL/ZeroBias8b4e8/crab_noRECO-l1t-v96p27_HCAL__8b4e8/170922_121528/0000/L1Ntuple_*.root");
  //  hcal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_HCAL/ZeroBias8b4e9/crab_noRECO-l1t-v96p27_HCAL__8b4e9/170922_121556/0000/L1Ntuple_*.root");
  //  hcal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_HCAL/ZeroBias8b4e10/crab_noRECO-l1t-v96p27_HCAL__8b4e10/170922_121624/0000/L1Ntuple_*.root");
  
  // ECAL
  ecal.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_ECAL/ZeroBias8b4e1/crab_noRECO-l1t-v96p27_ECAL__8b4e1/170922_105053/0000/L1Ntuple_*.root");
  ecal.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_ECAL/ZeroBias8b4e2/crab_noRECO-l1t-v96p27_ECAL__8b4e2/170922_105120/0000/L1Ntuple_*.root");
  ecal.push_back("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_ECAL/ZeroBias8b4e3/crab_noRECO-l1t-v96p27_ECAL__8b4e3/170922_105143/0000/L1Ntuple_*.root");
  //  ecal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_ECAL/ZeroBias8b4e4/crab_noRECO-l1t-v96p27_ECAL__8b4e4/170922_104858/0000/L1Ntuple_*.root");
  //  ecal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_ECAL/ZeroBias8b4e5/crab_noRECO-l1t-v96p27_ECAL__8b4e5/170922_104929/0000/L1Ntuple_*.root");
  //  ecal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_ECAL/ZeroBias8b4e6/crab_noRECO-l1t-v96p27_ECAL__8b4e6/170922_104957/0000/L1Ntuple_*.root");
  //  ecal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_ECAL/ZeroBias8b4e7/crab_noRECO-l1t-v96p27_ECAL__8b4e7/170922_105023/0000/L1Ntuple_*.root");
  //  ecal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_ECAL/ZeroBias8b4e8/crab_noRECO-l1t-v96p27_ECAL__8b4e8/170922_105208/0000/L1Ntuple_*.root");
  //  ecal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_ECAL/ZeroBias8b4e9/crab_noRECO-l1t-v96p27_ECAL__8b4e9/170922_105232/0000/L1Ntuple_*.root");
  //  ecal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_ECAL/ZeroBias8b4e10/crab_noRECO-l1t-v96p27_ECAL__8b4e10/170922_105256/0000/L1Ntuple_*.root");
  }

  else {
  // Load these files instead when testing
  towers.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_NoPUS/ZeroBias8b4e1/crab_noRECO-l1t-v96p27_NoPUS__8b4e1/170915_101410/0000/L1Ntuple_6.root");
  hcal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_HCAL/ZeroBias8b4e1/crab_noRECO-l1t-v96p27_HCAL__8b4e1/170922_121415/0000/L1Ntuple_6.root");
  ecal.push_back("root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/ZeroBias/HighPU/noRECO-l1t-v96p27_ECAL/ZeroBias8b4e1/crab_noRECO-l1t-v96p27_ECAL__8b4e1/170922_105053/0000/L1Ntuple_6.root");
  }

  cout << "Loading up the TChain..." << endl;
  TChain * eventTree = new TChain("l1EventTree/L1EventTree");
  TChain * treeL1Towemu = new TChain("l1CaloTowerEmuTree/L1CaloTowerTree");
  TChain * treeTPemu = new TChain("l1CaloTowerEmuTree/L1CaloTowerTree");
  TChain * treeL1emu = new TChain("l1UpgradeEmuTree/L1UpgradeTree");
  TChain * treeL1HCALemu = new TChain("l1UpgradeEmuTree/L1UpgradeTree");
  TChain * treeL1ECALemu = new TChain("l1UpgradeEmuTree/L1UpgradeTree");
  
  int minFiles = std::min( std::min( towers.size(), hcal.size() ), ecal.size() );

  for(uint i = 0; i < minFiles; ++i) {
    eventTree->Add(towers[i].c_str());
    treeL1Towemu->Add(towers[i].c_str());
    treeTPemu->Add(towers[i].c_str());
    treeL1emu->Add(towers[i].c_str());
    treeL1HCALemu->Add(hcal[i].c_str());
    treeL1ECALemu->Add(ecal[i].c_str());
  }

  L1Analysis::L1AnalysisEventDataFormat           *event_ = new L1Analysis::L1AnalysisEventDataFormat();
  L1Analysis::L1AnalysisL1CaloTowerDataFormat     *l1Towemu_ = new L1Analysis::L1AnalysisL1CaloTowerDataFormat();
  L1Analysis::L1AnalysisCaloTPDataFormat          *l1TPemu_ = new L1Analysis::L1AnalysisCaloTPDataFormat();
  L1Analysis::L1AnalysisL1UpgradeDataFormat       *l1emu_ = new L1Analysis::L1AnalysisL1UpgradeDataFormat();
  L1Analysis::L1AnalysisL1UpgradeDataFormat       *l1HCALemu_ = new L1Analysis::L1AnalysisL1UpgradeDataFormat();
  L1Analysis::L1AnalysisL1UpgradeDataFormat       *l1ECALemu_ = new L1Analysis::L1AnalysisL1UpgradeDataFormat();
 
  eventTree->SetBranchAddress("Event", &event_);
  treeL1Towemu->SetBranchAddress("L1CaloTower", &l1Towemu_);
  treeTPemu->SetBranchAddress("CaloTP", &l1TPemu_);
  treeL1emu->SetBranchAddress("L1Upgrade", &l1emu_);
  treeL1HCALemu->SetBranchAddress("L1Upgrade", &l1HCALemu_);
  treeL1ECALemu->SetBranchAddress("L1Upgrade", &l1ECALemu_);
  
  // get number of entries
  Long64_t nentriesTowers;
  Long64_t nentriesHCAL;
  Long64_t nentriesECAL;
  Long64_t nentries;

  nentriesTowers = treeL1emu->GetEntries();
  nentriesHCAL = treeL1HCALemu->GetEntries();
  nentriesECAL = treeL1ECALemu->GetEntries();
 
  nentries = std::min(std::min(nentriesTowers,nentriesHCAL),nentriesECAL);

  // Initialise histograms

  // Tower hists
  TH1D* hAllTowEt = new TH1D("towerEt", ";Tower E_{T}; # Towers", 40, -0.5, 39.5);
  TH1D* hAllTowEta = new TH1D("towerEta", "Towers vs iEta;iEta; # Towers", N_IETA_BINS, -40., 40.);
  TH1D* hTowTPETEta = new TH1D("towerTPETEta", "Average tower TP E_{T} vs. iEta; iEta; Average tower TP E_{T}", N_IETA_BINS, -40., 40.);

  TH1D* hTowPhiB = new TH1D("towerPhiB", "Towers vs iPhi in Barrel;iPhi; # Towers", N_IPHI_BINS, 0., 72.);
  TH1D* hTowPhiE = new TH1D("towerPhiE", "Towers vs iPhi in End cap;iPhi; # Towers", N_IPHI_BINS, 0., 72.);

  TH1D* hTowTPETphiB = new TH1D("towerTPETPhiB", "Average tower TP E_{T} vs. iPhi in Barrel;iPhi; Average tower TP E_{T}", N_IPHI_BINS, 0., 72.);
  TH1D* hTowTPETphiE = new TH1D("towerTPETPhiE", "Average tower TP E_{T} vs. iPhi in End cap;iPhi; Average tower TP E_{T}", N_IPHI_BINS, 0., 72.);
 
  // HCAL hists
  TH1D* hHCALTPEt = new TH1D("hcalTPEt", "HCAL;TP E_{T}; # TPs / Event", 20, 0., 200.);
  TH1D* hHCALTPEta = new TH1D("hcalTPEta", "HCAL # TPs vs iEta;iEta; # TPs / Event", N_IETA_BINS, -40., 40.);
  TH1D* hHCALTPETEta = new TH1D("hcalTPETEta", "Average HCAL TP E_{T} vs. iEta; iEta; Average HCAL TP E_{T}", N_IETA_BINS, -40., 40.);
 
  TH1D* hHCALTPPhi = new TH1D("hcalTPPhi", "HCAL # TPs vs iPhi;iPhi; # TPs / Event", N_IPHI_BINS, 0., 72.);
  TH1D* hHCALTPPhiHB = new TH1D("hcalTPPhiHB", "HCAL # TPs vs iPhi in HB;iPhi; # TPs / Event", N_IPHI_BINS, 0., 72.);
  TH1D* hHCALTPPhiHE = new TH1D("hcalTPPhiHE", "HCAL # TPs vs iPhi in HE;iPhi; # TPs / Event", N_IPHI_BINS, 0., 72.);
  TH1D* hHCALTPPhiHF = new TH1D("hcalTPPhiHF", "HCAL # TPs vs iPhi in HF;iPhi; # TPs / Event", N_IPHI_BINS, 0., 72.);

  TH1D* hHCALTPETphi = new TH1D("hcalTPETPhi", "Average HCAL TP E_{T} vs. iPhi;iPhi; Average HCAL TP E_{T}", N_IPHI_BINS, 0., 72.);
  TH1D* hHCALTPETphiHB = new TH1D("hcalTPETPhiHB", "Average HCAL TP E_{T} vs. iPhi in HB;iPhi; Average HCAL TP E_{T}", N_IPHI_BINS, 0., 72.);
  TH1D* hHCALTPETphiHE = new TH1D("hcalTPETPhiHE", "Average HCAL TP E_{T} vs. iPhi in HE;iPhi; Average HCAL TP E_{T}", N_IPHI_BINS, 0., 72.);
  TH1D* hHCALTPETphiHF = new TH1D("hcalTPETPhiHF", "Average HCAL TP E_{T} vs. iPhi in HF;iPhi; Average HCAL TP E_{T}", N_IPHI_BINS, 0., 72.);

  TH2D* hHCALTPEtaPhi = new TH2D("hcaletaphi", "HCAL TP occupancy;TP iEta;TP iPhi", N_IETA_BINS, -40., 40., N_IPHI_BINS, 0., 72.);
  TH2D* hHCALavgTPETEtaPhi = new TH2D("hcalavgtpetetaphi", "HCAL average TP E_{T} per bin;TP iEta;TP iPhi", N_IETA_BINS, -40., 40., N_IPHI_BINS, .0, 72.);
  
  TH1D* hHCALTPPhiforTT28 = new TH1D("hcaltpphiTT28", "HCAL # TPs vs iPhi for TT28;iPhi;# TPs / Event", N_IPHI_BINS, 0., 72.);
  TH1D* hHCALTPETphiforTT28 = new TH1D("hcaltpetphiTT28", "Average HCAL TP E_{T} vs iPhi for TT28;iPhi; Average HCAL TP E_{T}", N_IPHI_BINS, 0., 72.);
  TH2D* hHCALavgTPETforTT28 = new TH2D("hcalavgetTT28", "HCAL average TP E_{T} per bin for TT28;TP iEta;TP iPhi", N_IETA_BINS, -40., 40., N_IPHI_BINS, .0, 72.);

  // ECAL hists
  TH1D* hECALTPEt = new TH1D("ecalTPEt", "ECAL;TP E_{T}; # TPs / Event", 20, 0., 200.);
  TH1D* hECALTPEta = new TH1D("ecalTPEta", "ECAL # TPs vs iEta;iEta; # TPs / Event", N_IETA_BINS, -40., 40.);
  TH1D* hECALTPETEta = new TH1D("ecalTPETEta", "Average ECAL TP E_{T} vs. iEta; iEta; Average ECAL TP E_{T}", N_IETA_BINS, -40., 40.);

  TH1D* hECALTPPhi = new TH1D("ecalTPPhi", "ECAL # TPs vs iPhi;iPhi; # TPs / Event", N_IPHI_BINS, 0., 72.);
  TH1D* hECALTPPhiEB = new TH1D("ecalTPPhiEB", "ECAL # TPs vs iPhi in EB;iPhi; # TPs / Event", N_IPHI_BINS, 0., 72.);
  TH1D* hECALTPPhiEE = new TH1D("ecalTPPhiEE", "ECAL # TPs vs iPhi in EE;iPhi; # TPs / Event", N_IPHI_BINS, 0., 72.);
  
  TH1D* hECALTPETphi = new TH1D("ecalTPETPhi", "Average ECAL TP E_{T} vs. iPhi;iPhi; Average ECAL TP E_{T}", N_IPHI_BINS, 0., 72.);
  TH1D* hECALTPETphiEB = new TH1D("ecalTPETPhiEB", "Average ECAL TP E_{T} vs. iPhi in EB;iPhi; Average ECAL TP E_{T}", N_IPHI_BINS, 0., 72.);
  TH1D* hECALTPETphiEE = new TH1D("ecalTPETPhiEE", "Average ECAL TP E_{T} vs. iPhi in EE;iPhi; Average ECAL TP E_{T}", N_IPHI_BINS, 0., 72.);
  
  TH2D* hECALTPEtaPhi = new TH2D("ecaletaphi", "ECAL TP occupancy;TP iEta;TP iPhi", N_IETA_BINS, -40., 40., N_IPHI_BINS, .0, 72.);
  TH2D* hECALavgTPETEtaPhi = new TH2D("ecalavgtpetetaphi", "ECAL average TP E_{T} per bin;TP iEta;TP iPhi", N_IETA_BINS, -40., 40., N_IPHI_BINS, .0, 72.);

  TH1D* hECALTPPhiforTT28 = new TH1D("ecaltpphiTT28", "ECAL # TPs vs iPhi for TT28;iPhi;# TPs / Event", N_IPHI_BINS, 0., 72.);
  TH1D* hECALTPETphiforTT28 = new TH1D("ecaltpetphiTT28", "Average ECAL TP E_{T} vs iPhi for TT28;iPhi; Average ECAL TP E_{T}", N_IPHI_BINS, 0., 72.);
  TH2D* hECALavgTPETforTT28 = new TH2D("ecalavgetTT28", "ECAL average TP E_{T} per bin for TT28;TP iEta;TP iPhi", N_IETA_BINS, -40., 40., N_IPHI_BINS, .0, 72.);
  
  // MET hists
  TH1D* hMetPhi = new TH1D("hMetPhi", ";MET Phi;# Events", N_IPHI_BINS, 0., 144.); // each bin (and therefore range) is 2*iPhi
  TH2D* hMetPhiEcalHcal = new TH2D("hMetPhiEcalHcal", "MET Phi ECAL vs HCAL;MET Phi ECAL;MET Phi HCAL", N_IPHI_BINS, 0., 144., N_IPHI_BINS, 0., 144.);
  TH2D* hMetPhiEcalTotal = new TH2D("hMetPhiEcalTotal", "MET Phi ECAL vs total;MET Phi ECAL;MET Phi total", N_IPHI_BINS, 0., 144., N_IPHI_BINS, 0., 144.);
  TH2D* hMetPhiHcalTotal = new TH2D("hMetPhiHcalTotal", "MET Phi HCAL vs total;MET Phi HCAL;MET Phi total", N_IPHI_BINS, 0., 144., N_IPHI_BINS, 0., 144.);

  TH1D* hMetScal = new TH1D("hMetScal", ";MET scalar sum; # Events", 20, 100., 200);
  TH2D* hMetScalEcalHcal = new TH2D("hMetScalEcalHcal", "MET scalar ECAL vs HCAL;MET scalar ECAL;MET scalar HCAL", 40, 0., 200, 40, 0., 200);
  TH2D* hMetScalEcaltotal = new TH2D("hMetScalEcaltotal", "MET scalar ECAL vs sum;MET scalar ECAL;MET scalar sum", 40, 0., 200, 20, 100., 200);
  TH2D* hMetScalHcaltotal = new TH2D("hMetScalHcaltotal", "MET scalar HCAL vs sum;MET scalar HCAL;MET scalar sum", 40, 0., 200, 20, 100., 200);

  // ETT hists
  TH1D* hEttScal = new TH1D("hEttScal", ";ETT scalar sum; # Events", 2*N_ETT_BINS, 0., 2*ETT_RANGE);
  TH2D* hEttScalEcalHcal = new TH2D("hEttScalEcalHcal", "ETT scalar ECAL vs HCAL;ETT scalar ECAL;ETT scalar HCAL", N_ETT_BINS, 0., ETT_RANGE, N_ETT_BINS, 0., ETT_RANGE);
  TH2D* hEttScalEcaltotal = new TH2D("hEttScalEcaltotal", "ETT scalar ECAL vs sum;ETT scalar ECAL;ETT scalar sum", N_ETT_BINS, 0., ETT_RANGE, 2*N_ETT_BINS, 0., 2*ETT_RANGE);
  TH2D* hEttScalHcaltotal = new TH2D("hEttScalHcaltotal", "ETT scalar HCAL vs sum;ETT scalar HCAL;ETT scalar sum", N_ETT_BINS, 0., ETT_RANGE, 2*N_ETT_BINS, 0., 2*ETT_RANGE);

  // Histogram arrays for storing individual event information
  TH2D* hECALTPETEtaPhiIndiv[N_INDIV_EVENTS] = { NULL };
  TH2D* hHCALTPETEtaPhiIndiv[N_INDIV_EVENTS] = { NULL };

  // Main loop

  // Number of entries to run over
  int nEvents;
  if (isTest == true) nEvents = 1000;
  else nEvents = 1500000;
  
  for (Long64_t jentry = 0; jentry < nEvents; ++jentry) {
    // initialise some variables
    int nHCALTPemu(0), nECALTPemu(0), nTowemu(-1);
    double hcalTPEtEm(0), ecalTPEtEm(0), towEtemu(0);
    int hcalTPEtaEm(0), ecalTPEtaEm(0), towEtaemu(0);
    int hcalTPPhiEm(0), ecalTPPhiEm(0), towPhiemu(0);

    double l1MetEmu(0.);
    double l1MetHCALEmu(0.);
    double l1MetECALEmu(0.);

    double l1EttEmu(0.);
    double l1EttHCALEmu(0.);
    double l1EttECALEmu(0.);

    double l1MetPhiEmu(-1.);
    double l1MetPhiHCALEmu(-1.);
    double l1MetPhiECALEmu(-1.);

    // run number
    int run(306042);

    //counter
    if( (jentry % 10000) == 0 ) cout << "Done " << jentry << " events of " << nEvents << endl;
    if( (jentry % 1000) == 0 ) cout << "." << flush;

    eventTree->GetEntry(jentry);
    
    run = event_->run;
    int lumi = event_->lumi;
    int event = event_->event;
    
    treeL1Towemu->GetEntry(jentry);
    treeTPemu->GetEntry(jentry);
    treeL1emu->GetEntry(jentry);
    treeL1HCALemu->GetEntry(jentry);
    treeL1ECALemu->GetEntry(jentry);

    nTowemu = l1Towemu_->nTower;

    nHCALTPemu = l1TPemu_->nHCALTP;
    nECALTPemu = l1TPemu_->nECALTP;

    // Retrieve MET, ETT from emulator tree
  
    for (unsigned int c = 0; c < l1emu_->nSums; ++c) {
      if( l1emu_->sumBx[c] != 0 ) continue;
      if( l1emu_->sumType[c] == L1Analysis::kTotalEt ) l1EttEmu = l1emu_->sumEt[c];
      if( l1emu_->sumType[c] == L1Analysis::kMissingEt ) {
	      l1MetEmu = l1emu_->sumEt[c];
        l1MetPhiEmu = l1emu_->sumIPhi[c];
      }
    }

    // Only loop over events with MET > 100 GeV and set overflow events (MET > 200 GeV) to 200 GeV
    if (l1MetEmu < 99.9) continue;
    if (l1MetEmu > 200.) l1MetEmu = 200.;

    ++nPassing;

    // Retrieve MET and ETT for HCAL and ECAL from emulator tree

    for (unsigned int c = 0; c < l1HCALemu_->nSums; ++c) {
       if( l1HCALemu_->sumBx[c] != 0 ) continue;
       if( l1HCALemu_->sumType[c] == L1Analysis::kTotalEt ) l1EttHCALEmu = l1HCALemu_->sumEt[c];
       if( l1HCALemu_->sumType[c] == L1Analysis::kMissingEt ) {
	      l1MetHCALEmu = l1HCALemu_->sumEt[c];
        if (l1MetHCALEmu > 200.) l1MetHCALEmu = 200.;
        l1MetPhiHCALEmu = l1HCALemu_->sumIPhi[c];
       }
    }

    for (unsigned int c = 0; c < l1ECALemu_->nSums; ++c) {
      if( l1ECALemu_->sumBx[c] != 0 ) continue;
      if( l1ECALemu_->sumType[c] == L1Analysis::kTotalEt ) l1EttECALEmu = l1ECALemu_->sumEt[c];
      if( l1ECALemu_->sumType[c] == L1Analysis::kMissingEt ) {
	      l1MetECALEmu = l1ECALemu_->sumEt[c];
        if (l1MetECALEmu > 200.) l1MetECALEmu = 200.;
        l1MetPhiECALEmu = l1ECALemu_->sumIPhi[c];
      }
    } 

    // Fill ETT scalar histos
    hEttScal->Fill(l1EttEmu);
    hEttScalEcalHcal->Fill(l1EttECALEmu, l1EttHCALEmu);
    hEttScalEcaltotal->Fill(l1EttECALEmu, l1EttEmu);
    hEttScalHcaltotal->Fill(l1EttHCALEmu, l1EttEmu);

    // Initialise histograms in arrays for filling with individual event info
    if (N_INDIV_EVENTS > nPassing) {
      ostringstream hcalHistNameStream;
      hcalHistNameStream << "hcalTPETetaphiindiv" << nPassing;
      hHCALTPETEtaPhiIndiv[(int) nPassing - 1] = new TH2D(hcalHistNameStream.str().c_str(), "HCAL TP E_{T} for single event;iEta;iPhi", N_IETA_BINS, -40., 40., N_IPHI_BINS, 0., 72.);
    
      ostringstream ecalHistNameStream;
      ecalHistNameStream << "ecalTPETetaphiindiv" << nPassing;
      hECALTPETEtaPhiIndiv[(int) nPassing - 1] = new TH2D(ecalHistNameStream.str().c_str(), "ECAL TP E_{T} for single event;iEta;iPhi", N_IETA_BINS, -40., 40., N_IPHI_BINS, 0., 72.);
    }

    // Retrieve HCAL objects from emulator tree
 
    for (uint tpIt = 0; tpIt < nHCALTPemu; ++tpIt) {
      hcalTPEtEm = l1TPemu_->hcalTPet[tpIt];
      hcalTPEtaEm = l1TPemu_->hcalTPieta[tpIt];
      hcalTPPhiEm = l1TPemu_->hcalTPiphi[tpIt];

      // Fill arrays and histos for each detector section
      if (abs(hcalTPEtaEm) <= 16) {
        hHCALTPPhiHB->Fill(hcalTPPhiEm);
        hHCALTPETphiHB->Fill(hcalTPPhiEm, hcalTPEtEm);
      }

      else if (abs(hcalTPEtaEm) > 16 && abs(hcalTPEtaEm) <= 28) {
        hHCALTPPhiHE->Fill(hcalTPPhiEm);
        hHCALTPETphiHE->Fill(hcalTPPhiEm, hcalTPEtEm);
        if (abs(hcalTPEtaEm) == 28) {
          hHCALTPPhiforTT28->Fill(hcalTPPhiEm);
          hHCALTPETphiforTT28->Fill(hcalTPPhiEm, hcalTPEtEm);
          hHCALavgTPETforTT28->Fill(hcalTPEtaEm, hcalTPPhiEm, hcalTPEtEm);
        }
      }

      else {
        hHCALTPPhiHF->Fill(hcalTPPhiEm);
        hHCALTPETphiHF->Fill(hcalTPPhiEm, hcalTPEtEm);
      }

	    hHCALTPEt->Fill(hcalTPEtEm);
	    hHCALTPEta->Fill(hcalTPEtaEm);
      hHCALTPETEta->Fill(hcalTPEtaEm, hcalTPEtEm);
      hHCALTPPhi->Fill(hcalTPPhiEm);
      hHCALTPETphi->Fill(hcalTPPhiEm, hcalTPEtaEm);
	    hHCALTPEtaPhi->Fill(hcalTPEtaEm, hcalTPPhiEm);
      // Fill eta-phi histogram with the TP ETs
      hHCALavgTPETEtaPhi->Fill(hcalTPEtaEm, hcalTPPhiEm, hcalTPEtEm);
      if (N_INDIV_EVENTS > nPassing)
        hHCALTPETEtaPhiIndiv[(int) nPassing - 1]->Fill(hcalTPEtaEm, hcalTPPhiEm, hcalTPEtEm);

    }

    // Retrieve ECAL objects from emulator tree

    for (uint tpIt = 0; tpIt < nECALTPemu; ++tpIt){
      ecalTPEtEm = l1TPemu_->ecalTPet[tpIt];
      ecalTPEtaEm = l1TPemu_->ecalTPieta[tpIt];
      ecalTPPhiEm = l1TPemu_->ecalTPiphi[tpIt];

      if (abs(ecalTPEtaEm) <= 16) {
        hECALTPPhiEB->Fill(ecalTPPhiEm);
        hECALTPETphiEB->Fill(ecalTPPhiEm, ecalTPEtEm);
      }

      else if (abs(ecalTPEtaEm) > 16 && abs(ecalTPEtaEm) <= 28) {
        hECALTPPhiEE->Fill(ecalTPPhiEm);
        hECALTPETphiEE->Fill(ecalTPPhiEm, ecalTPEtEm);
        if (abs(ecalTPEtaEm) == 28) {
          hECALTPPhiforTT28->Fill(ecalTPPhiEm);
          hECALTPETphiforTT28->Fill(ecalTPPhiEm, ecalTPEtEm);
          hECALavgTPETforTT28->Fill(ecalTPEtaEm, ecalTPPhiEm, ecalTPEtEm);
        }
      }

      hECALTPEt->Fill(ecalTPEtEm);
      hECALTPEta->Fill(ecalTPEtaEm);
      hECALTPETEta->Fill(ecalTPEtaEm, ecalTPEtEm);
      hECALTPPhi->Fill(ecalTPPhiEm);
      hECALTPETphi->Fill(ecalTPPhiEm, ecalTPEtEm);
      hECALTPEtaPhi->Fill(ecalTPEtaEm, ecalTPPhiEm);
      hECALavgTPETEtaPhi->Fill(ecalTPEtaEm, ecalTPPhiEm, ecalTPEtEm);
      if (N_INDIV_EVENTS > nPassing)
        hECALTPETEtaPhiIndiv[(int) nPassing - 1]->Fill(ecalTPEtaEm, ecalTPPhiEm, ecalTPEtEm);

    }
    
    // Retrieve tower objects from the emulator tree

    for(uint towIt = 0; towIt < nTowemu; ++towIt){
      towEtemu  = l1Towemu_->iet[towIt];
      towEtaemu = l1Towemu_->ieta[towIt];
      towPhiemu = l1Towemu_->iphi[towIt];

      if (abs(towEtaemu) <= 16) {
        hTowPhiB->Fill(towPhiemu);
        hTowTPETphiB->Fill(towPhiemu, towEtemu);
      }

      else if (abs(towEtaemu) > 16 && abs(towEtaemu) <= 28) {
        hTowPhiE->Fill(towPhiemu);
        hTowTPETphiE->Fill(towPhiemu, towEtemu);
      } // Ignoring HF

      hAllTowEt->Fill(towEtemu);
      hAllTowEta->Fill(towEtaemu);
      hTowTPETEta->Fill(towEtaemu, towEtemu);

    }

    // Fill MET scalar histos
    hMetScal->Fill(l1MetEmu);
    hMetScalEcalHcal->Fill(l1MetECALEmu, l1MetHCALEmu);
    hMetScalEcaltotal->Fill(l1MetECALEmu, l1MetEmu);
    hMetScalHcaltotal->Fill(l1MetHCALEmu, l1MetEmu);

    // Fill MET phi histos
    hMetPhi->Fill(l1MetPhiEmu);
    hMetPhiEcalHcal->Fill(l1MetPhiECALEmu, l1MetPhiHCALEmu);
    hMetPhiEcalTotal->Fill(l1MetPhiECALEmu, l1MetPhiEmu);
    hMetPhiHcalTotal->Fill(l1MetPhiHCALEmu, l1MetPhiEmu);

  }
  
  // End event loop, now plot histos
  
  TCanvas* canvas = new TCanvas("canvas","",750,700);

  int ecalColour = 2;
  int hcalColour = 4;
  int towerColour = 3;

  // Clone histograms for other plots, rather than filling twice
  TH2D* hHCALavgTPETperEvEtaPhi = (TH2D*)hHCALavgTPETEtaPhi->Clone("hcalavgevetaphi");
  hHCALavgTPETperEvEtaPhi->Scale(1. / nPassing); // because formatPlot2D doesn't normalise
  hHCALavgTPETperEvEtaPhi->SetTitle("HCAL average TP E_{T} per event");

  TH2D* hECALavgTPETperEvEtaPhi = (TH2D*)hECALavgTPETEtaPhi->Clone("ecalavgevetaphi");
  hECALavgTPETperEvEtaPhi->Scale(1. / nPassing);
  hECALavgTPETperEvEtaPhi->SetTitle("ECAL average TP E_{T} per event");

  // Plot TP ET vs iEta for ECAL, HCAL and towers
  // Need to do divide before plotting occupancy otherwise normalisation is wrong
  hECALTPETEta->Divide(hECALTPEta);
  hECALTPETEta->Scale(nPassing); // Offset normalisation in formatPlot1D
  formatPlot1D(hECALTPETEta, ecalColour);
  canvas->SaveAs("./Plots/ECAL/ECALTPETEta.pdf");

  hHCALTPETEta->Divide(hHCALTPEta);
  hHCALTPETEta->Scale(nPassing);
  formatPlot1D(hHCALTPETEta, hcalColour);
  canvas->SaveAs("./Plots/HCAL/HCALTPETEta.pdf");

  hTowTPETEta->Divide(hAllTowEta);
  hTowTPETEta->Scale(nPassing);
  formatPlot1D(hTowTPETEta, towerColour);
  canvas->SaveAs("./Plots/Towers/TowTPETEta.pdf");

  // Plot nTPs vs iEta for ECAL, HCAL and towers
  formatPlot1D(hECALTPEta, ecalColour);
  canvas->SaveAs("./Plots/ECAL/ECALTPEta.pdf");

  formatPlot1D(hHCALTPEta, hcalColour);
  canvas->SaveAs("./Plots/HCAL/HCALTPEta.pdf");

  formatPlot1D(hAllTowEta, towerColour);
  canvas->SaveAs("./Plots/Towers/TowEta.pdf");

  // Plot TP ET vs iPhi for ECAL, EB, EE, HCAL, HB, HE, HF, tower B, tower E, TT28
  hECALTPETphi->Divide(hECALTPPhi);
  hECALTPETphi->Scale(nPassing);
  formatPlot1D(hECALTPETphi, ecalColour);
  canvas->SaveAs("./Plots/ECAL/ECALTPETphi.pdf");
  
  hECALTPETphiEB->Divide(hECALTPPhiEB);
  hECALTPETphiEB->Scale(nPassing);
  formatPlot1D(hECALTPETphiEB, ecalColour);
  canvas->SaveAs("./Plots/ECAL/ECALTPETphiEB.pdf");

  hECALTPETphiEE->Divide(hECALTPPhiEE);
  hECALTPETphiEE->Scale(nPassing);
  formatPlot1D(hECALTPETphiEE, ecalColour);
  canvas->SaveAs("./Plots/ECAL/ECALTPETphiEE.pdf");

  hECALTPETphiforTT28->Divide(hECALTPPhiforTT28);
  hECALTPETphiforTT28->Scale(nPassing);
  formatPlot1D(hECALTPETphiforTT28, ecalColour);
  canvas->SaveAs("./Plots/ECAL/ECALTPETphiforTT28.pdf");

  hHCALTPETphi->Divide(hHCALTPPhi);
  hHCALTPETphi->Scale(nPassing);
  formatPlot1D(hHCALTPETphi, hcalColour);
  canvas->SaveAs("./Plots/HCAL/HCALTPETphi.pdf");

  hHCALTPETphiHB->Divide(hHCALTPPhiHB);
  hHCALTPETphiHB->Scale(nPassing);
  formatPlot1D(hHCALTPETphiHB, hcalColour);
  canvas->SaveAs("./Plots/HCAL/HCALTPETphiHB.pdf");

  hHCALTPETphiHE->Divide(hHCALTPPhiHE);
  hHCALTPETphiHE->Scale(nPassing);
  formatPlot1D(hHCALTPETphiHE, hcalColour);
  canvas->SaveAs("./Plots/HCAL/HCALTPETphiHE.pdf");

  hHCALTPETphiHF->Divide(hHCALTPPhiHF);
  hHCALTPETphiHF->Scale(nPassing);
  formatPlot1D(hHCALTPETphiHF, hcalColour);
  canvas->SaveAs("./Plots/HCAL/HCALTPETphiHF.pdf");

  hHCALTPETphiforTT28->Divide(hHCALTPPhiforTT28);
  hHCALTPETphiforTT28->Scale(nPassing);
  formatPlot1D(hHCALTPETphiforTT28, hcalColour);
  canvas->SaveAs("./Plots/HCAL/HCALTPETphiforTT28.pdf");

  hTowTPETphiB->Divide(hTowPhiB);
  hTowTPETphiB->Scale(nPassing);
  formatPlot1D(hTowTPETphiB, towerColour);
  canvas->SaveAs("./Plots/Towers/TowTPETphiB.pdf");

  hTowTPETphiE->Divide(hTowPhiE);
  hTowTPETphiE->Scale(nPassing);
  formatPlot1D(hTowTPETphiE, towerColour);
  canvas->SaveAs("./Plots/Towers/TowTPETphiE.pdf");

  // Plot nTPs vs iPhi for ECAL, EB, EE, HCAL, HB, HE, HF, tower B, tower E, TT28
  formatPlot1D(hECALTPPhi, ecalColour);
  canvas->SaveAs("./Plots/ECAL/ECALTPPhi.pdf");

  formatPlot1D(hECALTPPhiEB, ecalColour);
  canvas->SaveAs("./Plots/ECAL/ECALTPPhiEB.pdf");

  formatPlot1D(hECALTPPhiEE, ecalColour);
  canvas->SaveAs("./Plots/ECAL/ECALTPPhiEE.pdf");

  formatPlot1D(hECALTPPhiforTT28, ecalColour);
  canvas->SaveAs("./Plots/ECAL/ECALTPPhiforTT28.pdf");

  formatPlot1D(hHCALTPPhi, hcalColour);
  canvas->SaveAs("./Plots/HCAL/HCALTPPhi.pdf");

  formatPlot1D(hHCALTPPhiHB, hcalColour);
  canvas->SaveAs("./Plots/HCAL/HCALTPPhiHB.pdf");

  formatPlot1D(hHCALTPPhiHE, hcalColour);
  canvas->SaveAs("./Plots/HCAL/HCALTPPhiHE.pdf");

  formatPlot1D(hHCALTPPhiHF, hcalColour);
  canvas->SaveAs("./Plots/HCAL/HCALTPPhiHF.pdf");

  formatPlot1D(hHCALTPPhiforTT28, hcalColour);
  canvas->SaveAs("./Plots/HCAL/HCALTPPhiforTT28.pdf");

  formatPlot1D(hTowPhiB, towerColour);
  canvas->SaveAs("./Plots/Towers/TowPhiBarrel.pdf");

  formatPlot1D(hTowPhiE, towerColour);
  canvas->SaveAs("./Plots/Towers/TowPhiEndcap.pdf");

  // Plot 2D TP occupancy and ET for ECAL HCAL, TT28
  formatPlot2D(hECALTPEtaPhi);
  canvas->SaveAs("./Plots/ECAL/ECALTPEtaPhi.pdf");

  formatPlot2D(hHCALTPEtaPhi);
  canvas->SaveAs("./Plots/HCAL/HCALTPEtaPhi.pdf");

  hECALavgTPETEtaPhi->Divide(hECALTPEtaPhi); // Normalise the TP ET by the number of TPs in the eta-phi bin
  formatPlot2D(hECALavgTPETEtaPhi);
  canvas->SaveAs("./Plots/ECAL/ECALavgTPETEtaPhi.pdf");

  hHCALavgTPETEtaPhi->Divide(hHCALTPEtaPhi);
  formatPlot2D(hHCALavgTPETEtaPhi);
  canvas->SaveAs("./Plots/HCAL/HCALavgTPETEtaPhi.pdf");

  hECALavgTPETforTT28->Divide(hECALTPEtaPhi);
  formatPlot2D(hECALavgTPETforTT28);
  canvas->SaveAs("./Plots/ECAL/ECALavgTPETforTT28.pdf");

  hHCALavgTPETforTT28->Divide(hHCALTPEtaPhi);
  formatPlot2D(hHCALavgTPETforTT28);
  canvas->SaveAs("./Plots/HCAL/HCALavgTPETforTT28.pdf");

  formatPlot2D(hECALavgTPETperEvEtaPhi);
  canvas->SaveAs("./Plots/ECAL/ECALavgTPETperEvEtaPhi.pdf");

  formatPlot2D(hHCALavgTPETperEvEtaPhi);
  canvas->SaveAs("./Plots/HCAL/HCALavgTPETperEvEtaPhi.pdf");

  // Plot MET Phi 1D
  formatPlot1D(hMetPhi, towerColour);
  canvas->SaveAs("./Plots/MetPhi.pdf");

  // Plot MET Phi 2D
  formatPlot2D(hMetPhiEcalHcal);
  canvas->SaveAs("./Plots/MetPhiEcalHcal.pdf");

  formatPlot2D(hMetPhiEcalTotal);
  canvas->SaveAs("./Plots/MetPhiEcalTotal.pdf");

  formatPlot2D(hMetPhiHcalTotal);
  canvas->SaveAs("./Plots/MetPhiHcalTotal.pdf");

  // Plot MET scalar 1D
  canvas->SetLogy(1); // Sets y-axis to log(10)
  hMetScal->Scale(nPassing);
  formatPlot1D(hMetScal, towerColour);
  canvas->SaveAs("./Plots/MetScal.pdf");
  canvas->SetLogy(0); // Revert y-axis back to linear scale

  // Plot MET scalar 2D
  formatPlot2D(hMetScalEcalHcal);
  canvas->SaveAs("./Plots/MetScalEcalHcal.pdf");

  formatPlot2D(hMetScalEcaltotal);
  canvas->SaveAs("./Plots/MetScalEcaltotal.pdf");

  formatPlot2D(hMetScalHcaltotal);
  canvas->SaveAs("./Plots/MetScalHcaltotal.pdf");

  // Plot ETT scalar 1D
  hEttScal->Scale(nPassing);
  formatPlot1D(hEttScal, towerColour);
  canvas->SaveAs("Plots/EttScal.pdf");

  // Plot ETT scalar 2D
  formatPlot2D(hEttScalEcalHcal);
  canvas->SaveAs("Plots/EttScalEcalHcal.pdf");

  formatPlot2D(hEttScalEcaltotal);
  canvas->SaveAs("Plots/EttScalEcaltotal.pdf");

  formatPlot2D(hEttScalHcaltotal);
  canvas->SaveAs("Plots/EttScalEHcaltotal.pdf");

  // Plot histograms for individual events
  for (int x = 0; x < min(N_INDIV_EVENTS, nPassing); ++x) {
    formatPlot2D(hECALTPETEtaPhiIndiv[x]);
    string saveNameE = "./Plots/Individual_Events/" + to_string(x) + "_ECALTPETEtaPhiIndiv.pdf";
    canvas->SaveAs(saveNameE.c_str());

    formatPlot2D(hHCALTPETEtaPhiIndiv[x]);
    string saveNameH = "./Plots/Individual_Events/" + to_string(x) + "_HCALTPETEtaPhiIndiv.pdf";
    canvas->SaveAs(saveNameH.c_str());
  }

}
