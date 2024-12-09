//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////

#include "Board.hh"
#include "Action.hh"
  
void Board::create_new_citizen(Pos p, CitizenType t, int pl) {
  int id = fresh_id;
  ++fresh_id;
  _my_assert(not citizens.count(id), "Identifier is not fresh.");

  citizens[id] = Citizen(t, id, pl, p, (t == Builder ? NoWeapon : Hammer), citizen_ini_life(t));

  _my_assert(grid[p.i][p.j].is_empty(),        "Cell is already full.");

  grid[p.i][p.j].id = id;

  if      (t == Builder) player2builders[pl].insert(id);
  else                  player2warriors [pl].insert(id);
}


Board::Board(istream& is, int seed) {
  set_random_seed(seed);
  *static_cast<Settings*>(this) = Settings::read_settings(is);

  player2builders   = vector<set<int>>(num_players());
  player2warriors   = vector<set<int>>(num_players());
  player2barricades = vector<set<Pos>>(num_players());
  
  names          = vector<string>(num_players());
  scr            = vector<int>   (num_players(), 0);
  stats          = vector<double>(num_players(), 0);

  rnd = 0;
  day = true;

  fresh_id = 0;
  read_generator_and_grid(is);

  for (auto& p : citizens) fresh_id = max(fresh_id,p.first);
  ++fresh_id;
  
  _my_assert(ok(), "Invariants are not satisfied.");
}

void Board::check_is_good_initial_fixed_board () const {
  int num_builders = 0;
  int num_warriors = 0;
  int num_money = 0;
  int num_food = 0;
  int num_guns = 0;
  int num_bazookas = 0;

  vector<int> n_barricades(num_players(),0);

  _my_assert(int(grid.size()) == board_rows(), "Fixed board has wrong number of rows.");
  _my_assert(int(grid[0].size()) == board_cols(), "Fixed board has wrong number of cols.");
  
  
  // Count everything
  // Check who is on a barricade (same player)
  // Ini life of builder and warrior
  // Barricades with proper resistance  
  for (int i = 0; i < board_rows(); ++i)
    for (int j = 0; j < board_cols(); ++j) {
      Cell c = grid[i][j];
      if (c.bonus == Food) ++num_food;
      else if (c.bonus == Money) ++num_money;
      else if (c.weapon == Gun) ++num_guns;
      else if (c.weapon == Bazooka) ++num_bazookas;

      if (c.id != -1) {
	Citizen ci = citizen(c.id);
	if (ci.type == Builder) {
	  ++num_builders;
	  _my_assert(ci.life == builder_ini_life(), "Fixed board had builder with wrong initial life.");
	}
	else if (ci.type == Warrior){
	  ++num_warriors;
	  _my_assert(ci.life == warrior_ini_life(), "Fixed board had warrior with wrong initial life.");
	}
	if (c.resistance != -1) { // Somebody on a barricade
	  _my_assert(ci.player == c.b_owner, "Fixed board has Unit of wrong player on a barricade.");
	}
      }

      if (c.resistance != -1){
	++n_barricades[c.b_owner];
	_my_assert(c.resistance >= 0 and c.resistance <= barricade_max_resistance(),
		   "Fixed board has barricade with wrong resistance");
      }
    }

  for (int pl = 0; pl < num_players(); ++pl)
    _my_assert(n_barricades[pl] <= max_num_barricades(), "Fixed board has too many barricades.");
  
  _my_assert(num_money == num_ini_money(),"Fixed board has wrong number of initial money.");
  _my_assert(num_food == num_ini_food(),"Fixed board has wrong number of initial food.");
  _my_assert(num_guns == num_ini_guns(),"Fixed board has wrong number of initial guns.");
  _my_assert(num_bazookas == num_ini_bazookas(),"Fixed board has wrong number of initial bazookas.");
  _my_assert(num_builders%num_players() == 0 and num_builders/num_players() == num_ini_builders(),
	     "Fixed board has wrong number of initial builders");
  _my_assert(num_warriors%num_players() == 0 and num_warriors/num_players() == num_ini_warriors(),
	     "Fixed board has wrong number of initial warriors");
}

void Board::print_settings (ostream& os) const {

  os <<   version()                                                          << endl;
  os                                                                         << endl;
  os <<  "NUM_PLAYERS"               << "\t" <<  num_players()               << endl;
  os <<  "NUM_DAYS"                  << "\t" <<  num_days()                  << endl;
  os <<  "NUM_ROUNDS_PER_DAY"        << "\t" <<  num_rounds_per_day()        << endl;
  os <<  "BOARD_ROWS"                << "\t" <<  board_rows()                << endl;
  os <<  "BOARD_COLS"                << "\t" <<  board_cols()                << endl;
  os <<  "NUM_INI_BUILDERS"          << "\t" <<  num_ini_builders()          << endl;
  os <<  "NUM_INI_WARRIORS"          << "\t" <<  num_ini_warriors()          << endl;
  os <<  "NUM_INI_MONEY"             << "\t" <<  num_ini_money()             << endl;
  os <<  "NUM_INI_FOOD"              << "\t" <<  num_ini_food()              << endl;
  os <<  "NUM_INI_GUNS"              << "\t" <<  num_ini_guns()              << endl;
  os <<  "NUM_INI_BAZOOKAS"          << "\t" <<  num_ini_bazookas()          << endl;
  os <<  "BUILDER_INI_LIFE"          << "\t" <<  builder_ini_life()          << endl;
  os <<  "WARRIOR_INI_LIFE"          << "\t" <<  warrior_ini_life()          << endl;
  os <<  "MONEY_POINTS"              << "\t" <<  money_points()              << endl;
  os <<  "KILL_BUILDER_POINTS"       << "\t" <<  kill_builder_points()       << endl;
  os <<  "KILL_WARRIOR_POINTS"       << "\t" <<  kill_warrior_points()       << endl;
  os <<  "FOOD_INCR_LIFE"            << "\t" <<  food_incr_life()            << endl;
  os <<  "LIFE_LOST_IN_ATTACK"       << "\t" <<  life_lost_in_attack()       << endl;
  os <<  "BUILDER_STRENGTH_ATTACK"   << "\t" <<  builder_strength_attack()   << endl;
  os <<  "HAMMER_STRENGTH_ATTACK"    << "\t" <<  hammer_strength_attack()    << endl;
  os <<  "GUN_STRENGTH_ATTACK"       << "\t" <<  gun_strength_attack()       << endl;
  os <<  "BAZOOKA_STRENGTH_ATTACK"   << "\t" <<  bazooka_strength_attack()   << endl;
  os <<  "BUILDER_STRENGTH_DEMOLISH" << "\t" <<  builder_strength_demolish() << endl;
  os <<  "HAMMER_STRENGTH_DEMOLISH"  << "\t" <<  hammer_strength_demolish()  << endl;
  os <<  "GUN_STRENGTH_DEMOLISH"     << "\t" <<  gun_strength_demolish()     << endl;
  os <<  "BAZOOKA_STRENGTH_DEMOLISH" << "\t" <<  bazooka_strength_demolish() << endl;
  os <<  "NUM_ROUNDS_REGEN_BUILDER"  << "\t" <<  num_rounds_regen_builder()  << endl;
  os <<  "NUM_ROUNDS_REGEN_WARRIOR"  << "\t" <<  num_rounds_regen_warrior()  << endl;
  os <<  "NUM_ROUNDS_REGEN_FOOD"     << "\t" <<  num_rounds_regen_food()     << endl;
  os <<  "NUM_ROUNDS_REGEN_MONEY"    << "\t" <<  num_rounds_regen_money()    << endl;
  os <<  "NUM_ROUNDS_REGEN_WEAPON"   << "\t" <<  num_rounds_regen_weapon()   << endl;
  os <<  "BARRICADE_RESISTANCE_STEP" << "\t" <<  barricade_resistance_step() << endl;
  os <<  "BARRICADE_MAX_RESISTANCE"  << "\t" <<  barricade_max_resistance()  << endl;
  os <<  "MAX_NUM_BARRICADES"        << "\t" <<  max_num_barricades()        << endl;

}


void Board::print_names (ostream& os) const {
  os << "names         ";
  for (int pl = 0; pl < num_players(); ++pl) os << ' ' << name(pl);
  os << endl;
}


void Board::print_state (ostream& os) {

  // Should start with the same format of Info::read_grid.
  // Then other data describing the state.

  os << endl << endl;

  os << "   ";
  for (int j = 0; j < board_cols(); ++j)
    os << j / 10;
  os << endl;

  os << "   ";
  for (int j = 0; j < board_cols(); ++j)
    os << j % 10;
  os << endl;

  for (int i = 0; i < board_rows(); ++i) {
    os << i / 10 << i % 10 << " ";
    for (int j = 0; j < board_cols(); ++j) {
      const Cell& c = grid[i][j];
      if      (c.type   == Building) os << 'B';
      else if (c.weapon == Gun)      os << 'G';
      else if (c.weapon == Bazooka)  os << 'Z';
      else if (c.bonus  == Money)    os << 'M';
      else if (c.bonus  == Food)     os << 'F';
      else if (c.id     != -1) {
	if (c.resistance == -1) {
	  if (citizens[c.id].type == Builder) os << 'C'; // builder without barricade
	  else                                os << 'W'; // warrior without barricade
	}
	else {
	  if (citizens[c.id].type == Builder) os << 'c'; // buider with barricade
	  else                                os << 'w'; // warrior with barricade
	}
      }
      else if (c.resistance != -1) os << 'b'; // barricade with no citizen
      else                           os << '.';
    }
    os << endl;
  }

  os << endl << "citizens" << endl;
  os << citizens.size() << endl;
  os << "type\tid\tplayer\trow\tcolumn\tweapon\tlife" << endl;
  for (const auto& ci : citizens) {
    os << CitizenType2char(ci.second.type) << "\t";
    os << ci.second.id << "\t";
    os << ci.second.player << "\t";
    os << ci.second.pos.i << "\t";
    os << ci.second.pos.j << "\t";
    os << WeaponType2char(ci.second.weapon) << "\t";
    os << ci.second.life << endl;
  }

  os << endl << "barricades" << endl;
  // Collect them
  vector<Pos> barricades;
  for (int i = 0; i < board_rows(); ++i)
    for (int j = 0; j < board_cols(); ++j)
      if (grid[i][j].resistance != -1) barricades.push_back(Pos(i,j));
  os << barricades.size() << endl;
  os << "player\trow\tcolumn\tresistance" << endl;
  for (const auto& p : barricades) {
    os << grid[p.i][p.j].b_owner << "\t";
    os << p.i << "\t";
    os << p.j << "\t";
    os << grid[p.i][p.j].resistance << endl;
  }

  os << endl;

  os << "round " << rnd << endl;
  os << "day " << day << endl;
  os << endl;

  os << "score";
  for (auto s : scr) os << "\t" << s;
  os << endl;

  os << endl;

  os << "status";
  for (auto s : stats) os << "\t" << s;
  os << endl;

  os << endl;
}


void Board::print_results () const {
  int max_score = 0;
  vector<int> v;
  for (int pl = 0; pl < num_players(); ++pl) {

    cerr << "info: player " <<  name(pl)
         << " got score "   << score(pl) << endl;

    if      (score(pl) == max_score)  v.push_back(pl);
    else if (score(pl) >  max_score) {
      max_score = score(pl);
      v = vector<int>(1, pl);
    }
  }

  cerr << "info: player(s)";
  for (int pl : v) cerr << " " << name(pl);
  cerr << " got top score" << endl;
}

// Returns whether c1 wins
bool Board::first_citizen_wins_attack (const Citizen& c1, const Citizen& c2) {
  int c1_strength = weapon_strength_attack(c1.weapon);
  int c2_strength = weapon_strength_attack(c2.weapon);
  int M = 1000;
  int num = random(0,M);
  double threshold = double(c1_strength)/(c1_strength + c2_strength)*M;
  return num < threshold;
}

void Board::perform_attack (Citizen& ci, Citizen& ci2, set<int>& killed, vector<pair<pair<CitizenType,int>,int>>& citizens_to_regenerate) {
  bool first_wins = first_citizen_wins_attack(ci,ci2);
  Citizen& winner = (first_wins ? ci  : ci2);
  Citizen& loser =  (first_wins ? ci2 : ci );

  loser.life -= life_lost_in_attack();
  if (loser.life <= 0) { // Dead!!!
    kill(loser.id,killed);
    citizens_to_regenerate.push_back({{loser.type,loser.player},num_rounds_regen_citizen(loser.type)});
    if (loser.type == Builder)  scr[winner.player] += kill_builder_points();
    else                        scr[winner.player] += kill_warrior_points();
  }
}      

bool Board::execute(const Command&    m,
                    set<int>&         killed,
		    vector<pair<BonusType,int>>&   bonus_to_regenerate,
		    vector<pair<WeaponType,int>>&  weapon_to_regenerate,
		    vector<pair<pair<CitizenType,int>,int>>& citizens_to_regenerate
		    ) {

  int         id     =                  m.id;
  Dir         dir    =            Dir(m.dir);
  CommandType c_type = CommandType(m.c_type);
  
  if (not command_type_ok(c_type)) {
    //cerr << "warning: invalid command type in command: " << c_type << endl;
    return false;
  }

  if (killed.count(id)) return false; // Citizen has been killed in this round

  
  Citizen&      ci = citizens[id];
  CitizenType type = ci.type;    
  int           pl = ci.player;
  Pos           op = ci.pos;
  Cell&         oc = grid[op.i][op.j];

  if (not dir_ok(dir)) {
    //cerr << "warning: invalid dir in command: " << dir << endl;
    return false;
  }
  
  if (c_type == Move) {

    Pos np = op + dir;
    if (not pos_ok(np)) {
      //cerr << "warning: cannot move to position " << np << " out of the board." << endl;
      return false;
    }

    Cell& nc = grid[np.i][np.j];
    if (nc.type == Building) {
      //cerr << "warning: cannot move to position " << np << " with a building." << endl;
      return false;
    }
    else if (nc.bonus == Food) { // Take food and move
      bonus_to_regenerate.push_back({nc.bonus,num_rounds_regen_food()});
      nc.bonus = NoBonus;
      nc.id = id;
      oc.id = -1;
      ci.pos = np;
      ci.life = min(ci.life + food_incr_life(), citizen_ini_life(type));
    }
    else if (nc.bonus == Money) { // Take money and move
      bonus_to_regenerate.push_back({nc.bonus,num_rounds_regen_money()});
      nc.bonus = NoBonus;
      nc.id = id;
      oc.id = -1;
      ci.pos = np;
      scr[pl] += money_points();
    }
    else if (nc.weapon != NoWeapon) {
      if (type == Builder) { // Moves and weapon disappears	
	weapon_to_regenerate.push_back({nc.weapon,num_rounds_regen_weapon()});
	nc.weapon = NoWeapon;
	nc.id = id;
	oc.id = -1;
	ci.pos = np;	
      }
      else { // Warrior: moves and takes strongest weapon. Weapon disappears
	weapon_to_regenerate.push_back({nc.weapon,num_rounds_regen_weapon()});
	ci.weapon = strongestWeapon(ci.weapon,nc.weapon);
	nc.weapon = NoWeapon;
	nc.id = id;
	oc.id = -1;
	ci.pos = np;	
      }
    }
    else if (nc.resistance != -1) { // Barricade
      if (day) {
	if      (nc.id != -1)      return false; // Also citizen: not attack to rivals, do not step on friends
	else if (nc.b_owner != pl) return false; // No citizen and barricade of another rival at day
	else {  // Same clan free barricade : moves there
	  nc.id = id;
	  oc.id = -1;
	  ci.pos = np;
	}
      }
      else { // night
	if (nc.b_owner != pl) { // rival barricade --> demolish independently of whether there is a citizen
	  nc.resistance -= weapon_strength_demolish(ci.weapon);
	  if (nc.resistance <= 0) { // Barricade disappears
	    player2barricades[nc.b_owner].erase(np);
	    nc.resistance = -1;
	    nc.b_owner = -1;
	  }	  
	}
	else { // own barricade
	  if (nc.id != -1) return false; // occupied --> ignore move
	  else { // free --> move there
	    nc.id = id;
	    oc.id = -1;
	    ci.pos = np;	   
	  }
	}
      }
    }
    else if (nc.id != -1) { // Citizen (pero no barricada)
      if (day) return false;
      else if (citizens[nc.id].player == pl) return false; // Never attack same clan
      else { // Night and other clan: attack but not move!!!!
	perform_attack(ci,citizens[nc.id],killed,citizens_to_regenerate);
      }
    }
    else { // Free cell, move there
      nc.id = id;
      oc.id = -1;
      ci.pos = np;
    }    
  }
  else if (c_type == Build) {
    if (type == Warrior) {
      //cerr << "warning: warriors cannot build." << endl;
      return false;
    }
    
    if (not day) {
      // cerr << "warning: cannot construct at night." << endl;
      return false;
    }
    
    Pos np = op + dir;
    if (not pos_ok(np)) {
      //cerr << "warning: cannot construct in position " << np << " out of the board." << endl;
      return false;
    }

    Cell& nc = grid[np.i][np.j];
    if (nc.type == Building or nc.bonus != NoBonus or nc.weapon != NoWeapon or nc.id != -1) {
      //cerr << "warning: cannot construct in non-empty position " << np << endl;
      return false;
    }

    if (nc.resistance != -1 and nc.b_owner != pl) {
      //cerr << "warning: cannot construct on a rival barricade at position " << np << endl;
      return false;
    }

    if (oc.resistance != -1) {
      //cerr << "warning: cannot construct from a barricade. Position " << op << endl;
      return false;
    }

    // Trying to construct new barricade
    if (nc.resistance == -1 and  int(player2barricades[pl].size()) >= max_num_barricades()) {
      //cerr << "warning: player " << pl << " is trying to construct more than " << max_num_barricades() << " barricades." << endl;
      return false;
    }

    // Construeix o incrementa la barricada a la direccio
    if (nc.resistance == -1) { // new barricade
      nc.resistance = barricade_resistance_step();
      nc.b_owner = pl;
      player2barricades[pl].insert(np);
    }
    else nc.resistance = min(nc.resistance + barricade_resistance_step(), barricade_max_resistance());
  }

  return true;
}


void Board::kill (int id, set<int>& killed) {

  _my_assert(not killed.count(id), "Already killed");

  auto it = citizens.find(id);
  _my_assert(it != citizens.end(), "Could not find citizen to be killed");

  Citizen& ci = it->second;
  int      pl = ci.player;

  grid[ci.pos.i][ci.pos.j].id = -1;

  if (ci.type == Builder) {
    _my_assert(player2builders[pl].count(id), "Builder to kill is not registered.");
    player2builders[pl].erase(id);
  }
  else if (ci.type == Warrior) {
    _my_assert(player2warriors[pl].count(id),   "Warrior to kill is not registered.");
    player2warriors[pl].erase(id);
  }

  citizens.erase(it);
  killed.insert(id);
}

bool Board::is_good_pos_to_regen ( const Pos& p) const {

  if (not grid[p.i][p.j].is_empty()) return false;
  
  for (int i = p.i - 2; i <= p.i + 2; ++i) { // Check nobody is nearby
    for (int j = p.j - 2; j <= p.j + 2; ++j) {
      if (pos_ok(Pos(i,j)) and grid[i][j].id != -1) return false;  
    }
  }
  return true;
}

pair<bool,Pos> Board::get_random_pos_where_regenerate ( ) {
  vector<Pos> res;
  for (int i = 0; i < board_rows(); ++i){
    for (int j = 0; j < board_cols(); ++j) {
      if (is_good_pos_to_regen(Pos(i,j))) res.push_back(Pos(i,j));
    }
  }
  
  if (res.size() != 0)  return {true,res[random(0,res.size()-1)]};
  else                  return {false,Pos()};
}

void Board::regenerate_citizens (vector<pair<pair<CitizenType,int>,int>>& to_regen) {
  vector<pair<pair<CitizenType,int>,int>> new_to_regen;
  for (auto p : to_regen) {
    --p.second;
    if (p.second == 0) { // Regenerate now
      pair<bool,Pos> x = get_random_pos_where_regenerate();      
      if (not x.first) {p.second = 1; new_to_regen.push_back(p);} // Regenerate next round
      else {
	create_new_citizen(x.second, p.first.first, p.first.second);
      }
    }
    else new_to_regen.push_back(p); // To be regenerated later
  }
  to_regen = new_to_regen;
}

void Board::regenerate_bonus (vector<pair<BonusType,int>>& to_regen) {
  vector<pair<BonusType,int>> new_to_regen;
  for (auto p : to_regen) {
    --p.second;
    if (p.second == 0) { // Regenerate now
      pair<bool,Pos> x = get_random_pos_where_regenerate();
      if (not x.first) {
	p.second = 1; new_to_regen.push_back(p);
      } // Regenerate next round
      else {
	Pos pos = x.second;
	_my_assert(grid[pos.i][pos.j].is_empty(),        "Cell is already full.");
	grid[pos.i][pos.j].bonus = p.first;
      }
    }
    else new_to_regen.push_back(p); // To be regenerated later
  }
  to_regen = new_to_regen;  
}

void Board::regenerate_weapons (vector<pair<WeaponType,int>>& to_regen) {
  vector<pair<WeaponType,int>> new_to_regen;
  for (auto p : to_regen) {
    --p.second;
    if (p.second == 0) { // Regenerate now
      pair<bool,Pos> x = get_random_pos_where_regenerate();
      if (not x.first) {p.second = 1; new_to_regen.push_back(p);} // Regenerate next round
      else {
	Pos pos = x.second;
	_my_assert(grid[pos.i][pos.j].is_empty(),        "Cell is already full.");
	grid[pos.i][pos.j].weapon = p.first;
      }
    }
    else new_to_regen.push_back(p); // To be regenerated later
  }
  to_regen = new_to_regen;  
}

void Board::deteriorate_barricades ( ){

  // If we are at the last round of night (i.e. next is day), remove them
  if (rnd%num_rounds_per_day() == num_rounds_per_day() - 1) {
    for (int pl = 0; pl < num_players(); ++pl) player2barricades[pl].clear();
        
    for (int i = 0; i < board_rows(); ++i){
      for (int j = 0; j < board_cols(); ++j) {
	if (grid[i][j].resistance != -1) {
	  grid[i][j].resistance = -1;
	  grid[i][j].b_owner = -1;
	}
      }
    }
  }
  
}

void Board::next (const vector<Action>& act, ostream& os) {

  _my_assert(ok(), "Invariants are not satisfied.");

  int npl = num_players();
  _my_assert(int(act.size()) == npl, "Size should be number of players.");

  // Elements to be regenerated
  static vector<pair<BonusType,int>> bonus_to_regenerate; // int is rounds to wait
  static vector<pair<WeaponType,int>> weapons_to_regenerate;
  static vector<pair<pair<CitizenType,int>,int>> citizens_to_regenerate; // <<citizen,player>,rounds>

  
  // Chooses (at most) one command per citizen.
  set<int> seen;
  vector<vector<Command>> v(npl);
  for (int pl = 0; pl < npl; ++pl)
    for (const Command& m : act[pl].v) {
      int id     = m.id;
      int c_type = m.c_type;
      int dir    = m.dir;


      auto it = citizens.find(id);

      if (it == citizens.end()) {
        //cerr << "warning: invalid id : " << id << endl;
      }

      else if (it->second.player != pl) {
        //cerr << "warning: citizen " << id << " of player " << it->second.player
	//   << " not owned by " << pl << endl;
      }
      else {
        // Here an assert as repetitions should have already been filtered out.
        _my_assert(not seen.count(id), "More than one command for the same citizen.");
        seen.insert(id);

        v[pl].push_back(Command(id, c_type, dir));
      }
    }


  // Makes all players' commands using a random order,
  // but respecting the relative order of the citizens of the same player.
  // Permutations are not equally likely to avoid favoring leading clans.
  int num = 0; // Counts number of pending commands
  for (int pl = 0; pl < npl; ++pl) num += v[pl].size();

  set<int> killed;
  vector<Command> commands_done;
  vector<int> index(npl, 0);
  while (num--) {
    int q = 0; // Counts number of players with some action pending
    for (int pl = 0; pl < npl; ++pl) q += index[pl] < (int)v[pl].size();
    _my_assert(q > 0, "q > 0 in next.");
    int ran = random(1,q); // Decide whether 1st, 2nd, 3rd,, player with something pending is chosen
    int pl = -1;
    int acum = 0;
    while (acum < ran) {
      ++pl;
      acum += index[pl] < (int)v[pl].size(); // If index > ..., then player has nothing pending
                                             // and does not count
    }

    const Command& m = v[pl][index[pl]++];
    if (execute(m, killed, bonus_to_regenerate, weapons_to_regenerate, citizens_to_regenerate))
      commands_done.push_back(m);

  }
  os << "commands" << endl;
  Action::print(commands_done, os);

  regenerate_citizens(citizens_to_regenerate);  
  regenerate_bonus(bonus_to_regenerate);
  regenerate_weapons(weapons_to_regenerate);
  deteriorate_barricades();
  
  ++rnd;
  day = (rnd%(num_rounds_per_day()) < num_rounds_per_day()/2);

  _my_assert(ok(), "Invariants are not satisfied.");
}


//************************************************************
//                     BOARD GENERATION
//************************************************************

int Board::generate_street (int s_id, int length) {
  int filled = 0;
  vector<Dir> dirs = {Up, Down, Left, Right};
  Dir last_dir = dirs[random(0,dirs.size()-1)];
  Pos p = get_ok_pos_for_initial_street();
  street_plan[p.i][p.j] = s_id;
  ++filled;
  while (length > 0) {
    random_shuffle(dirs.begin(),dirs.end());
    Dir new_possible_dir = Up; // Explore the possibility of turning
    bool dir_found = false;
    for (auto& d:dirs) {
      if (pos_ok_for_street(s_id,p+d)){
	new_possible_dir = d;
	dir_found = true;
	break;
      }
    }
    
    if (random(1,8) != 1 and pos_ok_for_street(s_id,p+last_dir)){ // Continue same direction
      p += last_dir;
      street_plan[p.i][p.j] = s_id;
      --length;
      ++filled;
    }
    else if (dir_found){ // Turn
      last_dir = new_possible_dir;
      _my_assert(cell(p+new_possible_dir).is_empty(), "Cell no empty");
      p += new_possible_dir;
      _my_assert(pos_ok_for_street(s_id,p), "Pos not ok for street.");
      street_plan[p.i][p.j] = s_id;
      --length;
      ++filled;
    }
    else // Stop
      return filled;
  }
  return filled;
}

void Board::generate_buildings (int num_building_cells, int num_streets){
  street_plan = vector<vector<int>>(board_rows(),vector<int>(board_cols(),0));
  
  int num_streets_pending = num_streets;
  while (num_streets_pending > 0) {
    int length;
    if (num_streets_pending != 1) length = num_building_cells/num_streets_pending;
    else length = num_building_cells;
    num_building_cells -= generate_street(num_streets_pending,length);
    --num_streets_pending;
  }

  for (int i = 0; i < board_rows(); ++i) {
    for (int j = 0; j < board_cols(); ++j){
      if (street_plan[i][j] != 0) {
	grid[i][j].type = Building;
      }
    }
  }

}

Pos Board::get_empty_pos ( ){
  while (true) {
    int i = random(0,board_rows()-1);
    int j = random(0,board_cols()-1);
    if (cell(i,j).is_empty()) return Pos(i,j);
  }
}

Pos Board::get_ok_pos_for_street(int s_id) {
  while (true) {
    int i = random(1,board_rows()-2);
    int j = random(1,board_cols()-2);
    if (pos_ok_for_street(s_id, Pos(i,j))) return Pos(i,j);
  }
}

Pos Board::get_ok_pos_for_initial_street() {
  while (true) {
    int i = random(1,board_rows()-2);
    int j = random(1,board_cols()-2);
    if (pos_ok_for_initial_street(Pos(i,j))) return Pos(i,j);
  }
}

bool Board::pos_ok_for_street(int s_id, const Pos& p){
  int i = p.i, j = p.j;
  if (not pos_ok(p)) return false;
  if (street_plan[i][j] != 0) return false;
  if (i == 0) return false;
  if (i == board_rows() -1) return false;
  if (j == 0) return false;
  if (j == board_cols() -1) return false;

  int num_occupied = 0;
  vector<Dir> dirs = {Up, Down, Left, Right};
  for (auto& d: dirs) {
    Pos newPos = p + d; // Will exists because p is not on a border
    int ni = newPos.i, nj = newPos.j;
    if (street_plan[ni][nj] != 0 and street_plan[ni][nj] != s_id) return false;
    else if (street_plan[ni][nj] == s_id) ++num_occupied;
  }

  vector<pair<int,int>> diags = {{1,1},{1,-1},{-1,1},{-1,-1}};
  for (auto& d : diags) {
    Pos newPos = Pos(p.i + d.first, p.j + d.second);
    int ni = newPos.i, nj = newPos.j;
    if (street_plan[ni][nj] != 0 and street_plan[ni][nj] != s_id) return false;
    else if (street_plan[ni][nj] == s_id) ++num_occupied;
  }
      
  return num_occupied <= 2;
}

bool Board::pos_ok_for_initial_street(const Pos& p){
  int i = p.i, j = p.j;
  if (not pos_ok(p)) return false;
  if (street_plan[i][j] != 0) return false;
  if (i == 0) return false;
  if (i == board_rows() -1) return false;
  if (j == 0) return false;
  if (j == board_cols() -1) return false;

  int num_occupied = 0;
  vector<Dir> dirs = {Up, Down, Left, Right};
  for (auto& d: dirs) {
    Pos newPos = p + d; // Will exists because p is not on a border
    int n_i = newPos.i, n_j = newPos.j;
    if (street_plan[n_i][n_j] != 0) return false;
  }

  vector<pair<int,int>> diags = {{1,1},{1,-1},{-1,1},{-1,-1}};
  for (auto& d : diags) {
    Pos newPos = Pos(p.i + d.first, p.j + d.second);
    int n_i = newPos.i, n_j = newPos.j;
    if (street_plan[n_i][n_j] != 0) return false;
  }
      
  return true;
}

void Board::explore_from(vector<vector<int>>& G, int i, int j, int n) {
  G[i][j] = n;
  for (auto& d:{Up,Down,Left,Right}) {
    Pos np = Pos(i,j) + d;
    if (pos_ok(np) and G[np.i][np.j] == -1) explore_from(G,np.i,np.j,n);
  }
}

int Board::num_connected_components( ){
  vector<vector<int>> G(board_rows(),vector<int>(board_cols(),-1));

  for (int i = 0; i < board_rows(); ++i) {
    for (int j = 0; j < board_cols(); ++j){
      if (grid[i][j].type == Building) G[i][j] = -2;
    }
  }

  int n = 0;    
  for (int i = 0; i < board_rows(); ++i) {
    for (int j = 0; j < board_cols(); ++j){
      if (G[i][j] == -1) {
	explore_from(G,i,j,n);
	++n;
      }
    }
  }

  return n;
}

void Board::generate_random_board ( ){
  int rows = board_rows();
  int cols = board_cols();

  
  // Generate buildings (leaving space for citizens)
  static const int num_building_cells = 0.20*rows*cols; // 20% buildings
  static const int num_streets = 5;
    
  do {
    // Create grid
    grid = vector<vector<Cell>>(rows, vector<Cell>(cols));
    generate_buildings(num_building_cells, num_streets);
  } while (num_connected_components() != 1);

  
  // Generate citizens
  for (int pl = 0; pl < num_players(); ++pl) {
    for (int i = 0; i < num_ini_builders(); ++i) {
      Pos p = get_empty_pos();
      create_new_citizen(p,Builder,pl);
    }
    for (int i = 0; i < num_ini_warriors(); ++i) {
      Pos p = get_empty_pos();
      create_new_citizen(p,Warrior,pl);
    }
  }

  // Generate bonus
  for (int i = 0; i < num_ini_food(); ++i) {
    Pos p = get_empty_pos();
    grid[p.i][p.j].bonus = Food;
  }


  for (int i = 0; i < num_ini_money(); ++i) {
    Pos p = get_empty_pos();
    grid[p.i][p.j].bonus = Money;
  }

  
  // Generate weapons
  for (int i = 0; i < num_ini_guns(); ++i) {
    Pos p = get_empty_pos();
    grid[p.i][p.j].weapon = Gun;
  }

  for (int i = 0; i < num_ini_bazookas(); ++i) {
    Pos p = get_empty_pos();
    grid[p.i][p.j].weapon = Bazooka;
  }

  
}
