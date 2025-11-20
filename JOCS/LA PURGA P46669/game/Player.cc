//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////

#include "Player.hh"

void Player::reset (ifstream& is) {

  // Should read what Board::print_state() prints.
  // Should fill the same data structures as
  // Board::Board (istream& is, int seed), except for settings and names.
  // THESE DATA STRUCTURES MUST BE RESET: maps WITH clear(), etc.


  *(Action*)this = Action();

  citizens         .clear();
  player2builders  .clear();
  player2warriors  .clear();
  player2barricades.clear();
  
  player2builders   = vector<set<int>>(num_players());
  player2warriors   = vector<set<int>>(num_players());
  player2barricades = vector<set<Pos>>(num_players());

  read_grid(is);

  string s;
  is >> s >> rnd;
  _my_assert(s == "round", "Expected 'round' while parsing.");
  _my_assert(rnd >= 0 and rnd < num_rounds(), "Round is not ok.");

  is >> s >> day;
  _my_assert(s == "day", "Expected 'day' while parsing.");
  
  is >> s;
  _my_assert(s == "score", "Expected 'score' while parsing.");
  scr = vector<int>(num_players());
  for (auto& s : scr) {
    is >> s;
    _my_assert(s >= 0, "Score cannot be negative.");
  }

  is >> s;
  _my_assert(s == "status", "Expected 'status' while parsing.");
  stats = vector<double>(num_players());
  for (auto& st : stats) {
    is >> st;
    _my_assert(st == -1 or (st >= 0 and st <= 1), "Status is not ok.");
  }

  _my_assert(ok(), "Invariants are not satisfied.");
  //  forget();
}
