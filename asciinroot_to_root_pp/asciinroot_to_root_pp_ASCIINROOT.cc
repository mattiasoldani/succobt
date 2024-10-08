/*
# # # # # # # # # #
# Original tool by Valerio (https://baltig.infn.it/insulab/tbtools)
# Edited by Mattia and Gemma initially for the NanoCal 2023 beamtest @ T9
# # # # # # # # # #
# Open all the input data and write the selected content into a unique single-spill ROOT file
# # # # # # # # # #
*/

#include <string>
#include <sstream>
#include "Riostream.h"
#include "TClassTable.h"
#include "TFile.h"
#include "TTree.h"

void asciinroot_to_root_pp_ASCIINROOT(string inFileName="", string outFileName="", string inFileNameTree="") {

std::cout<<"--> converting file "<<inFileName<<std::endl;
std::cout<<"-->       into file "<<outFileName<<std::endl;

// ELEMENTS IN THE SETUP
// REMEMBER THE DIGITIZERS:
// #0 - 1742
// #1 - 1742
// #2 - 1730

const int nDigiCh0 = 32 ;
const int nDigiCh1 = 32 ;
const int nDigiCh2 = 0 ;
const int nDigiCh = nDigiCh0+nDigiCh1+nDigiCh2 ;
const int lWf0 = 1024 ;
const int lWf1 = 1024 ;
const int lWf2 = 1024 ;
const int lWfDummyPre = 5 ;
const int lWfDummyPost = 2 ;

const int nDigiFastCh0 = 4 ;
const int nDigiFastCh1 = 4 ;
const int nDigiFastCh2 = 0 ;
const int nDigiFastCh = nDigiFastCh0+nDigiFastCh1+nDigiFastCh2 ;
const int lWfFast0 = 1024 ;
const int lWfFast1 = 1024 ;
const int lWfFast2 = 1024 ;
const int lWfFastDummyPre = 2 ;
const int lWfFastDummyPost = 2 ;

const int nSiliPlanes = 4 ;
const int nGonio = 5 ;

// OPEN INPUT TEXT FILE
std::cout<<"--> opening input ASCII"<<std::endl;
ifstream myfile;
string line;
Int_t lineNumber=0;
myfile.open(inFileName.c_str()); 

// OPEN INPUT ROOT FILE(S) AND GET TREE(S)
std::cout<<"--> opening input trees"<<std::endl;
Int_t dummy=0;
Int_t treevt=0;
TFile* inFileTree = new TFile(inFileNameTree.c_str());
TTree* ntree0=(TTree*)inFileTree->Get("h3");  // #0 - 1742
TTree* ntree1=(TTree*)inFileTree->Get("h4");  // #1 - 1742
//TTree* ntree2=(TTree*)inFileTree->Get("h2");  // #2 - 1730

// CREATE OUTPUT TREE AND ROOT FILE
std::cout<<"--> opening output file"<<std::endl;
TFile* outFile = new TFile(outFileName.c_str(),"RECREATE"); 
TTree* t = new TTree("t","");

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// vvv VARIABLES IN OUTPUT TREE (ASSOCIATED TO OBJECTS IN MEMORY HERE)

// >>> EVENT NR., SPILL NR., ETCETERA - FROM ASCII
Int_t iTime;
Int_t iATime;
Int_t eventNumber;
Int_t ntEv;
Int_t iDummy;
Int_t iScan;

t->Branch("eventNumber",&eventNumber,"eventNumber/I");
t->Branch("iScan",&iScan,"iScan/I"); 

// >>> SILICON TRACKERS - FROM ASCII
Float_t xRaw[nSiliPlanes];
UShort_t nStripHit[nSiliPlanes];
UShort_t nHit[nSiliPlanes];

t->Branch("xRaw",xRaw,Form("xRaw[%d]/F",nSiliPlanes));
t->Branch("nStripHit",nStripHit,Form("nStripHit[%d]/s",nSiliPlanes));
t->Branch("nHit",nHit,Form("nHit[%d]/s",nSiliPlanes));

//// >>> GONIOMETER - FROM ASCII
Float_t gonio[nGonio];

t->Branch("gonio",gonio,Form("gonio[%d]/F",nGonio));

// >>> DIGITIZER FAST ANALYSIS DATA (PH, TIME, ETCETERA) - FROM ASCII
UShort_t digiBase[nDigiCh];    
UShort_t digiPh[nDigiCh];      
UShort_t digiTime[nDigiCh];    
UShort_t digiTime05[nDigiCh];  

t->Branch("digiBase",digiBase,Form("digiBase[%d]/s",nDigiCh));
t->Branch("digiPh",digiPh,Form("digiPh[%d]/s",nDigiCh));
t->Branch("digiTime",digiTime,Form("digiTime[%d]/s",nDigiCh));
t->Branch("digiTime05",digiTime05,Form("digiTime05[%d]/s",nDigiCh));

// >>> DIGITIZER WAVEFORMS
UShort_t wave[nDigiCh][lWf0] = {0};
UShort_t wavefast[nDigiFastCh][lWf0] = {0};

for (Int_t i=0;i<nDigiCh;i++) {t->Branch(Form("wave%d",i),wave[i],Form("wave%d[%d]/s",i,lWf0));}
for (Int_t i=0;i<nDigiFastCh;i++) {t->Branch(Form("wavefast%d",i),wavefast[i],Form("wavefast%d[%d]/s",i,lWf0));}

// ^^^ END OF VARIABLES IN OUTPUT TREE
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// UNPACK INPUT TREE(S)

// #0 - 1742
Int_t t_eventNumber;
UShort_t t_idigi_0[(lWfDummyPre+lWf0+lWfDummyPost)*nDigiCh0];
UShort_t t_idigifast_0[(lWfDummyPre+lWf0+lWfDummyPost)*nDigiFastCh0];

ntree0->SetBranchAddress("Idigi3",&t_eventNumber);
ntree0->SetBranchAddress("Idigi_742a", &t_idigi_0[0]);
ntree0->SetBranchAddress("Idigifast_742a", &t_idigifast_0[0]);

// #1 - 1742
UShort_t t_idigi_1[(lWfDummyPre+lWf1+lWfDummyPost)*nDigiCh1];
UShort_t t_idigifast_1[(lWfDummyPre+lWf1+lWfDummyPost)*nDigiFastCh1];

ntree1->SetBranchAddress("Idigi_742b", &t_idigi_1[0]);
ntree1->SetBranchAddress("Idigifast_742b", &t_idigifast_1[0]);

// #2 - 1730
//UShort_t t_idigi_2[(lWfDummyPre+lWf2+lWfDummyPost)*nDigiCh2];

//ntree2->SetBranchAddress("Idigi_730", &t_idigi_2[0]);

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// vvv SCAN ON INPUT TEXT FILE
if (myfile.is_open()) {
while(getline(myfile,line)) {

  if(lineNumber%1==0){std::cout<<"--> scanning ASCII file, line "<<lineNumber<<"..."<<std::endl;}
  
  // FROM THE ASCII (MANUALLY SET THE DETAILS)
  std::stringstream ss(line);      
  for (Int_t i=0;i<nSiliPlanes;i++) ss >> xRaw[i];
  for (Int_t i=0;i<nSiliPlanes;i++) ss >> nStripHit[i];
  //for (Int_t i=0;i<nSiliPlanes;i++) ss >> nHit[i];
  //for (Int_t idigi=0;idigi<nDigiCh2;idigi++) ss >> digiBase[nDigiCh0+nDigiCh1+idigi] ;
  //for (Int_t idigi=0;idigi<nDigiCh2;idigi++) ss >> digiPh[nDigiCh0+nDigiCh1+idigi] ;
  //for (Int_t idigi=0;idigi<nDigiCh2;idigi++) ss >> digiTime[nDigiCh0+nDigiCh1+idigi] ;
  //for (Int_t idigi=0;idigi<nDigiCh2;idigi++) 0 >> digiTime05[nDigiCh0+nDigiCh1+idigi] ;  // dump
  for (Int_t idigi=0;idigi<(nDigiCh0+nDigiCh1);idigi++) ss >> digiBase[idigi] ;
  for (Int_t idigi=0;idigi<(nDigiCh0+nDigiCh1);idigi++) ss >> digiTime[idigi] ;
  for (Int_t idigi=0;idigi<(nDigiCh0+nDigiCh1);idigi++) ss >> digiTime05[idigi] ;
  for (Int_t idigi=0;idigi<(nDigiCh0+nDigiCh1);idigi++) ss >> digiPh[idigi] ;
  for (Int_t i=0;i<nGonio;i++) ss >> gonio[i];
  ss >> iDummy >> iScan >> iDummy >> iDummy >> iDummy >> iDummy >> iDummy >> iDummy ;
  ss >> eventNumber;
  
  // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
  // vvv LOOP ON ENTRIES IN TREE(S) FOR EACH EVENT IN TEXT FILE
  while(treevt<ntree0->GetEntries()){  // choose the input tree from which to get the event number here
      
    // GET SINGLE ENTRY
    ntree0->GetEntry(treevt);  // #0 - 1742
    ntree1->GetEntry(treevt);  // #1 - 1742
    //ntree2->GetEntry(treevt);  // #2 - 1730
    std::cout<<"--> event "<<t_eventNumber<<" in ROOT tree and "<<eventNumber<<" in ASCII"<<std::endl;
  
    // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    // vvv INSPECT SINGLE ENTRY, AND IF IT MATCHES ONE OF THE ASCII ENTRIES...
    
    // if involving ASCII event info - select all ROOT events matching ASCII event number
    if(t_eventNumber==eventNumber){
        
    // if involving ASCII event info - select all ROOT events matching ASCII event number with different time-dependent rules
    //if(
    //((iATime<1686824091) && (t_eventNumber==ntEv)) // before run 650276 (tweak)
    //||
    //((iATime>=1686824091) && (t_eventNumber==eventNumber)) // run 650276 and after (standard)
    //) {
        
    // if involving ASCII event info - select all ROOT events matching ASCII event number but excluding some specific values of other ASCII variables
    //if ((t_eventNumber==eventNumber)&&(iATime!=1686824091)) {
  
      // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
      // vvv UNPACK VARIABLES FROM TREE #0 - 1742
      
      // >>> STANDARD WAVEFORMS
      int j=0;
      int k=-1;
      for (int ii=0; ii<(lWfDummyPre+lWf0+lWfDummyPost)*nDigiCh0; ii++){
        if( ii%(lWfDummyPre+lWf0+lWfDummyPost)==0) {
          j=0;
          k++;
        }
        if(
        (ii%(lWfDummyPre+lWf0+lWfDummyPost))>=lWfDummyPre && 
        (ii%(lWfDummyPre+lWf0+lWfDummyPost))<(lWfDummyPre+lWf0)
        ){  
          wave[k][j]=t_idigi_0[ii];
          j++;
        }
      }
      
      // >>> FAST WAVEFORMS
      j=0;
      k=-1;
      for (int ii=0; ii<(lWfFastDummyPre+lWfFast0+lWfFastDummyPost)*nDigiFastCh0; ii++){
        if( ii%(lWfFastDummyPre+lWfFast0+lWfFastDummyPost)==0) {
          j=0;
          k++;
        }
        if(
        (ii%(lWfFastDummyPre+lWfFast0+lWfFastDummyPost))>=lWfFastDummyPre && 
        (ii%(lWfFastDummyPre+lWfFast0+lWfFastDummyPost))<(lWfFastDummyPre+lWfFast0)
        ){  
          wavefast[k][j]=t_idigifast_0[ii];
          j++;
        }
      }
      
      // ^^^ END OF UNPACK VARIABLES FROM TREE #0 - 1742
      // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

      // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
      // vvv UNPACK VARIABLES FROM TREE #1 - 1742
      
      // >>> STANDARD WAVEFORMS
      j=0;
      k=nDigiCh0-1;
      for (int ii=0; ii<(lWfDummyPre+lWf1+lWfDummyPost)*nDigiCh1; ii++){
        if( ii%(lWfDummyPre+lWf1+lWfDummyPost)==0) {
          j=0;
          k++;
        }
        if(
        (ii%(lWfDummyPre+lWf1+lWfDummyPost))>=lWfDummyPre && 
        (ii%(lWfDummyPre+lWf1+lWfDummyPost))<(lWfDummyPre+lWf1)
        ){  
          wave[k][j]=t_idigi_1[ii];
          j++;
        }
      }
      
      // >>> FAST WAVEFORMS
      j=0;
      k=nDigiFastCh0-1;
      for (int ii=0; ii<(lWfFastDummyPre+lWfFast1+lWfFastDummyPost)*nDigiFastCh1; ii++){
        if( ii%(lWfFastDummyPre+lWfFast1+lWfFastDummyPost)==0) {
          j=0;
          k++;
        }
        if(
        (ii%(lWfFastDummyPre+lWfFast1+lWfFastDummyPost))>=lWfFastDummyPre && 
        (ii%(lWfFastDummyPre+lWfFast1+lWfFastDummyPost))<(lWfFastDummyPre+lWfFast1)
        ){  
          wavefast[k][j]=t_idigifast_1[ii];
          j++;
        }
      }
      
      // ^^^ END OF UNPACK VARIABLES FROM TREE #1 - 1742
      // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

      // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
      // vvv UNPACK VARIABLES FROM TREE #2 - 1730
      
      //// >>> STANDARD WAVEFORMS (NO FAST WAVEFORMS HERE)
      //j=0;
      //k=nDigiCh0+nDigiCh1-1;
      //for (int ii=0; ii<(lWfDummyPre+lWf2+lWfDummyPost)*nDigiCh2; ii++){
      //  if( ii%(lWfDummyPre+lWf2+lWfDummyPost)==0) {
      //    j=0;
      //    k++;
      //  }
      //  if(
      //  (ii%(lWfDummyPre+lWf2+lWfDummyPost))>=lWfDummyPre && 
      //  (ii%(lWfDummyPre+lWf2+lWfDummyPost))<(lWfDummyPre+lWf2)
      //  ){  
      //    wave[k][j]=t_idigi_2[ii];
      //    j++;
      //  }
      //}
      //
      //// ^^^ END OF UNPACK VARIABLES FROM TREE #2 - 1730
      //// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

      // FILL OUTPUT TREE WITH THE ASSOCIATED OBJECTS IN MEMORY
      t->Fill();
      treevt++;
      break;
      
    } else {treevt++;}  // end of if involving ASCII event info
    // ^^^ END OF INSPECT SINGLE ENTRY
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  }  // end of while (on trees)
  lineNumber++;
  // ^^^ END OF LOOP ON ENTRIES IN TREE(S)
  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  
}  // end of while (on ASCII)
myfile.close();
} else {std::cout<<"--> ERROR: can't open file!!"<<std::endl;}  // end of if
// ^^^ END OF SCAN ON TEXT FILE
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// WRITE OUTPUT TREE IN OUTPUT ROOT FILE
outFile->cd();
t->Write();
outFile->Close();

return;
}
