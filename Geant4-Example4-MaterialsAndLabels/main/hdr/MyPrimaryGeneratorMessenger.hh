#ifndef HDR_MYPRIMARYGENERATORMESSENGER_HH_
#define HDR_MYPRIMARYGENERATORMESSENGER_HH_

#include "G4UImessenger.hh"

class MyPrimaryGeneratorAction;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWithABool;
class G4UIcommand;

class MyPrimaryGeneratorMessenger: public G4UImessenger
{
public:
	MyPrimaryGeneratorMessenger(MyPrimaryGeneratorAction *myPrimaryGeneratorAction);
	~MyPrimaryGeneratorMessenger();

	void SetNewValue(G4UIcommand *command, G4String newValues);
	G4String GetCurrentValue(G4UIcommand *command);

private:
	MyPrimaryGeneratorAction *myPrimaryGeneratorAction = nullptr;
	G4UIcmdWith3VectorAndUnit *setGunPositionCmd;
	G4UIcmdWith3Vector *setGunDirectionCmd;
	G4UIcmdWithABool *setBeamDispersionCmd;
	G4UIcommand *setBeamDistributionCmd;
};


#endif /* HDR_MYPRIMARYGENERATORMESSENGER_HH_ */
