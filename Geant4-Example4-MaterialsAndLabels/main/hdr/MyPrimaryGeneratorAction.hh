#ifndef MYPRIMARYGENERATORACTION_HH_
#define MYPRIMARYGENERATORACTION_HH_

#include "G4VUserPrimaryGeneratorAction.hh"
#include "MyDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class G4ParticleGun;
class G4ParticleDefinition;
class G4Event;
class MyPrimaryGeneratorMessenger;

class MyPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
public:
	MyPrimaryGeneratorAction(MyDetectorConstruction* myDetectorConstruction);
	~MyPrimaryGeneratorAction();

	void GeneratePrimaries(G4Event* anEvent);

	G4ThreeVector GetGunPosition() const { return gunPosition; }
	void SetGunPosition(G4ThreeVector gunPosition);

	G4ThreeVector GetGunDirection() const { return gunDirection; }
	void SetGunDirection(G4ThreeVector gunDirection);

	G4bool GetAimGunAtOrigin() const { return aimGunAtOrigin; }
	void SetAimGunAtOrigin(G4bool aimGunAtOrigin);

	G4bool GetBeamDispersionOnOff() const { return beamDispersionOn; }
	void SetBeamDispersionOnOff(G4bool beamDispersionOn);

	G4String GetRandomDistribution() const { return randomDistribution; }
	G4double GetAmplitudeOrSigma() const { return amplitudeOrSigma; }
	void SetBeamDistribution(G4String& randomDistribution, G4double amplitudeOrSigma);

private:
	G4bool IsGunInsideLab(G4ThreeVector gunPosition) const;
	G4ThreeVector GetUnitThreeVector(const G4ThreeVector) const;
	G4ThreeVector RandomiseDirection();

private:
	MyPrimaryGeneratorMessenger *myPrimaryGeneratorMessenger;
	MyDetectorConstruction *myDetectorConstruction;
	G4ParticleGun *particleGun;
	G4ParticleDefinition *electron;
	G4ParticleDefinition *positron;
	G4ParticleDefinition *opticalphoton;
	G4ParticleDefinition *gamma;
	G4ParticleDefinition *proton;
	G4ParticleDefinition *antiproton;

	G4double gunEnergy;
	G4ThreeVector gunPosition, gunDirection, halfLabSize;
	G4bool aimGunAtOrigin = true;
	G4bool beamDispersionOn = true;
	G4String randomDistribution = "uniform";
	G4double amplitudeOrSigma;
};

#endif /* MYPRIMARYGENERATORACTION_HH_ */
