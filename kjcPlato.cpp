#include "kjcPlato.h"
#include "kjcPlatoState.h"
#include "kjcPlatoProblem.h"
#include "Debug.h"

namespace kjc {
	Plato::Plato() {
		SetName("Plato");
		this->mModel = new kjc::PlatoModel();
		this->mHasGold = false;
		this->mHuntinDaWumpus = false;
	}

	Plato::~Plato() {
		delete this->mModel;
	}

	ai::Agent::Action * Plato::Program(const ai::Agent::Percept *percept) {
		ai::Agent::Action *action = new ai::Agent::WumpusAction;
		// Start parsing percepts by adding information to the knowledge base
		if (!this->ParsePercepts(percept, action)) {
			action->SetCode(ai::Agent::WumpusAction::QUIT);
			return action;
		}

		if (this->mHasGold || this->mHuntinDaWumpus) {
			return action;
		}

		// Get the cell that corresponds to our current position and update its visited state
		this->mModel->SetCurrentCellToVisited();

		// Search for the goal and return the actions as they come off the queue
		this->SearchAndGetAction(action);

		return action;
	}

	bool Plato::ParsePercepts(const ai::Agent::Percept *percept, ai::Agent::Action *action) {
		// If we have the gold, quit
		if (this->mHasGold) {
			return false;
		}

		// Find our current position
		int x = this->mModel->GetCurrentX();
		int y = this->mModel->GetCurrentY();
		// bool isStench = false;
		// bool isBreeze = false;
		// Get the cell to determine if we have visited this cell before so we can skip parameter parsing
		Cell currentCell = this->mModel->GetCell(x, y);

		if (currentCell.isVisited()) {
			return true;
		}

		if (percept->GetAtom("SCREAM").GetValue() != "") {
			this->mModel->SetWumpusDead();
			this->mHuntinDaWumpus = false;
			action->SetCode(ai::Agent::WumpusAction::NOOP);
		}

		// Check for glitter
		if (percept->GetAtom("GLITTER").GetValue() != "") {
			DEBUG("GRABBING GOLD");
			action->SetCode(ai::Agent::WumpusAction::GRAB);
			this->mHasGold = true;
			return true;
		}

		// Check for breezes
		if (percept->GetAtom("BREEZE").GetValue() != "") {
			DEBUG("BREEZE IS FELT");
			// isBreeze = true;
			this->mModel->mKb->TellBreezy(x, y, true);
		} else {
			DEBUG("BREEZE IS NOT FELT");
			this->mModel->mKb->TellBreezy(x, y, false);
		}

		// Check for stenches
		if (percept->GetAtom("STENCH").GetValue() != "") {
			DEBUG("STENCH IS SMELT");
			// isStench = true;
			this->mModel->mKb->TellStench(x, y, true);

			if (!this->mModel->WumpusDead()) {
			this->mHuntinDaWumpus = true;

				// Determine what direction I'm in
				if (this->mModel->mKb->WumpusAt(x,y+1)) {
					if (this->mModel->GetCurrentDirection() != D_NORTH) {
						action->SetCode(ai::Agent::WumpusAction::TURN_RIGHT);
					} else {
						action->SetCode(ai::Agent::WumpusAction::SHOOT);
					}
				} else if (this->mModel->mKb->WumpusAt(x+1,y)) {
					if (this->mModel->GetCurrentDirection() != D_EAST) {
						action->SetCode(ai::Agent::WumpusAction::TURN_RIGHT);
					} else {
						action->SetCode(ai::Agent::WumpusAction::SHOOT);
					}
				} else if (this->mModel->mKb->WumpusAt(x,y-1)) {
					if (this->mModel->GetCurrentDirection() != D_SOUTH) {
						action->SetCode(ai::Agent::WumpusAction::TURN_RIGHT);
					} else {
						action->SetCode(ai::Agent::WumpusAction::SHOOT);
					}
				} else if (this->mModel->mKb->WumpusAt(x-1,y)) {
					if (this->mModel->GetCurrentDirection() != D_WEST) {
						action->SetCode(ai::Agent::WumpusAction::TURN_RIGHT);
					} else {
						action->SetCode(ai::Agent::WumpusAction::SHOOT);
					}
				}
			}

		} else {
			this->mHuntinDaWumpus = false;
			DEBUG("STENCH IS NOT SMELT");
			this->mModel->mKb->TellStench(x, y, false);
		}

		// // If we feel a breeze or smell a stench in (1,1), then quit
		// if ((isStench || isBreeze) && (x == 1 && y == 1)) {
		// 	return false;
		// }

		return true;
	}

	void Plato::SearchAndGetAction(ai::Agent::Action *action) {
		// Check if our action queue is empty and search for a goal if it is
		if (this->mActionQueue.empty()) {
			if (!this->SearchForGoal()) {
				action->SetCode(ai::Agent::WumpusAction::QUIT);
				return;
			}
		}

		if (!this->mActionQueue.empty()) {
			// Get the next action
			kjc::PlatoAction a = this->mActionQueue.front();
			// Pop the action from the queue
			this->mActionQueue.pop_front();

			// Determine what to do based on the action that was provided
			switch (a.GetType()) {
				case A_FORWARD:
					// Move forward
					this->mModel->UpdateCurrentPosition();
					action->SetCode(ai::Agent::WumpusAction::FORWARD);
				break;
				case A_LEFT:
					// Turn left
					this->mModel->UpdateCurrentDirection(T_LEFT);
					action->SetCode(ai::Agent::WumpusAction::TURN_LEFT);
				break;
				case A_RIGHT:
					// Turn right
					this->mModel->UpdateCurrentDirection(T_RIGHT);
					action->SetCode(ai::Agent::WumpusAction::TURN_RIGHT);
				break;
				default:
					action->SetCode(ai::Agent::WumpusAction::QUIT);
			}
		}
	}

	bool Plato::SearchForGoal() {
		bool finished = false;

		while (!finished) {
			kjc::PlatoState *initialState = new PlatoState();
			initialState->SetX(this->mModel->GetCurrentX());
			initialState->SetY(this->mModel->GetCurrentY());
			initialState->SetDirection(this->mModel->GetCurrentDirection());

			// std::cout << "initialState: " << *initialState << std::endl;

			kjc::PlatoProblem *problem = new PlatoProblem(initialState, this->mModel);
			ai::Search::Fringe *frontier = new ai::Search::AStarFringe();

			ai::Search::Algorithm *algorithm = new ai::Search::Graph(problem, frontier);

			if (algorithm->Search()) {
				// std::cout << "Searching for goal" << std::endl;
				std::list<ai::Search::Node *> *solution = algorithm->GetSolution().GetList();
	            std::list<ai::Search::Node *>::const_iterator it;
	            
	            double cost = 0;
	            int depth   = 0;
	            for(it = solution->begin(); it != solution->end(); it++) {
	                if((*it)->GetAction()) {
	                    const kjc::PlatoAction * const action = dynamic_cast<const kjc::PlatoAction * const>((*it)->GetAction());
	                    std::cout << *action << " ";
	                    this->mActionQueue.push_back(*action);
	                }
	                if((*it)->GetState()) {
	                    const kjc::PlatoState * const state = dynamic_cast<const kjc::PlatoState * const>((*it)->GetState());
	                    std::cout << *state << " ";
	                }
	                cost = (*it)->GetPathCost();
	                depth = (*it)->GetDepth();
	                std::cout << std::endl;
	              }
	            
	            size_t nodes_generated = algorithm->GetNumberNodesGenerated();
	            size_t nodes_stored    = algorithm->GetMaxNodesStored();
	            std::cout << "Results: T " << cost << " " << depth << " "
	                      << nodes_generated << " " << nodes_stored << std::endl;
	            finished = true;
	          }
	        else {
	        	std::cout << "Failed to find goal" << std::endl;
	            size_t nodes_generated = algorithm->GetNumberNodesGenerated();
	            size_t nodes_stored    = algorithm->GetMaxNodesStored();
	            double cost = -1;
	            int depth = -1;
	            std::cout << "Results: F " << cost << " " << depth << " "
	                      << nodes_generated << " " << nodes_stored << std::endl;

	            return false;
	        }

	        delete algorithm;
		}

		return true;
	}
}