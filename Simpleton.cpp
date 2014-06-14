#include "Simpleton.h"

namespace ai
{
  namespace Sample
  {

    Simpleton::Simpleton()
    {
      SetName("Simpleton");
    }
    
    Simpleton::~Simpleton()
    {
    }
    
    ai::Agent::Action * Simpleton::Program(const ai::Agent::Percept * percept)
    {
      ai::Agent::Action *action = new ai::Agent::WumpusAction;
      unsigned int     i;
      
      for(i = 0; i < percept->NumAtom(); i++)
	{
	  ai::Agent::PerceptAtom a = percept->GetAtom(i);
	  if(a.GetName() == "BREEZE")
	    {
	      std::cout << "Drafty here." << std::endl;
	    }
	  else if(a.GetName() == "STENCH")
	    {
	      std::cout << "Smelly here." << std::endl;
	    }
	  else if(a.GetName() == "SCREAM")
	    {
	      std::cout << "Ow, my ears!" << std::endl;
	    }
	  else if(a.GetName() == "GLITTER")
	    {
	      std::cout << "Shiny!" << std::endl;
	    }
	  else if(a.GetName() == "BUMP")
	    {
	      std::cout << "Ouch, my toe!" << std::endl;
	    }
	}
      
      action->SetCode(ai::Agent::WumpusAction::FORWARD);
      
      return action;
    }
  }
}
