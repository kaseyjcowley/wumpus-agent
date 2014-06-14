#include "kjcPlatoAction.h"

// Action strings for enum
std::string actions[] = {"FORWARD", "LEFT", "RIGHT", "SHOOT"};

namespace kjc {
	// Constructors and Destructor
	PlatoAction::PlatoAction() {
	}

	PlatoAction::PlatoAction(const int &type) {
		this->mType = type;
	}

	PlatoAction::PlatoAction(const PlatoAction &rhs) {
		*this = rhs;
	}

	PlatoAction::~PlatoAction() {
	}

	// Overridden Operators
	PlatoAction &PlatoAction::operator=(const PlatoAction &rhs) {
		this->mType = rhs.mType;
		return *this;
	}

	bool PlatoAction::operator==(const PlatoAction &rhs) const {
		return (this->mType == rhs.mType);
	}

	// Getters and Setters
	void PlatoAction::SetType(const int &type) {
		this->mType = type;
	}

	int PlatoAction::GetType() const {
		return this->mType;
	}

	// Methods
	std::string PlatoAction::EnumToString() const {
		return actions[this->mType];
	}

	// Display Methods
	void PlatoAction::Display(std::ostream &os) const {
		os << "Current Action: " << this->mType << std::endl;
	}

	std::ostream &operator<<(std::ostream &os, const PlatoAction &rhs) {
		rhs.Display(os);
		return os;
	}
}