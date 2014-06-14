#pragma once

#include "ai_search.h"
#include "kjcPlatoModel.h"

namespace kjc {
	class PlatoProblem : public ai::Search::Problem {
	public:
		// Constructors and Destructor
		PlatoProblem(ai::Search::State *initialState, kjc::PlatoModel *model);
		virtual ~PlatoProblem();

		// Methods
		virtual bool GoalTest(const ai::Search::State * const state_in) const;
		virtual bool FindSuccessors(const ai::Search::State * const state_in,
									std::vector<ai::Search::ActionStatePair> &results_out) const;
		virtual double StepCost(const ai::Search::State  * const state1,
								const ai::Search::Action * const action_in,
								const ai::Search::State  * const state2) const;

	private:
		kjc::PlatoModel *mModel;
		int mSearches;
	};
}