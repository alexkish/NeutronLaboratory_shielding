#include <G4UnitsTable.hh>
#include <G4VVisManager.hh>
#include <G4Circle.hh>
#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include "ScreenHit.hh"

G4Allocator<ScreenHit> ScreenHitAllocator;

ScreenHit::ScreenHit() {}

ScreenHit::~ScreenHit()
{
	if(m_pParticleType) delete m_pParticleType;
	if(m_pParentType) delete m_pParentType;
	if(m_pCreatorProcess) delete m_pCreatorProcess;
	if(m_pDepositingProcess) delete m_pDepositingProcess;
}

ScreenHit::ScreenHit(const ScreenHit &hScreenHit):G4VHit()
{
	m_pPreMomDir = hScreenHit.m_pPreMomDir;
	m_pPreKinNrg = hScreenHit.m_pPreKinNrg;
	//m_pPostMomDir = hScreenHit.m_pPostMomDir;
	m_iTrackId = hScreenHit.m_iTrackId;
	m_iParentId = hScreenHit.m_iParentId;
	m_pParticleType = hScreenHit.m_pParticleType;
	m_pParentType = hScreenHit.m_pParentType ;
	m_pCreatorProcess = hScreenHit.m_pCreatorProcess ;
	m_pDepositingProcess = hScreenHit.m_pDepositingProcess ;
	m_hPosition = hScreenHit.m_hPosition;
	m_dEnergyDeposited = hScreenHit.m_dEnergyDeposited;
	m_dKineticEnergy = hScreenHit.m_dKineticEnergy ;
	m_dTime = hScreenHit.m_dTime;
}

const ScreenHit &
ScreenHit::operator=(const ScreenHit &hScreenHit)
{
	m_pPreMomDir = hScreenHit.m_pPreMomDir;
	m_pPreKinNrg = hScreenHit.m_pPreKinNrg;
	//m_pPostMomDir = hScreenHit.m_pPostMomDir;
	m_iTrackId = hScreenHit.m_iTrackId;
	m_iParentId = hScreenHit.m_iParentId;
	m_pParticleType = hScreenHit.m_pParticleType;
	m_pParentType = hScreenHit.m_pParentType ;
	m_pCreatorProcess = hScreenHit.m_pCreatorProcess ;
	m_pDepositingProcess = hScreenHit.m_pDepositingProcess ;
	m_hPosition = hScreenHit.m_hPosition;
	m_dEnergyDeposited = hScreenHit.m_dEnergyDeposited;
	m_dKineticEnergy = hScreenHit.m_dKineticEnergy ;
	m_dTime = hScreenHit.m_dTime;
	
	return *this;
}

G4int
ScreenHit::operator==(const ScreenHit &hScreenHit) const
{
	return ((this == &hScreenHit) ? (1) : (0));
}

void ScreenHit::Draw()
{
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	
	if(pVVisManager)
	{
		G4Circle hCircle(m_hPosition);
		G4Colour hColour(1.000, 0.973, 0.184);
		G4VisAttributes hVisAttributes(hColour);
		
		hCircle.SetScreenSize(0.1);
		hCircle.SetFillStyle(G4Circle::filled);
		hCircle.SetVisAttributes(hVisAttributes);
		pVVisManager->Draw(hCircle);
	}
}

void ScreenHit::Print()
{
	G4cout << "-------------------- Screen hit --------------------" 
		<< "Id: " << m_iTrackId
		<< " Particle: " << *m_pParticleType
		<< " ParentId: " << m_iParentId
		<< " ParentType: " << *m_pParentType << G4endl
		<< "CreatorProcess: " << *m_pCreatorProcess
		<< " DepositingProcess: " << *m_pDepositingProcess << G4endl
		<< "Position: " << m_hPosition.x()/CLHEP::mm
		<< " " << m_hPosition.y()/CLHEP::mm
		<< " " << m_hPosition.z()/CLHEP::mm
		<< " mm" << G4endl
		<< "EnergyDeposited: " << m_dEnergyDeposited/CLHEP::keV << " keV"
		<< " KineticEnergyLeft: " << m_dKineticEnergy/CLHEP::keV << " keV"
		<< " Time: " << m_dTime/CLHEP::s << " s" << G4endl;
}

