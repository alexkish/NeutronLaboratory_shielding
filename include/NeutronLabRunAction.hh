#ifndef __RUNACTION_H__
#define __RUNACTION_H__

#include <G4UserRunAction.hh>

class G4Run;

class NeutronLabAnalysisManager;

class NeutronLabRunAction: public G4UserRunAction
{
public:
	NeutronLabRunAction(NeutronLabAnalysisManager *pAnalysisManager=0);
	~NeutronLabRunAction();

public:
	void BeginOfRunAction(const G4Run *pRun);
	void EndOfRunAction(const G4Run *pRun);

private:
	NeutronLabAnalysisManager *m_pAnalysisManager;
};

#endif
