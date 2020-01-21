#include <G4Geantino.hh>
#include <G4IonTable.hh>
#include <G4ThreeVector.hh>
#include <G4ParticleTable.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWith3Vector.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithABool.hh>
#include <G4Tokenizer.hh>
#include <G4ios.hh>
#include <fstream>
#include <iomanip>

#include "NeutronLabParticleSource.hh"

#include "NeutronLabParticleSourceMessenger.hh"

NeutronLabParticleSourceMessenger::NeutronLabParticleSourceMessenger(NeutronLabParticleSource *pParticleSource):
	m_pParticleSource(pParticleSource), m_bShootIon(false)
{
	m_pParticleTable = G4ParticleTable::GetParticleTable();

	// create directory
	m_pDirectory = new G4UIdirectory("/neutronlab/gun/");
	m_pDirectory->SetGuidance("Particle Source control commands.");

	// list available particles
	m_pListCmd = new G4UIcmdWithoutParameter("/neutronlab/gun/List", this);
	m_pListCmd->SetGuidance("List available particles.");
	m_pListCmd->SetGuidance(" Invoke G4ParticleTable.");

	// set particle  
	m_pParticleCmd = new G4UIcmdWithAString("/neutronlab/gun/particle", this);
	m_pParticleCmd->SetGuidance("Set particle to be generated.");
	m_pParticleCmd->SetGuidance(" (geantino is default)");
	m_pParticleCmd->SetGuidance(" (ion can be specified for shooting ions)");
	m_pParticleCmd->SetParameterName("particleName", true);
	m_pParticleCmd->SetDefaultValue("geantino");
	G4String candidateList;
	G4int nPtcl = m_pParticleTable->entries();

	for(G4int i = 0; i < nPtcl; i++)
	{
		candidateList += m_pParticleTable->GetParticleName(i);
		candidateList += " ";
	}
	candidateList += "ion ";
	m_pParticleCmd->SetCandidates(candidateList);

	// particle direction
	m_pDirectionCmd = new G4UIcmdWith3Vector("/neutronlab/gun/direction", this);
	m_pDirectionCmd->SetGuidance("Set momentum direction.");
	m_pDirectionCmd->SetGuidance("Direction needs not to be a unit vector.");
	m_pDirectionCmd->SetParameterName("Px", "Py", "Pz", true, true);
	m_pDirectionCmd->SetRange("Px != 0 || Py != 0 || Pz != 0");

	// particle energy
	m_pEnergyCmd = new G4UIcmdWithADoubleAndUnit("/neutronlab/gun/energy", this);
	m_pEnergyCmd->SetGuidance("Set kinetic energy.");
	m_pEnergyCmd->SetParameterName("Energy", true, true);
	m_pEnergyCmd->SetDefaultUnit("GeV");
	//m_pEnergyCmd->SetUnitCategory("Energy");
	//m_pEnergyCmd->SetUnitCandidates("eV keV MeV GeV TeV");

	m_pPositionCmd = new G4UIcmdWith3VectorAndUnit("/neutronlab/gun/position", this);
	m_pPositionCmd->SetGuidance("Set starting position of the particle.");
	m_pPositionCmd->SetParameterName("X", "Y", "Z", true, true);
	m_pPositionCmd->SetDefaultUnit("cm");
	//m_pPositionCmd->SetUnitCategory("Length");
	//m_pPositionCmd->SetUnitCandidates("microm mm cm m km");

	// ion 
	m_pIonCmd = new G4UIcommand("/neutronlab/gun/ion", this);
	m_pIonCmd->SetGuidance("Set properties of ion to be generated.");
	m_pIonCmd->SetGuidance("[usage] /gun/ion Z A Q E");
	m_pIonCmd->SetGuidance("        Z:(int) AtomicNumber");
	m_pIonCmd->SetGuidance("        A:(int) AtomicMass");
	m_pIonCmd->SetGuidance("        Q:(int) Charge of Ion (in unit of e)");
	m_pIonCmd->SetGuidance("        E:(double) Excitation energy (in keV)");

	G4UIparameter *param;

	param = new G4UIparameter("Z", 'i', false);
	param->SetDefaultValue("1");
	m_pIonCmd->SetParameter(param);
	param = new G4UIparameter("A", 'i', false);
	param->SetDefaultValue("1");
	m_pIonCmd->SetParameter(param);
	param = new G4UIparameter("Q", 'i', true);
	param->SetDefaultValue("0");
	m_pIonCmd->SetParameter(param);
	param = new G4UIparameter("E", 'd', true);
	param->SetDefaultValue("0.0");
	m_pIonCmd->SetParameter(param);

	// source distribution type
	m_pTypeCmd = new G4UIcmdWithAString("/neutronlab/gun/type", this);
	m_pTypeCmd->SetGuidance("Sets source distribution type.");
	m_pTypeCmd->SetGuidance("Either Point or Volume");
	m_pTypeCmd->SetParameterName("DisType", true, true);
	m_pTypeCmd->SetDefaultValue("Point");
	m_pTypeCmd->SetCandidates("Point Volume");

	// source shape
	m_pShapeCmd = new G4UIcmdWithAString("/neutronlab/gun/shape", this);
	m_pShapeCmd->SetGuidance("Sets source shape type.");
	m_pShapeCmd->SetParameterName("Shape", true, true);
	m_pShapeCmd->SetDefaultValue("NULL");
	m_pShapeCmd->SetCandidates("Sphere Cylinder");

	// center coordinates
	m_pCenterCmd = new G4UIcmdWith3VectorAndUnit("/neutronlab/gun/center", this);
	m_pCenterCmd->SetGuidance("Set center coordinates of source.");
	m_pCenterCmd->SetParameterName("X", "Y", "Z", true, true);
	m_pCenterCmd->SetDefaultUnit("cm");
	m_pCenterCmd->SetUnitCandidates("nm mum mm cm m km");

	// half height of source
	m_pHalfzCmd = new G4UIcmdWithADoubleAndUnit("/neutronlab/gun/halfz", this);
	m_pHalfzCmd->SetGuidance("Set z half length of source.");
	m_pHalfzCmd->SetParameterName("Halfz", true, true);
	m_pHalfzCmd->SetDefaultUnit("cm");
	m_pHalfzCmd->SetUnitCandidates("nm mum mm cm m km");

	// radius of source  
	m_pRadiusCmd = new G4UIcmdWithADoubleAndUnit("/neutronlab/gun/radius", this);
	m_pRadiusCmd->SetGuidance("Set radius of source.");
	m_pRadiusCmd->SetParameterName("Radius", true, true);
	m_pRadiusCmd->SetDefaultUnit("cm");
	m_pRadiusCmd->SetUnitCandidates("nm mum mm cm m km");

	// confine to volume(s)
	m_pConfineCmd = new G4UIcmdWithAString("/neutronlab/gun/confine", this);
	m_pConfineCmd->SetGuidance("Confine source to volume(s) (NULL to unset).");
	m_pConfineCmd->SetGuidance("usage: confine VolName1 VolName2 ...");
	m_pConfineCmd->SetParameterName("VolName", true, true);
	m_pConfineCmd->SetDefaultValue("NULL");

	// angular distribution
	m_pAngTypeCmd = new G4UIcmdWithAString("/neutronlab/gun/angtype", this);
	m_pAngTypeCmd->SetGuidance("Sets angular source distribution type");
	m_pAngTypeCmd->SetGuidance("Possible variables are: iso direction");
	m_pAngTypeCmd->SetParameterName("AngDis", true, true);
	m_pAngTypeCmd->SetDefaultValue("iso");
	m_pAngTypeCmd->SetCandidates("iso direction");

	// energy distribution
	m_pEnergyTypeCmd = new G4UIcmdWithAString("/neutronlab/gun/energytype", this);
	m_pEnergyTypeCmd->SetGuidance("Sets energy distribution type");
	m_pEnergyTypeCmd->SetGuidance("Possible variables are: Mono, Kr83m");
	m_pEnergyTypeCmd->SetParameterName("EnergyDis", true, true);
	m_pEnergyTypeCmd->SetDefaultValue("Mono");
	m_pEnergyTypeCmd->SetCandidates("Mono Kr83m");

    // number of particles to be generated
    m_pNumberOfParticlesToBeGeneratedCmd
        = new G4UIcmdWithAnInteger("/neutronlab/gun/numberofparticles", this);
    m_pNumberOfParticlesToBeGeneratedCmd->SetGuidance("Number of particles generated in one event");
    m_pNumberOfParticlesToBeGeneratedCmd->SetParameterName("NumParticles", true, true);
    m_pNumberOfParticlesToBeGeneratedCmd->SetDefaultValue(1);

	// verbosity
	m_pVerbosityCmd = new G4UIcmdWithAnInteger("/neutronlab/gun/verbose", this);
	m_pVerbosityCmd->SetGuidance("Set Verbose level for gun");
	m_pVerbosityCmd->SetGuidance(" 0 : Silent");
	m_pVerbosityCmd->SetGuidance(" 1 : Limited information");
	m_pVerbosityCmd->SetGuidance(" 2 : Detailed information");
	m_pVerbosityCmd->SetParameterName("level", false);
	m_pVerbosityCmd->SetRange("level>=0 && level <=2");
}

NeutronLabParticleSourceMessenger::~NeutronLabParticleSourceMessenger()
{
	delete m_pTypeCmd;
	delete m_pShapeCmd;
	delete m_pCenterCmd;
	delete m_pHalfzCmd;
	delete m_pRadiusCmd;
	delete m_pConfineCmd;
	delete m_pAngTypeCmd;
	delete m_pEnergyTypeCmd;
	delete m_pVerbosityCmd;
	delete m_pIonCmd;
	delete m_pParticleCmd;
	delete m_pPositionCmd;
	delete m_pDirectionCmd;
	delete m_pEnergyCmd;
	delete m_pListCmd;

	delete m_pDirectory;
}

void
NeutronLabParticleSourceMessenger::SetNewValue(G4UIcommand * command, G4String newValues)
{
	if(command == m_pTypeCmd)
		m_pParticleSource->SetPosDisType(newValues);

	else if(command == m_pShapeCmd)
		m_pParticleSource->SetPosDisShape(newValues);

	else if(command == m_pCenterCmd)
		m_pParticleSource->SetCenterCoords(m_pCenterCmd->GetNew3VectorValue(newValues));

	else if(command == m_pHalfzCmd)
		m_pParticleSource->SetHalfZ(m_pHalfzCmd->GetNewDoubleValue(newValues));

	else if(command == m_pRadiusCmd)
		m_pParticleSource->SetRadius(m_pRadiusCmd->GetNewDoubleValue(newValues));

	else if(command == m_pAngTypeCmd)
		m_pParticleSource->SetAngDistType(newValues);

	else if(command == m_pConfineCmd)
		m_pParticleSource->ConfineSourceToVolume(newValues);

	else if(command == m_pEnergyTypeCmd)
		m_pParticleSource->SetEnergyDisType(newValues);

	else if(command == m_pVerbosityCmd)
		m_pParticleSource->SetVerbosity(m_pVerbosityCmd->GetNewIntValue(newValues));

	else if(command == m_pParticleCmd)
	{
		if(newValues == "ion")
		{
			m_bShootIon = true;
		}
		else
		{
			m_bShootIon = false;
			G4ParticleDefinition * pd = m_pParticleTable->FindParticle(newValues);

			if(pd != NULL)
			{
				m_pParticleSource->SetParticleDefinition(pd);
			}
		}
	}

	else if(command == m_pIonCmd)
	{
		if(m_bShootIon)
		{
			G4Tokenizer next(newValues);

			// check argument
			m_iAtomicNumber = StoI(next());
			m_iAtomicMass = StoI(next());
			G4String sQ = next();

			if(sQ.isNull())
			{
				m_iIonCharge = m_iAtomicNumber;
			}
			else
			{
				m_iIonCharge = StoI(sQ);
				sQ = next();
				if(sQ.isNull())
				{
					m_dIonExciteEnergy = 0.0;
				}
				else
				{
					m_dIonExciteEnergy = StoD(sQ) * CLHEP::keV;
				}
			}

			G4ParticleDefinition *ion;

			//ion = m_pParticleTable->G4IonTable::GetIon(m_iAtomicNumber, m_iAtomicMass, m_dIonExciteEnergy);
			ion = G4IonTable::GetIonTable()->GetIon(m_iAtomicNumber, m_iAtomicMass, m_dIonExciteEnergy);
			if(ion == 0)
			{
				G4cout << "Ion with Z=" << m_iAtomicNumber;
				G4cout << " A=" << m_iAtomicMass << "is not be defined" << G4endl;
			}
			else
			{
				m_pParticleSource->SetParticleDefinition(ion);
				m_pParticleSource->SetParticleCharge(m_iIonCharge *CLHEP::eplus);
			}
		}
		else
		{
			G4cout << "Set /neutronlab/gun/particle to ion before using /neutronlab/gun/ion command";
			G4cout << G4endl;
		}
	}

	else if(command == m_pListCmd)
		m_pParticleTable->DumpTable();

	else if(command == m_pDirectionCmd)
	{
		m_pParticleSource->SetAngDistType("direction");
		m_pParticleSource->SetParticleMomentumDirection(m_pDirectionCmd->GetNew3VectorValue(newValues));
	}

	else if(command == m_pEnergyCmd)
	{
		m_pParticleSource->SetEnergyDisType("Mono");
		m_pParticleSource->SetMonoEnergy(m_pEnergyCmd->GetNewDoubleValue(newValues));
	}

	else if(command == m_pPositionCmd)
	{
		m_pParticleSource->SetPosDisType("Point");
		m_pParticleSource->SetCenterCoords(m_pPositionCmd->GetNew3VectorValue(newValues));
	}
	else
		G4cout << "Error entering command" << G4endl;
}

