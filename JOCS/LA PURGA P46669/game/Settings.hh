#ifndef Settings_hh
#define Settings_hh


#include "Structs.hh"


/**
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
   * Returns the number of days a match lasts.
   */
  int num_days () const;

  /**
   * Returns the number rounds each day lasts. It is an even number.
   Half of these rounds are daylight and the other half are night.   
   */
  int num_rounds_per_day () const;

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
   * Returns the initial number of builders.
   */  
  int num_ini_builders() const;

  /**
   * Returns the initial number of warriors.
   */  
  int num_ini_warriors() const;

    /**
   * Returns the initial number of money items.
   */  
  int num_ini_money() const;

    /**
   * Returns the initial number of food items.
   */  
  int num_ini_food() const;

    /**
   * Returns the initial number of guns.
   */
  int num_ini_guns() const;

    /**
   * Returns the initial number of bazookas.
   */  
  int num_ini_bazookas() const;

  
  /**
   * Returns the initial life of a builder. It is also her maximum life.
   */  
  int builder_ini_life() const;

  /**
   * Returns the initial life of a warrior. It is also her maximum life.
   */  
  int warrior_ini_life() const;

  /**
   * Returns the initial life of a citizen type. It is also her maximum life.
   */  
  int citizen_ini_life(CitizenType ct) const;

  /**
   * Returns the number of points that collecting one unit of money gives.
   */  
  int money_points() const;

  /**
   * Returns the number of points that killing a builder gives.
   */  
  int kill_builder_points() const;

  /**
   * Returns the number of points that killing a warrior gives.
   */  
  int kill_warrior_points() const;

  /**
   * Returns the increment of life that eating a unit of food gives.
   */  
  int food_incr_life() const;

  /**
   * Returns the number of life points lost when losing an attack.
   */  
  int life_lost_in_attack() const;

  /**
   * Returns strength of a builder in an attack
   */  
  int builder_strength_attack() const;

  /**
   * Returns strength of a hammer in an attack
   */  
  int hammer_strength_attack() const;
  
  /**
   * Returns strength of a gun in an attack
   */  
  int gun_strength_attack() const;
  
  /**
   * Returns strength of a bazooka in an attack
   */  
  int bazooka_strength_attack() const;

  /**
   * Returns attack strength of a weapon. It returns the attack strength of a builder if w is NoWeapon.
   */  
  int weapon_strength_attack(WeaponType w) const;
  
  /**
   * Returns strength of a builder when demolishing a barricade
   */  
  int builder_strength_demolish() const;

  /**
   * Returns strength of a hammer when demolishing a barricade
   */  
  int hammer_strength_demolish() const;
  
  /**
   * Returns strength of a gun when demolishing a barricade
   */  
  int gun_strength_demolish() const;
  
  /**
   * Returns strength of a bazooka when demolishing a barricade
   */  
  int bazooka_strength_demolish() const;

  /**
   * Returns demolish strength of a weapon. It returns the demolish strength of a builder if w is NoWeapon
   */  
  int weapon_strength_demolish(WeaponType w) const;

  /**
   * Returns the number of rounds to wait for a builder to be regenerated.
   */  
  int num_rounds_regen_builder() const;

  /**
   * Returns the number of rounds to wait for a warrior to be regenerated.
   */  
  int num_rounds_regen_warrior() const;

  /**
   * Returns the number of rounds to wait for a citizen to be regenerated.
   */  
  int num_rounds_regen_citizen(CitizenType ci) const;

  /**
   * Returns the number of rounds to wait for food to be regenerated.
   */  
  int num_rounds_regen_food() const;

  /**
   * Returns the number of rounds to wait for money to be regenerated.
   */  
  int num_rounds_regen_money() const;

  /**
   * Returns the number of rounds to wait for a weapon to be regenerated.
   */  
  int num_rounds_regen_weapon() const;
  
  /**
   * Returns the resistance given to a barricade in a build action.
   */  
  int barricade_resistance_step() const;

  /**
   * Returns the maximum resistance a barricade can achieve.
   */  
  int barricade_max_resistance() const;

  /**
   * Returns the maximum number of barricades each player can have
   */  
  int max_num_barricades () const;

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
  
  /**
   * Returns whether round r is at night.
   */
  bool is_round_night (int r) const;
  
  /**
   * Returns whether round r is day.
   */
  bool is_round_day (int r) const;
  
  //////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////


private:

  friend class Info;
  friend class Board;
  friend class Game;
  friend class SecGame;
  friend class Player;

  int NUM_PLAYERS;
  int NUM_DAYS;
  int NUM_ROUNDS_PER_DAY;
  int BOARD_ROWS;
  int BOARD_COLS;
  int NUM_INI_BUILDERS;
  int NUM_INI_WARRIORS;
  int NUM_INI_MONEY;
  int NUM_INI_FOOD;
  int NUM_INI_GUNS;
  int NUM_INI_BAZOOKAS;
  int BUILDER_INI_LIFE;
  int WARRIOR_INI_LIFE;
  int MONEY_POINTS;
  int KILL_BUILDER_POINTS;
  int KILL_WARRIOR_POINTS;
  int FOOD_INCR_LIFE;

  int LIFE_LOST_IN_ATTACK;
  int BUILDER_STRENGTH_ATTACK;
  int HAMMER_STRENGTH_ATTACK;
  int GUN_STRENGTH_ATTACK;   
  int BAZOOKA_STRENGTH_ATTACK;

  int BUILDER_STRENGTH_DEMOLISH;
  int HAMMER_STRENGTH_DEMOLISH; 
  int GUN_STRENGTH_DEMOLISH;    
  int BAZOOKA_STRENGTH_DEMOLISH;

  int NUM_ROUNDS_REGEN_BUILDER;
  int NUM_ROUNDS_REGEN_WARRIOR;
  int NUM_ROUNDS_REGEN_FOOD;
  int NUM_ROUNDS_REGEN_MONEY;
  int NUM_ROUNDS_REGEN_WEAPON;

  int BARRICADE_RESISTANCE_STEP;
  int BARRICADE_MAX_RESISTANCE;
  int MAX_NUM_BARRICADES;
  
  /**
   * Reads the settings from a stream.
   */
  static Settings read_settings (istream& is);

  bool ok () const;
};

inline string Settings::version () {
  return string(GAME_NAME) + " " + string(VERSION);
}

inline int Settings::num_players                  () const { return NUM_PLAYERS                    ;}
inline int Settings::num_days                     () const { return NUM_DAYS                       ;}
inline int Settings::num_rounds_per_day           () const { return NUM_ROUNDS_PER_DAY             ;}
inline int Settings::num_rounds                   () const { return num_days()*num_rounds_per_day();}
inline int Settings::board_rows                   () const { return BOARD_ROWS                     ;}
inline int Settings::board_cols                   () const { return BOARD_COLS                     ;}
inline int Settings::num_ini_builders             () const { return NUM_INI_BUILDERS               ;}
inline int Settings::num_ini_warriors             () const { return NUM_INI_WARRIORS               ;}
inline int Settings::num_ini_money                () const { return NUM_INI_MONEY                  ;}
inline int Settings::num_ini_food                 () const { return NUM_INI_FOOD                   ;}
inline int Settings::num_ini_guns                 () const { return NUM_INI_GUNS                   ;}
inline int Settings::num_ini_bazookas             () const { return NUM_INI_BAZOOKAS               ;}
inline int Settings::builder_ini_life             () const { return BUILDER_INI_LIFE               ;}
inline int Settings::warrior_ini_life             () const { return WARRIOR_INI_LIFE               ;}
inline int Settings::money_points                 () const { return MONEY_POINTS                   ;}
inline int Settings::kill_builder_points          () const { return KILL_BUILDER_POINTS            ;}
inline int Settings::kill_warrior_points          () const { return KILL_WARRIOR_POINTS            ;}
inline int Settings::food_incr_life               () const { return FOOD_INCR_LIFE                 ;}
inline int Settings::life_lost_in_attack          () const { return LIFE_LOST_IN_ATTACK            ;}
inline int Settings::builder_strength_attack      () const { return BUILDER_STRENGTH_ATTACK        ;}
inline int Settings::hammer_strength_attack       () const { return HAMMER_STRENGTH_ATTACK         ;}
inline int Settings::gun_strength_attack          () const { return GUN_STRENGTH_ATTACK            ;}
inline int Settings::bazooka_strength_attack      () const { return BAZOOKA_STRENGTH_ATTACK        ;}
inline int Settings::builder_strength_demolish    () const { return BUILDER_STRENGTH_DEMOLISH      ;}
inline int Settings::hammer_strength_demolish     () const { return HAMMER_STRENGTH_DEMOLISH       ;}
inline int Settings::gun_strength_demolish        () const { return GUN_STRENGTH_DEMOLISH          ;}
inline int Settings::bazooka_strength_demolish    () const { return BAZOOKA_STRENGTH_DEMOLISH      ;}
inline int Settings::num_rounds_regen_builder     () const { return NUM_ROUNDS_REGEN_BUILDER       ;}
inline int Settings::num_rounds_regen_warrior     () const { return NUM_ROUNDS_REGEN_WARRIOR       ;}
inline int Settings::num_rounds_regen_food        () const { return NUM_ROUNDS_REGEN_FOOD          ;}
inline int Settings::num_rounds_regen_money       () const { return NUM_ROUNDS_REGEN_MONEY         ;}
inline int Settings::num_rounds_regen_weapon      () const { return NUM_ROUNDS_REGEN_WEAPON        ;}
inline int Settings::barricade_resistance_step    () const { return BARRICADE_RESISTANCE_STEP      ;}
inline int Settings::barricade_max_resistance     () const { return BARRICADE_MAX_RESISTANCE       ;}
inline int Settings::max_num_barricades           () const { return MAX_NUM_BARRICADES             ;}

inline int Settings::citizen_ini_life          (CitizenType ct) const {
  switch(ct) {
  case Builder: return builder_ini_life(); break;
  case Warrior: return warrior_ini_life(); break;
  default: return -1;    
  }
}

inline int Settings::weapon_strength_attack (WeaponType w) const {
  switch(w) {
  case Hammer:   return hammer_strength_attack();  break;
  case Gun:      return gun_strength_attack();     break;
  case Bazooka:  return bazooka_strength_attack(); break;
  case NoWeapon: return builder_strength_attack(); break;
  default: return -1;
  }
}

inline int Settings::weapon_strength_demolish (WeaponType w) const {
  switch(w) {
  case Hammer:   return hammer_strength_demolish();  break;
  case Gun:      return gun_strength_demolish();     break;
  case Bazooka:  return bazooka_strength_demolish(); break;
  case NoWeapon: return builder_strength_demolish(); break;
  default: return -1;
  }
}

inline int Settings::num_rounds_regen_citizen(CitizenType ci) const {
  switch(ci) {
  case Builder:   return num_rounds_regen_builder();  break;
  case Warrior:   return num_rounds_regen_warrior();  break;
  default: return -1;
  }
}

inline bool Settings::player_ok (int pl) const {
  return pl >= 0 and pl < num_players();
}

inline bool Settings::pos_ok (int i, int j) const {
  return i >= 0 and i < board_rows() and j >= 0 and j < board_cols();
}

inline bool Settings::pos_ok (Pos p) const {
  return pos_ok(p.i, p.j);
}

inline bool Settings::is_round_day (int r) const {
  return r%num_rounds_per_day() < num_rounds_per_day()/2;
}

inline bool Settings::is_round_night (int r) const {
  return not is_round_day(r);
}

inline bool Settings::ok() const {

  if (builder_strength_demolish() > hammer_strength_demolish()) return false;
  if (hammer_strength_demolish() > gun_strength_demolish()) return false;
  if (gun_strength_demolish() > bazooka_strength_demolish()) return false;

  if (builder_strength_attack() > hammer_strength_attack()) return false;
  if (hammer_strength_attack() > gun_strength_attack()) return false;
  if (gun_strength_attack() > bazooka_strength_attack()) return false;

  if (barricade_resistance_step() > barricade_max_resistance()) return false;

  return true;
  
}
#endif
