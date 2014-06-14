#include "Aristotle.h"
#include <cstdio>

namespace ai
{
  namespace Sample
  {

    const int BUF_SIZE = 1024;
    static char buf[BUF_SIZE];
    //static char buf2[BUF_SIZE];
    
    Aristotle::Aristotle()
    {
      SetName("Aristotle");

      x = 1;
      y = 1;
      
      /* I know there is no pit in 1,1 */
      std::sprintf(buf, "!P_1_1");
      kb.AddSentence(buf);
    }
    
    Aristotle::~Aristotle()
    {
    }
    
    ai::Agent::Action * Aristotle::Program(const ai::Agent::Percept * percept)
    {
      ai::Agent::Action *action = new ai::Agent::WumpusAction;

      /* Either there is a breeze, or there is not a breeze */
      if(percept->GetAtom("BREEZE").GetName() != "")
	{
	  std::sprintf(buf, "B_%d_%d", x, y);
	}
      else
	{
	  std::sprintf(buf, "! B_%d_%d", x, y);
	}
      /* remember the breeze percept */
      kb.AddSentence(buf);

      /* Ask if I know there is a pit to the east */
      {
	bool rval;
	ai::PL::KnowledgeBase question;
	sprintf(buf, "P_%d_%d", x + 1, y);
	question.AddSentence(buf);
	rval = (kb.DPLL_Entails(question) == ai::PL::Symbol::KNOWN_TRUE);
	if(rval)
	  {
	    std::cout << "Pit to the east" << std::endl;
	  }
      }
      /* Ask if I know there is not a pit to the east */
      {
	bool rval;
	ai::PL::KnowledgeBase question;
	sprintf(buf, "(!P_%d_%d)", x + 1, y);
	question.AddSentence(buf);
	rval = (kb.DPLL_Entails(question) == ai::PL::Symbol::KNOWN_TRUE);
	if(rval)
	  {
	    std::cout << "No pit to the east" << std::endl;
	  }
      }

      action->SetCode(ai::Agent::WumpusAction::FORWARD);
      
      return action;
    }
  }
}
