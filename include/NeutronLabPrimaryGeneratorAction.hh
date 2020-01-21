#ifndef __PRIMARYGENERATORACTION_H__
#define __PRIMARYGENERATORACTION_H__

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ThreeVector.hh>
#include <globals.hh>

class NeutronLabParticleSource;

class G4Event;

class NeutronLabPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
public:
	NeutronLabPrimaryGeneratorAction();
	~NeutronLabPrimaryGeneratorAction();

public:
	const long *GetEventSeeds() { return m_lSeeds; }
	const G4String &GetParticleTypeOfPrimary() { return m_hParticleTypeOfPrimary; }
	G4double GetEnergyOfPrimary() { return m_dEnergyOfPrimary; }
	G4ThreeVector GetPositionOfPrimary() { return m_hPositionOfPrimary; }

	void GeneratePrimaries(G4Event *pEvent);

  private:
	long m_lSeeds[2];
	G4String m_hParticleTypeOfPrimary;
	G4double m_dEnergyOfPrimary;
	G4ThreeVector m_hPositionOfPrimary;

	NeutronLabParticleSource *m_pParticleSource;
};

#endif
