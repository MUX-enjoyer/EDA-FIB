#ifndef Settings_hh
#define Settings_hh


#include "Structs.hh"


/** \file
 * Contains a class to store all the game settings that do not change
 * during a game, except the names of the players.
 */


/**
 * Stores most of the game settings.
 */
class Settings {

public:
    /**
   * Returns a string with the game name and version.
   */
  static string version ();

  /**
   * Returns the number of players in the game.
   */
  int num_players () const;

  /**
   * Returns the number of rounds a match lasts.
   */
  int num_rounds () const;

  /**
   * Returns the number of rows of the board.
   */
  int board_rows () const;

  /**
   * Returns the number of columns of the board.
   */
  int board_cols () const;

  /**
   * Returns the initial number of soldiers per clan
   */
  int num_ini_soldiers_per_clan () const;
  
  /**
   * Returns the initial number of poison units per soldier
   */
  int soldier_ini_poisons () const;

  /**
   * Returns the maximum amount of poisons per soldier
   */
  int soldier_max_poisons ( ) const;

  /**
   * Returns the points obtained por killing a soldier
   */
  int points_for_killing_soldier () const;

  /**
   * Returns the maximum points of health that a professor can have
   */
  int professor_max_health () const;

  /**
   * Returns the initial points of health of a professor (after reborn or at the start of the game)
   */
  int professor_ini_health () const;

  /**
   * Returns the number of points of health that are decremented when a professor is thrown poison at
   */
  int professor_health_decrease () const;

  /**
   * Returns the points obtained por killing a professor
   */
  int points_for_killing_professor () const;

  /**
   * Returns the number of rounds needed for a professor to be reborn after it is dead
   */
  int professor_rounds_rebirth () const;

  /**
   * Returns the number of money items in the board
   */
  int num_money () const;

  /** 
   * Returns the minimum number of points the a money item can give.
   */ 
  int min_money_value () const;

  /**
   * Returns the maximum number of points the a money item can give.
   */
  int max_money_value () const;

  /**
   * Returns the number of health kits in the board
   */
  int num_health_kits () const;

  /**
   * Returns the number of health points gained if a kit value is collected
   */
  int health_kit_value () const;

  /**
   * Returns the number of poison items in the board
   */  
  int num_poison_kits () const;

  /**
   * Returns the number of poison units gained if a poison kit is collected
   */
  int poison_kit_value () const;

  /**
   * Returns the number of safety boxes in the board
   */  
  int num_safety_boxes () const;

    /**
   * Returns the points given if the gold inside a safety box is collected
   */  
  int gold_value () const;

  /**
   * Returns the size of the hints of any safety box
   */  
  int size_hints_per_box () const;

  
  /**
   * Returns whether pl is a valid player identifier.
   */
  bool player_ok (int pl) const;

  /**
   * Returns whether (i, j) is a position inside the board.
   */
  bool pos_ok (int i, int j) const;

  /**
   * Returns whether p is a position inside the board.
   */
  bool pos_ok (Pos p) const;

private:
  
  friend class Info;
  friend class Board;
  friend class Game;
  friend class SecGame;
  friend class Player;

  int NUM_PLAYERS;
  int NUM_ROUNDS;
  int BOARD_ROWS;
  int BOARD_COLS;
  
  int INITIAL_SOLDIERS_PER_CLAN;
  int INITIAL_SOLDIERS_POISONS;
  int SOLDIER_MAX_POISONS;
  int POINTS_FOR_KILLING_SOLDIER;

  int PROFESSOR_MAX_HEALTH;
  int INITIAL_PROFESSOR_HEALTH;
  int PROFESSOR_HEALTH_DECREASE;
  int POINTS_FOR_KILLING_PROFESSOR;
  int PROFESSOR_ROUNDS_REBIRTH;
  
  int NUM_MONEY;
  int MIN_MONEY_VALUE;
  int MAX_MONEY_VALUE;

  int NUM_HEALTH_KITS;
  int HEALTH_KIT_VALUE;

  int NUM_POISON_KITS;
  int POISON_KIT_VALUE;

  int NUM_SAFETY_BOXES;
  int GOLD_VALUE;
  int SIZE_HINTS_PER_BOX;
  
  /**
   * Reads the settings from a stream.
   */
  static Settings read_settings (istream& is);

  
  bool ok () const;
};


inline string Settings::version () {
  return "MoneyHeist 1.0";
}

inline int Settings::num_players                  () const { return NUM_PLAYERS                    ;}
inline int Settings::num_rounds                   () const { return NUM_ROUNDS                     ;}
inline int Settings::board_rows                   () const { return BOARD_ROWS                     ;}
inline int Settings::board_cols                   () const { return BOARD_COLS                     ;}

inline int Settings::num_ini_soldiers_per_clan    () const { return INITIAL_SOLDIERS_PER_CLAN      ;}
inline int Settings::soldier_ini_poisons          () const { return INITIAL_SOLDIERS_POISONS       ;}
inline int Settings::soldier_max_poisons          () const { return SOLDIER_MAX_POISONS       ;}
inline int Settings::points_for_killing_soldier   () const { return POINTS_FOR_KILLING_SOLDIER     ;}

inline int Settings::professor_max_health         () const { return PROFESSOR_MAX_HEALTH           ;}
inline int Settings::professor_ini_health         () const { return INITIAL_PROFESSOR_HEALTH       ;}
inline int Settings::professor_health_decrease    () const { return PROFESSOR_HEALTH_DECREASE      ;}
inline int Settings::points_for_killing_professor () const { return POINTS_FOR_KILLING_PROFESSOR   ;}
inline int Settings::professor_rounds_rebirth     () const { return PROFESSOR_ROUNDS_REBIRTH       ;}

inline int Settings::num_money                    () const { return NUM_MONEY                      ;}
inline int Settings::min_money_value              () const { return MIN_MONEY_VALUE                ;}
inline int Settings::max_money_value              () const { return MAX_MONEY_VALUE                ;}

inline int Settings::num_health_kits              () const { return NUM_HEALTH_KITS                ;}
inline int Settings::health_kit_value             () const { return HEALTH_KIT_VALUE               ;}

inline int Settings::num_poison_kits              () const { return NUM_POISON_KITS                ;}
inline int Settings::poison_kit_value             () const { return POISON_KIT_VALUE               ;}

inline int Settings::num_safety_boxes             () const { return NUM_SAFETY_BOXES               ;}
inline int Settings::gold_value                   () const { return GOLD_VALUE                     ;}
inline int Settings::size_hints_per_box           () const { return SIZE_HINTS_PER_BOX              ;}

inline bool Settings::player_ok (int pl) const {
  return pl >= 0 and pl < num_players();
}

inline bool Settings::pos_ok (int i, int j) const {
  return i >= 0 and i < board_rows() and j >= 0 and j < board_cols();
}

inline bool Settings::pos_ok (Pos p) const {
  return pos_ok(p.i, p.j);
}


inline bool Settings::ok() const {
  if (num_players() != 4) return false;
  return true;
}

#endif
