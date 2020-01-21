#include <G4SDManager.hh>
#include <G4Run.hh>
#include <G4Event.hh>
#include <G4HCofThisEvent.hh>
#include <G4Step.hh>

#include <numeric>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TParameter.h>

#include "NeutronLabConstruction.hh"
#include "NeutronLabPrimaryGeneratorAction.hh"
#include "NeutronLabEventData.hh"

#include "ScreenHit.hh"


#include "NeutronLabAnalysisManager.hh"

NeutronLabAnalysisManager::NeutronLabAnalysisManager(NeutronLabPrimaryGeneratorAction *pPrimaryGeneratorAction)
{
	m_iScreenHitsCollectionID = -1;

	m_hDataFilename = "events.root";

	m_pPrimaryGeneratorAction = pPrimaryGeneratorAction;

	m_pEventData = new NeutronLabEventData();
}

NeutronLabAnalysisManager::~NeutronLabAnalysisManager()
{
}

void
NeutronLabAnalysisManager::BeginOfRun(const G4Run *pRun)
{
	m_pTreeFile = new TFile(m_hDataFilename.c_str(), "RECREATE", "File containing event data for NeutronLab");
	m_pTree = new TTree("t1", "Tree containing event data for NeutronLab");

	gROOT->ProcessLine("#include <vector>");

	m_pTree->Branch("eventid", &m_pEventData->m_iEventId, "eventid/I");
	m_pTree->Branch("nsteps", &m_pEventData->m_iNbSteps, "nsteps/I");
	m_pTree->Branch("etot", &m_pEventData->m_fTotalEnergyDeposited, "etot/F");
	m_pTree->Branch("trackid", "vector<int>", &m_pEventData->m_pTrackId);
	m_pTree->Branch("type", "vector<string>", &m_pEventData->m_pParticleType);
	m_pTree->Branch("parentid", "vector<int>", &m_pEventData->m_pParentId);
	m_pTree->Branch("parenttype", "vector<string>", &m_pEventData->m_pParentType);
	m_pTree->Branch("creaproc", "vector<string>", &m_pEventData->m_pCreatorProcess);
	m_pTree->Branch("edproc", "vector<string>", &m_pEventData->m_pDepositingProcess);
	m_pTree->Branch("xp", "vector<float>", &m_pEventData->m_pX);
	m_pTree->Branch("yp", "vector<float>", &m_pEventData->m_pY);
	m_pTree->Branch("zp", "vector<float>", &m_pEventData->m_pZ);
	m_pTree->Branch("ed", "vector<float>", &m_pEventData->m_pEnergyDeposited);
	m_pTree->Branch("time", "vector<float>", &m_pEventData->m_pTime);
	m_pTree->Branch("type_pri", "vector<string>", &m_pEventData->m_pPrimaryParticleType);
	m_pTree->Branch("xp_pri", &m_pEventData->m_fPrimaryX, "xp_pri/F");
	m_pTree->Branch("yp_pri", &m_pEventData->m_fPrimaryY, "yp_pri/F");
	m_pTree->Branch("zp_pri", &m_pEventData->m_fPrimaryZ, "zp_pri/F");

	m_pTree->Branch("mX_pre", "vector<float>", &m_pEventData->m_pPreMomDirX);
	m_pTree->Branch("mY_pre", "vector<float>", &m_pEventData->m_pPreMomDirY);
	m_pTree->Branch("mZ_pre", "vector<float>", &m_pEventData->m_pPreMomDirZ);
	m_pTree->Branch("ekin_pre", "vector<double>", &m_pEventData->m_pPreKinNrg);
	//m_pTree->Branch("mX_post", "vector<float>", &m_pEventData->m_pPostMomDirX);
	//m_pTree->Branch("mY_post", "vector<float>", &m_pEventData->m_pPostMomDirY);
	//m_pTree->Branch("mZ_post", "vector<float>", &m_pEventData->m_pPostMomDirZ);
	//m_pTree->Branch("ekin_post", "vector<double>", &m_pEventData->m_pPostKinNrg);

	m_pTree->SetMaxTreeSize(10e9);
	m_pTree->AutoSave();

	m_pNbEventsToSimulateParameter = new TParameter<int>("nbevents", m_iNbEventsToSimulate);
	m_pNbEventsToSimulateParameter->Write();
}

void
NeutronLabAnalysisManager::EndOfRun(const G4Run *pRun)
{
	m_pTreeFile->Write();
	m_pTreeFile->Close();
}

void
NeutronLabAnalysisManager::BeginOfEvent(const G4Event *pEvent)
{
	if(m_iScreenHitsCollectionID == -1)
	{
		G4SDManager *pSDManager = G4SDManager::GetSDMpointer();
		m_iScreenHitsCollectionID = pSDManager->GetCollectionID("ScreenHitsCollection");
	} 
}

void
NeutronLabAnalysisManager::EndOfEvent(const G4Event *pEvent)
{
	G4HCofThisEvent* pHCofThisEvent = pEvent->GetHCofThisEvent();
	ScreenHitsCollection	* pScreenHitsCollection 	= 0;

	G4int iNbScreenHits 	= 0.;
	
	if(pHCofThisEvent)
	{
		if(m_iScreenHitsCollectionID != -1)
		{
			pScreenHitsCollection = (ScreenHitsCollection *)(pHCofThisEvent->GetHC(m_iScreenHitsCollectionID));
			iNbScreenHits = (pScreenHitsCollection)?(pScreenHitsCollection->entries()):(0);
		}
	}

	if(iNbScreenHits)
	{
		m_pEventData->m_iEventId = pEvent->GetEventID();

		m_pEventData->m_pPrimaryParticleType->push_back(m_pPrimaryGeneratorAction->GetParticleTypeOfPrimary());

		m_pEventData->m_fPrimaryX = m_pPrimaryGeneratorAction->GetPositionOfPrimary().x();
		m_pEventData->m_fPrimaryY = m_pPrimaryGeneratorAction->GetPositionOfPrimary().y();
		m_pEventData->m_fPrimaryZ = m_pPrimaryGeneratorAction->GetPositionOfPrimary().z();

		G4int iNbSteps 					= 0;

		G4float fTotalEnergyDeposited 	= 0.;
		G4float fKineticEnergy 			= 0.;

		for(G4int i=0; i<iNbScreenHits; i++)
		{
			ScreenHit *pHit = (*pScreenHitsCollection)[i];

			//if(pHit->GetParticleType() != "opticalphoton") 	// include all particles
			if(pHit->GetParticleType() == "gamma" || pHit->GetParticleType() == "neutron")
			{
				m_pEventData->m_pTrackId->push_back(pHit->GetTrackId());
				m_pEventData->m_pParentId->push_back(pHit->GetParentId());

				m_pEventData->m_pParticleType->push_back(pHit->GetParticleType());
				m_pEventData->m_pParentType->push_back(pHit->GetParentType());
				m_pEventData->m_pCreatorProcess->push_back(pHit->GetCreatorProcess());
				m_pEventData->m_pDepositingProcess->push_back(pHit->GetDepositingProcess());

				m_pEventData->m_pX->push_back(pHit->GetPosition().x()/CLHEP::mm);
				m_pEventData->m_pY->push_back(pHit->GetPosition().y()/CLHEP::mm);
				m_pEventData->m_pZ->push_back(pHit->GetPosition().z()/CLHEP::mm);

				m_pEventData->m_pPreMomDirX->push_back(pHit->GetPreMomDir().x()/CLHEP::mm);
				m_pEventData->m_pPreMomDirY->push_back(pHit->GetPreMomDir().y()/CLHEP::mm);
				m_pEventData->m_pPreMomDirZ->push_back(pHit->GetPreMomDir().z()/CLHEP::mm);
				m_pEventData->m_pPreKinNrg->push_back(pHit->GetPreKinEnergy()/CLHEP::keV);
				//m_pEventData->m_pPostMomDirX->push_back(pHit->GetPostMomDir().x()/mm);
				//m_pEventData->m_pPostMomDirY->push_back(pHit->GetPostMomDir().y()/mm);
				//m_pEventData->m_pPostMomDirZ->push_back(pHit->GetPostMomDir().z()/mm);
				//m_pEventData->m_pPostKinNrg->push_back(pHit->GetPostKinEnergy()/keV);

				fTotalEnergyDeposited += pHit->GetEnergyDeposited()/CLHEP::keV;
				m_pEventData->m_pEnergyDeposited->push_back(pHit->GetEnergyDeposited()/CLHEP::keV);

				fKineticEnergy = pHit->GetKineticEnergy()/CLHEP::keV;
				m_pEventData->m_pKineticEnergy->push_back(pHit->GetKineticEnergy()/CLHEP::keV);
				m_pEventData->m_pTime->push_back(pHit->GetTime()/CLHEP::second);

				iNbSteps++;
			}
		};

		m_pEventData->m_iNbSteps 				= iNbSteps;
		m_pEventData->m_fTotalEnergyDeposited 	= fTotalEnergyDeposited;


		//m_pEventData->m_pPmtHits->resize(iNbTopPmts+iNbBottomPmts+iNbTopVetoPmts+iNbBottomVetoPmts, 0);

/*		// Pmt hits
		for(G4int i=0; i<iNbPmtHits; i++)
			(*(m_pEventData->m_pPmtHits))[(*pPmtHitsCollection)[i]->GetPmtNb()]++;

		m_pEventData->m_iNbTopPmtHits =
			accumulate(m_pEventData->m_pPmtHits->begin(), m_pEventData->m_pPmtHits->begin()+iNbTopPmts, 0);
		m_pEventData->m_iNbBottomPmtHits =
			accumulate(m_pEventData->m_pPmtHits->begin()+iNbTopPmts, m_pEventData->m_pPmtHits->end(), 0);
*/
		//if(fKineticEnergy>1.)
		m_pTree->Fill();


		m_pEventData->Clear();
	}
}

void
NeutronLabAnalysisManager::Step(const G4Step *pStep)
{
/*	G4StepPoint* point1 = pStep->GetPreStepPoint();
    G4StepPoint* point2 = pStep->GetPostStepPoint();

	G4TouchableHandle touch1 	= point1->GetTouchableHandle();
	G4VPhysicalVolume *volume1 	= touch1->GetVolume();
	G4String name1 = volume1->GetName();
	
	G4cout << name1 << G4endl; 
*/
}

/*
G4bool
CollimatorAnalysisManager::FilterEvent(CollimatorEventData *pEventData)
{
	G4double dEnergyDepositedSensitiveRegion = 0.;

	vector<float> *pX = pEventData->m_pX;
	vector<float> *pY = pEventData->m_pY;
	vector<float> *pZ = pEventData->m_pZ;
	vector<float> *pEnergyDeposited = pEventData->m_pEnergyDeposited;

	const G4double dDriftLength = CollimatorDetectorConstruction::GetGeometryParameter("DriftLength");
	const G4double dRadius = CollimatorDetectorConstruction::GetGeometryParameter("TeflonCylinderInnerRadius");

	for(G4int i=0; i<pEnergyDeposited->size(); i++)
	{
		if((*pZ)[i] < 0. && (*pZ)[i] > -dDriftLength && std::sqrt((*pX)[i]*(*pX)[i] + (*pY)[i]*(*pY)[i]) < dRadius)
			dEnergyDepositedSensitiveRegion += (*pEnergyDeposited)[i];
	}

	if(dEnergyDepositedSensitiveRegion > 0.)
		return false;
	else
		return true;
}
*/
	
