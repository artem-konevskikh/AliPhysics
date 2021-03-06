//////////////////////////////////////////////////////////////////////
//
// $Id: AliFlowAnalyser.h 18618 2007-05-16 15:38:22Z snelling $
//
// Author: Emanuele Simili
//
//////////////////////////////////////////////////////////////////////
//
// Description: ALICE flow analysis for AliFlowEvent(s), 
// adapted from the STAR flow analysis .
// Original Authors:                 Raimond Snellings & Art Poskanzer
//
//////////////////////////////////////////////////////////////////////

#ifndef ALIFLOWANALYSER_H
#define ALIFLOWANALYSER_H

#include <TVector2.h>
#include <TFile.h>
#include "AliFlowConstants.h"

class TH1F;
class TH1D;
class TH2F;
class TH2D;
class TH3F;
class TProfile;
class TProfile2D;
class TOrdCollection;
class TClonesArray;

class AliFlowTrack;
class AliFlowV0;
class AliFlowEvent;
class AliFlowSelection;

class AliFlowAnalyser {


public:
  
  AliFlowAnalyser(const AliFlowSelection* flowSelect = 0); 	// Constructor with selection object (default selection if no one given)
  virtual  ~AliFlowAnalyser(); 					// Default destructor (no actions)

 // Steps of the flow analysis
  Bool_t   Init() ;						// Books histograms for flow analysis
  Bool_t   Finish() ;						// Saves histograms, Closes stuff

 // Analysis of 1 event (can be called from outside)
  Bool_t   Analyze(AliFlowEvent* flowEvent = 0) ; 		// Fills the defaults histograms (init them first!) and performs the calculation for the given event         

 // Resolution corrections to v_n (call it at the end of the evts loop)
  Bool_t   Resolution() ;				 	// Calculates resolution and mean flow values
 
 // Weights calculation and saving (call it at the end of the evts loop)
  void     Weightening() ;  					// Calculates weights and fills PhiWgt histograms

 // Options
  void	   SetSub(Int_t sub = 0)          			{ fSub = sub ; }		// Set subevents (1=eta,0=rnd,-1=charged)
  void	   SetEtaSub()          			 	{ SetSub(1) ; }		// Set subevents (1=eta,0=rnd,-1=charged)
  void     SetV1Ep1Ep2(Bool_t v1Ep1Ep2 = kTRUE) 		{ fV1Ep1Ep2 = v1Ep1Ep2 ; }	// Switches the v_1{EP1,EP2} calculation on/off
  void	   SetShuffle(Bool_t sh = kTRUE)  			{ fShuffle = sh ; }		// Set to re-shuffle evt tracks
  void     SetUsePhiWgt(Bool_t pw = kTRUE)  			{ fReadPhiWgt = pw ; }		// Set to use phi weights (true by default...if there)
  void 	   SetUseBayWgt(Bool_t bw = kTRUE) 			{ fBayWgt = bw ; } 		// Set to use bayesian weights for p.id. (false by default)
  void     SetUsePtWgt(Bool_t ptw = kTRUE)		        { fPtWgt = ptw ; }	 	// uses pT as a weight for RP determination
  void     SetUseEtaWgt(Bool_t etw = kTRUE)		        { fEtaWgt = etw ; }	 	// uses eta as a weight for RP determination
  void     SetUseOnePhiWgt(Bool_t opw = kTRUE)			{ fOnePhiWgt = opw ; } 		// just one wgt histogram
  void     SetUseFirstLastPhiWgt(Bool_t flw = kTRUE)		{ fOnePhiWgt = !flw ; }		// uses 3 wgt histograms
  void	   SetFlowForV0(Bool_t v0 = kTRUE) 			{ fV0loop = v0 ; }		// Enables Flow study for v0
  void	   SetTrackLoop(Bool_t trkl = kTRUE) 			{ fTrackLoop = trkl ; }		// Enables Tracks loop (keep it kTRUE)
  void	   SetCustomRespFunc(Bool_t crf = kTRUE) 		{ fCustomRespFunc = crf ; }	// Enables to use a custom detector response function
  //void     SetDebugg(Int_t db = 1) ; 				// set the cout's for debug (default is 1)

 // Histograms
  void     SetPtRangevEta(Float_t lo, Float_t hi)		{ fPtRangevEta[0] = lo ; fPtRangevEta[1] = hi ; }  // Sets the pt range for the v(eta) histograms.
  void     SetEtaRangevPt(Float_t lo, Float_t hi)		{ fEtaRangevPt[0] = lo ; fEtaRangevPt[1] = hi ; }  // Sets the |eta| range for the v(pt) histograms.
  //void     SetMaxLabel(Int_t lab = 100)    			{ fMaxLabel = lab ; }
  
 // Output 
  void	   SetHistFileName(TString name) 			{ fHistFileName = name ; }  	// Sets output file name
  TString  GetHistFileName() const				{ return fHistFileName ; }

 // Phi Weights 
  TString  GetWgtFileName() const 				{ return (TString)fPhiWgtFile->GetName() ; }
  void     FillWgtArrays(TFile* wgtFile) ;			// Loads phi & bayesian weights from file (flowPhiWgt.hist.root) and fills the arrays

 // Results
  Float_t  GetRunBayesian(Int_t nPid=2, Int_t selN=0) const ;  	// Normalized Particle abundance (all events up to here)
  void     PrintRunBayesian(Int_t selN=0) const ;		// Prints the normalized Particle abundance (up to here)
  void     PrintEventQuantities() const ; 			// Prints event by event calculated quantities
  Float_t  Res(Int_t eventN, Int_t harN) const 	     		{ return fRes[eventN][harN]; }	  // Returns the calculated resolution for the RP
  Float_t  ResErr(Int_t eventN, Int_t harN) const 	        { return fResErr[eventN][harN]; } // Returns the estimated error on the resolution 


 protected:
 
 // Internal methods to fill the histogram
  Bool_t   FillFromFlowEvent(AliFlowEvent* fFlowEvent) ;	// Fills internal variables and array from Flow Events
  void     FillEventHistograms(AliFlowEvent* fFlowEvent) ;	// Fills Events' histograms (from AliFlowEvent)
  void     FillParticleHistograms(TClonesArray* fFlowTracks) ;  	// Fills Tracks' histograms (from AliFlowTrack)
  void     FillV0Histograms(TClonesArray* fFlowV0s) ; 		// Fills V0s' histograms
  Int_t    HarmonicsLoop(AliFlowTrack* fFlowTrack) ; 		// Harmonics & Selections histograms (from AliFlowTracks)
  //void     FillLabels() ;					// fills an histogram of Labels (the ones from ESD) 

 // Weights plugged to the event
  void     FillBayesianWgt(AliFlowEvent* fFlowEvent) ; 		// Plugs the bayesian weights (fBayesianWgt[0]*) into the AliFlowEvent
  void 	   FillEvtPhiWgt(AliFlowEvent* fFlowEvent) ; 		// Plugs the PhiWeights (fPhiWgt*, etc.) into the AliFlowEvent
 
 // Resolution Calculation
  Double_t Chi(Double_t res) ;  			 	// Calculates chi from the event plane resolution
  Double_t ResEventPlane(Double_t chi) ;		 	// Calculates the event plane resolution as a function of chi
  Double_t ResEventPlaneK2(Double_t chi) ;		 	// Calculates the event plane resolution as a function of chi for the case k=2.
  Double_t ResEventPlaneK3(Double_t chi) ;		 	// Calculates the event plane resolution as a function of chi for the case k=3.
  Double_t ResEventPlaneK4(Double_t chi) ;		 	// Calculates the event plane resolution as a function of chi for the case k=4.


 private:

 // to make the code checker happy
  AliFlowAnalyser(const AliFlowAnalyser &flowAnal) ; 		// Copy Constructor (dummy)
  AliFlowAnalyser &operator=(const AliFlowAnalyser &flowAnal) ; // Assignment Operator

 // Flags
  Bool_t   	   fTrackLoop ;		     			//! tracks main loop
  Bool_t   	   fV0loop ;		     			//! correlation analysis is done also for neutral secundary vertex
  Bool_t   	   fShuffle ;		     			//! to randomly reshuffle tracks
  Bool_t   	   fV1Ep1Ep2;		     			//! Flag for v_1{EP1,EP2} calculation on/off
  Bool_t   	   fEtaSub;		     			//! eta subevents
  Bool_t   	   fReadPhiWgt ;		     		//! Phi Weights are applied to Phi distrib. (default is false)
  Bool_t   	   fBayWgt ;		     			//! Bayesian Weights are applied to P.Id. (default is false) 
  Bool_t   	   fRePid ;		     			//! Re-Calculates the P.Id. basing on the bayesian wgts (if plugged in)
  Bool_t   	   fCustomRespFunc ;		     		//! A custom "detector response function" is used for P.Id (default is false -> the combined response function from the ESD will be used)
  Int_t   	   fSub;		     			//! subevents (1=eta,0=rnd,-1=charged)

  Bool_t   	   fPtWgt ;		     			//! flag to use pT as a weight for RP determination
  Bool_t   	   fEtaWgt ;		     			//! flag to use eta as a weight for RP determination
  Bool_t   	   fOnePhiWgt ; 	     			//! if kTRUE: just one phi-wgt histogram, if kFALSE: three phi-wgt histogram (TPC+,TPC-,cross)

 // Files
  TFile*           fHistFile ; 				        //! histograms file (output)
  TFile* 	   fPhiWgtFile ; 			 	//! phi weight file 
  TString	   fHistFileName ;			    	//! Output File Name (histograms from flow analysis)
  //TString	   fFlowEvtFileName ;			    	//! Input file name (Flow Events)

 // enumerators 			    
  Int_t            fEventNumber ;	  		    	//! progressive enumeration of AliFlowEvents
  Int_t            fTrackNumber ;	  		    	//! progressive enumeration of AliFlowTracks
  Int_t            fV0Number ;	  		            	//! progressive enumeration of AliFlowV0s
  //Int_t 	   fNumberOfEvents ;			    	//! total number of AliFlowEvents in file
  Int_t 	   fNumberOfTracks ;			    	//! total number of tracks in the current event
  Int_t 	   fNumberOfV0s ;			    	//! total number of v0s in the current event
  Int_t            fPidId ;	  		    		//! Particle Id hypothesys of the track (0..4 for e,mu,pi,k,p)
  Int_t            fSelParts ;	  		    		//! n. of tracks selected for correlation analysis
  Int_t            fSelV0s ;	  		    		//! n. of v0s selected for correlation analysis

  Int_t 	   fTotalNumberOfEvents ;			//! total number of analyzed AliFlowEvents
  Int_t 	   fTotalNumberOfTracks ;			//! total number of analyzed tracks
  Int_t 	   fTotalNumberOfV0s ;			    	//! total number of analyzed v0s

 // Internal pointers
  AliFlowEvent*     fFlowEvent ;      				//! pointer to AliFlowEvent
  AliFlowTrack*     fFlowTrack ;      				//! pointer to AliFlowTrack
  AliFlowV0*        fFlowV0 ;      				//! pointer to AliFlowV0
  AliFlowSelection* fFlowSelect ;     				//! selection object
  TClonesArray*     fFlowTracks ;			        //! pointer to the TrackCollection
  TClonesArray*     fFlowV0s ;  			        //! pointer to the V0Collection

  Float_t           fVertex[3] ;				//! Event's Vertex position 

 // For weights
  Int_t      	    fPhiBins ;     				//! n. of phi bins     
  Float_t 	    fPhiMin ;    				//! wgt histo range (phi)
  Float_t  	    fPhiMax ;     				//! wgt histo range (phi) 

  AliFlowConstants::PhiWgt_t    fPhiWgt ;	 	    	//! PhiWgt Array (all TPC)
  AliFlowConstants::PhiWgt_t    fPhiWgtPlus ;  		    	//! PhiWgt Array (TPC+)
  AliFlowConstants::PhiWgt_t    fPhiWgtMinus ;  	    	//! PhiWgt Array (TPC-)
  AliFlowConstants::PhiWgt_t    fPhiWgtCross ;  	    	//! PhiWgt Array (TPC/)     

 // For bayesian weights
  Double_t fBayesianWgt[AliFlowConstants::kSels][AliFlowConstants::kPid] ;  	//! Bayesian weights (expected particle abundance)
  TVector2 fQ[AliFlowConstants::kSels][AliFlowConstants::kHars];			//! flow vector
  Float_t  fPsi[AliFlowConstants::kSels][AliFlowConstants::kHars];			//! event plane angle
  UInt_t   fMult[AliFlowConstants::kSels][AliFlowConstants::kHars];                  	//! multiplicity
  Float_t  fQnorm[AliFlowConstants::kSels][AliFlowConstants::kHars];                    //! Q/Sqrt(Mult)
  TVector2 fQSub[AliFlowConstants::kSubs][AliFlowConstants::kSels][AliFlowConstants::kHars];      	//! flow vector subs
  Float_t  fPsiSub[AliFlowConstants::kSubs][AliFlowConstants::kSels][AliFlowConstants::kHars];    	//! plane angle of subevents
  UInt_t   fMultSub[AliFlowConstants::kSubs][AliFlowConstants::kSels][AliFlowConstants::kHars];   	//! multiplicity subs
  Float_t  fRes[AliFlowConstants::kSels][AliFlowConstants::kHars];			//! event plane resolution
  Float_t  fResErr[AliFlowConstants::kSels][AliFlowConstants::kHars];			//! event plane resolution error

 // for Histograms
  TString           fLabel ;             			//! label axis : rapidity or pseudorapidity
  Float_t 	    fEtaMin ;     				//! histo range (eta)
  Float_t 	    fEtaMax ;     				//! histo range (eta) 
  Float_t 	    fPtMin ; 					//! histo range (pt)	   
  Float_t 	    fPtMax ; 					//! histo range (pt) 	   
  Float_t 	    fPtMaxPart ;   				//! max pt for _part histo
  Int_t   	    fEtaBins ;     				//! n. of eta bins
  Int_t   	    fPtBins ;      				//! n. of pt bins     
  Int_t 	    fPtBinsPart ;   				//! n. of pt bins for _part histo
  Float_t 	    fPtRangevEta[2] ;				//! pt range for the v(eta) histograms.
  Float_t 	    fEtaRangevPt[2] ;				//! |eta| range for the v(pt) histograms.
  Int_t 	    fMaxLabel ;             			//! for the MC labels histogram (max bin)

  TOrdCollection*   fPhiWgtHistList ;     			//! Weights:  histogram list
  TOrdCollection*   fVnResHistList ;     			//! Resolution and Vn:  histogram list
 
// for Single histograms

 // *****************
 // EVENTs HISTOGRAMS
 // *****************
  TH1F*     fHistTrigger;                 		   //! histogram ...
  TH1F*     fHistMult;                    		   //! histogram ...
  TH1F*     fHistV0Mult; 				   //! histogram ...
  TH1F*     fHistOrigMult;                		   //! histogram ...
  TH1F*     fHistMultOverOrig;            		   //! histogram ...
  TH1F*     fHistMultEta;                 		   //! histogram ...
  TH1F*     fHistCent;                    		   //! histogram ...
  TH1F*     fHistVertexZ;                 		   //! histogram ...
  TH2F*     fHistVertexXY2D;              		   //! histogram ...
  TH2F*     fHistEnergyZDC;              		   //! histogram ...
  TH1F*     fHistPartZDC;              		      	   //! histogram ...
  TProfile* fHistPidMult;                 		   //! histogram ...
  TH1F*     fHistBayPidMult;     		      	   //! histogram ...
  TH1F*     fHistEtaSym;                  		   //! histogram ...
  TH1F*     fHistEtaSymPart;                  		   //! histogram ...
  TH2F*     fHistEtaSymVerZ2D;            		   //! histogram ...
  TH2F*     fHistEtaSymVerZ2DPart;            		   //! histogram ...
 // selected (TR & V0)
  TH1F*     fHistMultPart;                		   //! histogram ...
  TH1F*     fHistV0MultPart;         			   //! histogram ...
  TH1F*     fHistBayPidMultPart;     		      	   //! histogram ...
  TH1F*     fHistMultPartUnit; 		  	    	   //! histogram ...
  
 // *****************
 // TRACKs HISTOGRAMS (all tracks)
 // *****************
  TH1F*     fHistPtot ;                 		   //! histogram ...
  TH1F*     fHistPt ;                 		   	   //! histogram ...
  TH1F*     fHistCharge;                  		   //! histogram ...
  TH1F*     fHistDcaGlobal;               		   //! histogram ...
  TH1F*     fHistDca;                     		   //! histogram ...
  TH1F*     fHistTransDca;                     		   //! histogram ...
  TH1F*     fHistChi2;                  		   //! histogram ...
  TH1F*     fHistLenght;           		           //! histogram ...
  TH1F*     fHistInvMass ;				   //! histogram ...
  TH1F*     fHistFitOverMax;           		           //! histogram ...
  TH2D*     fHistPhiPtCon ;				   //! histogram ...
  TH2D*     fHistPhiPtUnc ;				   //! histogram ...
  TH2D*     fHistPtPhiPos ;                 		   //! histogram ...
  TH2D*     fHistPtPhiNeg ;                 		   //! histogram ...
  TH3F*     fHistAllEtaPtPhi3D;              		   //! histogram ...
  TProfile* fHistCosPhi;                  		   //! histogram ...
  TH2F*     fHistPidPt;              		   	   //! histogram ...
  TH1F*     fHistPhi ;                 		   	   //! histogram ...
  TH1F*     fHistPhiCons ;                 		   //! histogram ...
  TH2D*     fHistYieldAll2D;              		   //! histogram ...
  TH2D*     fHistYieldCon2D;              		   //! histogram ...
  TH2D*     fHistYieldUnc2D;              		   //! histogram ...
  TH3F*     fHistConsEtaPtPhi3D;              		   //! histogram ...
  TH3F*     fHistGlobEtaPtPhi3D;             		   //! histogram ...
  TH3F*     fHistUncEtaPtPhi3D ;             		   //! histogram ...
  // fit & dE/dX for each detector (all tracks)
  TH1F*     fHistChi2ITS;                 		   //! histogram ...
  TH1F*     fHistChi2normITS;                 		   //! histogram ...
  TH1F*     fHistFitPtsITS;               		   //! histogram ...
  TH1F*     fHistMaxPtsITS;               		   //! histogram ...
  TH2F*     fHistMeanDedxPos2DITS;           		   //! histogram ...
  TH2F*     fHistMeanDedxNeg2DITS;           		   //! histogram ...
  // -
  TH1F*     fHistChi2TPC;                 		   //! histogram ...
  TH1F*     fHistChi2normTPC;                 		   //! histogram ...
  TH1F*     fHistFitPtsTPC;               		   //! histogram ...
  TH1F*     fHistMaxPtsTPC;               		   //! histogram ...
  TH1F*     fHistFitOverMaxTPC;           		   //! histogram ...
  TH2F*     fHistMeanDedxPos2D;           		   //! histogram ...
  TH2F*     fHistMeanDedxNeg2D;           		   //! histogram ...
  // -
  TH1F*     fHistChi2TRD;                 		   //! histogram ...
  TH1F*     fHistChi2normTRD;                 		   //! histogram ...
  TH1F*     fHistFitPtsTRD;               		   //! histogram ...
  TH1F*     fHistMaxPtsTRD;               		   //! histogram ...
  TH2F*     fHistMeanDedxPos2DTRD;           		   //! histogram ...
  TH2F*     fHistMeanDedxNeg2DTRD;           		   //! histogram ...
  // -
  TH1F*     fHistChi2TOF;                 		   //! histogram ...
  TH1F*     fHistChi2normTOF;                 		   //! histogram ...
  TH1F*     fHistFitPtsTOF;               		   //! histogram ...
  TH1F*     fHistMaxPtsTOF;               		   //! histogram ...
  TH2F*     fHistMeanDedxPos2DTOF;           		   //! histogram ...
  TH2F*     fHistMeanDedxNeg2DTOF;           		   //! histogram ...
  // detector response for particle type (all tracks, based on Pid)
  TH2F*     fHistMeanTPCPiPlus ;        		   //! histogram ...
  TH2F*     fHistMeanTPCPiMinus ;       		   //! histogram ...
  TH2F*     fHistMeanTPCProton ;        		   //! histogram ...
  TH2F*     fHistMeanTPCPbar ;          		   //! histogram ...
  TH2F*     fHistMeanTPCKplus ;         		   //! histogram ...
  TH2F*     fHistMeanTPCKminus ;        		   //! histogram ...
  TH2F*     fHistMeanTPCDeuteron ;      		   //! histogram ...
  TH2F*     fHistMeanTPCAntiDeuteron ;  		   //! histogram ...
  TH2F*     fHistMeanTPCPositron ;      		   //! histogram ...
  TH2F*     fHistMeanTPCElectron ;      		   //! histogram ...
  TH2F*     fHistMeanTPCMuonPlus ;      		   //! histogram ...
  TH2F*     fHistMeanTPCMuonMinus ;     		   //! histogram ...
  // -
  TH2F*     fHistMeanITSPiPlus ;			   //! histogram ...
  TH2F*     fHistMeanITSPiMinus ;			   //! histogram ...
  TH2F*     fHistMeanITSProton ;			   //! histogram ...
  TH2F*     fHistMeanITSPbar ;  			   //! histogram ...
  TH2F*     fHistMeanITSKplus ; 			   //! histogram ...
  TH2F*     fHistMeanITSKminus ;			   //! histogram ...
  TH2F*     fHistMeanITSDeuteron ;			   //! histogram ...
  TH2F*     fHistMeanITSAntiDeuteron ;  		   //! histogram ...
  TH2F*     fHistMeanITSPositron ;			   //! histogram ...
  TH2F*     fHistMeanITSElectron ;			   //! histogram ...
  TH2F*     fHistMeanITSMuonPlus ;			   //! histogram ...
  TH2F*     fHistMeanITSMuonMinus ;			   //! histogram ...
  // -
  TH2F*     fHistMeanTOFPiPlus ;			   //! histogram ...
  TH2F*     fHistMeanTOFPiMinus ;			   //! histogram ...
  TH2F*     fHistMeanTOFProton ;			   //! histogram ...
  TH2F*     fHistMeanTOFPbar ;  			   //! histogram ...
  TH2F*     fHistMeanTOFKplus ; 			   //! histogram ...
  TH2F*     fHistMeanTOFKminus ;			   //! histogram ...
  TH2F*     fHistMeanTOFDeuteron ;			   //! histogram ...
  TH2F*     fHistMeanTOFAntiDeuteron ;  		   //! histogram ...
  TH2F*     fHistMeanTOFPositron ;			   //! histogram ...
  TH2F*     fHistMeanTOFElectron ;			   //! histogram ...
  TH2F*     fHistMeanTOFMuonPlus ;			   //! histogram ...
  TH2F*     fHistMeanTOFMuonMinus ;			   //! histogram ...
  // -
  TH2F*     fHistMeanTRDPiPlus ;			   //! histogram ...
  TH2F*     fHistMeanTRDPiMinus ;			   //! histogram ...
  TH2F*     fHistMeanTRDProton ;			   //! histogram ...
  TH2F*     fHistMeanTRDPbar ;  			   //! histogram ...
  TH2F*     fHistMeanTRDKplus ; 			   //! histogram ...
  TH2F*     fHistMeanTRDKminus ;			   //! histogram ...
  TH2F*     fHistMeanTRDDeuteron ;			   //! histogram ...
  TH2F*     fHistMeanTRDAntiDeuteron ;  		   //! histogram ...
  TH2F*     fHistMeanTRDPositron ;			   //! histogram ...
  TH2F*     fHistMeanTRDElectron ;			   //! histogram ...
  TH2F*     fHistMeanTRDMuonPlus ;			   //! histogram ...
  TH2F*     fHistMeanTRDMuonMinus ;			   //! histogram ...
  // pid probability for all particle (all tracks)
  TH1F*     fHistPidPiPlus;               		   //! histogram ...
  TH1F*     fHistPidPiMinus;              		   //! histogram ...
  TH1F*     fHistPidProton;               		   //! histogram ...
  TH1F*     fHistPidAntiProton;           		   //! histogram ...
  TH1F*     fHistPidKplus;                		   //! histogram ...
  TH1F*     fHistPidKminus;               		   //! histogram ...
  TH1F*     fHistPidDeuteron;             		   //! histogram ...
  TH1F*     fHistPidAntiDeuteron;         		   //! histogram ...
  TH1F*     fHistPidElectron;             		   //! histogram ...
  TH1F*     fHistPidPositron;             		   //! histogram ...
  TH1F*     fHistPidMuonMinus;            		   //! histogram ...
  TH1F*     fHistPidMuonPlus;             		   //! histogram ...
  // pid probability for particle type (all tracks, based on Pid)
  TH1F*     fHistPidPiPlusPart;           		   //! histogram ...
  TH1F*     fHistPidPiMinusPart;          		   //! histogram ...
  TH1F*     fHistPidProtonPart;           		   //! histogram ...
  TH1F*     fHistPidAntiProtonPart;       		   //! histogram ...
  TH1F*     fHistPidKplusPart;            		   //! histogram ...
  TH1F*     fHistPidKminusPart;           		   //! histogram ...
  TH1F*     fHistPidDeuteronPart;         		   //! histogram ...
  TH1F*     fHistPidAntiDeuteronPart;     		   //! histogram ...
  TH1F*     fHistPidElectronPart;         		   //! histogram ...
  TH1F*     fHistPidPositronPart;         		   //! histogram ...
  TH1F*     fHistPidMuonMinusPart;        		   //! histogram ...
  TH1F*     fHistPidMuonPlusPart;         		   //! histogram ...
  // MC labels from the simulation (all tracks)
  //TH2F*     fLabHist;        	          		   //! histogram ... 
 // *****************
 // selected TRACKS
 // *****************
  TProfile* fHistBinEta;                  		   //! histogram ...
  TProfile* fHistBinPt;                   		   //! histogram ...
  //
  TH3F*     fHistEtaPtPhi3DPart ;			   //! histogram ...
  TH2D*     fHistYieldPart2D;             		   //! histogram ...
  TH1F*     fHistDcaGlobalPart ;			   //! histogram ...
  TH1F*     fHistInvMassPart ;			 	   //! histogram ...
  TH3F*     fHistEtaPtPhi3DOut ;			   //! histogram ...
  TH2D*     fHistYieldOut2D;             		   //! histogram ...
  TH1F*     fHistDcaGlobalOut ;				   //! histogram ...
  TH1F*     fHistInvMassOut ;				   //! histogram ...
  TH3F*     fHistMeanDedxPos3DPart ;			   //! histogram ...
  TH3F*     fHistMeanDedxNeg3DPart ;			   //! histogram ...
  TH3F*     fHistMeanDedxPos3DPartITS ;			   //! histogram ...
  TH3F*     fHistMeanDedxNeg3DPartITS ;			   //! histogram ...
//

 // *****************
 // V0s HISTOGRAMS (all v0s)
 // *****************
  TH1F*     fHistV0Mass; 	  			   //! histogram ...
  TH3F*     fHistV0EtaPtPhi3D;	  			   //! histogram ...
  TH2D*     fHistV0YieldAll2D;    			   //! histogram ...
  TH2D*     fHistV0PYall2D;    			   	   //! histogram ...
  TH1F*     fHistV0Dca;	  	  			   //! histogram ...
  TH1F*     fHistV0Chi2;	  			   //! histogram ...
  TH1F*     fHistV0Lenght;	  			   //! histogram ...
  TH1F*     fHistV0Sigma;	  			   //! histogram ...
  TProfile* fHistV0CosPhi;	           		   //! histogram ... 
  TH2D*     fHistV0MassPtSlices;    			   //! histogram ...
 // *****************
 // selected V0s
 // *****************
  TProfile* fHistV0BinEta;	           		   //! histogram ... 
  TProfile* fHistV0BinPt;	           		   //! histogram ... 
  TProfile* fHistV0sbBinEta;	           		   //! histogram ... 
  TProfile* fHistV0sbBinPt;	           		   //! histogram ...
  //
  TH1F*     fHistV0MassWin ;     			   //! histogram ...
  TH3F*     fHistV0EtaPtPhi3DPart ; 			   //! histogram ...
  TH2D*     fHistV0YieldPart2D;    			   //! histogram ...
  TH1F*     fHistV0DcaPart ;          			   //! histogram ...
  TH1F*     fHistV0LenghtPart ;	  			   //! histogram ...
  TH1F*     fHistV0sbMassSide ;    			   //! histogram ...
  TH3F*     fHistV0sbEtaPtPhi3DPart ; 			   //! histogram ...
  TH2D*     fHistV0sbYieldPart2D;    			   //! histogram ...
  TH1F*     fHistV0sbDcaPart ;          		   //! histogram ...
  TH1F*     fHistV0sbLenghtPart ;	  		   //! histogram ...

// for each harmonic, each selection, and each sub-event

 // *****************
 // SUB-EVENTs HISTOGRAMS
 // *****************
  struct AliHistSubHars {
   TH1F*     fHistPsiSubs;	  		   	   //! histogram ...
  };

  struct AliHistSubs {
   struct AliHistSubHars fHistSubHar[AliFlowConstants::kHars];         //! structure array ... 
  };
  struct AliHistSubs fHistSub[AliFlowConstants::kSels*AliFlowConstants::kSubs];    //! structure ...

// for each harmonic and each selection

  struct AliHistFullHars 
  {
   // weights
    TH1D*       fHistPhiPlus;				   //! histogram ...
    TH1D*       fHistPhiMinus;				   //! histogram ...
    TH1D*       fHistPhiAll;				   //! histogram ...
    TH1D*       fHistPhiWgtPlus;			   //! histogram ...
    TH1D*       fHistPhiWgtMinus;			   //! histogram ...
    TH1D*       fHistPhiWgtAll; 			   //! histogram ...
    TH1D*       fHistPhiFlatPlus;			   //! histogram ...
    TH1D*       fHistPhiFlatMinus;			   //! histogram ...
    TH1D*       fHistPhiFlatAll;			   //! histogram ...
    TH1D*       fHistPhi;				   //! histogram ...
    TH1D*       fHistPhiWgt;				   //! histogram ...
    TH1D*       fHistPhiFlat;				   //! histogram ...
   // flow (events)
    TH1F*       fHistPsi;				   //! histogram ...
    TH1F*       fHistPsiSubCorr;			   //! histogram ...
    TH1F*       fHistPsiSubCorrDiff;			   //! histogram ...
    TH1F*       fHistPsiDiff;				   //! histogram ...
    TH1F*       fHistMult;				   //! histogram ...
    TH1F*       fHistQnorm;				   //! histogram ...
   // flow (tracks)
    TH1F*       fHistPhiCorr;				   //! histogram ...
    TProfile2D* fHistvObs2D;				   //! histogram ...
    TProfile*   fHistvObsEta;				   //! histogram ...
    TProfile*   fHistvObsPt;				   //! histogram ...
    TH2D*       fHistv2D;				   //! histogram ...
    TH1D*       fHistvEta;				   //! histogram ...
    TH1D*       fHistvPt;				   //! histogram ...
   // flow (v0s)					   
    TH1F*       fHistV0PhiCorr;   			   //! histogram ...
    TProfile2D* fHistV0vObs2D;   			   //! histogram ...
    TProfile*   fHistV0vObsEta;  			   //! histogram ...
    TProfile*   fHistV0vObsPt;   			   //! histogram ...
    TH2D*	fHistV0v2D;	 			   //! histogram ...
    TH1D*	fHistV0vEta;	 			   //! histogram ...	 
    TH1D*	fHistV0vPt;	 			   //! histogram ...	 
   // flow (v0s sidebands)
    TProfile*   fHistV0sbvObsEtaSx ; 			   //! histogram ...  
    TProfile*   fHistV0sbvObsPtSx ;   			   //! histogram ...
    TProfile*   fHistV0sbvObsEtaDx ;  			   //! histogram ...
    TProfile*   fHistV0sbvObsPtDx ;   			   //! histogram ...
    TH1F*       fHistV0sbPhiCorr ;   			   //! histogram ...
    TProfile2D* fHistV0sbvObs2D ;   			   //! histogram ...
    TProfile*   fHistV0sbvObsEta ;  			   //! histogram ...
    TProfile*   fHistV0sbvObsPt ;   			   //! histogram ...
    TH2D*	fHistV0sbv2D ;      			   //! histogram ...
    TH1D*	fHistV0sbvEta ;     			   //! histogram ...
    TH1D*	fHistV0sbvPt ;      			   //! histogram ...
   // check (tracks used for R.P.)
    TH1F*       fHistYieldPt ;				   //! histogram ...
    TH3F*       fHistEtaPtPhi3D ;			   //! histogram ...
    TH2D*       fHistYield2D ;				   //! histogram ...
    TH1F*       fHistDcaGlob ;				   //! histogram ...
   // check (tracks excluded)
    TH1F*	fHistYieldPtout;			   //! histogram ...
    TH3F*	fHistEtaPtPhi3Dout ;			   //! histogram ...
    TH2D*	fHistYield2Dout ;			   //! histogram ...
    TH1F*	fHistDcaGlobout ;			   //! histogram ...
  };

// for each selection

  struct AliHistFulls 
  {
   TH1F*     fHistBayPidMult;				  //! histogram ...
  // flow (events)
   TProfile* fHistCos;					  //! histogram ...
   TH1F*     fHistRes;					  //! histogram ...
   TProfile* fHistvObs; 				  //! histogram ...
   TH1D*     fHistv;					  //! histogram ...
   TProfile* fHistV0vObs;				  //! histogram ...
   TProfile* fHistV0sbvObsSx;				  //! histogram ...
   TProfile* fHistV0sbvObsDx;				  //! histogram ...
   TH1D*     fHistV0v;				  	  //! histogram ...
  // wgt, evts, trks, v0s (as defined above)
   struct AliHistFullHars  fHistFullHar[AliFlowConstants::kHars];	  //! structure array ...
  };
  struct AliHistFulls fHistFull[AliFlowConstants::kSels];             //! structure array ...

  ClassDef(AliFlowAnalyser,0)              // macro for rootcint
};

#endif



// lame = not productive; poorly designed; uncool ...
