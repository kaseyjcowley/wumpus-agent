#include <vector>
#include <cstdlib>
#include "kjcPlatoProblem.h"
#include "kjcPlatoAction.h"
#include "kjcPlatoState.h"
#include "Debug.h"

namespace kjc {
	// Constructors and Destructor
	PlatoProblem::PlatoProblem(ai::Search::State *initialState, kjc::PlatoModel *model)
		: ai::Search::Problem(initialState), mModel(model)
	{
	}

	PlatoProblem::~PlatoProblem() {
	}

	// Methods
	bool PlatoProblem::GoalTest(const ai::Search::State * const state_in) const {
		// Cast the state to a state we can recognize
		const kjc::PlatoState * const state = dynamic_cast<const kjc::PlatoState * const>(state_in);

		Cell state_cell = this->mModel->GetCell(state->GetX(), state->GetY());

		if (!state_cell.isVisited() && this->mModel->mKb->isSafe(state->GetX(), state->GetY())) {
			return true;
		}

		return false;
	}

	bool PlatoProblem::FindSuccessors(const ai::Search::State * const state_in,
								std::vector<ai::Search::ActionStatePair> &results_out) const {
		const kjc::PlatoState * const state = dynamic_cast<const kjc::PlatoState * const>(state_in);

		// Generate the left state
		kjc::PlatoState *leftState = new PlatoState();
		leftState->SetX(state->GetX());
		leftState->SetY(state->GetY());
		int direction = state->GetDirection() == D_NORTH ? (int)D_WEST : state->GetDirection()-1;
		leftState->SetDirection(direction);

		// Generate the right state
		kjc::PlatoState *rightState = new PlatoState();
		rightState->SetX(state->GetX());
		rightState->SetY(state->GetY());
		rightState->SetDirection((state->GetDirection() == D_WEST ? D_NORTH : state->GetDirection()+1));

		// Generate the forward state, making sure it's safe to go there before you do
		kjc::PlatoState *forwardState = new PlatoState();
		bool isLegal = true;
		// Set the X and Y values based on the direction of the agent
		switch (state->GetDirection()) {
			case D_NORTH:
				if (state->GetY()+1 <= this->mModel->sHEIGHT) {
					forwardState->SetY(state->GetY()+1);
					forwardState->SetX(state->GetX());
				} else {
					isLegal = false;
				}
			break;
			case D_EAST:
				if (state->GetX()+1 <= this->mModel->sWIDTH) {
					forwardState->SetX(state->GetX()+1);
					forwardState->SetY(state->GetY());
				} else {
					isLegal = false;
				}
			break;
			case D_SOUTH:
				if (state->GetY()-1 >= 1) {
					forwardState->SetY(state->GetY()-1);
					forwardState->SetX(state->GetX());
				} else {
					isLegal = false;
				}
			break;
			case D_WEST:
				if (state->GetX()-1 >= 1) {
					forwardState->SetX(state->GetX()-1);
					forwardState->SetY(state->GetY());
				} else {
					isLegal = false;
				}
			break;
		}

		// Left Action
		kjc::PlatoAction *actionLeft = new PlatoAction(A_LEFT);
		// Right Action
		kjc::PlatoAction *actionRight = new PlatoAction(A_RIGHT);

		// Check for the legality and safety of the forward state before adding an action state pair
		if (isLegal && this->mModel->mKb->isSafe(forwardState->GetX(), forwardState->GetY())) {
			// Set the direction of the forward state
			forwardState->SetDirection(state->GetDirection());
			// Forward action
			kjc::PlatoAction *actionForward = new PlatoAction();
			actionForward->SetType(A_FORWARD);
			// Action state pair
			ai::Search::ActionStatePair aspForward(forwardState, actionForward);
			results_out.push_back(aspForward);
		} else {
			delete forwardState;
		}

		ai::Search::ActionStatePair aspLeft(leftState, actionLeft);
		ai::Search::ActionStatePair aspRight(rightState, actionRight);

		results_out.push_back(aspLeft);
		results_out.push_back(aspRight);

		return (results_out.size() > 0);
	}

	double PlatoProblem::StepCost(const ai::Search::State  * const state1,
							const ai::Search::Action * const action_in,
							const ai::Search::State  * const state2) const {

		const kjc::PlatoAction * const action = dynamic_cast<const kjc::PlatoAction * const>(action_in);

		// All actions cost 1, unless it's shoot, then it costs 10
		double totalCost = 1.0;

		if (action->GetType() == A_SHOOT) {
			totalCost = 10.0;
		}

		return totalCost;
	}
}