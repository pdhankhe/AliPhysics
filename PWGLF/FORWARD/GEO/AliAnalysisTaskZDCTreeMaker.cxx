/**************************************************************************
 * Copyright(c) 1998-2008, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/////////////////////////////////////////////////////////////
//							   //
//	Class to analyze ZDC data			   //
//							   //
/////////////////////////////////////////////////////////////

#include <TTree.h>
//#include <TList.h>
#include <TFile.h>
#include <TString.h>
#include <TCanvas.h>

#include "AliAnalysisManager.h"
#include "AliInputEventHandler.h"
#include "AliVEvent.h"
#include "AliESD.h"
#include "AliESDEvent.h"
#include "AliESDHeader.h"
#include "AliESDInputHandler.h"
#include "AliESDZDC.h"
#include "AliMultiplicity.h"
#include "AliAODHandler.h"
#include "AliAODEvent.h"
#include "AliAODHeader.h"
#include "AliAODVertex.h"
#include "AliAODVZERO.h"
#include "AliAODZDC.h"
#include "AliAODMCHeader.h"
#include "AliMCEventHandler.h"
#include "AliMCEvent.h"
#include "AliHeader.h"
#include "AliAODMCParticle.h"
#include "AliAnalysisTaskSE.h"
#include "AliGenEventHeader.h"
#include "AliGenHijingEventHeader.h"
#include "AliPhysicsSelectionTask.h"
#include "AliPhysicsSelection.h"
#include "AliTriggerAnalysis.h"
#include "AliCentrality.h"
#include "AliAnalysisTaskZDCTreeMaker.h"

ClassImp(AliAnalysisTaskZDCTreeMaker)


//________________________________________________________________________
AliAnalysisTaskZDCTreeMaker::AliAnalysisTaskZDCTreeMaker():
  AliAnalysisTaskSE(),
    fDebug(0),
    fAnalysisInput("ESD"),
    fIsMCInput(kFALSE),
    fUseSpecialOutput(kFALSE),
    fOutput(0x0),
    fCentralityTree(0x0),
    fIsEventSelected(kFALSE),
    fIsPileupFromSPD(kFALSE),
    /*fxVertex(0),	 
    fyVertex(0),	 
    fzVertex(0),	 
    fVertexer3d(kFALSE),*/
    fNTracklets(0),
    fIsV0ATriggered(0),
    fIsV0CTriggered(0),
    fMultV0A(0),	 
    fMultV0C(0), 
    fESDFlag(0),	 
    /*fZNCEnergy(0), 
    fZPCEnergy(0),  
    fZNAEnergy(0),  
    fZPAEnergy(0),*/
    fZEM1Energy(0), 
    fZEM2Energy(0),
    //fCentralityV0M(0),
    fCentralityV0A(0),
    fCentralityV0C(0),
    fCentralityCL1(0),
    fCentralityZNA(0),
    fCentralityZPA(0),    
    fCentralityZNC(0),
    fCentralityZPC(0)    
{   
   // Default constructor

  for(int i=0; i<400; i++) fTracklEta[i] = -999.;
  fNClusters[0]=fNClusters[1]=0;
  for(int i=0; i<8; i++) fRingMultV0[i] = 0.;
  for(Int_t itow=0; itow<5; itow++){
     fZNCtower[itow]=0.;  
     fZPCtower[itow]=0.;  
     fZNAtower[itow]=0.;  
     fZPAtower[itow]=0.;  
     fZNCtowerLG[itow]=0.;
     fZPCtowerLG[itow]=0.;
     fZNAtowerLG[itow]=0.;
     fZPAtowerLG[itow]=0.;

  }
  /*for(Int_t itdc=0; itdc<32; itdc++){
    for(Int_t ihit=0; ihit<4; ihit++) fTDCvalues[itdc][ihit]=9999;
  }*/
  
}   

//________________________________________________________________________
AliAnalysisTaskZDCTreeMaker::AliAnalysisTaskZDCTreeMaker(const char *name):
  AliAnalysisTaskSE(name),
    fDebug(0),
    fAnalysisInput("ESD"),
    fIsMCInput(kFALSE),
    fUseSpecialOutput(kFALSE),
    fOutput(0x0),
    fCentralityTree(0x0),
    fIsEventSelected(kFALSE),
    fIsPileupFromSPD(kFALSE),
    /*fxVertex(0),	 
    fyVertex(0),	 
    fzVertex(0),	 
    fVertexer3d(kFALSE),*/
    fNTracklets(0),
    fIsV0ATriggered(0),
    fIsV0CTriggered(0),
    fMultV0A(0),	 
    fMultV0C(0), 
    fESDFlag(0),	 
    /*fZNCEnergy(0), 
    fZPCEnergy(0),  
    fZNAEnergy(0),  
    fZPAEnergy(0),*/
    fZEM1Energy(0), 
    fZEM2Energy(0),
    //fCentralityV0M(0),
    fCentralityV0A(0),
    fCentralityV0C(0),
    fCentralityCL1(0),
    fCentralityZNA(0),
    fCentralityZPA(0),    
    fCentralityZNC(0),
    fCentralityZPC(0)    
    
{
  // Default constructor

  for(int i=0; i<400; i++) fTracklEta[i] = -999.;
  fNClusters[0]=fNClusters[1]=0;
  for(int i=0; i<8; i++) fRingMultV0[i] = 0.;
 
  for(Int_t itow=0; itow<5; itow++){
     fZNCtower[itow]=0.;  
     fZPCtower[itow]=0.;  
     fZNAtower[itow]=0.;  
     fZPAtower[itow]=0.;  
     fZNCtowerLG[itow]=0.;
     fZPCtowerLG[itow]=0.;
     fZNAtowerLG[itow]=0.;
     fZPAtowerLG[itow]=0.;

  }
  /*for(Int_t itdc=0; itdc<32; itdc++){
    for(Int_t ihit=0; ihit<4; ihit++) fTDCvalues[itdc][ihit]=9999;
  }*/
  
  // Output slot #1 writes into a TList container
  DefineOutput(1, TList::Class()); 
  //DefineOutput(1, TTree::Class()); 
  
}
 
//________________________________________________________________________
AliAnalysisTaskZDCTreeMaker::~AliAnalysisTaskZDCTreeMaker()
{
  // Destructor
  if(fOutput && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()){
    delete fOutput; fOutput=0;
  } 
  /*if(fCentralityTree && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()){
    delete fCentralityTree; fCentralityTree=0;
  } 
  */ 
}  

//________________________________________________________________________
void AliAnalysisTaskZDCTreeMaker::UserCreateOutputObjects()
{
  // Create the output containers
  if(fDebug>1) printf("AliAnalysisTaskZDCTreeMaker::UserCreateOutputObjects() \n");

  if (fUseSpecialOutput) OpenFile(1);

  // Several histograms are more conveniently managed in a TList
  fOutput = new TList;
  fOutput->SetOwner();
  //fOutput->SetName("output");

    fCentralityTree = new TTree("fCentralityTree", "Centrality vs. multiplicity tree");
    //
    fCentralityTree->Branch("trigClass",&fTrigClass,"trigClass/C");
    fCentralityTree->Branch("eventSelected",&fIsEventSelected,"eventSelected/O");
    fCentralityTree->Branch("pileupSPD",&fIsPileupFromSPD,"pileupSPD/O");
    /*fCentralityTree->Branch("xVertex", &fxVertex,"xVertex/D");
    fCentralityTree->Branch("yVertex", &fyVertex,"yVertex/D");
    fCentralityTree->Branch("zVertex", &fzVertex,"zVertex/D");
    fCentralityTree->Branch("vertexer3d", &fVertexer3d,"vertexer3d/O");*/
    fCentralityTree->Branch("nTracklets", &fNTracklets,"nTracklets/I");
    fCentralityTree->Branch("tracklEta", fTracklEta,"tracklEta[400]/D");
    fCentralityTree->Branch("nClusters", fNClusters,"nClusters[2]/I");
    //
    fCentralityTree->Branch("isV0ATriggered", &fIsV0ATriggered,"isV0ATriggered/I");
    fCentralityTree->Branch("isV0CTriggered", &fIsV0CTriggered,"isV0CTriggered/I");
    fCentralityTree->Branch("multV0A", &fMultV0A,"multV0A/F");
    fCentralityTree->Branch("multV0C", &fMultV0C,"multV0C/F");
    fCentralityTree->Branch("ringmultV0", fRingMultV0,"ringmultV0[8]/F");
    
    fCentralityTree->Branch("esdFlag", &fESDFlag,"esdFlag/i");
    /*fCentralityTree->Branch("zncEnergy",  &fZNCEnergy,  "zncEnergy/F");
    fCentralityTree->Branch("zpcEnergy",  &fZPCEnergy,  "zpcEnergy/F");
    fCentralityTree->Branch("znaEnergy",  &fZNAEnergy,  "znaEnergy/F");
    fCentralityTree->Branch("zpaEnergy",  &fZPAEnergy,  "zpaEnergy/F");*/
    fCentralityTree->Branch("zem1Energy", &fZEM1Energy, "zem1Energy/F");
    fCentralityTree->Branch("zem2Energy", &fZEM2Energy, "zem2Energy/F");

    fCentralityTree->Branch("znctower", fZNCtower, "znctower[5]/F");
    fCentralityTree->Branch("zpctower", fZPCtower, "zpctower[5]/F");
    fCentralityTree->Branch("znatower", fZNAtower, "znatower[5]/F");
    fCentralityTree->Branch("zpatower", fZPAtower, "zpatower[5]/F");
    fCentralityTree->Branch("znctowerLG", fZNCtowerLG, "znctowerLG[5]/F");
    fCentralityTree->Branch("zpctowerLG", fZPCtowerLG, "zpctowerLG[5]/F");
    fCentralityTree->Branch("znatowerLG", fZNAtowerLG, "znatowerLG[5]/F");
    fCentralityTree->Branch("zpatowerLG", fZPAtowerLG, "zpatowerLG[5]/F");

//    fCentralityTree->Branch("tdc", fTDCvalues, "tdc[32][4]/I");
//    fCentralityTree->Branch("tdcCorr", fTDCcorr, "tdcCorr[32][4]/F");
     
    //fCentralityTree->Branch("centrV0mult", &fCentralityV0M, "centrV0mult/F");
    fCentralityTree->Branch("centrV0Amult", &fCentralityV0A, "centrV0Amult/F");
    fCentralityTree->Branch("centrV0Cmult", &fCentralityV0C, "centrV0Cmult/F");
    fCentralityTree->Branch("centrSPDclu1", &fCentralityCL1, "centrSPDclu1/F");
    fCentralityTree->Branch("centrZNA", &fCentralityZNA, "centrZNA/F");
    fCentralityTree->Branch("centrZPA", &fCentralityZPA, "centrZPA/F");
    fCentralityTree->Branch("centrZNC", &fCentralityZNC, "centrZNC/F");
    fCentralityTree->Branch("centrZPC", &fCentralityZPC, "centrZPC/F");
  
    fOutput->Add(fCentralityTree);      
    PostData(1, fOutput);
  
  //PostData(1, fCentralityTree);
}

//________________________________________________________________________
void AliAnalysisTaskZDCTreeMaker::UserExec(Option_t */*option*/)
{
  // Execute analysis for current event:
  if(fDebug>1) printf(" **** AliAnalysisTaskZDCTreeMaker::UserExec() \n");
  
  if (!InputEvent()) {
    Printf("ERROR: InputEvent not available");
    return;
  }

  if(fAnalysisInput.CompareTo("ESD")==0){
      
      //printf(" \t ***Analizing ESD ev. %d\n",fNev);
      
      AliESDEvent* esd = dynamic_cast<AliESDEvent*> (InputEvent());
      if(!esd) return;
      
      // Select PHYSICS events (type=7, for data)
      if(!fIsMCInput && esd->GetEventType()!=7) return; 

      for(int ir=0; ir<8; ir++) fRingMultV0[ir]=0.;
            
      // ***** Trigger selection
      TString triggerClass = esd->GetFiredTriggerClasses();
      sprintf(fTrigClass,"%s",triggerClass.Data());
      
      // use response of AliPhysicsSelection
      fIsEventSelected = (((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected() & AliVEvent::kAnyINT);       
      fIsPileupFromSPD = esd->IsPileupFromSPD(6, 0.8);
      //fIsPileupFromSPD = esd->IsPileupFromSPDInMultBins();

      AliCentrality *centrality = esd->GetCentrality();
      //fCentralityV0M = centrality->GetCentralityPercentile("V0M");
      fCentralityV0A = centrality->GetCentralityPercentile("V0A");
      fCentralityV0C = centrality->GetCentralityPercentile("V0C");
      fCentralityCL1 = centrality->GetCentralityPercentile("CL1");
      fCentralityZNA = centrality->GetCentralityPercentile("ZNA");
      fCentralityZPA = centrality->GetCentralityPercentile("ZPA");
      fCentralityZNC = centrality->GetCentralityPercentile("ZNC");
      fCentralityZPC = centrality->GetCentralityPercentile("ZPC");
                
      /*const AliESDVertex *vertex = esd->GetPrimaryVertexSPD();
      fxVertex = vertex->GetX();
      fyVertex = vertex->GetY();
      fzVertex = vertex->GetZ();
      if(vertex->IsFromVertexer3D()) fVertexer3d = kTRUE;
      else fVertexer3d = kFALSE;*/
      
      const AliMultiplicity *mult = esd->GetMultiplicity();
      fNTracklets = mult->GetNumberOfTracklets();
      for(int itr=0; itr<fNTracklets; itr++){
         if(itr<400) fTracklEta[itr] = mult->GetEta(itr);
      }
      
      for(Int_t ilay=0; ilay<2; ilay++){
        fNClusters[ilay] = mult->GetNumberOfITSClusters(ilay);
      }
          
      AliESDVZERO *vzeroESD = esd->GetVZEROData();
      fMultV0A = vzeroESD->GetMTotV0A();
      fMultV0C = vzeroESD->GetMTotV0C();
      //
      fIsV0ATriggered = vzeroESD->GetV0ADecision();
      fIsV0CTriggered = vzeroESD->GetV0CDecision();
      //
      for(Int_t iRing = 0; iRing < 8; ++iRing) {
    	for(Int_t i = 0; i < 8; ++i) {
//      	   fRingMultV0[iRing] += esd->GetVZEROEqMultiplicity(8*iRing+i);
      	   fRingMultV0[iRing] += vzeroESD->GetMultiplicity(8*iRing+i);
    	}
      }
        
      AliESDZDC *esdZDC = esd->GetESDZDC();
      
      fESDFlag =  esdZDC->GetESDQuality();   
      
      /*fZNCEnergy = (Float_t) (esdZDC->GetZDCN1Energy());
      fZPCEnergy = (Float_t) (esdZDC->GetZDCP1Energy());
      fZNAEnergy = (Float_t) (esdZDC->GetZDCN2Energy());
      fZPAEnergy = (Float_t) (esdZDC->GetZDCP2Energy());*/
      fZEM1Energy = (Float_t) (esdZDC->GetZDCEMEnergy(0));
      fZEM2Energy = (Float_t) (esdZDC->GetZDCEMEnergy(1));
       
      const Double_t * towZNC = esdZDC->GetZN1TowerEnergy();
      const Double_t * towZPC = esdZDC->GetZP1TowerEnergy();
      const Double_t * towZNA = esdZDC->GetZN2TowerEnergy();
      const Double_t * towZPA = esdZDC->GetZP2TowerEnergy();
      //
      const Double_t * towZNCLG = esdZDC->GetZN1TowerEnergyLR();
      const Double_t * towZPCLG = esdZDC->GetZP1TowerEnergyLR();
      const Double_t * towZNALG = esdZDC->GetZN2TowerEnergyLR();
      const Double_t * towZPALG = esdZDC->GetZP2TowerEnergyLR();
      //
      for(Int_t it=0; it<5; it++){
         fZNCtower[it] = (Float_t) (towZNC[it]);
         fZPCtower[it] = (Float_t) (towZPC[it]);
         fZNAtower[it] = (Float_t) (towZNA[it]); 
         fZPAtower[it] = (Float_t) (towZPA[it]);  
         fZNCtowerLG[it] = (Float_t) (towZNCLG[it]);
         fZPCtowerLG[it] = (Float_t) (towZPCLG[it]);
         fZNAtowerLG[it] = (Float_t) (towZNALG[it]); 
         fZPAtowerLG[it] = (Float_t) (towZPALG[it]);  
      }
      
      /*for(Int_t itdc=0; itdc<32; itdc++){
	 for(Int_t i=0; i<4; i++){
	   fTDCvalues[itdc][i] = esdZDC->GetZDCTDCData(itdc, i);
	 }
      }*/      

  }   
  else if(fAnalysisInput.CompareTo("AOD")==0){

      printf("\n \t *** Analysis on AODs is NOT implemented\n\n");
      

  }
  
  fCentralityTree->Fill();
 
  PostData(1, fOutput);
  
  //PostData(1, fCentralityTree);
   
}



//________________________________________________________________________
void AliAnalysisTaskZDCTreeMaker::Terminate(Option_t */*option*/)
{
  // Terminate analysis
  //
  printf(" **** AliAnalysisTaskZDCTreeMaker::Terminate() \n");
}
