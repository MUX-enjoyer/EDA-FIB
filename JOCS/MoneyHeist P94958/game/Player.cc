//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////

#include "Player.hh"

void Player::reset (ifstream& is) {

  // Should read what Board::print_state() prints.
  // Should fill the same data structures as
  // Board::Board (istream& is, int seed), except for settings and names.
  // THESE DATA STRUCTURES MUST BE RESET: maps WITH clear(), etc.


  *(Action*)this = Action();


  scr           = vector<int>(num_players());
  player_health = vector<int>(num_players());
  player_poison = vector<int>(num_players());

  stats = vector<double>(num_players());

  units          .clear();
  player2soldiers  = vector<set<int>>(num_players());
  player2professor = vector<int>(num_players());

  // safety_box_hints is reset inside read_grid
  read_grid(is);

  string s;
  is >> s >> rnd;  
  _my_assert(s == "round", "Expected 'round' while parsing." + s);
  _my_assert(rnd >= 0 and rnd < num_rounds(), "Round is not ok.");

  is >> s;
  _my_assert(s == "score", "Expected 'score' while parsing.");

  for (auto& s : scr) {
    is >> s;
    _my_assert(s >= 0, "Score cannot be negative.");
  }

  is >> s;
  _my_assert(s == "health", "Expected 'health' while parsing.");
  for (auto& s : player_health) {
    is >> s;
    _my_assert(s >= 0, "Health cannot be negative.");
  }

  is >> s;
  _my_assert(s == "poison", "Expected 'poison' while parsing.");
  for (auto& s : player_poison) {
    is >> s;
    _my_assert(s >= 0, "Poison cannot be negative.");
  }

  is >> s;
  _my_assert(s == "status", "Expected 'status' while parsing.");
  for (auto& st : stats) {
    is >> st;
    _my_assert(st == -1 or (st >= 0 and st <= 1), "Status is not ok.");
  }

  _my_assert(ok(), "Invariants are not satisfied.");
}
