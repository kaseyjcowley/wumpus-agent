#pragma once

#include <ai_wumpus.h>
#include <ai_pl.h>
#include <list>
#include "kjcPlatoModel.h"
#include "kjcPlatoAction.h"

namespace kjc {
	/**
	 * AGENT CLASS
	 */
	class Plato : public ai::Agent::AgentProgram {
	public:
		// Constructor and Destructor
		Plato();
		~Plato();
		virtual ai::Agent::Action * Program(const ai::Agent::Percept *percept);

		// Methods
		bool ParsePercepts(const ai::Agent::Percept *percept, ai::Agent::Action *action);
		void SearchAndGetAction(ai::Agent::Action *action);
		bool SearchForGoal();

	private:
		kjc::PlatoModel *mModel;
		bool mHasGold;
		std::list<kjc::PlatoAction> mActionQueue;
	};
}