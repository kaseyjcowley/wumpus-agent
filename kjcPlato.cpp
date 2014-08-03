#include "kjcPlato.h"
#include "kjcPlatoState.h"
#include "kjcPlatoProblem.h"
#include "Debug.h"

#include <fstream>
#include <iostream>

namespace kjc {
	Plato::Plato() {
		SetName("Plato");
		this->mModel = new kjc::PlatoModel();
		this->mHasGold = false;
		this->firstShot = false;
	}

	Plato::~Plato() {
		delete this->mModel;
	}

	ai::Agent::Action * Plato::Program(const ai::Agent::Percept *percept) {
		ai::Agent::Action *action = new ai::Agent::WumpusAction;
		// If we've got the gold, then just quit
		if (this->mHasGold) {
			action->SetCode(ai::Agent::WumpusAction::QUIT);
			return action;
		}

		// Start parsing percepts by adding information to the knowledge base
		this->ParsePercepts(percept, action);

		std::ofstream myfile;
		myfile.open("PropLogic.txt");
		myfile << *(this->mModel->mKb);
		myfile.close();

		switch (action->GetCode()) {
			case ai::Agent::WumpusAction::GRAB:
				this->mHasGold = true;
				return action;
			case ai::Agent::WumpusAction::SHOOT:
			case ai::Agent::WumpusAction::QUIT:
				return action;
		}

		// Get the cell that corresponds to our current position and update its visited state
		this->mModel->SetCurrentCellToVisited();

		// Search for the goal and return the actions as they come off the queue
		this->SearchAndGetAction(action);

		return action;
	}

	void Plato::ParsePercepts(const ai::Agent::Percept *percept, ai::Agent::Action *action) {
		// Find our current position
		int x = this->mModel->GetCurrentX();
		int y = this->mModel->GetCurrentY();
		bool isStench = false;

		Cell currentCell = this->mModel->GetCell(x,y);

		// Check for screams
		if (percept->GetAtom("SCREAM").GetValue() != "") {
			this->mModel->SetWumpusDead();
			this->mModel->mKb->TellWumpusDead();
		} else if (firstShot && !this->mModel->WumpusDead()) {
			action->SetCode(ai::Agent::WumpusAction::QUIT);
			return;
		}

		// Check for glitter
		if (percept->GetAtom("GLITTER").GetValue() != "") {
			action->SetCode(ai::Agent::WumpusAction::GRAB);
		}

		if (currentCell.isVisited()) {
			return;
		}

		// Check for breezes
		if (percept->GetAtom("BREEZE").GetValue() != "") {
			this->mModel->mKb->TellBreezy(x, y, true);
		} else {
			this->mModel->mKb->TellBreezy(x, y, false);
		}

		// Check for stenches
		if (!this->mModel->WumpusDead()) {
			if (percept->GetAtom("STENCH").GetValue() != "") {
				isStench = true;
				this->mModel->mKb->TellStench(x, y, true);
			} else {
				this->mModel->mKb->TellStench(x, y, false);
			}
		}

		if (isStench && ((x == 1 && y == 1) || (x == 2 && y == 1))) {
			action->SetCode(ai::Agent::WumpusAction::SHOOT);
			this->firstShot = true;
		}
	}

	void Plato::SearchAndGetAction(ai::Agent::Action *action) {
		// Check if our action queue is empty and search for a goal if it is
		if (this->mActionQueue.empty()) {
			if (!this->SearchForGoal()) {
				if (this->mModel->FindWumpus() && this->mModel->mWumpusLocation == NULL && !this->mModel->WumpusDead()) {
					this->mModel->mWumpusLocation = new PlatoState();
					this->mModel->mWumpusLocation->SetX(this->mModel->GetWumpusX());
					this->mModel->mWumpusLocation->SetY(this->mModel->GetWumpusY());
				} else {
					action->SetCode(ai::Agent::WumpusAction::QUIT);
					return;
				}
			}
		}

		if (!this->mActionQueue.empty()) {
			// Get the next action
			kjc::PlatoAction a = this->mActionQueue.front();
			// Pop the action from the queue
			// Determine what to do based on the action that was provided
			switch (a.GetType()) {
				case A_FORWARD:
					// Check to see if we need to shoosten the Wumpus
					if (!this->mModel->WumpusDead() && this->mModel->mWumpusLocation != NULL && this->mModel->NextCellIsWumpus()) {
						action->SetCode(ai::Agent::WumpusAction::SHOOT);
					} else {
						// Move forward
						this->mModel->UpdateCurrentPosition();
						action->SetCode(ai::Agent::WumpusAction::FORWARD);
						this->mActionQueue.pop_front();
					}
				break;
				case A_LEFT:
					// Turn left
					this->mModel->UpdateCurrentDirection(T_LEFT);
					action->SetCode(ai::Agent::WumpusAction::TURN_LEFT);
					this->mActionQueue.pop_front();
				break;
				case A_RIGHT:
					// Turn right
					this->mModel->UpdateCurrentDirection(T_RIGHT);
					action->SetCode(ai::Agent::WumpusAction::TURN_RIGHT);
					this->mActionQueue.pop_front();
				break;
				default:
					action->SetCode(ai::Agent::WumpusAction::NOOP);
					this->mActionQueue.pop_front();
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
	                    // std::cout << *action << " ";
	                    this->mActionQueue.push_back(*action);
	                }
	                if((*it)->GetState()) {
	                    // const kjc::PlatoState * const state = dynamic_cast<const kjc::PlatoState * const>((*it)->GetState());
	                    // std::cout << *state << " ";
	                }
	                cost = (*it)->GetPathCost();
	                depth = (*it)->GetDepth();
	                // std::cout << std::endl;
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