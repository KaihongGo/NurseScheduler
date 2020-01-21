//
//  Nurse.h
//  RosterDesNurses
//

#ifndef __Nurse__
#define __Nurse__

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "MyTools.h"
#include "Demand.h"

using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;


//-----------------------------------------------------------------------------
//
//  C l a s s   C o n t r a c t
//
//  A contract as defined in the subject
//
//-----------------------------------------------------------------------------
class Contract{

public:
   //Id of the contract and index in the vector intToContract_
   //
   const int id_;

   // Name of the contract
   //
   const string name_;

   // Minimum and maximum total number of shifts over the time period
   //
   const int minTotalShifts_, maxTotalShifts_;

   // Minimum and maximum number of consecutive days worked
   //
   const int minConsDaysWork_, maxConsDaysWork_;

   // Minimum and maximum number of consecutive days off
   //
   const int minConsDaysOff_, maxConsDaysOff_;

   // Maximum number of weekends worked, and complete weekend constraint
   //
   const int maxTotalWeekends_;
   const int needCompleteWeekends_;

   // Constructor and Destructor
   //
   Contract(int id, string name, int minTotalShifts, int maxTotalShifts,
      int minConsDaysWork, int maxConsDaysWork,
      int minConsDaysOff, int maxConsDaysOff,
      int maxTotalWeekends, int needCompleteWeekends) :
         id_(id), name_(name), minTotalShifts_(minTotalShifts), maxTotalShifts_(maxTotalShifts),
         minConsDaysWork_(minConsDaysWork), maxConsDaysWork_(maxConsDaysWork),
         minConsDaysOff_(minConsDaysOff), maxConsDaysOff_(maxConsDaysOff),
         maxTotalWeekends_(maxTotalWeekends), needCompleteWeekends_(needCompleteWeekends) {
   };

   // Display methods: toString + override operator<< (easier)
   //
   string toString();
   friend std::ostream& operator<< (std::ostream& outs, Contract obj) {return outs << obj.toString();}
};


//-----------------------------------------------------------------------------
//
//  C l a s s   P o s i t i o n
//
//  A position (job) is a set of skills that a nurse may possess
//
//-----------------------------------------------------------------------------

class Position{

public:
   // Constructor and Destructor
   //
   Position(int index, int nbSkills, vector<int> skills);

   ~Position() {}

public:
   // Index of the position
   //
   const int id_;

   // Number of skills
   //
   const int nbSkills_;

   // Vector of skills for this position.
   // For simplicity, the skill indices are sorted.
   //
   const vector<int> skills_;

private:
   // Positions that are below and above this one in the hierarchy
   // this is deduced from the dominance criterion implemented in compare()
   //
   vector<Position*> positionsBelow_;
   vector<Position*> positionsAbove_;
   int nbBelow_, nbAbove_;

   // Rarity of the skills that appear in this position
   //
   vector<double> skillRarity_;

   // Rank of the position with regard to the dominance criterion in compare()
   // rank i contains all the positions that are dominated only by positions
   // with a rank smaller than i (the smaller rank is 0)
   //
   int rank_;

public:

  // basic getters
  //
  int id() {return id_;}
  int nbSkills() {return nbSkills_;}
  int skill(int sk) {return skills_[sk];}
  vector<int> skills() {return skills_;}
  int nbBelow() {return nbBelow_;}
  int nbAbove() {return nbAbove_;}
  Position* positionsBelow(int i) {return positionsBelow_[i];}
  Position* positionsAbove(int i) {return positionsAbove_[i];}
  double skillRarity(int sk) {return skillRarity_[sk];}
  int rank() {return rank_;}

  // basic setters
  //
  void rank(int i) {rank_ = i;}

	// Display method: toString
	//
	string toString() const;

	// Compare this position with the input position
	// The dominance criterion is that a position p1 with skills sk1 dominates p2
	// with skills sk2 if and only if (sk1 contains sk2) and sk1 has more skills
	// than sk2
	// The function returns 1 if this position dominates, -1 if it is dominated
	// and 0 if there is no dominance
	//
	int compare(const Position &p);

	// returns true if the position shares at least one skill with the input position
	//
	bool shareSkill(const Position&p);

	// set positions above and below
	//
	void addBelow(Position* pPosition);
	void addAbove(Position* pPosition);

	// reset the list of positions below and above
	//
	void resetAbove();
	void resetBelow();

	// update the rarity of the skills
	// the input is the vector of the rarity of all the skills
	// the vector is sorted without record of the corresponding skill because it
	// is used only to compare two positions with the same rank
	//
	void updateRarities(vector<double> allRarities);

};



//-----------------------------------------------------------------------------
//
//  C l a s s   N u r s e
//
//  Class that contains all the attributes describing the characteristics and
//  the planning of each nurse
//
//-----------------------------------------------------------------------------

class Nurse {

public:

   // Constructor and destructor
   // Note : need both with const Contract and (non-const) Contract because non-const is used in our code,
   //        and const is needed so that we can override the operator= and have vector<Nurse>. We need to
   //        override it because vector members should have some properties (assignable a.o., which implies
   //        non-const)
   //
   Nurse(int id, string name, int nbSkills, vector<int> skills, Contract* contract);
   Nurse(int id, string name, int nbSkills, vector<int> skills, const Contract* contract);
   ~Nurse();


   // the constant attibutes of the nurses are public
public:

   //-----------------------------------------------------------------------------
   // Constant characteristics of the nurses (no set method)
   //-----------------------------------------------------------------------------
   // Id of the nurse (=entry in the vector<Nurse> theNurse of the Scenario)
   //
   const int id_;

   // name of the nurse
   //
   const std::string name_;

   // number of skills and vector of the skills indices
   // for simplicity, the vector of skills is sorted
   //
   const int nbSkills_;
   const vector<int> skills_;

   // Her contract type
   //
   const Contract* pContract_;

protected:
   //-----------------------------------------------------------------------------
   // Other constant characteristics of the nurses that could not be set in the
   // constructor
   // (only getters for these fields)
   //-----------------------------------------------------------------------------

public:
   // Basic getters
   //
   int minTotalShifts() const  {return pContract_->minTotalShifts_;}
   int maxTotalShifts() const {return pContract_->maxTotalShifts_;}
   int minConsDaysWork() const {return pContract_->minConsDaysWork_;}
   int maxConsDaysWork() const {return pContract_->maxConsDaysWork_;}
   int minConsDaysOff() const {return pContract_->minConsDaysOff_;}
   int maxConsDaysOff() const {return pContract_->maxConsDaysOff_;}
   int maxTotalWeekends() const {return pContract_->maxTotalWeekends_;}
   int needCompleteWeekends() const {return pContract_->needCompleteWeekends_;}

   // Avanced getters
   //
   bool hasSkill(int skill) const;
   string contractName() {return pContract_->name_;}

   // Display methods: toString
   //
   string toString() const;

   // Assignment (requested to build a vector<Nurse>)
   //
   Nurse& operator=(const Nurse& n);
};

//-----------------------------------------------------------------------------
//
//  C l a s s   P r e f e r e n c e s
//
//  Describes the preferences of a nurse for a certain period of time
//  They are given as a vector (entry = nurseId).
//  Each element is a map<int,set<int>> whose keys are the days, and values are the sets of wished shift(s) OFF on that day.
//
//-----------------------------------------------------------------------------
class Preferences{

public:
	// Constructor and destructor
	Preferences();
	~Preferences();

	// Constructor with initialization to a given number of nurses
	Preferences(int nbNurses, int nbDays, int nbShifts);

	// Initialization with a map corresponding to the input nurses and no wished Shift-Off.
	Preferences(vector<Nurse>& pNurses, int nbDays, int nbShifts);

protected:
	// Number of nurses
	//
	int nbNurses_;

	// Number of days considered in that case
	//
	int nbDays_;

	// Total number of possible shifts
	//
	int nbShifts_;

	// For each nurse, maps the day to the set of shifts that he/she wants to have off
	//
	map<int, map<int,std::set<int> > > wishesOff_;

public:

	// For a given day, and a given shift, adds it to the wish-list for OFF-SHIFT
	void addShiftOff(int nurse, int day, int shift);

	// Adds the whole day to the wish-list
	void addDayOff(int nurse, int day);

	map<int,std::set<int> >* nurseWishesOff(int id) {return &wishesOff_[id];}

	// True if the nurses wants that shift off
	bool wantsTheShiftOff(int nurse, int day, int shift);

	// True if the nurses wants the whole day off
	bool wantsTheDayOff(int nurse, int day);

	// Total number of shifts off that the nurse wants
	int howManyShiftsOff(int nurse);

	// Number of whole days off that the nurse wants
	int howManyDaysOff(int nurse, int dayMin, int dayMax);

	// add another week preferences at the end of the current one
	//
	void push_back(Preferences* pDemand);

	// Keep the preferences relative to the days in [begin,end)
	Preferences* keep(int begin, int end);

	// Remove the preferences relative to the nbDays first days
	Preferences* removeNFirstDay(int nbDays);


	// Display methods: toString + override operator<< (easier)
	//
	string toString();
	friend std::ostream& operator<< (std::ostream& outs, Preferences obj) {return outs << obj.toString();}
};

#endif
