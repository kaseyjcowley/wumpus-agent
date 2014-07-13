#include <cmath>
#include "kjcPlatoModel.h"
#include "Debug.h"

// Direction strings for enum
std::string directions[] = {"NORTH", "EAST", "SOUTH", "WEST"};

/**
 * CELL IMPLEMENTATION
 */
namespace kjc {
	// Constructors and Destructor
	Cell::Cell() {
	}

	Cell::Cell(const Cell &rhs) {
		*this = rhs;
	}

	Cell::Cell(const int &x, const int &y, const bool &visited) {
		this->mX = x;
		this->mY = y;
		this->mVisited = visited;
	}

	Cell::~Cell() {
	}

	// Overidden Operators
	Cell &Cell::operator=(const Cell &rhs) {
		this->mX = rhs.mX;
		this->mY = rhs.mY;
		this->mVisited = rhs.mVisited;

		return *this;
	}

	bool Cell::operator==(const Cell &rhs) const {
		return (this->mX == rhs.mX && this->mY == rhs.mY && this->mVisited == rhs.mVisited);
	}

	// Getters and Setters
	void Cell::SetX(const int &x) {
		this->mX = x;
	}

	void Cell::SetY(const int &y) {
		this->mY = y;
	}

	void Cell::SetVisited(const bool &visited) {
		this->mVisited = visited;
	}

	int Cell::GetX() const {
		return this->mX;
	}

	int Cell::GetY() const {
		return this->mY;
	}

	bool Cell::isVisited() const {
		return this->mVisited;
	}

	// Display functions
	void Cell::Display(std::ostream &out) const {
		out << "Cell's X: " << this->mX << std::endl
			<< "Cell's Y: " << this->mY << std::endl
			<< "Cell visited?: " << (int)this->mVisited << std::endl;
	}

	std::ostream &operator<<(std::ostream &os, const Cell &rhs) {
		rhs.Display(os);
		return os;
    }

/**
 * MODEL IMPLEMENTATION
 */

	// Constructor and Destructor
	PlatoModel::PlatoModel() {
		// Set the starting position
		this->mCurrentX = this->mCurrentY = 1;
		// We start out facing east
		this->mCurrentDirection = D_EAST;
		// Allocate the knowledge base
		this->mKb = new WumpusKnowledgeBase();

		// Set the Wumpus dead flag to false
		this->mWumpusDead = false;
		this->mWumpusX = this->mWumpusY = 0;
		this->mWumpusLocation = NULL;

		// Populate the environment with cells
		for (int x = 1; x <= sWIDTH; x++) {
			for (int y = 1; y <= sHEIGHT; y++) {
				// Allocate a new cell
				Cell newCell(x, y, false);
				// Add the cell to the environment map
				if (!this->AddCell(newCell)) {
					std::cout << "Error adding new cell from model constructor: " << newCell << std::endl;
				}
			}
		}
	}

	PlatoModel::~PlatoModel() {
		delete mKb;
		delete mWumpusLocation;
	}

	// Getters and Setters
	void PlatoModel::SetCurrentX(const int &x) {
		this->mCurrentX = x;
	}

	void PlatoModel::SetCurrentY(const int &y) {
		this->mCurrentY = y;
	}

	void PlatoModel::SetCurrentDirection(const int &direction) {
		this->mCurrentDirection = direction;
	}

	void PlatoModel::SetWumpusDead() {
		delete this->mWumpusLocation;
		this->mWumpusLocation = NULL;
		this->mWumpusDead = true;
		std::cout << "MODEL IS SETTING WUMPUS AS DEAD: " << (int)this->mWumpusDead << std::endl;
	}

	int PlatoModel::GetCurrentX() const {
		return  this->mCurrentX;		
	}

	int PlatoModel::GetCurrentY() const {
		return  this->mCurrentY;
	}

	int PlatoModel::GetCurrentDirection() const {
		return this->mCurrentDirection;
	}

	bool PlatoModel::WumpusDead() const {
		return this->mWumpusDead;
	}

	int PlatoModel::GetWumpusX() const {
		return this->mWumpusX;
	}

	int PlatoModel::GetWumpusY() const {
		return this->mWumpusY;
	}

	// Methods
	bool PlatoModel::AddCell(const Cell &cell) {
		int key = this->CellToKey(cell);
		this->mEnvironment[key] = cell;

		return true;
	}

	Cell PlatoModel::GetCell(const int &x, const int &y) const {
		int key = this->PositionToKey(x, y);
		std::map<const int, Cell>::const_iterator it = this->mEnvironment.find(key);

		return it->second;
	}

	int PlatoModel::CellToKey(const Cell &cell) {
		return this->PositionToKey(cell.GetX(), cell.GetY());
	}

	// Takes two integers and concatenates them
	int PlatoModel::PositionToKey(const int &x, const int &y) const {
		int times = 1;
    	while (times <= y) {
    		times *= 10;
    	}

    	return x * times + y;
	}

	std::string PlatoModel::EnumToString() const {
		return directions[this->mCurrentDirection];
	}

	void PlatoModel::SetCurrentCellToVisited() {
		Cell currentCell = this->GetCell(this->mCurrentX, this->mCurrentY);

		// If we haven't visited this cell already, update the current cell to be visited
		if (!currentCell.isVisited()) {
			currentCell.SetVisited(true);

			if (!this->AddCell(currentCell)) {
				std::cout << "Error adding new cell from Agent Program: " << currentCell << std::endl;
			}
		}
	}

	void PlatoModel::UpdateCurrentDirection(const int &turn) {
		switch (turn) {
			// If we turn left, and are facing north, we need to wrap around the values
			// Otherwise, we just subtract one from the direction we are facing
			case T_LEFT:
				if (this->mCurrentDirection == D_NORTH) {
					this->mCurrentDirection = D_WEST;
				} else {
					this->mCurrentDirection--;
				}
			break;
			// If we turn right, just add 1 and modulo by the total directions in order to wrap around
			case T_RIGHT:
				if (this->mCurrentDirection == D_WEST) {
					this->mCurrentDirection = D_NORTH;
				} else {
					this->mCurrentDirection++;
				}
			break;
			default:
				std::cout << "Error: an invalid turn enum value was passed to UpdateCurrentDirection" << std::endl;
		}
	}

	void PlatoModel::UpdateCurrentPosition() {
		// We need to figure out what direction we are facing in order to update the current positions
		switch (this->mCurrentDirection) {
			case D_NORTH:
				this->mCurrentY++;
			break;
			case D_EAST:
				this->mCurrentX++;
			break;
			case D_SOUTH:
				this->mCurrentY--;
			break;
			case D_WEST:
				this->mCurrentX--;
			break;
		}
	}

	bool PlatoModel::FindWumpus() {
		for (int x = 1; x <= sWIDTH; x++) {
			for (int y = 1; y <= sHEIGHT; y++) {
				if (this->mKb->WumpusAt(x,y)) {
					this->mWumpusX = x;
					this->mWumpusY = y;
					return true;
				}
			}
		}

		return false;
	}

	bool PlatoModel::NextCellIsWumpus() const {
		return ((std::abs(this->mWumpusX - this->mCurrentX) == 1 && this->mCurrentY == this->mWumpusY) || (std::abs(this->mWumpusY - this->mCurrentY) && this->mCurrentX == this->mWumpusX));
	}

	// Display functions
	void PlatoModel::Display(std::ostream &out) const {
		// Basic facts about environment
		out << "Current X: " << this->mCurrentX << std::endl
			<< "Current Y: " << this->mCurrentY << std::endl
			<< "Current Direction: " << this->EnumToString() << std::endl << std::endl;

		// Loop over all cells and output the key/value pair
		for (std::map<const int, Cell>::const_iterator it = this->mEnvironment.begin(); it != this->mEnvironment.end(); ++it) {
			out << "Key: " << it->first << ", Value: " << it->second << std::endl;
		}
	}

	std::ostream &operator<<(std::ostream &os, const kjc::PlatoModel &model) {
		model.Display(os);
		return os;
    }
}