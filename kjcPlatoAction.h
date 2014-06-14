#pragma once

#include <iostream>
#include "ai_search.h"

enum {
	A_FORWARD,
	A_LEFT,
	A_RIGHT,
	A_SHOOT
};

namespace kjc {
	class PlatoAction : public ai::Search::Action {
	public:
		// Constructors and Destructor
		PlatoAction();
		PlatoAction(const int &type);
		PlatoAction(const PlatoAction &rhs);
		virtual ~PlatoAction();

		// Overridden Operators
		PlatoAction &operator=(const PlatoAction &rhs);
		bool operator==(const PlatoAction &rhs) const;

		// Getters and Setters
		void SetType(const int &type);
		int GetType() const;

		// Methods
		std::string EnumToString() const;

		// Display Methods
		virtual void Display(std::ostream &os) const;

	private:
		int mType;
	};

	std::ostream &operator<<(std::ostream &os, const PlatoAction &rhs);
}