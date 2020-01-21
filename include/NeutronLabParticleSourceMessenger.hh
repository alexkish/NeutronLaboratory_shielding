#ifndef __PARTICLESOURCEMESSENGER_H__
#define __PARTICLESOURCEMESSENGER_H__

#include <G4UImessenger.hh>
#include <globals.hh>

class NeutronLabParticleSource;

class G4ParticleTable;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

class NeutronLabParticleSourceMessenger: public G4UImessenger
{
public:
	NeutronLabParticleSourceMessenger(NeutronLabParticleSource *pParticleSource);
	~NeutronLabParticleSourceMessenger();
  
	void SetNewValue(G4UIcommand *pCommand, G4String hNewValues);
 
private:
	NeutronLabParticleSource *m_pParticleSource;
	G4ParticleTable *m_pParticleTable;
    
     G4UIdirectory              *m_pDirectory;

     G4UIcmdWithAString         *m_pTypeCmd;
     G4UIcmdWithAString         *m_pShapeCmd;
     G4UIcmdWith3VectorAndUnit  *m_pCenterCmd;
     G4UIcmdWithADoubleAndUnit  *m_pHalfzCmd;
     G4UIcmdWithADoubleAndUnit  *m_pRadiusCmd;
     G4UIcmdWithAString         *m_pConfineCmd;         
     G4UIcmdWithAString         *m_pAngTypeCmd;
     G4UIcmdWithAString         *m_pEnergyTypeCmd;
     G4UIcmdWithAnInteger       *m_pNumberOfParticlesToBeGeneratedCmd;
     G4UIcmdWithAnInteger       *m_pVerbosityCmd;
     G4UIcommand                *m_pIonCmd;
     G4UIcmdWithAString         *m_pParticleCmd;
     G4UIcmdWith3VectorAndUnit  *m_pPositionCmd;
     G4UIcmdWith3Vector         *m_pDirectionCmd;
     G4UIcmdWithADoubleAndUnit  *m_pEnergyCmd;
     G4UIcmdWithoutParameter    *m_pListCmd;

     G4bool   m_bShootIon; 
     G4int    m_iAtomicNumber;
     G4int    m_iAtomicMass;
     G4int    m_iIonCharge;
     G4double m_dIonExciteEnergy;
};

#endif
