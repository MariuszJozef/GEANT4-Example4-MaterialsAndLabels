#include "MyPrimaryGeneratorAction.hh"
#include "MyPrimaryGeneratorMessenger.hh"
#include "G4RunManager.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction(MyDetectorConstruction* myDetectorConstruction)
:
myDetectorConstruction(myDetectorConstruction),
halfLabSize(myDetectorConstruction->GetHalfLabSize()),
amplitudeOrSigma(0.2)
{
	myPrimaryGeneratorMessenger = new MyPrimaryGeneratorMessenger(this);

	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;

	electron = particleTable->FindParticle(particleName="e-");
	positron = particleTable->FindParticle(particleName="e+");
	opticalphoton = particleTable->FindParticle(particleName="opticalphoton");
	gamma = particleTable->FindParticle(particleName="gamma");
	proton = particleTable->FindParticle(particleName="proton");
	antiproton = particleTable->FindParticle(particleName="anti_proton");

	G4ParticleDefinition *particle = electron;

	gunEnergy = 25*MeV;
	gunPosition = G4ThreeVector(0, 0, -halfLabSize.z());
	gunDirection = G4ThreeVector(0, 0, 1);

	particleGun = new G4ParticleGun(1);
	particleGun->SetParticleDefinition(particle);
	particleGun->SetParticleEnergy(gunEnergy);
}

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction()
{
	delete particleGun;
	delete myPrimaryGeneratorMessenger;
}

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	particleGun->SetParticlePosition(gunPosition);
	SetGunDirection(gunDirection);
	if (beamDispersionOn)
	{
		particleGun->SetParticleMomentumDirection(gunDirection
				+ RandomiseDirection());
	}
	else
	{
		particleGun->SetParticleMomentumDirection(gunDirection);
	}
	particleGun->GeneratePrimaryVertex(anEvent);

//	Particle name and energy labels are displayed via MyDetectorConstruction
	myDetectorConstruction->SetLabelParticleGunName(
			particleGun->GetParticleDefinition()->GetParticleName());
	myDetectorConstruction->SetLabelParticleGunEnergy(
			particleGun->GetParticleEnergy());
}

void MyPrimaryGeneratorAction::SetGunPosition(G4ThreeVector gunPosition)
{
	if (IsGunInsideLab(gunPosition))
	{
		this->gunPosition = gunPosition;
		// Takes effect upon: /run/beamOn
		G4RunManager::GetRunManager()->ReinitializeGeometry();
		// Effective immediately; removes beam
//		G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
	}
	else
	{
		G4cerr << "Gun position must be inside lab volume" << G4endl;
	}
}

bool MyPrimaryGeneratorAction::IsGunInsideLab(G4ThreeVector gunPosition) const
{
	if (  ( abs(gunPosition.x()) <= halfLabSize.x() ) &&
		  ( abs(gunPosition.y()) <= halfLabSize.y() ) &&
		  ( abs(gunPosition.z()) <= halfLabSize.z() )
		)
		return true;
	else
		false;
}

G4ThreeVector MyPrimaryGeneratorAction::GetUnitThreeVector(const G4ThreeVector aThreeVector) const
{
	G4ThreeVector itsUnitVector = aThreeVector;
	G4double magnitude = pow( aThreeVector.x(), 2);
	magnitude += pow(aThreeVector.y(), 2);
	magnitude += pow(aThreeVector.z(), 2);
	magnitude = sqrt(magnitude);
	return itsUnitVector /= magnitude;
}

void MyPrimaryGeneratorAction::SetAimGunAtOrigin(G4bool aimGunAtOrigin)
{
	this->aimGunAtOrigin = aimGunAtOrigin;
}

void MyPrimaryGeneratorAction::SetGunDirection(G4ThreeVector gunDirection)
{
	if (aimGunAtOrigin)
	{
//		Aim the particle gun at the origin irrespective of gunPosition
//		unless gunDirection was explicitly set via user command
		this->gunDirection = -GetUnitThreeVector(gunPosition);
	}
	else
	{
		// gunDirection as set by the user's command
		this->gunDirection = GetUnitThreeVector(gunDirection);
	}
}

G4ThreeVector MyPrimaryGeneratorAction::RandomiseDirection()
{
	G4ThreeVector randomiseDirection = G4ThreeVector();

	if (randomDistribution == "u" || randomDistribution == "uniform")
	{
		randomiseDirection = G4ThreeVector(
				G4UniformRand(), G4UniformRand(), G4UniformRand());
		// Shift these random numbers from (0, 1) interval to (-0.5, 0.5)
		randomiseDirection -= G4ThreeVector(0.5, 0.5, 0.5);
		randomiseDirection *= amplitudeOrSigma;
	}
	else if (randomDistribution == "g" || randomDistribution == "gaussian"
			|| randomDistribution == "Gaussian")
	{
		randomiseDirection = G4ThreeVector(
			G4RandGauss::shoot(0, amplitudeOrSigma),
			G4RandGauss::shoot(0, amplitudeOrSigma),
			G4RandGauss::shoot(0, amplitudeOrSigma));
	}
	return randomiseDirection;
}

void MyPrimaryGeneratorAction::SetBeamDispersionOnOff(G4bool beamDispersionOn)
{
	this->beamDispersionOn = beamDispersionOn;
}

void MyPrimaryGeneratorAction::SetBeamDistribution(
		G4String& randomDistribution, G4double amplitudeOrSigma)
{
	this->randomDistribution = randomDistribution;
	this->amplitudeOrSigma = amplitudeOrSigma;
}

