#include "MyDetectorMessenger.hh"
#include "MyDetectorConstruction.hh"
#include "MyRunAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

MyDetectorMessenger::MyDetectorMessenger(MyDetectorConstruction* myDetectorConstruction)
: G4UImessenger(),
  myDetectorConstruction(myDetectorConstruction)
{
	setLabMaterialCmd = new G4UIcmdWithAString("/material/lab", this);
	setLabMaterialCmd->SetGuidance("Select Material of the laboratory.");
	setLabMaterialCmd->SetParameterName("newLabMaterial",false);
	setLabMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	setTrapezoidMaterialCmd = new G4UIcmdWithAString("/material/trapezoid", this);
	setTrapezoidMaterialCmd->SetGuidance("Select Material of trapezoid.");
	setTrapezoidMaterialCmd->SetParameterName("newTrapezoidMaterial",false);
	setTrapezoidMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	setSphereMaterialCmd = new G4UIcmdWithAString("/material/sphere", this);
	setSphereMaterialCmd->SetGuidance("Select Material of sphere.");
	setSphereMaterialCmd->SetParameterName("newSphereMaterial",false);
	setSphereMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	setConeMaterialCmd = new G4UIcmdWithAString("/material/cone", this);
	setConeMaterialCmd->SetGuidance("Select Material of cone.");
	setConeMaterialCmd->SetParameterName("newConeMaterial",false);
	setConeMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	setTetrahedronMaterialCmd = new G4UIcmdWithAString("/material/tetrahedron", this);
	setTetrahedronMaterialCmd->SetGuidance("Select Material of tetrahedron.");
	setTetrahedronMaterialCmd->SetParameterName("newTetrahedronMaterial",false);
	setTetrahedronMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	setTorusMaterialCmd = new G4UIcmdWithAString("/material/torus", this);
	setTorusMaterialCmd->SetGuidance("Select Material of torus.");
	setTorusMaterialCmd->SetParameterName("newTorusMaterial",false);
	setTorusMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	setMaterialLabelsCmd = new G4UIcmdWithABool("/material/showLabels", this);
	setMaterialLabelsCmd->SetGuidance("True or false");
	setMaterialLabelsCmd->SetParameterName("trueOrFalse", false);
	setMaterialLabelsCmd->SetDefaultValue(true);
	setMaterialLabelsCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

MyDetectorMessenger::~MyDetectorMessenger()
{
	delete setLabMaterialCmd;
	delete setTrapezoidMaterialCmd;
	delete setSphereMaterialCmd;
	delete setConeMaterialCmd;
	delete setTetrahedronMaterialCmd;
	delete setTorusMaterialCmd;
	delete setMaterialLabelsCmd;
	delete setDisplayLabelsCmd;
}

void MyDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
	if (command == setLabMaterialCmd)
	{
		if (newValue == "vacuum")
			newValue = "G4_Galactic";
		myDetectorConstruction->SetLabMaterial(newValue);
	}
	else if (command == setTrapezoidMaterialCmd)
	{
		myDetectorConstruction->SetTrapezoidMaterial(newValue);
	}
	else if (command == setSphereMaterialCmd)
	{
		myDetectorConstruction->SetSphereMaterial(newValue);
	}
	else if (command == setConeMaterialCmd)
	{
		myDetectorConstruction->SetConeMaterial(newValue);
	}
	else if (command == setTetrahedronMaterialCmd)
	{
		myDetectorConstruction->SetTetrahedronMaterial(newValue);
	}
	else if (command == setTorusMaterialCmd)
	{
		myDetectorConstruction->SetTorusMaterial(newValue);
	}
	else if (command == setMaterialLabelsCmd)
	{
		myDetectorConstruction->SetDisplayMaterialLabels(
				setMaterialLabelsCmd->GetNewBoolValue(newValue));
	}
}

G4String MyDetectorMessenger::GetCurrentValue(G4UIcommand* command)
{
	G4String currentValue;

	if (command == setLabMaterialCmd)
	{
		currentValue = myDetectorConstruction->GetLabMaterial();
		if (currentValue == "G4_Galactic")
			currentValue = "vacuum (G4_Galactic)";
	}
	else if (command == setTrapezoidMaterialCmd)
	{
		currentValue = myDetectorConstruction->GetTrapezoidMaterial();
	}
	else if (command == setSphereMaterialCmd)
	{
		currentValue = myDetectorConstruction->GetSphereMaterial();
	}
	else if (command == setConeMaterialCmd)
	{
		currentValue = myDetectorConstruction->GetConeMaterial();
	}
	else if (command == setTetrahedronMaterialCmd)
	{
		currentValue = myDetectorConstruction->GetTetrahedronMaterial();
	}
	else if (command == setTorusMaterialCmd)
	{
		currentValue = myDetectorConstruction->GetTorusMaterial();
	}
	else if (command == setMaterialLabelsCmd)
	{
		G4bool displayMaterialLabels = myDetectorConstruction->GetDisplayMaterialLabels();
		if (displayMaterialLabels)
			currentValue = "true";
		else
			currentValue = "false";
	}

	return currentValue;
}
