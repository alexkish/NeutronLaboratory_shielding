#ifndef __AnalysisManager_H__
#define __AnalysisManager_H__

#include <globals.hh>

#include <TParameter.h>

class G4Run;
class G4Event;
class G4Step;

class TFile;
class TTree;

class NeutronLabEventData;
class NeutronLabPrimaryGeneratorAction;

class NeutronLabAnalysisManager
{
public:
	NeutronLabAnalysisManager(NeutronLabPrimaryGeneratorAction *pPrimaryGeneratorAction);
	virtual ~NeutronLabAnalysisManager();

public:
	virtual void BeginOfRun(const G4Run *pRun); 
	virtual void EndOfRun(const G4Run *pRun); 
	virtual void BeginOfEvent(const G4Event *pEvent); 
	virtual void EndOfEvent(const G4Event *pEvent); 
	virtual void Step(const G4Step *pStep);	

	void SetDataFilename(const G4String &hFilename) { m_hDataFilename = hFilename; }
	void SetNbEventsToSimulate(G4int iNbEventsToSimulate) { m_iNbEventsToSimulate = iNbEventsToSimulate; }

private:
	G4bool FilterEvent(NeutronLabEventData *pEventData);

private:
	G4int m_iScreenHitsCollectionID;

	G4String m_hDataFilename;
	G4int m_iNbEventsToSimulate;

	TFile *m_pTreeFile;
	TTree *m_pTree;
	TParameter<int> *m_pNbEventsToSimulateParameter;

	NeutronLabPrimaryGeneratorAction *m_pPrimaryGeneratorAction;

	NeutronLabEventData *m_pEventData;
};

#endif
