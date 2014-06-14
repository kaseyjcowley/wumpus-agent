#include "kjcPlatoState.h"
#include "kjcPlatoModel.h"

namespace kjc {
	// Constructors and Destructor
	PlatoState::PlatoState() : mX(1), mY(1), mDirection(D_EAST) {
	}

	PlatoState::PlatoState(const PlatoState &rhs) {
		*this = rhs;
	}

	PlatoState::~PlatoState() {
	}

	// Overridden Operators
	PlatoState &PlatoState::operator=(const PlatoState &rhs) {
		this->mX = rhs.mX;
		this->mY = rhs.mY;
		this->mDirection = rhs.mDirection;
		return *this;
	}

	bool PlatoState::operator==(const PlatoState &rhs) const {
		return (this->mX == rhs.mX && this->mY == rhs.mY && this->mDirection == rhs.mDirection);
	}

	// Setters and Getters
	void PlatoState::SetX(const int &x) {
		this->mX = x;
	}

	void PlatoState::SetY(const int &y) {
		this->mY = y;
	}

	void PlatoState::SetDirection(const int &direction) {
		this->mDirection = direction;
	}

	int PlatoState::GetX() const {
		return this->mX;
	}

	int PlatoState::GetY() const {
		return this->mY;
	}

	int PlatoState::GetDirection() const {
		return this->mDirection;
	}

	// Methods
	bool PlatoState::IsEqual(const ai::Search::State * const state_in) const {
		return true;
	}

	// Display Methods
	void PlatoState::Display(std::ostream &out) const {
		out << "State X: " << this->mX << std::endl
			<< "State Y: " << this->mY << std::endl
			<< "State's Direction: " << this->mDirection << std::endl;
	}

	std::ostream &operator<<(std::ostream &os, const PlatoState &rhs) {
		rhs.Display(os);
		return os;
	}
}