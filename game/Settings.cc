//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////  

#include "Settings.hh"

Settings Settings::read_settings (istream& is) {
  Settings r;
  string s, v;

  // Version, compared part by part.
  istringstream vs(version());
  while (!vs.eof()) {
    is >> s;
    vs >> v;
    _my_assert(s == v, "Problems when reading.");
  };

  is >> s >> r.NUM_PLAYERS;
  _my_assert(s == "NUM_PLAYERS", "Expected 'NUM_PLAYERS' while parsing.");
  _my_assert(r.NUM_PLAYERS == 4, "Wrong NUM_PLAYERS.");

  is >> s >> r.NUM_DAYS;
  _my_assert(s == "NUM_DAYS", "Expected 'NUM_DAYS' while parsing.");
  _my_assert(r.NUM_DAYS >= 1, "Wrong NUM_DAYS.");
  
  is >> s >> r.NUM_ROUNDS_PER_DAY;
  _my_assert(s == "NUM_ROUNDS_PER_DAY", "Expected 'NUM_ROUNDS_PER_DAY' while parsing.");
  _my_assert(r.NUM_ROUNDS_PER_DAY >= 1,   "Wrong NUM_ROUNDS_PER_DAY.");
  _my_assert(r.NUM_ROUNDS_PER_DAY%2 == 0, "Wrong NUM_ROUNDS_PER_DAY.");  

  is >> s >> r.BOARD_ROWS;
  _my_assert(s == "BOARD_ROWS", "Expected 'BOARD_ROWS' while parsing.");
  _my_assert(r.BOARD_ROWS >= 12 and r.BOARD_ROWS <= 25, "BOARD_ROWS should be in [12,25].");

  is >> s >> r.BOARD_COLS;
  _my_assert(s == "BOARD_COLS", "Expected 'BOARD_COLS' while parsing.");
  _my_assert(r.BOARD_COLS >= 12 and r.BOARD_COLS <= 50, "BOARD_COLS should be in [12,50].");

  is >> s >> r.NUM_INI_BUILDERS;
  _my_assert(s == "NUM_INI_BUILDERS", "Expected 'NUM_INI_BUILDERS' while parsing.");
  _my_assert(r.NUM_INI_BUILDERS >= 1 and r.NUM_INI_BUILDERS <= 6 , "Wrong NUM_INI_BUILDERS.");

  is >> s >> r.NUM_INI_WARRIORS;
  _my_assert(s == "NUM_INI_WARRIORS", "Expected 'NUM_INI_WARRIORS' while parsing.");
  _my_assert(r.NUM_INI_WARRIORS >= 1 and r.NUM_INI_WARRIORS <= 4, "Wrong NUM_INI_WARRIORS.");
  
  is >> s >> r.NUM_INI_MONEY;
  _my_assert(s == "NUM_INI_MONEY", "Expected 'NUM_INI_MONEY' while parsing.");
  _my_assert(r.NUM_INI_MONEY >= 0 and r.NUM_INI_MONEY <= 10, "Wrong NUM_INI_MONEY.");
  
  is >> s >> r.NUM_INI_FOOD;
  _my_assert(s == "NUM_INI_FOOD", "Expected 'NUM_INI_FOOD' while parsing.");
  _my_assert(r.NUM_INI_FOOD >= 0 and r.NUM_INI_FOOD <= 10, "Wrong NUM_INI_FOOD.");
  
  is >> s >> r.NUM_INI_GUNS;
  _my_assert(s == "NUM_INI_GUNS", "Expected 'NUM_INI_GUNS' while parsing.");
  _my_assert(r.NUM_INI_GUNS >= 0 and r.NUM_INI_GUNS <= 5, "Wrong NUM_INI_GUNS.");
  
  is >> s >> r.NUM_INI_BAZOOKAS;
  _my_assert(s == "NUM_INI_BAZOOKAS", "Expected 'NUM_INI_BAZOOKAS' while parsing.");
  _my_assert(r.NUM_INI_BAZOOKAS >= 0 and r.NUM_INI_BAZOOKAS <= 4, "Wrong NUM_INI_BAZOOKAS.");
  
  is >> s >> r.BUILDER_INI_LIFE;
  _my_assert(s == "BUILDER_INI_LIFE", "Expected 'BUILDER_INI_LIFE' while parsing.");
  _my_assert(r.BUILDER_INI_LIFE >= 1, "Wrong BUILDER_INI_LIFE.");

  is >> s >> r.WARRIOR_INI_LIFE;
  _my_assert(s == "WARRIOR_INI_LIFE", "Expected 'WARRIOR_INI_LIFE' while parsing.");
  _my_assert(r.WARRIOR_INI_LIFE >= 1, "Wrong WARRIOR_INI_LIFE.");

  is >> s >> r.MONEY_POINTS;
  _my_assert(s == "MONEY_POINTS", "Expected 'MONEY_POINTS' while parsing.");
  _my_assert(r.MONEY_POINTS >= 1, "Wrong MONEY_POINTS.");

  is >> s >> r.KILL_BUILDER_POINTS;
  _my_assert(s == "KILL_BUILDER_POINTS", "Expected 'KILL_BUILDER_POINTS' while parsing. Found " +  s);
  _my_assert(r.KILL_BUILDER_POINTS >= 1, "Wrong KILL_BUILDER_POINTS");

  is >> s >> r.KILL_WARRIOR_POINTS;
  _my_assert(s == "KILL_WARRIOR_POINTS", "Expected 'KILL_WARRIOR_POINTS' while parsing. Found " + s);
  _my_assert(r.KILL_WARRIOR_POINTS >= 1, "Wrong KILL_WARRIOR_POINTS.");

  is >> s >> r.FOOD_INCR_LIFE;
  _my_assert(s == "FOOD_INCR_LIFE", "Expected 'FOOD_INCR_LIFE' while parsing.");
  _my_assert(r.FOOD_INCR_LIFE >= 1, "Wrong FOOD_INCR_LIFE.");

  is >> s >> r.LIFE_LOST_IN_ATTACK;
  _my_assert(s == "LIFE_LOST_IN_ATTACK", "Expected 'LIFE_LOST_IN_ATTACK' while parsing.");
  _my_assert(r.LIFE_LOST_IN_ATTACK >= 1, "Wrong LIFE_LOST_IN_ATTACK");
  
  is >> s >> r.BUILDER_STRENGTH_ATTACK;
  _my_assert(s == "BUILDER_STRENGTH_ATTACK", "Expected 'BUILDER_STRENGTH_ATTACK' while parsing.");
  _my_assert(r.BUILDER_STRENGTH_ATTACK >= 1, "Wrong BUILDER_STRENGTH_ATTACK.");

  is >> s >> r.HAMMER_STRENGTH_ATTACK;
  _my_assert(s == "HAMMER_STRENGTH_ATTACK", "Expected 'HAMMER_STRENGTH_ATTACK' while parsing.");
  _my_assert(r.HAMMER_STRENGTH_ATTACK >= 1, "Wrong HAMMER_STRENGTH_ATTACK.");

  is >> s >> r.GUN_STRENGTH_ATTACK;
  _my_assert(s == "GUN_STRENGTH_ATTACK", "Expected 'GUN_STRENGTH_ATTACK' while parsing.");
  _my_assert(r.GUN_STRENGTH_ATTACK >= 1, "Wrong GUN_STRENGTH_ATTACK.");

  is >> s >> r.BAZOOKA_STRENGTH_ATTACK;
  _my_assert(s == "BAZOOKA_STRENGTH_ATTACK", "Expected 'BAZOOKA_STRENGTH_ATTACK' while parsing.");
  _my_assert(r.BAZOOKA_STRENGTH_ATTACK >= 1, "Wrong BAZOOKA_STRENGTH_ATTACK.");

  is >> s >> r.BUILDER_STRENGTH_DEMOLISH;
  _my_assert(s == "BUILDER_STRENGTH_DEMOLISH", "Expected 'BUILDER_STRENGTH_DEMOLISH' while parsing.");
  _my_assert(r.BUILDER_STRENGTH_DEMOLISH >= 1, "Wrong BUILDER_STRENGTH_DEMOLISH.");

  is >> s >> r.HAMMER_STRENGTH_DEMOLISH;
  _my_assert(s == "HAMMER_STRENGTH_DEMOLISH", "Expected 'HAMMER_STRENGTH_DEMOLISH' while parsing.");
  _my_assert(r.HAMMER_STRENGTH_DEMOLISH >= 1, "Wrong HAMMER_STRENGTH_DEMOLISH.");

  is >> s >> r.GUN_STRENGTH_DEMOLISH;
  _my_assert(s == "GUN_STRENGTH_DEMOLISH", "Expected 'GUN_STRENGTH_DEMOLISH' while parsing.");
  _my_assert(r.GUN_STRENGTH_DEMOLISH >= 1, "Wrong GUN_STRENGTH_DEMOLISH.");

  is >> s >> r.BAZOOKA_STRENGTH_DEMOLISH;
  _my_assert(s == "BAZOOKA_STRENGTH_DEMOLISH", "Expected 'BAZOOKA_STRENGTH_DEMOLISH' while parsing.");
  _my_assert(r.BAZOOKA_STRENGTH_DEMOLISH >= 1, "Wrong BAZOOKA_STRENGTH_DEMOLISH.");

  is >> s >> r.NUM_ROUNDS_REGEN_BUILDER;
  _my_assert(s == "NUM_ROUNDS_REGEN_BUILDER", "Expected 'NUM_ROUNDS_REGEN_BUILDER' while parsing.");
  _my_assert(r.NUM_ROUNDS_REGEN_BUILDER >= 1, "Wrong NUM_ROUNDS_REGEN_BUILDER.");

  is >> s >> r.NUM_ROUNDS_REGEN_WARRIOR;
  _my_assert(s == "NUM_ROUNDS_REGEN_WARRIOR", "Expected 'NUM_ROUNDS_REGEN_WARRIOR' while parsing.");
  _my_assert(r.NUM_ROUNDS_REGEN_WARRIOR >= 1, "Wrong NUM_ROUNDS_REGEN_WARRIOR.");

  is >> s >> r.NUM_ROUNDS_REGEN_FOOD;
  _my_assert(s == "NUM_ROUNDS_REGEN_FOOD", "Expected 'NUM_ROUNDS_REGEN_FOOD' while parsing.");
  _my_assert(r.NUM_ROUNDS_REGEN_FOOD >= 1, "Wrong NUM_ROUNDS_REGEN_FOOD.");

  is >> s >> r.NUM_ROUNDS_REGEN_MONEY;
  _my_assert(s == "NUM_ROUNDS_REGEN_MONEY", "Expected 'NUM_ROUNDS_REGEN_MONEY' while parsing.");
  _my_assert(r.NUM_ROUNDS_REGEN_MONEY >= 1, "Wrong NUM_ROUNDS_REGEN_MONEY.");
  
  is >> s >> r.NUM_ROUNDS_REGEN_WEAPON;
  _my_assert(s == "NUM_ROUNDS_REGEN_WEAPON", "Expected 'NUM_ROUNDS_REGEN_WEAPON' while parsing.");
  _my_assert(r.NUM_ROUNDS_REGEN_WEAPON >= 1, "Wrong NUM_ROUNDS_REGEN_WEAPON.");

  is >> s >> r.BARRICADE_RESISTANCE_STEP;
  _my_assert(s == "BARRICADE_RESISTANCE_STEP", "Expected 'BARRICADE_RESISTANCE_STEP' while parsing.");
  _my_assert(r.BARRICADE_RESISTANCE_STEP >= 1, "Wrong BARRICADE_RESISTANCE_STEP.");

  is >> s >> r.BARRICADE_MAX_RESISTANCE;
  _my_assert(s == "BARRICADE_MAX_RESISTANCE", "Expected 'BARRICADE_MAX_RESISTANCE' while parsing.");
  _my_assert(r.BARRICADE_MAX_RESISTANCE >= 1, "Wrong BARRICADE_MAX_RESISTANCE.");

  is >> s >> r.MAX_NUM_BARRICADES;
  _my_assert(s == "MAX_NUM_BARRICADES", "Expected 'MAX_NUM_BARRICADES' while parsing.");
  _my_assert(r.MAX_NUM_BARRICADES >= 1, "Wrong MAX_NUM_BARRICADES.");

  _my_assert(r.ok(),"Settings invariants not fulfilled.");
  
  return r;
}
