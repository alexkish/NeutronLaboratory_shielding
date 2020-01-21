#include <Randomize.hh>
#include <sys/time.h>

#include "NeutronLabAnalysisManager.hh"
#include "NeutronLabRunAction.hh"

NeutronLabRunAction::NeutronLabRunAction(NeutronLabAnalysisManager *pAnalysisManager)
{
	m_pAnalysisManager = pAnalysisManager;
}

NeutronLabRunAction::~NeutronLabRunAction()
{

}

void
NeutronLabRunAction::BeginOfRunAction(const G4Run *pRun)
{
	if(m_pAnalysisManager)
		m_pAnalysisManager->BeginOfRun(pRun);

	struct timeval hTimeValue;
	gettimeofday(&hTimeValue, NULL);

	//CLHEP::HepRandom::setTheEngine(new CLHEP::DRand48Engine);
	//CLHEP::HepRandom::setTheEngine(new CLHEP::RandFlat);
	//CLHEP::HepRandom::setTheSeed(hTimeValue.tv_usec);
}

void
NeutronLabRunAction::EndOfRunAction(const G4Run *pRun)
{
	if(m_pAnalysisManager)
		m_pAnalysisManager->EndOfRun(pRun);
}
