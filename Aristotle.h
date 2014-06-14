#ifndef _ARISTOTLE_H_
#define _ARISTOTLE_H_

#include <ai_wumpus.h>
#include <ai_pl.h>

namespace ai
{
  namespace Sample
  {
    class Aristotle : public ai::Agent::AgentProgram
    {
    public:
      Aristotle();
      ~Aristotle();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
      
    protected:
      /* Things I know */
      ai::PL::KnowledgeBase kb;

      /* my location */
      int x;
      int y;
    private:
    };
  }
}

#endif /* _ARISTOTLE_H_ */
