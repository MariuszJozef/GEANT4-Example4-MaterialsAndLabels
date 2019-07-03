#ifndef HDR_MYRUNACTION_HH_
#define HDR_MYRUNACTION_HH_

#include "G4UserRunAction.hh"

class G4Run;
class MyDetectorConstruction;

class MyRunAction: public G4UserRunAction
{
public:
	MyRunAction(MyDetectorConstruction *myDetectorConstruction);
	virtual ~MyRunAction();

	virtual void BeginOfRunAction(const G4Run* run);
	virtual void EndOfRunAction(const G4Run* run);

private:
	MyDetectorConstruction *myDetectorConstruction;;
};

#endif /* HDR_MYRUNACTION_HH_ */
