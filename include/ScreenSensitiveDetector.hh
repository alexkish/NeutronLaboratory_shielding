#ifndef __ScreenSensitiveDetector_H__
#define __ScreenSensitiveDetector_H__

#include <G4VSensitiveDetector.hh>

#include "ScreenHit.hh"

class G4Step;
class G4HCofThisEvent;

class ScreenSensitiveDetector: public G4VSensitiveDetector
{
public:
	ScreenSensitiveDetector(G4String hName);
	~ScreenSensitiveDetector();

	void Initialize(G4HCofThisEvent *pHitsCollectionOfThisEvent);
	G4bool ProcessHits(G4Step *pStep, G4TouchableHistory *pHistory);
	void EndOfEvent(G4HCofThisEvent *pHitsCollectionOfThisEvent);

private:
	ScreenHitsCollection* m_pScreenHitsCollection;

	std::map<int,G4String> m_hParticleTypes;
};

#endif // __XENON10PLXESENSITIVEDETECTOR_H__

