#include "NeutronLabEventData.hh"

NeutronLabEventData::NeutronLabEventData()
{
	m_iEventId = 0;

	m_fTotalEnergyDeposited = 0.;
	m_iNbSteps = 0;

	m_pTrackId = new vector<int>;
	m_pParentId = new vector<int>;
	m_pParticleType = new vector<string>;
	m_pParentType = new vector<string>;
	m_pCreatorProcess = new vector<string>;
	m_pDepositingProcess = new vector<string>;
	m_pX = new vector<float>;
	m_pY = new vector<float>;
	m_pZ = new vector<float>;
	m_pEnergyDeposited = new vector<float>;
	m_pKineticEnergy = new vector<float>;
	m_pTime = new vector<float>;

	m_pPrimaryParticleType = new vector<string>;
	m_fPrimaryX = 0.;
	m_fPrimaryY = 0.;
	m_fPrimaryZ = 0.;	
	m_fPrimaryE	= 0.;

	// kinetic energy and momentum	
	m_pPreMomDirX = new vector<float>;
	m_pPreMomDirY = new vector<float>;
	m_pPreMomDirZ = new vector<float>;
 	m_pPreKinNrg = new vector<double>;
 	//m_pPostMomDirX = new vector<float>;
 	//m_pPostMomDirY = new vector<float>;
 	//m_pPostMomDirZ = new vector<float>;
 	//m_pPostKinNrg = new vector<double>;
	
}

NeutronLabEventData::~NeutronLabEventData()
{
	delete m_pTrackId;
	delete m_pParentId;
	delete m_pParticleType;
	delete m_pParentType;
	delete m_pCreatorProcess;
	delete m_pDepositingProcess;
	delete m_pX;
	delete m_pY;
	delete m_pZ;
	delete m_pEnergyDeposited;
	delete m_pKineticEnergy;
	delete m_pTime;

	delete m_pPrimaryParticleType;

	// kinetic energy and momentum
	delete m_pPreMomDirX;
	delete m_pPreMomDirY;
	delete m_pPreMomDirZ;
	delete m_pPreKinNrg;
	//delete m_pPostMomDirX;
	//delete m_pPostMomDirY;
	//delete m_pPostMomDirZ;
	//delete m_pPostKinNrg;
}

void
NeutronLabEventData::Clear()
{
	m_iEventId = 0;

	m_fTotalEnergyDeposited = 0.0;
	m_iNbSteps = 0;

	m_pTrackId->clear();
	m_pParentId->clear();
	m_pParticleType->clear();
	m_pParentType->clear();
	m_pCreatorProcess->clear();
	m_pDepositingProcess->clear();
	m_pX->clear();
	m_pY->clear();
	m_pZ->clear();
	m_pEnergyDeposited->clear();
	m_pKineticEnergy->clear();
	m_pTime->clear();

	m_pPrimaryParticleType->clear();
	m_fPrimaryX = 0.;
	m_fPrimaryY = 0.;
	m_fPrimaryZ = 0.;
	m_fPrimaryE	= 0.;

	// kinetic energy and momentum
	m_pPreMomDirX->clear();
	m_pPreMomDirY->clear();
	m_pPreMomDirZ->clear();
	m_pPreKinNrg->clear();
	//m_pPostMomDirX->clear();
	//m_pPostMomDirY->clear();
	//m_pPostMomDirZ->clear();
	//m_pPostKinNrg->clear();

}

