#include "Settings.hh"


Settings Settings::read_settings (istream& is) {
  Settings r;
  string s, v;

  // Version, compared part by part.
  istringstream vs(version());
  while (!vs.eof()) {
    is >> s;
    vs >> v;
    assert(s == v);
  };
  
  is >> s >> r.NUM_PLAYERS;
  _my_assert(s == "NUM_PLAYERS", "Expected 'NUM_PLAYERS' while parsing.");
  _my_assert(r.NUM_PLAYERS == 4, "NUM_PLAYERS should be 4.");

  is >> s >> r.NUM_ROUNDS;
  _my_assert(s == "NUM_ROUNDS", "Expected 'NUM_ROUNDS' while parsing.");
  _my_assert(r.NUM_ROUNDS >= 1,   "NUM_ROUNDS should be >=1.");

  is >> s >> r.BOARD_ROWS;
  _my_assert(s == "BOARD_ROWS", "Expected 'BOARD_ROWS' while parsing.");
  _my_assert(r.BOARD_ROWS == 60, "BOARD_COLS should be 60.");
  
  is >> s >> r.BOARD_COLS;
  _my_assert(s == "BOARD_COLS", "Expected 'BOARD_COLS' while parsing.");
  _my_assert(r.BOARD_COLS == 60, "BOARD_COLS should be 60.");

  is >> s >> r.INITIAL_SOLDIERS_PER_CLAN;
  _my_assert(s == "INITIAL_SOLDIERS_PER_CLAN", "Expected 'INITIAL_SOLDIERS_PER_CLAN' while parsing.");
  _my_assert(r.INITIAL_SOLDIERS_PER_CLAN >= 1, "INITIAL_SOLDIERS_PER_CLAN should be >= 1.");

  is >> s >> r.INITIAL_SOLDIERS_POISONS;
  _my_assert(s == "INITIAL_SOLDIERS_POISONS", "Expected 'INITIAL_SOLDIERS_POISONS' while parsing.");
  _my_assert(r.INITIAL_SOLDIERS_POISONS >= 1, "INITIAL_SOLDIERS_POISONS should be >= 1.");

  is >> s >> r.SOLDIER_MAX_POISONS;
  _my_assert(s == "SOLDIER_MAX_POISONS", "Expected 'SOLDIER_MAX_POISONS' while parsing.");
  _my_assert(r.SOLDIER_MAX_POISONS >= 1, "SOLDIER_MAX_POISONS should be >= 1.");

    is >> s >> r.POINTS_FOR_KILLING_SOLDIER;
  _my_assert(s == "POINTS_FOR_KILLING_SOLDIER", "Expected 'POINTS_FOR_KILLING_SOLDIER' while parsing.");
  _my_assert(r.POINTS_FOR_KILLING_SOLDIER >= 1, "POINTS_FOR_KILLING_SOLDIER should be >= 1.");

    is >> s >> r.PROFESSOR_MAX_HEALTH;
  _my_assert(s == "PROFESSOR_MAX_HEALTH", "Expected 'PROFESSOR_MAX_HEALTH' while parsing.");
  _my_assert(r.PROFESSOR_MAX_HEALTH >= 1, "PROFESSOR_MAX_HEALTH should be >= 1.");

  is >> s >> r.INITIAL_PROFESSOR_HEALTH;
  _my_assert(s == "INITIAL_PROFESSOR_HEALTH", "Expected 'INITIAL_PROFESSOR_HEALTH' while parsing.");
  _my_assert(r.INITIAL_PROFESSOR_HEALTH >= 1, "INITIAL_PROFESSOR_HEALTH should be >= 1.");

    is >> s >> r.PROFESSOR_HEALTH_DECREASE;
  _my_assert(s == "PROFESSOR_HEALTH_DECREASE", "Expected 'PROFESSOR_HEALTH_DECREASE' while parsing.");
  _my_assert(r.PROFESSOR_HEALTH_DECREASE >= 1, "PROFESSOR_HEALTH_DECREASE should be >= 1.");

    is >> s >> r.POINTS_FOR_KILLING_PROFESSOR;
  _my_assert(s == "POINTS_FOR_KILLING_PROFESSOR", "Expected 'POINTS_FOR_KILLING_PROFESSOR' while parsing.");
  _my_assert(r.POINTS_FOR_KILLING_PROFESSOR >= 1, "POINTS_FOR_KILLING_PROFESSOR should be >= 1.");

    is >> s >> r.PROFESSOR_ROUNDS_REBIRTH;
  _my_assert(s == "PROFESSOR_ROUNDS_REBIRTH", "Expected 'PROFESSOR_ROUNDS_REBIRTH' while parsing.");
  _my_assert(r.PROFESSOR_ROUNDS_REBIRTH >= 1, "PROFESSOR_ROUNDS_REBIRTH should be >= 1.");

    is >> s >> r.NUM_MONEY;
  _my_assert(s == "NUM_MONEY", "Expected 'NUM_MONEY' while parsing.");
  _my_assert(r.NUM_MONEY >= 1, "NUM_MONEY should be >= 1.");

    is >> s >> r.MIN_MONEY_VALUE;
  _my_assert(s == "MIN_MONEY_VALUE", "Expected 'MIN_MONEY_VALUE' while parsing.");
  _my_assert(r.MIN_MONEY_VALUE >= 1, "MIN_MONEY_VALUE should be >= 1.");

    is >> s >> r.MAX_MONEY_VALUE;
  _my_assert(s == "MAX_MONEY_VALUE", "Expected 'MAX_MONEY_VALUE' while parsing.");
  _my_assert(r.MAX_MONEY_VALUE >= 1, "MAX_MONEY_VALUE should be >= 1.");
  _my_assert(r.MAX_MONEY_VALUE >= r.MIN_MONEY_VALUE, "MAX_MONEY_VALUE should be >= MIN_MONEY_VALUE.");

  is >> s >> r.NUM_HEALTH_KITS;
  _my_assert(s == "NUM_HEALTH_KITS", "Expected 'NUM_HEALTH_KITS' while parsing.");
  _my_assert(r.NUM_HEALTH_KITS >= 1, "NUM_HEALTH_KITS should be >= 1.");

    is >> s >> r.HEALTH_KIT_VALUE;
  _my_assert(s == "HEALTH_KIT_VALUE", "Expected 'HEALTH_KIT_VALUE' while parsing.");
  _my_assert(r.HEALTH_KIT_VALUE >= 1, "HEALTH_KIT_VALUE should be >= 1.");

  is >> s >> r.NUM_POISON_KITS;
  _my_assert(s == "NUM_POISON_KITS", "Expected 'NUM_POISON_KITS' while parsing.");
  _my_assert(r.NUM_POISON_KITS >= 1, "NUM_POISON_KITS should be >= 1.");

  is >> s >> r.POISON_KIT_VALUE;
  _my_assert(s == "POISON_KIT_VALUE", "Expected 'POISON_KIT_VALUE' while parsing.");
  _my_assert(r.POISON_KIT_VALUE >= 1, "POISON_KIT_VALUE should be >= 1.");

  is >> s >> r.NUM_SAFETY_BOXES;
  _my_assert(s == "NUM_SAFETY_BOXES", "Expected 'NUM_SAFETY_BOXES' while parsing.");
  _my_assert(r.NUM_SAFETY_BOXES >= 1, "NUM_SAFETY_BOXES should be >= 1.");

  is >> s >> r.GOLD_VALUE;
  _my_assert(s == "GOLD_VALUE", "Expected 'GOLD_VALUE' while parsing.");
  _my_assert(r.GOLD_VALUE > r.MAX_MONEY_VALUE, "GOLD_VALUE should be >= MAX_MONEY_VALUE.");

  is >> s >> r.SIZE_HINTS_PER_BOX;
  _my_assert(s == "SIZE_HINTS_PER_BOX", "Expected 'SIZE_HINTS_PER_BOX' while parsing.");
  _my_assert(r.SIZE_HINTS_PER_BOX %3 == 0, "SIZE_HINTS_PER_BOX should be multiple of 3.");

  return r;
}
