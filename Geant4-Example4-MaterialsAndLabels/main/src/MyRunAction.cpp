#include "MyRunAction.hh"
#include "MyDetectorConstruction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include <fstream>
#include "G4UnitsTable.hh"

MyRunAction::MyRunAction(MyDetectorConstruction* myDetectorConstruction)
:
G4UserRunAction(),
myDetectorConstruction(myDetectorConstruction)
{

}

MyRunAction::~MyRunAction() {}

void MyRunAction::BeginOfRunAction(const G4Run*)
{
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4String fileName = "./vis-dynamicLabels.macro";

	std::ofstream outFile;
	outFile.open(fileName);
	// Create and write to the file if it doesn't exit, otherwise overwrite it
	if (outFile.is_open())
	{
		outFile << "#ParticleGun:" << G4endl;
		outFile << "/vis/set/textColour cyan" << G4endl;
		outFile << "/vis/scene/add/text2D -0.9 0.7 12 ! ! gun particle: "
				+ myDetectorConstruction->GetLabelParticleGunName() << G4endl;

		G4String particleGunEnergyAndUnit
		= G4BestUnit(myDetectorConstruction->GetLabelParticleGunEnergy(), "Energy");
		outFile << "/vis/scene/add/text2D -0.9 0.6 12 ! ! gun energy: "
				+ particleGunEnergyAndUnit<< G4endl;

		if (myDetectorConstruction->GetDisplayMaterialLabels())
		{
			outFile << "#LabMaterial:" << G4endl;
			outFile << "/vis/set/textColour white" << G4endl;
			outFile << "/vis/scene/add/text -25 -25 -25 cm 10 0 0 "
					+ myDetectorConstruction->GetLabMaterial() << G4endl;

			outFile << "#TrapezoidMaterial:" << G4endl;
			outFile << "/vis/scene/add/text 0.5 5 0 cm 10 0 0 "
					+ myDetectorConstruction->GetTrapezoidMaterial() << G4endl;

			outFile << "#SphereMaterial:" << G4endl;
			outFile << "/vis/scene/add/text 0 -1.5 0 cm 10 0 0 "
					+ myDetectorConstruction->GetSphereMaterial() << G4endl;

			outFile << "#TetrahedronMaterial:" << G4endl;
			outFile << "/vis/scene/add/text 4 2 1 cm 10 0 0 "
					+ myDetectorConstruction->GetTetrahedronMaterial() << G4endl;

			outFile << "#ConeMaterial:" << G4endl;
			outFile << "/vis/scene/add/text -13 0 0 cm 10 0 0 "
					+ myDetectorConstruction->GetConeMaterial() << G4endl;

			outFile << "#TorusMaterial:" << G4endl;
			outFile << "/vis/scene/add/text 13 4 0 cm 10 0 0 "
					+ myDetectorConstruction->GetTorusMaterial() << G4endl;
		}

		outFile << "#Scale interval set to yellow colour:" << G4endl;
		outFile << "/vis/set/textColour yellow" << G4endl;
		outFile << "/vis/scene/add/scale" << G4endl;

		outFile.close();
//		G4cout << "Text written to file " << fileName << G4endl;

		G4UImanager *uiManager = G4UImanager::GetUIpointer();
		uiManager->ApplyCommand("/control/execute vis-reInit.macro");
		uiManager->ApplyCommand("/control/execute vis-dynamicLabels.macro");
	}
	else
	{
		G4cerr << "Could not create file " << fileName << G4endl;
	}
}


