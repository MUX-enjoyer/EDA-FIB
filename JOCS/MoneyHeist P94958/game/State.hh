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
   * Returns a copy of the cell at (i, j).
   */
  Cell cell (int i, int j) const;

  /**
   * Returns a copy of the cell at p.
   */
  Cell cell (Pos p) const;

  /**
   * Returns the a copy of the unit with identifier id.
   */
  Unit unit (int id) const;

  /**
   * Returns the ids of the soldiers of a player
   */
  vector<int> soldiers (int pl) const;

  /**
   * Returns the id of the professor of a player
   */
  int professor (int pl) const;
  
  /**
   * Returns the current available poison of a player
   */
  int available_poison (int pl) const;

  /**
   * Returns the current available health points of a player
   */
  int available_health (int pl) const;

  /**
   * Returns the current score of a player.
   */
  int score (int pl) const;

  /*
   * Returns the hints of safety box with identifier id_box (note that 0 <= id_box < num_safety_boxes() )
   */
  vector<int> safety_box_hints (int id_box) const;
  
  /**
   * Returns the percentage of cpu time used so far, in the
   * range [0.0 --- 1.0] or a value lesser than 0 if the player is dead.
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
  vector<int>              player_health; // total health kits owned by player
  vector<int>              player_poison; // total poision owned by player

  vector<double>           stats; // -1 -> dead, 0..1 -> % of cpu time limit
  int                      rnd;
  
  vector<Unit>             units; // from 0 to 4 + 4*INITIAL_SOLDIERS_PER_CLAN - 1
  vector< set<int> >       player2soldiers;
  vector<int>              player2professor;
  
  vector<vector<int>>      hints_boxes;
  vector<Pos>              pos_boxes;

  
  /**
   * Returns whether id is a valid unit identifier.
   */
  inline bool unit_ok (int id) const {
    return 0 <= id and id < int(units.size());
  }

};

inline int State::round () const {
  return rnd;
}

inline Cell State::cell (int i, int j) const {
  if (i >= 0 and i < (int)grid.size() and j >= 0 and j < (int)grid[i].size())
    return grid[i][j];
  else {
    cerr << "warning: cell requested for position " << Pos(i, j) << endl;
    return Cell();
  }
}

inline Cell State::cell (Pos p) const {
  return cell(p.i, p.j);
}

inline Unit State::unit (int id) const {
  if (not unit_ok(id)) {
    cerr << "warning: unit requested for identifier " << id << endl;
    return Unit();
  }
  else return units[id];
}

inline vector<int> State::soldiers (int pl) const {
  if (pl >= 0 and pl < (int) player2soldiers.size())
    return vector<int>(player2soldiers[pl].begin(), player2soldiers[pl].end());
  else {
    cerr << "warning: soldiers requested for player " << pl << endl;
    return vector<int>();
  }
}

inline int State::professor (int pl) const {
  if (pl >= 0 and pl < int(player2professor.size()))
    return player2professor[pl];
  else {
    cerr << "warning: professor requested for player " << pl << endl;
    return -1;
  }
}

inline int State::available_poison (int pl) const {
  if (pl >= 0 and pl < (int) player_poison.size()) {
    return player_poison[pl];
  }
  else {
    cerr << "warning: available_poison requested for player " << pl << endl;
    return -1;
  }
}

inline int State::available_health (int pl) const {
  if (pl >= 0 and pl < (int) player_health.size()) {
    return player_health[pl];
  }
  else {
    cerr << "warning: available_health requested for player " << pl << endl;
    return -1;
  }
}

inline int State::score (int pl) const {
  if (pl >= 0 and pl < (int) scr.size())
    return scr[pl];
  else {
    cerr << "warning: score requested for player " << pl << endl;
    return -1;
  }
}


inline double State::status (int pl) const {
  if (pl >= 0 and pl < (int)stats.size())
    return stats[pl];
  else {
    cerr << "warning: status requested for player " << pl << endl;
    return -2;
  }
}

inline vector<int> State::safety_box_hints (int id_box) const {
  if (id_box >= 0 and id_box < int(hints_boxes.size())) {
    return hints_boxes[id_box];
  }
  else {
    cerr << "warning: safety_box_hints requested for box id " << id_box << endl;
    return {};
  }
}

#endif
