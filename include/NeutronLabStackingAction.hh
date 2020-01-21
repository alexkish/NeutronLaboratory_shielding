#ifndef __STACKINGACTION_H__
#define __STACKINGACTION_H__

#include <globals.hh>
#include <G4UserStackingAction.hh>

class NeutronLabAnalysisManager;

class NeutronLabStackingAction: public G4UserStackingAction
{
public:
	NeutronLabStackingAction(NeutronLabAnalysisManager *pAnalysisManager=0);
	~NeutronLabStackingAction();
  
	virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
	virtual void NewStage();
	virtual void PrepareNewEvent();

private:
	NeutronLabAnalysisManager *m_pAnalysisManager;
};

#endif
