#ifndef _SIMPLETON_H_
#define _SIMPLETON_H_

#include <ai_wumpus.h>
#include <ai_pl.h>

namespace ai
{
  namespace Sample
  {

    class Simpleton : public ai::Agent::AgentProgram
    {
    public:
      Simpleton();
      ~Simpleton();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
    protected:

    private:
    };
  }
}

#endif /* _SIMPLETON_H_ */
