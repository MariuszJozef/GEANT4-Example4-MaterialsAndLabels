#ifndef HDR_MYDETECTORCONSTRUCTION_HH_
#define HDR_MYDETECTORCONSTRUCTION_HH_

#include "G4VUserDetectorConstruction.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
//class G4Material;
#include "G4Material.hh"
class MyDetectorMessenger;

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tet.hh"
#include "G4Cons.hh"
#include "G4Torus.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class MyDetectorConstruction: public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	virtual ~MyDetectorConstruction();
	virtual G4VPhysicalVolume* Construct();
	G4ThreeVector GetHalfLabSize() const { return halfLabSize; }

	G4String GetLabMaterial() { return labMaterial->GetName(); }
	void SetLabMaterial(const G4String&  labMaterial);

	G4String GetTrapezoidMaterial() { return trapezoidMaterial->GetName(); }
	void SetTrapezoidMaterial(const G4String& trapezoidMaterial);

	G4String GetSphereMaterial() { return sphereMaterial->GetName(); }
	void SetSphereMaterial(const G4String& sphereMaterial);

	G4String GetConeMaterial() { return coneMaterial->GetName(); }
	void SetConeMaterial(const G4String& coneMaterial);

	G4String GetTetrahedronMaterial() { return tetrahedronMaterial->GetName(); }
	void SetTetrahedronMaterial(const G4String& tetrahedronMaterial);

	G4String GetTorusMaterial() { return torusMaterial->GetName(); }
	void SetTorusMaterial(const G4String& torusMaterial);

	bool GetDisplayMaterialLabels() const { return displayMaterialLabels; }
	void SetDisplayMaterialLabels(bool displayMaterialLabels = true) {
		this->displayMaterialLabels = displayMaterialLabels;
	}

	G4String GetLabelParticleGunName() const { return labelParticleGunName; }
	void SetLabelParticleGunName(const G4String& labelParticleGunName) {
		this->labelParticleGunName = labelParticleGunName;
	}

	G4double GetLabelParticleGunEnergy() const { return labelParticleGunEnergy; }
	void SetLabelParticleGunEnergy(const G4double labelParticleGunEnergy) {
		this->labelParticleGunEnergy = labelParticleGunEnergy;
	}

private:
	void DisplayMaterialDynamicLabels();

	MyDetectorMessenger *myDetectorMessenger;

	G4ThreeVector halfLabSize;
	G4Box *solidLab = nullptr;
	G4Sphere *solidSphere = nullptr;
	G4Trd *solidTrapezoid = nullptr;
	G4Tet *solidTetrahedron = nullptr;
	G4Cons *solidCone = nullptr;
	G4Torus *solidTorus = nullptr;

	G4LogicalVolume *logicalLab = nullptr;
	G4LogicalVolume *logicalSphere = nullptr;
	G4LogicalVolume *logicalTrapezoid = nullptr;
	G4LogicalVolume *logicalTetrahedron = nullptr;
	G4LogicalVolume *logicalCone = nullptr;
	G4LogicalVolume *logicalTorus = nullptr;

	G4VPhysicalVolume *physicalLab = nullptr;

	G4Material *labMaterial = nullptr;
	G4Material *sphereMaterial = nullptr;
	G4Material *trapezoidMaterial = nullptr;
	G4Material *tetrahedronMaterial = nullptr;
	G4Material *coneMaterial = nullptr;
	G4Material *torusMaterial = nullptr;

	bool displayMaterialLabels = true;
	G4String labelParticleGunName;
	G4double labelParticleGunEnergy;

	void DefineMaterials();
	G4VPhysicalVolume* ConstructDetector();
};

#endif /* HDR_MYDETECTORCONSTRUCTION_HH_ */
