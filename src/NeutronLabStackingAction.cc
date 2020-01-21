#include <G4ios.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTypes.hh>
#include <G4Track.hh>
#include <G4Event.hh>
#include <G4VProcess.hh>
#include <G4StackManager.hh>
#include <G4TransportationManager.hh>


#include "NeutronLabAnalysisManager.hh"

#include "NeutronLabStackingAction.hh"

NeutronLabStackingAction::NeutronLabStackingAction(NeutronLabAnalysisManager *pAnalysisManager)
{
	m_pAnalysisManager = pAnalysisManager;
}

NeutronLabStackingAction::~NeutronLabStackingAction()
{
}

G4ClassificationOfNewTrack
NeutronLabStackingAction::ClassifyNewTrack(const G4Track *pTrack)
{

	G4ClassificationOfNewTrack hTrackClassification = fUrgent;
	
	// write decays of long-lived isotopes into the new events
	if(pTrack->GetDefinition()->GetParticleType() == "nucleus" && !pTrack->GetDefinition()->GetPDGStable())
	{
		if(pTrack->GetParentID() > 0 && pTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay")
			hTrackClassification = fPostpone;
	}

/*	// write another file with activated isotopes
	G4String particleType = pTrack->GetDefinition()->GetParticleType();
	G4String volumeName = " ";
 
	if (particleType == "nucleus" && pTrack->GetParentID()>0)
	{
		G4String processname = pTrack->GetCreatorProcess()->GetProcessName();
		G4Ions* ion = (G4Ions*) pTrack->GetDefinition();
		G4double lifetime = ion->GetPDGLifeTime();
		G4String particleName = pTrack->GetDefinition()->GetParticleName();
		volumeName = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->
		LocateGlobalPointAndSetup(pTrack->GetPosition())->GetName();
		G4int Z = ion->GetAtomicNumber();
		G4double excitationEnergy = ion->GetExcitationEnergy();
		//if (lifetime > 1e18*second) lifetime = -1; //too long-lived
		//if (lifetime > 0 || excitationEnergy > 0 || (Z > 30 && Z < 40))
		if (lifetime > 0)
		{
			G4int A = ion->GetAtomicMass();
			G4ThreeVector position = pTrack->GetPosition(); 
			G4double kinE = pTrack->GetKineticEnergy();
			// out into the file
			G4String fileName = "DD_activation_1e6.iso";
			std::ofstream foutIso(fileName,std::ios::app);
			foutIso << A << " " << Z << " " << particleName << " " << kinE/CLHEP::keV << " ";
			foutIso << position.x()/CLHEP::m << " " << position.y()/CLHEP::m << " " << position.z()/CLHEP::m << " ";
			foutIso << volumeName << " " << processname << G4endl;
			foutIso.close();
			// Cout
			//G4cout << A << " " << Z << " " << particleName << " " << kinE/keV << " ";
			//G4cout << position.x()/m << " " << position.y()/m << " " << position.z()/m << " ";
			//G4cout << volumeName << " " << processname << G4endl;
    	}
	}
*/
	return hTrackClassification;
}

void
NeutronLabStackingAction::NewStage()
{
}

void
NeutronLabStackingAction::PrepareNewEvent()
{ 
}








