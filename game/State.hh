#ifndef State_hh
#define State_hh


#include "Structs.hh"
#include "Settings.hh"

/**
 * Contains a class to store the current state of a game.
 */


/**
 * Stores the game state.
 */
class State {

public:

  /**
   * Returns the current round.
   */
  int round () const;

  /**
   * Returns whether the current round is night.
   */
  bool is_night () const;

  /**
   * Returns whether the current round is day.
   */
  bool is_day () const;

  /**
   * Returns a copy of the cell at (i, j).
   */
  Cell cell (int i, int j) const;

  /**
   * Returns a copy of the cell at p.
   */
  Cell cell (Pos p) const;

  /**
   * Returns the citizen with identifier id.
   */
  Citizen citizen (int id) const;

  /**
   * Returns the ids of the builders of a player.
   */
  vector<int> builders(int pl) const;

  /**
   * Returns the ids of the warriors of a player.
   */
  vector<int> warriors(int pl) const;


  /**
   * Returns the positions of the barricades owned by a player.
   */
  vector<Pos> barricades(int pl) const;

  /**
   * Returns the current score of a player.
   */
  int score (int pl) const;

  /**
   * Returns the percentage of cpu time used so far, in the
   * range [0.0 - 1.0] or a value lesser than 0 if the player is dead.
   */
   // NOTE: only returns a sensible value in server executions.
   // In local executions the returned value is meaningless.
  double status (int pl) const;


  //////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////  

private:

  friend class Info;
  friend class Board;
  friend class Game;
  friend class SecGame;
  friend class Player;

  vector< vector<Cell> >   grid;
  
  vector<int>              scr; // score of each player
  vector<double>           stats; // -1 -> dead, 0..1 -> % of cpu time limit
  int                      rnd;
  bool                     day;
  
  map<int, Citizen>        citizens;
  vector< set<int> >       player2builders;
  vector< set<int> >       player2warriors;
  vector< set<Pos> >       player2barricades;
  /**
   * Returns whether id is a valid citizen identifier.
   */
  inline bool citizen_ok (int id) const {
    return citizens.count(id);
  }
};

inline int State::round () const {
  return rnd;
}

inline bool State::is_day () const {
  return day;
}

inline bool State::is_night () const {
  return not day;
}

inline Cell State::cell (int i, int j) const {
  if (i >= 0 and i < (int)grid.size() and j >= 0 and j < (int)grid[i].size())
    return grid[i][j];
  else {
    //cerr << "warning: cell requested for position " << Pos(i, j) << endl;
    return Cell();
  }
}

inline Cell State::cell (Pos p) const {
  return cell(p.i, p.j);
}

inline Citizen State::citizen (int id) const {
  auto it = citizens.find(id);
  if (it != citizens.end()) {
    return it->second;
  }
  else {
    //cerr << "warning: citizen requested for identifier " << id << endl;
    return Citizen();
  }
}

inline vector<int> State::builders(int pl) const {
  if (pl >= 0 and pl < (int) player2builders.size())
    return vector<int>(player2builders[pl].begin(), player2builders[pl].end());
  else {
    //cerr << "warning: builders requested for player " << pl << endl;
    return vector<int>();
  }
}


inline vector<int> State::warriors(int pl) const {
  if (pl >= 0 and pl < (int) player2warriors.size())
    return vector<int>(player2warriors[pl].begin(), player2warriors[pl].end());
  else {
    //cerr << "warning: warriors requested for player " << pl << endl;
    return vector<int>();
  }
}

inline vector<Pos> State::barricades(int pl) const {
  if (pl >= 0 and pl < (int) player2barricades.size())
    return vector<Pos>(player2barricades[pl].begin(), player2barricades[pl].end());
  else {
    //cerr << "warning: barricades requested for player " << pl << endl;
    return vector<Pos>();
  }
}

inline int State::score (int pl) const {
  if (pl >= 0 and pl < (int) scr.size())
    return scr[pl];
  else {
    //cerr << "warning: score requested for player " << pl << endl;
    return -1;
  }
}


inline double State::status (int pl) const {
  if (pl >= 0 and pl < (int)stats.size())
    return stats[pl];
  else {
    //cerr << "warning: status requested for player " << pl << endl;
    return -2;
  }
}

#endif
