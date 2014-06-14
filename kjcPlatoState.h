#pragma once

#include "ai_search.h"

namespace kjc {
	class PlatoState : public ai::Search::State {
	public:
		// Constructors and Destructor
		PlatoState();
		PlatoState(const PlatoState &rhs);
		virtual ~PlatoState();

		// Overridden Operators
		PlatoState &operator=(const PlatoState &rhs);
		bool operator==(const PlatoState &rhs) const;

		// Setters and Getters
		void SetX(const int &x);
		void SetY(const int &y);
		void SetDirection(const int &direction);

		int GetX() const;
		int GetY() const;
		int GetDirection() const;

		// Methods
		virtual bool IsEqual(const ai::Search::State * const state_in) const;

		// Display Methods
		virtual void Display(std::ostream &out) const;
		
	private:
		int mX, mY;
		int mDirection;
	};

	std::ostream &operator<<(std::ostream &os, const PlatoState &rhs);
}