#include "MyDetectorConstruction.hh"
#include "MyDetectorMessenger.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4RotationMatrix.hh"

MyDetectorConstruction::MyDetectorConstruction()
: G4VUserDetectorConstruction(),
  halfLabSize(G4ThreeVector(30*cm, 30*cm, 30*cm)),
  displayMaterialLabels(true)
{
	myDetectorMessenger = new MyDetectorMessenger(this);
}

MyDetectorConstruction::~MyDetectorConstruction()
{
	delete myDetectorMessenger;
}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
	DefineMaterials();
	return ConstructDetector();
}

void MyDetectorConstruction::DefineMaterials()
{
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material *G4H2Oliquid = nist->FindOrBuildMaterial("G4_WATER");
	G4Material *G4H2Osteam = nist->FindOrBuildMaterial("G4_WATER_VAPOR");
	G4Material *G4Pb = nist->FindOrBuildMaterial("G4_Pb");

	G4Material *skin = nist->FindOrBuildMaterial("G4_SKIN_ICRP");
	G4Material *softTissue = nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP");
	G4Material *muscle = nist->FindOrBuildMaterial("G4_MUSCLE_WITH_SUCROSE");
	G4Material *bone = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");

	G4String symbol;
	G4double a, z, density;
	G4int ncomponents;

	G4Material *Al = new G4Material("Aluminium", z=13, a=26.98*g/mole, density=2.7*g/cm3);
	G4Material *Pb = new G4Material("Lead", z=82, a= 207.19*g/mole, density= 11.35*g/cm3);
	G4Material *Ti = new G4Material("Titanium", z=22, a=47.867*g/mole, density=4.54*g/cm3);

	G4Element *Cs = new G4Element("Cesium", symbol="Cs", z=55, a=132.9*g/mole);
	G4Element *I = new G4Element("Iodine", symbol="I", z=53, a=126.9*g/mole);
	G4Material *CsI = new G4Material("CsI", density=4.51*g/cm3, ncomponents=2);
	CsI->AddElement(I, .5);
	CsI->AddElement(Cs, .5);

	labMaterial = air;
	trapezoidMaterial = G4H2Oliquid;
	tetrahedronMaterial = bone;
	coneMaterial = skin;
	sphereMaterial = softTissue;
	torusMaterial = softTissue;
}

G4VPhysicalVolume* MyDetectorConstruction::ConstructDetector()
{
	G4bool checkOverlaps = true;
	G4double opacity = 0.4;

	G4VisAttributes* invisible = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	invisible->SetVisibility(false);

	G4VisAttributes *orange = new G4VisAttributes(G4Colour(1, 0.65, 0, opacity));
	orange->SetVisibility(true);
	orange->SetForceWireframe(true); // Whichever is last overwrites the previous one!
	orange->SetForceSolid(true);     // Whichever is last overwrites the previous one!

	G4VisAttributes *yellow = new G4VisAttributes(G4Colour(1, 1, 0, opacity));
	yellow->SetVisibility(true);
	yellow->SetForceWireframe(true);
	yellow->SetForceSolid(true);

	G4VisAttributes *cyan = new G4VisAttributes(G4Colour(0, 1, 1, opacity));
	cyan->SetVisibility(true);
	cyan->SetForceSolid(true);

	G4VisAttributes *magenta = new G4VisAttributes(G4Colour(1, 0, 1, opacity));
	magenta->SetVisibility(true);
	magenta->SetForceSolid(true);

	G4VisAttributes *brown = new G4VisAttributes(
			G4Colour(0.71, 0.4, 0.11, opacity));
	brown->SetVisibility(true);
	brown->SetForceSolid(true);

	solidLab = new G4Box("Lab",          //name
						halfLabSize.x(), halfLabSize.y(), halfLabSize.z());

	logicalLab = new G4LogicalVolume(solidLab,
									labMaterial, //material
									"Lab");      //name

	physicalLab = new G4PVPlacement(nullptr,   		 //no rotation
									G4ThreeVector(), //at (0,0,0)
									logicalLab,      //logical volume
									"Lab",           //name
									nullptr,         //mother volume
									false,           //no boolean operation
									0);              //copy number

	G4double halfLengthX1 = 8*cm, halfLengthX2 = 5*cm;
	G4double halfLengthY1 = 6*cm, halfLengthY2 = 4*cm;
	G4double halfLengthZ = 3*cm;
	solidTrapezoid = new G4Trd("Trapezoid",
								halfLengthX1, halfLengthX2,
								halfLengthY1, halfLengthY2,
								halfLengthZ);

	logicalTrapezoid = new G4LogicalVolume(solidTrapezoid,
	                                       trapezoidMaterial,
	                                       "Trapezoid");

	new G4PVPlacement(nullptr,					// no rotation
	                  G4ThreeVector(0, 0, 0), 	// at (x,y,z)
					  logicalTrapezoid,			// logical volume
	                  "Trapezoid",     			// name
	                  logicalLab, 		     	// logical mother volume
	                  false,           			// no boolean operations
	                  0,               			// copy number
	                  checkOverlaps); 			// checking overlaps

	logicalTrapezoid->SetVisAttributes(yellow);


	solidSphere = new G4Sphere("Sphere",
								1*cm,		   // innerRadius,
								2.5*cm,		   // outerRadius,
								0*deg,         // Starting phi
								360*deg,       // Delta phi
								0*deg,         // Starting theta
								100*deg);      // Delta theta

	logicalSphere = new G4LogicalVolume(solidSphere,
										sphereMaterial,
										"Sphere",
										0, 0, 0);

	G4RotationMatrix *rotation1 =
			new G4RotationMatrix(G4ThreeVector(1, 0, 0), 160*deg);
	new G4PVPlacement(rotation1,               	// rotation
	                  G4ThreeVector(0, 0, 0), 	// at (x,y,z)
					  logicalSphere, 			// logical volume
	                  "Sphere",      			// name
	                  logicalTrapezoid,     	// logical mother volume, note: NOT logicalLab
	                  false,           			// no boolean operations
	                  0,               			// copy number
	                  checkOverlaps); 			// checking overlaps

	logicalSphere->SetVisAttributes(orange);


	G4ThreeVector vertex1 = G4ThreeVector(-12*mm, 0, 0);
	G4ThreeVector vertex2 = G4ThreeVector(12*mm, 0, 0);
	G4ThreeVector vertex3 = G4ThreeVector(0, 20*mm, 0);
	G4ThreeVector vertex4 = G4ThreeVector(0, 0, 30*mm);
	solidTetrahedron = new G4Tet("Tetrahedron",
								vertex1, vertex2, vertex3, vertex4);

	logicalTetrahedron = new G4LogicalVolume(solidTetrahedron,
	                                       	 tetrahedronMaterial,
											 "Tetrahedron");

	G4RotationMatrix *rotation2 = new G4RotationMatrix();
	rotation2->rotateX(150*deg);
	rotation2->rotateY(0*deg);
	rotation2->rotateZ(5*deg);
	new G4PVPlacement(rotation2, 			          // rotation
	                  G4ThreeVector(4*cm, 2*cm, 1*cm), // at (x,y,z)
					  logicalTetrahedron, 		 	  // logical volume
	                  "Tetrahedron",      		 	  // name
	                  logicalTrapezoid,     	 	  // logical mother volume, note: NOT logicalLab
	                  false,           			 	  // no boolean operations
	                  0,               			 	  // copy number
	                  checkOverlaps); 			 	  // checking overlaps

	logicalTetrahedron->SetVisAttributes(cyan);


	G4double radiusBaseInner = 3*cm;
	G4double radiusBaseOuter = 3.4*cm;
	G4double radiusTopInner = 1.7*cm;
	G4double radiusTopOuter = 2*cm;
	G4double halfHeightZ = 2.5*cm;
	G4double anglePhiStart = 0;
	G4double anglePhiStop = 235*deg;
	solidCone = new G4Cons("Cone",
						   radiusBaseInner, radiusBaseOuter,
						   radiusTopInner, radiusTopOuter,
						   halfHeightZ,
						   anglePhiStart, anglePhiStop);

	logicalCone = new G4LogicalVolume(solidCone,
	                                  coneMaterial,
									  "Cone");

	new G4PVPlacement(nullptr,				        // no rotation
	                  G4ThreeVector(-13*cm, 0, 0), 	// at (x,y,z)
					  logicalCone, 					// logical volume
	                  "Cone",		      			// name
	                  logicalLab, 		    		// logical mother volume
	                  false,           				// no boolean operations
	                  0,               				// copy number
	                  checkOverlaps); 				// checking overlaps

	logicalCone->SetVisAttributes(magenta);


	G4double radius1min = 1.25*cm, radius1max = 2*cm, radius2 = 4*cm;
	G4double angle1 = 60*deg, angle2 = 2.5*rad;
	solidTorus = new G4Torus("Torus",
							radius1min, radius1max,
							radius2,
							angle1, angle2);

	logicalTorus = new G4LogicalVolume(solidTorus, torusMaterial, "Torus");

	G4RotationMatrix *rotation3 = new G4RotationMatrix();
//	rotation3->rotateX(50*deg);
	rotation3->rotateY(-55*deg);
	rotation3->rotateZ(55*deg);
	new G4PVPlacement(rotation3, 			        // rotation
					  G4ThreeVector(13*cm, 0, 0), 	// at (x,y,z)
					  logicalTorus,					// its logical volume
					  "Torus",		      			// its name
					  logicalLab, 		    		// logical mother volume
					  false,           				// no boolean operations
					  0,               				// copy number
					  checkOverlaps); 				// checking overlaps

	logicalTorus->SetVisAttributes(brown);

	return physicalLab;
}

void MyDetectorConstruction::SetLabMaterial(const G4String& labMaterial)
{
	G4Material *ptrToMaterial = G4NistManager::Instance()->FindOrBuildMaterial(labMaterial);
	if (ptrToMaterial)
	{
		this->labMaterial = ptrToMaterial;
		if (logicalLab)
		{
			logicalLab->SetMaterial(this->labMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
			G4cout << "Lab material changed to: " << GetLabMaterial() << G4endl;
		}
	}
	else
	{
		G4cerr << "Material not found" << G4endl;
	}
}

void MyDetectorConstruction::SetTrapezoidMaterial(const G4String& trapezoidMaterial)
{
	G4Material *ptrToMaterial = G4NistManager::Instance()->FindOrBuildMaterial(trapezoidMaterial);
	if (ptrToMaterial)
	{
		this->trapezoidMaterial = ptrToMaterial;
		if (logicalTrapezoid)
		{
			logicalTrapezoid->SetMaterial(this->trapezoidMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
			G4cout << "Trapezoid material changed to: " << GetTrapezoidMaterial() << G4endl;
		}
	}
	else
	{
		G4cerr << "Material not found" << G4endl;
	}
}

void MyDetectorConstruction::SetSphereMaterial(const G4String& sphereMaterial)
{
	G4Material *ptrToMaterial = G4NistManager::Instance()->FindOrBuildMaterial(sphereMaterial);
	if (ptrToMaterial)
	{
		this->sphereMaterial = ptrToMaterial;
		if (logicalSphere)
		{
			logicalSphere->SetMaterial(this->sphereMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
			G4cout << "Sphere material changed to: " << GetSphereMaterial() << G4endl;
		}
	}
	else
	{
		G4cerr << "Material not found" << G4endl;
	}
}

void MyDetectorConstruction::SetConeMaterial(const G4String& coneMaterial)
{
	G4Material *ptrToMaterial = G4NistManager::Instance()->FindOrBuildMaterial(coneMaterial);
	if (ptrToMaterial)
	{
		this->coneMaterial = ptrToMaterial;
		if (logicalCone)
		{
			logicalCone->SetMaterial(this->coneMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
			G4cout << "Cone material changed to: " << GetConeMaterial() << G4endl;
		}
	}
	else
	{
		G4cerr << "Material not found" << G4endl;
	}
}

void MyDetectorConstruction::SetTetrahedronMaterial(const G4String& tetrahedronMaterial)
{
	G4Material *ptrToMaterial = G4NistManager::Instance()->FindOrBuildMaterial(tetrahedronMaterial);
	if (ptrToMaterial)
	{
		this->tetrahedronMaterial = ptrToMaterial;
		if (logicalTetrahedron)
		{
			logicalTetrahedron->SetMaterial(this->tetrahedronMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
			G4cout << "Tetrahedron material changed to: " << GetTetrahedronMaterial() << G4endl;
		}
	}
	else
	{
		G4cerr << "Material not found" << G4endl;
	}
}

void MyDetectorConstruction::SetTorusMaterial(const G4String& torusMaterial)
{
	G4Material *ptrToMaterial = G4NistManager::Instance()->FindOrBuildMaterial(torusMaterial);
	if (ptrToMaterial)
	{
		this->torusMaterial = ptrToMaterial;
		if (logicalTorus)
		{
			logicalTorus->SetMaterial(this->torusMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
			G4cout << "Torus material changed to: " << GetTorusMaterial() << G4endl;
		}
	}
	else
	{
		G4cerr << "Material not found" << G4endl;
	}
}
