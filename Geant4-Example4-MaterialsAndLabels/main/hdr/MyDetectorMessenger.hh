#ifndef HDR_MYDETECTORMESSENGER_HH_
#define HDR_MYDETECTORMESSENGER_HH_

#include "G4UImessenger.hh"

class MyDetectorConstruction;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcommand;

class MyDetectorMessenger: public G4UImessenger
{
public:
	MyDetectorMessenger(MyDetectorConstruction*);
	virtual ~MyDetectorMessenger();

	virtual void SetNewValue(G4UIcommand* command, G4String newValue);
	virtual G4String GetCurrentValue(G4UIcommand* command);

private:
	MyDetectorConstruction *myDetectorConstruction;
	G4UIcmdWithAString *setLabMaterialCmd;
	G4UIcmdWithAString *setTrapezoidMaterialCmd;
	G4UIcmdWithAString *setSphereMaterialCmd;
	G4UIcmdWithAString *setConeMaterialCmd;
	G4UIcmdWithAString *setTetrahedronMaterialCmd;
	G4UIcmdWithAString *setTorusMaterialCmd;
	G4UIcmdWithABool *setMaterialLabelsCmd;
	G4UIcmdWithoutParameter *setDisplayLabelsCmd;
};

#endif /* HDR_MYDETECTORMESSENGER_HH_ */
