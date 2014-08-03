#include <iostream>
#include <string>
#include <cstdio>
#include <sstream>
#include <vector>
#include "WumpusKnowledgeBase.h"
#include "Debug.h"

namespace kjc {

	WumpusKnowledgeBase::WumpusKnowledgeBase()
  	: ai::PL::KnowledgeBase()
	{
		AddSentence("!P_1_1");
		AddSentence("!W_1_1");

		// Create the logic physics of the world
		// Breeze physics & Stench physics
		for (int x = 1; x <= WIDTH; x++) {
			for (int y = 1; y <= HEIGHT; y++) {
				// Add Breeze Physics
				AddLogicPhysicsSentence(x, y, "B", "P", "|", false);

				// Add Stench Physics
				AddLogicPhysicsSentence(x, y, "S", "W", "|", false);

				// At most 1 Wumpus
				AddLogicPhysicsSentence(x, y, "W", "!W", "&", true);
			}
		}

		// At least 1 Wumpus
		AddLogicPhysicsSentence("W", "|", true);
	}

	WumpusKnowledgeBase::~WumpusKnowledgeBase() {
	}

	void WumpusKnowledgeBase::AddLogicPhysicsSentence(const int &x, const int &y, const std::string &lhs, const std::string &rhs, const std::string &op, const bool allSquares) {
		std::ostringstream ss;
		std::string separator = " " + op + " ";

		ss << lhs << "_" << x << "_" << y << " <=> (";

		// If we should iterate over all squares
		if (allSquares) {
			for (int i = 1; i <= WIDTH; i++) {
				for (int j = 1; j <= HEIGHT; j++) {
					if (i == x && j == y) {
						continue;
					}
					ss << "(" << rhs << "_" << i << "_" << j << ")" << separator;
				}
			}
		} else {
			if (x-1 >= 1) {
				ss << rhs << "_" << x-1 << "_" << y << separator;
			}

			if (x+1 <= WIDTH) {
				ss << rhs << "_" << x+1 << "_" << y << separator;
			}

			if (y-1 >= 1) {
				ss << rhs << "_" << x << "_" << y-1 << separator;
			}

			if (y+1 <= HEIGHT) {
				ss << rhs << "_" << x << "_" << y+1 << separator;
			}
		}

		// Find last occurrence of | to remove it and replace with a ) to end the sentence
		std::string sentence = ss.str();
		unsigned pos = sentence.rfind(separator);
		sentence.replace(pos, std::string::npos, ")");

		// std::cout << sentence << std::endl;

		AddSentence(sentence);

		ss.str("");
	}

	void WumpusKnowledgeBase::AddLogicPhysicsSentence(const std::string &identifier, const std::string &op, bool allSquares) {
		std::ostringstream ss;
		std::string separator = " " + op + " ";

		for (int x = 1; x <= WIDTH; x++) {
			for (int y = 1; y <= HEIGHT; y++) {
				ss << "(" << identifier << "_" << x << "_" << y << ") " << op << " ";
			}
		}

		// Remove the trailing &
		std::string sentence = ss.str();
		unsigned pos = sentence.rfind(separator);
		sentence.erase(pos, std::string::npos);

		// std::cout << sentence << std::endl;

		AddSentence(sentence);

		ss.str("");
	}

	void WumpusKnowledgeBase::TellBreezy(const int &x, const int &y, const bool isBreezy) {
		/*
		tell B_X_Y <=> (P_X+1_Y V P_X_Y+1 V P_X-1_Y V P_X_Y-1)
		*/
		std::ostringstream ss;
		std::string negate = !isBreezy ? "(!" : "";
		std::string matchingParen = !negate.empty() ? ")" : "";

		ss << negate << "B_" << x << "_" << y << matchingParen;

		// std::cout << ss.str() << std::endl;

		std::string sentence = ss.str();

		AddSentence(sentence);

		ss.str("");
	}

	void WumpusKnowledgeBase::TellStench(const int &x, const int &y, const bool isStench) {
		// Determine whether we need to say it is stench or it is not stench
		std::ostringstream ss;
		std::string negate = !isStench ? "!" : "";

		ss << "(" << negate << "S_" << x << "_" << y << ")";

		// std::cout << ss.str() << std::endl;

		std::string sentence = ss.str();

		AddSentence(sentence);

		ss.str("");
	}

	bool WumpusKnowledgeBase::isSafe(const int &x, const int &y) {
		std::ostringstream ss;
		ai::PL::KnowledgeBase question;
		bool safe;

		ss << "((WD | (!W_" << x << "_" << y << ")) & (!P_" << x << "_" << y << "))";

		// std::cout << ss.str() << std::endl;

		question.AddSentence(ss.str());

		safe = (this->DPLL_Entails(question) == ai::PL::Symbol::KNOWN_TRUE);

		return safe;
	}

	bool WumpusKnowledgeBase::WumpusAt(const int &x, const int &y) {
		std::ostringstream ss;
		ai::PL::KnowledgeBase question;
		bool isWumpus;

		ss << "(W_" << x << "_" << y << ")";

		question.AddSentence(ss.str());

		isWumpus = (this->DPLL_Entails(question) == ai::PL::Symbol::KNOWN_TRUE);

		return isWumpus;
	}

	void WumpusKnowledgeBase::TellWumpusDead() {
		AddSentence("WD");
	}

	std::ostream &operator<<(std::ostream &os, const WumpusKnowledgeBase &rhs) {
		std::vector<ai::PL::Sentence *> sentences = rhs.GetSentences();

		for (unsigned int i = 0; i < sentences.size(); i++) {
			os << sentences[i]->ToString() << std::endl;
		}

		return os;
	}
}