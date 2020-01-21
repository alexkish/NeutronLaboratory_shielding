#ifndef __CollimatorEventData_H__
#define __EventsData_H__

#include <string>
#include <vector>

using std::string;
using std::vector;

class NeutronLabEventData
{
public:
	 NeutronLabEventData();
	~NeutronLabEventData();

public:
	void Clear();

public:
	int m_iEventId;								// the event ID
	float m_fTotalEnergyDeposited;				// total energy deposited in the ScintSD
	int m_iNbSteps;								// number of energy depositing steps
	vector<int> *m_pTrackId;					// id of the particle
	vector<int> *m_pParentId;					// id of the parent particle
	vector<string> *m_pParticleType;			// type of particle
	vector<string> *m_pParentType;				// type of parent particle
	vector<string> *m_pCreatorProcess;			// process in which the particle was created
	vector<string> *m_pDepositingProcess;		// energy depositing process
	vector<float> *m_pX;						// position of the step
	vector<float> *m_pY;
	vector<float> *m_pZ;
	vector<float> *m_pEnergyDeposited; 			// energy deposited in the step
	vector<float> *m_pKineticEnergy;			// particle kinetic energy after the step			
	vector<float> *m_pTime;						// time of the step
	vector<string> *m_pPrimaryParticleType;		// type of particle
	float m_fPrimaryX;							// position of the primary particle
	float m_fPrimaryY;
	float m_fPrimaryZ;
	float m_fPrimaryE;							// energy of the primary particle

	// kinetic energy and momentum
	vector<float>* m_pPreMomDirX;
	vector<float>* m_pPreMomDirY;
	vector<float>* m_pPreMomDirZ;
	vector<double>* m_pPreKinNrg;
	//vector<float>* m_pPostMomDirX;
	//vector<float>* m_pPostMomDirY;
	//vector<float>* m_pPostMomDirZ;
	//vector<double>* m_pPostKinNrg;

};

#endif
