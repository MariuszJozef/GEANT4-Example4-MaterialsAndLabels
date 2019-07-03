#ifndef MYPHYSICSLIST_HH_
#define MYPHYSICSLIST_HH_

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;

class MyPhysicsList: public G4VModularPhysicsList
{
public:
	MyPhysicsList();
	~MyPhysicsList();

protected:
	void ConstructParticle();
	void ConstructProcess();
	void SetCuts();

private:
	G4VPhysicsConstructor *myPhysicsListEM;
};

#endif /* MYPHYSICSLIST_HH_ */
