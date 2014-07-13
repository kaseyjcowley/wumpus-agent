#pragma once

#include <map>
#include <string>
#include "WumpusKnowledgeBase.h"
#include "kjcPlatoState.h"

// Directional Enum
enum {
	D_NORTH,
	D_EAST,
	D_SOUTH,
	D_WEST,
	D_COUNT
};

// Action to Direction enum
enum {
	T_LEFT,
	T_RIGHT
};

namespace kjc {
	/**
	 * CELL CLASS
	 */
	class Cell {
	public:
		// Constructors and Destructor
		Cell();
		Cell(const Cell &rhs);
		Cell(const int &x, const int &y, const bool &visited);
		~Cell();

		// Overidden Operators
		Cell &operator=(const Cell &rhs);
		bool operator==(const Cell &rhs) const;

		// Getters and Setters
		void SetX(const int &x);
		void SetY(const int &y);
		void SetVisited(const bool &visited);

		int GetX() const;
		int GetY() const;
		bool isVisited() const;

		// Display functions
		void Display(std::ostream &out) const;

	private:
		int mX, mY;
		bool mVisited;
	};

	/**
	 * MODEL CLASS
	 */
	class PlatoModel {
	public:
		// Constructor and Destructor
		PlatoModel();
		~PlatoModel();

		// Getters and Setters
		void SetCurrentX(const int &x);
		void SetCurrentY(const int &y);
		void SetCurrentDirection(const int &direction);
		void IncrementSearches();
		void SetWumpusDead();

		int GetCurrentX() const;
		int GetCurrentY() const;
		int GetCurrentDirection() const;
		int GetSearches() const;
		bool WumpusDead() const;
		int GetWumpusX() const;
		int GetWumpusY() const;

		// Methods
		bool AddCell(const Cell &cell);
		Cell GetCell(const int &x, const int &y) const;
		int CellToKey(const Cell &cell);
		int PositionToKey(const int &x, const int &y) const;
		std::string EnumToString() const;
		void SetCurrentCellToVisited();
		void UpdateCurrentDirection(const int &turn);
		void UpdateCurrentPosition();
		bool FindWumpus();
		bool NextCellIsWumpus() const;

		// Display functions
		void Display(std::ostream &out) const;

		// The Knowledgebase
		WumpusKnowledgeBase *mKb;

		// Height and Width of the world
		const static int sWIDTH = 4;
		const static int sHEIGHT = 4;

		PlatoState *mWumpusLocation;

	private:
		int mCurrentX, mCurrentY, mWumpusX, mWumpusY;
		int mCurrentDirection;
		std::map<const int, Cell> mEnvironment;
		bool mWumpusDead;
	};

	// Override the output operators
	std::ostream &operator<<(std::ostream &os, const Cell &rhs);
	std::ostream &operator<<(std::ostream &os, const kjc::PlatoModel &model);
}