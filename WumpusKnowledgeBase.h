#pragma once

#include <ai_pl.h>
#include <iostream>

namespace kjc {
	class WumpusKnowledgeBase : public ai::PL::KnowledgeBase {
	public:
		WumpusKnowledgeBase();
		virtual ~WumpusKnowledgeBase();
		void TellBreezy(const int &x, const int &y, const bool isBreezy);
		void TellStench(const int &x, const int &y, const bool isStench);
		void AddLogicPhysicsSentence(const int &x, const int &y,
									 const std::string &lhs, const std::string &rhs,
									 const std::string &op, const bool allSquares);
		void AddLogicPhysicsSentence(const std::string &identifier, const std::string &op, bool allSquares);
		bool isSafe(const int &x, const int &y);
		bool WumpusAt(const int &x, const int &y);
		void TellWumpusDead();
	protected:
		static const int WIDTH = 4;
		static const int HEIGHT = 4;
	private:
	};

	std::ostream &operator<<(std::ostream &os, const WumpusKnowledgeBase &rhs);
}