#include "MyPrimaryGeneratorMessenger.hh"
#include "MyPrimaryGeneratorAction.hh"

#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithABool.hh"
#include "G4PhysicalConstants.hh"

MyPrimaryGeneratorMessenger::MyPrimaryGeneratorMessenger(
		MyPrimaryGeneratorAction *myPrimaryGeneratorAction)
: G4UImessenger(),
  myPrimaryGeneratorAction(myPrimaryGeneratorAction)
{
	setGunPositionCmd = new G4UIcmdWith3VectorAndUnit("/gun/position", this);
	setGunPositionCmd->SetGuidance("Set gun position");
	setGunPositionCmd->SetParameterName("x", "y", "z", false);
	setGunPositionCmd->SetDefaultUnit("cm");
	setGunPositionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	setGunDirectionCmd = new G4UIcmdWith3Vector("/gun/direction", this);
	setGunDirectionCmd->SetGuidance("Set gun direction");
	setGunDirectionCmd->SetParameterName("x", "y", "z", false);
	setGunDirectionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	setBeamDispersionCmd = new G4UIcmdWithABool("/beam/dispersion", this);
	setBeamDispersionCmd->SetGuidance("True or false");
	setBeamDispersionCmd->SetParameterName("trueOrFalse", false);
	setBeamDispersionCmd->SetDefaultValue(true);
	setBeamDispersionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	setBeamDistributionCmd = new G4UIcommand("/beam/randomDistribution", this);
	setBeamDistributionCmd->SetGuidance("on or off u or g (uniform or Gaussian) amplitude or sigma");

	G4UIparameter *setRandomDistributionPrm =
			new G4UIparameter("RandomDistribution", 's', false);
	setRandomDistributionPrm->SetGuidance("u for uniform or g for Gaussian distribution");
	setRandomDistributionPrm->SetParameterName("distribution");
	setRandomDistributionPrm->SetDefaultValue("u");
	setBeamDistributionCmd->SetParameter(setRandomDistributionPrm);

	G4UIparameter *setAmplitudeOrSigma =
			new G4UIparameter("RandomDistribution", 'd', false);
	setAmplitudeOrSigma->SetGuidance("amplitude for uniform distribution or sigma for Gaussian");
	setAmplitudeOrSigma->SetParameterName("amplitudeOrSigma");
	setAmplitudeOrSigma->SetDefaultValue(0.25);
	setBeamDistributionCmd->SetParameter(setAmplitudeOrSigma);

	setBeamDistributionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
	setBeamDistributionCmd->SetToBeBroadcasted(false);
}

MyPrimaryGeneratorMessenger::~MyPrimaryGeneratorMessenger()
{
	delete setGunPositionCmd;
	delete setGunDirectionCmd;
	delete setBeamDispersionCmd;
	delete setBeamDistributionCmd;
}

void MyPrimaryGeneratorMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
	if( command == setGunPositionCmd )
	{
		myPrimaryGeneratorAction->SetGunPosition(
				setGunPositionCmd->GetNew3VectorValue(newValue));
	}
	else if( command == setGunDirectionCmd )
	{
		G4ThreeVector gunDirectionFromCmd =
				setGunDirectionCmd->GetNew3VectorValue(newValue);
		if (gunDirectionFromCmd == G4ThreeVector())
		{
			myPrimaryGeneratorAction->SetAimGunAtOrigin(true);
		}
		else
		{
			myPrimaryGeneratorAction->SetAimGunAtOrigin(false);
		}
		myPrimaryGeneratorAction->SetGunDirection(gunDirectionFromCmd);
	}
	else if ( command == setBeamDispersionCmd )
	{
		myPrimaryGeneratorAction->SetBeamDispersionOnOff(
				setBeamDispersionCmd->GetNewBoolValue(newValue));
	}
	else if ( command == setBeamDistributionCmd )
	{
		G4String randomDistribution;
		G4double amplitudeOrSigma;

		std::istringstream is(newValue);
		is >> randomDistribution >> amplitudeOrSigma;

		myPrimaryGeneratorAction->SetBeamDistribution(randomDistribution, amplitudeOrSigma);
	}
}

G4String MyPrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand *command)
{
	G4String currentValue;

	if ( command == setGunPositionCmd )
	{
		G4ThreeVector gunPosition = myPrimaryGeneratorAction->GetGunPosition();
		currentValue = setGunPositionCmd->ConvertToString(gunPosition, "cm");
	}

	if ( command == setGunDirectionCmd )
	{
		G4ThreeVector gunDirection = myPrimaryGeneratorAction->GetGunDirection();
		currentValue = setGunDirectionCmd->ConvertToString(gunDirection);
	}

	if ( command == setBeamDispersionCmd )
	{
		G4bool beamDispersionOn = myPrimaryGeneratorAction->GetBeamDispersionOnOff();
		if (beamDispersionOn)
			currentValue = "true";
		else
			currentValue = "false";
	}

	if ( command == setBeamDistributionCmd )
	{
		G4String randomDistribution = myPrimaryGeneratorAction->GetRandomDistribution();
		G4double amplitudeOrSigma = myPrimaryGeneratorAction->GetAmplitudeOrSigma();

		if (randomDistribution == "u")
			randomDistribution = "uniform";
		else if (randomDistribution == "g")
			randomDistribution = "gaussian";

		currentValue = randomDistribution;
		currentValue += ", amplitudeOrSigma = ";
		currentValue += setBeamDistributionCmd->ConvertToString(amplitudeOrSigma);
	}

	return currentValue;
}

