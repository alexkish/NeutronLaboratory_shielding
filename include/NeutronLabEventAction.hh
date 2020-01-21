#ifndef __EVENTACTION_H__
#define __EVENTACTION_H__

#include <G4UserEventAction.hh>

#include "NeutronLabAnalysisManager.hh"

class G4Event;

class NeutronLabEventAction : public G4UserEventAction
{
public:
	NeutronLabEventAction(NeutronLabAnalysisManager *pAnalysisManager = 0);
	~NeutronLabEventAction();

public:
	void BeginOfEventAction(const G4Event *pEvent);
	void EndOfEventAction(const G4Event *pEvent);

private:
	NeutronLabAnalysisManager *m_pAnalysisManager;
};

#endif
