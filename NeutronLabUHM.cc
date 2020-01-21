#include <string>
#include <sstream>
#include <unistd.h>

#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4UIterminal.hh>
#include <G4UItcsh.hh>
#include <G4VisExecutive.hh>

#include "NeutronLabConstruction.hh"
#include "PhysicsList.hh"
#include "NeutronLabPrimaryGeneratorAction.hh"
#include "NeutronLabAnalysisManager.hh"
#include "NeutronLabStackingAction.hh"
#include "NeutronLabRunAction.hh"
#include "NeutronLabEventAction.hh"

void usage();

int
main(int argc, char **argv)
{
	// switches
	int c = 0;

	std::stringstream hStream;
	
	bool bInteractive = false;
	bool bVisualize = false;
	bool bVrmlVisualize = false;
	bool bOpenGlVisualize = false;

	bool bMacroFile = false;
	std::string hMacroFilename, hDataFilename;
	int iNbEventsToSimulate = 0;

	// parse switches
	while((c = getopt(argc,argv,"v:f:o:n:i")) != -1)
	{
		switch(c)
		{
			case 'v':
				bVisualize = true;
				hStream.str(optarg);
				if(hStream.str() == "vrml")
					bVrmlVisualize = true;
				else if(hStream.str() == "opengl")
					bOpenGlVisualize = true;
				//else if(hStream,str() == "heprep")
				//	bHepRepVisualize = true;
				hStream.clear();
				break;

			case 'f':
				bMacroFile = true;
				hMacroFilename = optarg;
				break;

			case 'o':
				hDataFilename = optarg;
				break;

			case 'n':
				hStream.str(optarg);
				hStream.clear();
				hStream >> iNbEventsToSimulate;
				break;

			case 'i':
				bInteractive = true;
				break;

			default:
				usage();
		}
	}

	// create the run manager
	G4RunManager *pRunManager = new G4RunManager;

	// set user-defined initialization classes
	pRunManager->SetUserInitialization(new NeutronLabConstruction);
	pRunManager->SetUserInitialization(new PhysicsList);

	G4VisManager* pVisManager = new G4VisExecutive;
	pVisManager->Initialize();

	// create the primary generator action
	NeutronLabPrimaryGeneratorAction *pPrimaryGeneratorAction = new NeutronLabPrimaryGeneratorAction();

	// create an analysis manager object
	NeutronLabAnalysisManager *pAnalysisManager = new NeutronLabAnalysisManager(pPrimaryGeneratorAction);
	if(!hDataFilename.empty())
		pAnalysisManager->SetDataFilename(hDataFilename);
	if(iNbEventsToSimulate)
		pAnalysisManager->SetNbEventsToSimulate(iNbEventsToSimulate);

	// set user-defined action classes
	pRunManager->SetUserAction(pPrimaryGeneratorAction);
	pRunManager->SetUserAction(new NeutronLabStackingAction(pAnalysisManager));
	pRunManager->SetUserAction(new NeutronLabRunAction(pAnalysisManager));
	pRunManager->SetUserAction(new NeutronLabEventAction(pAnalysisManager));

	pRunManager->Initialize();

	G4UImanager* pUImanager = G4UImanager::GetUIpointer();

	G4UIsession * pUIsession = 0;
	if(bInteractive)
	{
		pUIsession = new G4UIterminal(new G4UItcsh);
	}

	std::string hCommand;

	if(bVisualize)
	{
		pUImanager->ApplyCommand("/vis/scene/create");
		if(bVrmlVisualize)
			pUImanager->ApplyCommand("/vis/open VRML2FILE");
		if(bOpenGlVisualize)
			pUImanager->ApplyCommand("/vis/open OGLIX");
		pUImanager->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 0 0 deg");
		pUImanager->ApplyCommand("/vis/viewer/zoom 1.0");
		pUImanager->ApplyCommand("/tracking/storeTrajectory 1");
		pUImanager->ApplyCommand("/vis/scene/add/trajectories");
	}

	if(bMacroFile)
	{
		hCommand = "/control/execute " + hMacroFilename;
		pUImanager->ApplyCommand(hCommand);
	}

	if(iNbEventsToSimulate)
	{
		hStream.str("");
		hStream.clear();
		hStream << "/run/beamOn " << iNbEventsToSimulate;
		pUImanager->ApplyCommand(hStream.str());
	}

	if(bInteractive)
	{
		pUIsession->SessionStart();

		delete pUIsession;
	}


	delete pAnalysisManager;

	if(bVisualize)
		delete pVisManager;
	delete pRunManager;
	return 0;
}

void
usage()
{
	exit(0);
}

