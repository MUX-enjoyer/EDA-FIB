//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////

#include "Board.hh"
#include "Action.hh"

Board::Board (istream& is, int seed) {
  set_random_seed(seed);
  *static_cast<Settings*>(this) = Settings::read_settings(is);

  player2soldiers = vector<set<int>>(num_players());
  player2professor       = vector<int>(num_players());

  names           = vector<string>(num_players());
  scr             = vector<int>   (num_players(), 0);
  player_health   = vector<int>   (num_players(), 0);
  player_poison   = vector<int>   (num_players(), 0);
  
  stats           = vector<double> (num_players(), 0);

  rnd = 0;

  fresh_id = 0;
  read_generator_and_grid(is);

  for (auto& p : units) fresh_id = max(fresh_id, p.id);
  ++fresh_id;
  
  _my_assert(ok(), "Invariants are not satisfied.");
}

void Board::print_settings (ostream& os) const {

  os <<   version()                                                                      << endl;
  os                                                                                     << endl;
  os <<  "NUM_PLAYERS"                   << "\t\t\t" <<  num_players()                   << endl;
  os <<  "NUM_ROUNDS"                    << "\t\t\t" <<  num_rounds()                    << endl;
  os <<  "BOARD_ROWS"                    << "\t\t\t" <<  board_rows()                    << endl;
  os <<  "BOARD_COLS"                    << "\t\t\t" <<  board_cols()                    << endl;

  os <<  "INITIAL_SOLDIERS_PER_CLAN"     << "\t" <<  num_ini_soldiers_per_clan()     << endl;
  os <<  "INITIAL_SOLDIERS_POISONS"      << "\t" <<  soldier_ini_poisons()           << endl;
  os <<  "SOLDIER_MAX_POISONS"           << "\t" <<  soldier_max_poisons()           << endl;
  os <<  "POINTS_FOR_KILLING_SOLDIER"    << "\t" <<  points_for_killing_soldier()    << endl;

  os <<  "PROFESSOR_MAX_HEALTH"          << "\t\t" <<  professor_max_health()          << endl;
  os <<  "INITIAL_PROFESSOR_HEALTH"      << "\t\t" <<  professor_ini_health()          << endl;
  os <<  "PROFESSOR_HEALTH_DECREASE"     << "\t" <<  professor_health_decrease()     << endl;
  os <<  "POINTS_FOR_KILLING_PROFESSOR"  << "\t" <<  points_for_killing_professor()  << endl;
  os <<  "PROFESSOR_ROUNDS_REBIRTH"      << "\t" <<  professor_rounds_rebirth()      << endl;

  os <<  "NUM_MONEY"                     << "\t\t\t" <<  num_money()                     << endl;
  os <<  "MIN_MONEY_VALUE"               << "\t\t\t" <<  min_money_value()               << endl;
  os <<  "MAX_MONEY_VALUE"               << "\t\t\t" <<  max_money_value()               << endl;

  os <<  "NUM_HEALTH_KITS"               << "\t\t\t" <<  num_health_kits()               << endl;
  os <<  "HEALTH_KIT_VALUE"              << "\t\t" <<  health_kit_value()              << endl;

  os <<  "NUM_POISON_KITS"               << "\t\t\t" <<  num_poison_kits()               << endl;
  os <<  "POISON_KIT_VALUE"              << "\t\t" <<  poison_kit_value()              << endl;

  os <<  "NUM_SAFETY_BOXES"              << "\t\t" <<  num_safety_boxes()              << endl;
  os <<  "GOLD_VALUE"                    << "\t\t" <<  gold_value()                    << endl;
  os <<  "SIZE_HINTS_PER_BOX"             << "\t\t" <<  size_hints_per_box()             << endl;
  
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
      if      (c.type   == Wall)                 os << 'W';
      else if (c.health_kit)                     os << 'H';
      else if (c.poison_kit)                     os << 'P';
      else if (c.box != -1)                      os << 'B';
      else if (c.gold)                           os << 'G';
      else if (c.money > 0)                      os << 'M';
      else if (c.id != -1 and
	       units[c.id].type == Soldier)      os << units[c.id].player;

      else if (c.id != -1 and
	       units[c.id].type == Professor)    os << 'X';
      else                                       os << '.';
    }
    os << endl;
  }

  os << endl << "units" << endl;
  os << units.size() << endl;
  os << "type\tid\tplayer\trow\tcolumn\thealth\tpoison\trounds" << endl;
  for (const auto& u : units) {
    os << UnitType2char(u.type) << "\t";
    os << u.id << "\t";
    os << u.player << "\t";
    os << u.pos.i << "\t";
    os << u.pos.j << "\t";
    os << u.health << "\t";
    os << u.poison << "\t";
    os << u.rounds_for_reborn << endl;
  }

  os << endl << "money" << endl;
  // Collect them
  vector<vector<int>> moneys;
  for (int i = 0; i < board_rows(); ++i)
    for (int j = 0; j < board_cols(); ++j)
      if (grid[i][j].money > 0) moneys.push_back({i,j,grid[i][j].money});
  os << moneys.size() << endl;
  os << "row\tcolumn\tamount" << endl;
  for (const auto& p : moneys) {
    os << p[0] << "\t";
    os << p[1] << "\t";
    os << p[2] << endl;
  }
  
  os << endl << "safety_box_hints" << endl;
  os << hints_boxes.size() << endl;
  for (uint i = 0; i < hints_boxes.size(); ++i) {
    vector<int>& v = hints_boxes[i];
    os << v.size() << " " << pos_boxes[i].i << " " << pos_boxes[i].j << endl;
    for (int x : v) os << x << " ";
    os << endl << endl;
  }
  
  os << "round " << rnd << endl;
  os << endl;

  os << "score";
  for (auto s : scr) os << "\t" << s;
  os << endl;
  os << endl;

  os << "health";
  for (auto s : player_health) os << "\t" << s;
  os << endl;
  os << endl;

  os << "poison";
  for (auto s : player_poison) os << "\t" << s;
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

bool Board::is_good_pos_to_regen ( const Pos& p) const {

  if (not grid[p.i][p.j].is_empty()) return false;
  
  for (int i = p.i - 2; i <= p.i + 2; ++i) { // Check nobody is nearby
    for (int j = p.j - 2; j <= p.j + 2; ++j) {
      if (pos_ok(Pos(i,j)) and (grid[i][j].id != -1 or grid[i][j].health_kit or grid[i][j].poison_kit or
				grid[i][j].box != -1 or grid[i][j].gold or grid[i][j].money > 0))
	return false;  
    }
  }
  return true;
}

Pos Board::get_random_pos_where_regenerate ( ) {
  vector<Pos> res;
  for (int i = 0; i < board_rows(); ++i){
    for (int j = 0; j < board_cols(); ++j) {
      if (is_good_pos_to_regen(Pos(i,j))) res.push_back(Pos(i,j));
    }
  }
  
  if (res.size() != 0)  return res[random(0,res.size()-1)];
  else                  return get_empty_pos();
}


Pos Board::get_random_pos_where_regenerate_gold ( ) {
  vector<Pos> res;
  for (int i = 0; i < board_rows(); ++i){
    for (int j = 0; j < board_cols(); ++j) {
      if (is_good_pos_to_regen_gold(Pos(i,j))) res.push_back(Pos(i,j));
    }
  }
  
  if (res.size() != 0)  return res[random(0,res.size()-1)];
  else                  return get_empty_pos();
}

// It has to be inside cell but not in the border
bool Board::pos_ok_for_gold (const Pos& p) const {
  return p.i >= 1 and p.j >= 1 and p.i <= (int)grid.size() - 2 and p.j <= (int)grid.size() - 2;
}

bool Board::is_good_pos_to_regen_gold (const Pos& p) const {

  if (not grid[p.i][p.j].is_empty()) return false;
  if (not pos_ok_for_gold(p)) return false;
    
  
  for (int i = p.i - 2; i <= p.i + 2; ++i) { // Check nobody is nearby and -2x2 is inside
    for (int j = p.j - 2; j <= p.j + 2; ++j) {
      if (not pos_ok(Pos(i,j))) return false;
      if (grid[i][j].type == Wall or grid[i][j].id != -1 or grid[i][j].health_kit or grid[i][j].poison_kit or
				grid[i][j].box != -1 or grid[i][j].gold or grid[i][j].money > 0) return false;
    }
  }
  return true;
}

bool Board::execute (const Command& m,
		     vector<pair<int,int>>& money_to_regenerate, // <num items, total amount>
		     int& health_kits_to_regenerate,
		     int& poison_kits_to_regenerate,
		     vector<vector<int>>& safety_boxes_to_regenerate,
		     vector<vector<int>>& s_killed, // list of soldiers killed by each player
		     vector<vector<int>>& p_killed) { // list of professors killed by each player

  int         id     =                  m.id;
  CommandType c_type = CommandType(m.c_type);
  
  
  if (not command_type_ok(c_type)) {
    cerr << "warning: invalid command type in command: " << c_type << endl;
    return false;
  }

  if (units[id].type == Soldier and
      units[id].pos  == Pos(-1,-1)) return false;  // killed in this round
  if (units[id].type == Professor and
      units[id].rounds_for_reborn > 0) return false; // killed in this rounds
  
  if (c_type == Move) {
    Dir dir = Dir(m.dir);
    int id = m.id;
    if (not dir_ok(dir)) {
      cerr << "warning: invalid dir in command: " << dir << endl;
      return false;
    }

    Unit&      un = units[id];
    UnitType type = un.type;    
    int        pl = un.player;
    Pos        op = un.pos; // original position
    Cell&      oc = grid[op.i][op.j]; // original cell

    Pos np = op + dir; // new position
    if (not pos_ok(np)) {
      cerr << "warning: cannot move to position " << np << " out of the board." << endl;
      return false;
    }

    Cell& nc = grid[np.i][np.j]; // new cell

    if (type == Soldier) {     // Si es un soldier
      if (nc.type == Wall){     // - Si es mou cap a Wall, ignorem el moviment
	cerr << "warning: cannot move to position " << np << " with wall." << endl;
	return false;
      }
      else if (nc.is_empty()) {     // - Si es mou a una casella buida, es mou cap allà
	nc.id = id;
	un.pos = np;
	oc.id = -1;
	//	cerr << "Move soldier " << id << " to empty position " << np << endl;
      }
      else if (nc.health_kit) {
	// - Si es mou a una casella on hi ha health --> el recull i s'hi mou
	//cerr << "Move soldier " << id << " to health kit at position " << np << endl;
	nc.id = id;
	nc.health_kit = false;
	un.pos = np;
	player_health[pl] += health_kit_value();
	oc.id = -1;
	++health_kits_to_regenerate;
      }
      else if (nc.poison_kit) {
	// - Si es mou a una casella on hi ha poison --> el recull i s'hi mou
	//	cerr << "Move soldier " << id << " to poison kit at position " << np << endl;
 	nc.id = id;
	nc.poison_kit = false;
	un.pos = np;
	player_poison[pl] += poison_kit_value();
	oc.id = -1;
	++poison_kits_to_regenerate;
      }
      else if (nc.money > 0) {
	// - Si es mou a una casella on hi ha money --> el recull i s'hi mou
	//	vector<pair<int,int>> money_to_regenerate, // <num items, total amount>
	//	cerr << "Move soldier " << id << " of player " << pl << " to money at position " << np << endl;
	money_to_regenerate[pl].first++;
	money_to_regenerate[pl].second += nc.money;
	nc.id = id;
	nc.money = 0;
	un.pos = np;
	oc.id = -1;
      }
      else if (nc.box != -1) { // - Si es mou a una casella on hi ha box --> ignorem el moviment
	cerr << "Move soldier to box at position " << np << endl;
	cerr << "warning: cannot move to a safety box" << endl;       
	return false;
      }
      else if (nc.id != -1) { // Cell with unit
	//	cerr << "Move soldier " << id << " to other unit at position " << np << endl;
	Unit &au = units[nc.id]; // attacked unit
	if (au.player == pl) return false; // unit del mateix clan --> ignorem moviment
	if (au.type == Soldier) {	 
	  // - Si es mou a una casella ocupada per un soldat rival --> si ell te poison el mata i perd 1 de poison
	  //                                                           (i reapareix en un altre lloc), pero no s'hi mou
	  //                                                           si no  te poison s'ignora el moviment
	  if (un.poison == 0) return false;
	  //	  cerr << "Soldier " << au.id << " will be killed by unit " << id << " with poison " << un.poison << endl;
	  nc.id = -1;
	  au.pos = Pos(-1,-1); // mark as killed
	  --un.poison; _my_assert(un.poison >= 0, "Negative poison after attack.");
	  _my_assert(player2soldiers[au.player].count(au.id) > 0, "Cannot remove soldier from player2soldiers");
	  player2soldiers[au.player].erase(au.id);
	  s_killed[pl].push_back(au.id);		  	 
	}
	else {
	  // - Si es mou a una casella ocupada per un professor rival --> si ell te poison l'ataca. Si com a resultat
	  //                                                                      el professor mor tampoc es fa el moviment.
	  //                                                                     el professor desapareix i ja renaixera.
	  //                                                              si no te poison s'ignora el moviment
	  _my_assert(au.type == Professor, "Type should be professor.");
	  if (au.player == pl) return false;
	  if (un.poison == 0) return false;
	  --un.poison; _my_assert(un.poison >= 0, "Negative poison after attack.");
	  au.health = max(0,au.health - professor_health_decrease());
	  if (au.health == 0) { // Professor dies
	    au.rounds_for_reborn = professor_rounds_rebirth() + 1; // +1 because it will automatically be decremented
	    nc.id = -1;	    
	    p_killed[pl].push_back(au.id);
	  }	  
	}
      }
    }
    else { // Professor
      _my_assert(type == Professor, "Unexpected type");
      if (nc.type == Wall){     // - Si es mou cap a Wall, ignorem el moviment
	cerr << "warning: cannot move to position " << np << " with wall." << endl;
	return false;
      }
      else if (nc.is_empty()) {     // - Si es mou a una casella buida, es mou cap allà
	nc.id = id;
	un.pos = np;
	oc.id = -1;
      }
      else if (nc.health_kit) {
	// - Si es mou a una casella on hi ha health --> el recull i s'hi mou
	nc.id = id;
	nc.health_kit = false;
	un.pos = np;
	player_health[pl] += health_kit_value();
	oc.id = -1;
	++health_kits_to_regenerate;
      }
      else if (nc.poison_kit) {
	// - Si es mou a una casella on hi ha poison --> el recull i s'hi mou
	nc.id = id;
	nc.poison_kit = false;
	un.pos = np;
	player_poison[pl] += poison_kit_value();
	oc.id = -1;
	++poison_kits_to_regenerate;
      }
      else if (nc.money > 0) {
	// - Si es mou a una casella on hi ha money --> el recull i s'hi mou
	money_to_regenerate[pl].first++;
	money_to_regenerate[pl].second += nc.money;
	nc.id = id;
	nc.money = 0;
	un.pos = np;
	oc.id = -1;
      }
      else if (nc.box != -1) { // - Si es mou a una casella on hi ha box --> ignorem el moviment
	cerr << "warning: cannot move to a safety box" << endl;       
	return false;
      }
      else if (nc.id != -1) { // Cell with unit
	return false;
      }
    }
    // - Si es mou cap a Wall, ignorem
    // - Si es mou a una casella buida, es mou cap allà
    // - Si es mou a una casella on hi ha money --> el recull i s'hi mou
    // - Si es mou a una casella on hi ha health --> el recull i s'hi mou
    // - Si es mou a una casella on hi ha poison --> el recull i s'hi mou
    // - Si es mou a una casella on hi ha box --> ignorem el moviment
    // - Si es mou a una casella ocupada per un soldat/professor del mateix clan o d'un altre ignorarem el moviment
  }
  else if (c_type == Open) {
    Dir dir = Dir(m.dir);
    vector<int> sol = m.sol;
    Unit&      un = units[id];
    // - Si el moviment esta fet per un soldat s'ignorara
    // - Si el moviment esta fet per un professor:
    //         * si es mou cap a una Box i la solucio es la bona aleshores recull el gold i desapareix la caixa
    //           que reapareixera en acabar la ronda
    //         * en qualsevol altres cas s'ignorara
    if (un.type == Soldier) return false;
    Pos np = un.pos + dir;
    if (not pos_ok(np)) return false;
    int box_id = grid[np.i][np.j].box;
    if (box_id != -1 and check_correct_opening(hints_boxes[box_id],sol)) { 
      // Correct!
      int pl = un.player;
      safety_boxes_to_regenerate[pl].push_back(box_id);
      return true;
    }
    return false;
  }
  else if (c_type == Charge) {
    Unit&      un = units[id];
    int        pl = un.player;
    // - Si el moviment esta fet per un professor s'ignorara
    // - Si el moviment esta fet per un soldat: recarreguem fins al tope de poison restany del poison global
    if (un.type == Professor) return false;
    int amount = min(soldier_max_poisons() - un.poison, player_poison[pl]);
    un.poison += amount; 
    player_poison[pl] -= amount;
    _my_assert(player_poison[pl] >= 0, "Error in poison decrement.");
    _my_assert(un.poison <= soldier_max_poisons(), "Error in poison decrement.");
    _my_assert(player_poison[pl] == 0 or un.poison == soldier_max_poisons(), "Error-2 in poison decrement.");      
  }
  else if (c_type == Heal) {
    Unit&      un = units[id];
    int        pl = un.player;
    // - Si el moviment esta fet per un soldat s'ignorara
    // - Si el moviment esta fet per un professor: recarreguem fins al tope de health restant del health global
    if (un.type == Soldier) return false;
    int amount = min(professor_max_health() - un.health, player_health[pl]);
    un.health += amount; 
    player_health[pl] -= amount;
    _my_assert(player_health[pl] >= 0, "Error in health decrement.");
    _my_assert(un.health <= professor_max_health(), "Error in health decrement.");
    _my_assert(player_health[pl] == 0 or un.health == professor_max_health(), "Error-2 in health decrement.");      
  }
  else {
    _my_assert(false,"Unexpected type of command");
  }
  return true;
}

void Board::regenerate_health (int n) {
  for (int i = 0; i < n; ++i) {
    Pos p =  get_random_pos_where_regenerate( );
    Cell& c = grid[p.i][p.j];
    _my_assert(c.is_empty(), "Generated money in already full cell");
    c.health_kit = true;
  }
}

void Board::regenerate_poison (int n) {
  for (int i = 0; i < n; ++i) {
    Pos p =  get_random_pos_where_regenerate( );
    Cell& c = grid[p.i][p.j];
    _my_assert(c.is_empty(), "Generated money in already full cell");
    c.poison_kit = true;
  }
}

void Board::regenerate_money (vector<pair<int,int>>& money_to_regenerate) { // <num items, total amount>.
  
  int total_units = 0;
  for (auto p : money_to_regenerate) total_units += p.first;

  
  for (int i = 0; i < total_units; ++i) {
    Pos p =  get_random_pos_where_regenerate( );
    Cell& c = grid[p.i][p.j];
    _my_assert(c.is_empty(), "Generated money in already full cell");
    int value = random(min_money_value(),max_money_value());
    c.money = value;
  }
}

void Board::update_score ( const vector<pair<int,int>>& money, // <units,amount>
		      const vector<vector<int>>& s_killed,
		      const vector<vector<int>>& p_killed,
		      const vector<vector<int>>& boxes
			   ) {
  for (int pl = 0; pl < num_players(); ++pl) {
    scr[pl] += money[pl].second; // money
    scr[pl] += s_killed[pl].size() * points_for_killing_soldier();
    scr[pl] += p_killed[pl].size() * points_for_killing_professor();
    scr[pl] += boxes[pl].size() * gold_value();
  }
}


void Board::next (const vector<Action>& act, ostream& os) {
  _my_assert(ok(), "Invariants are not satisfied.");

   int npl = num_players();
   _my_assert(int(act.size()) == npl, "Size should be number of players.");

  // Elements to be regenerated
   vector<pair<int,int>> money_to_regenerate(num_players(),{0,0}); // <num items, total amount>. This allows us to regenerate money at the end of the round and update the score
   int health_kits_to_regenerate = 0;
   int poison_kits_to_regenerate = 0;
   vector<vector<int>> safety_boxes_to_regenerate(num_players(),vector<int>()); // during next we will already delete the box. At the end, update scores and regenerate
   vector<vector<int>> s_killed(num_players()); // list of units of type soldier killed by each player
   vector<vector<int>> p_killed(num_players()); // list of units of type professor killed by each player

   // Chooses (at most) one command per unit.
  set<int> seen;
  vector<vector<Command>> v(npl);
  for (int pl = 0; pl < npl; ++pl)
    for (const Command& m : act[pl].v) {
      int id     = m.id;
      int c_type = m.c_type;
      int dir    = m.dir;
      const vector<int>& s = m.sol;

      if (id < 0 or id >= int(units.size()))
	cerr << "warning: unit " << id << " does not exist" << endl;
      else {
	auto it = units[id];
	
	if (it.player != pl){
	  cerr << "warning: unit " << id << " of player " << it.player << " not owned by " << pl << endl;
	}
	else {
	  // Here an assert as repetitions should have already been filtered out.
	  _my_assert(not seen.count(id), "More than one command for the same unit.");
	  seen.insert(id);
	  
	  v[pl].push_back(Command(id, c_type, dir, s));
	}
      }
    }

  // Makes all players' commands using a random order,
  // but respecting the relative order of the units of the same player.
  // Permutations are not equally likely to avoid favoring leading clans.
  int num = 0; // Counts number of pending commands
  for (int pl = 0; pl < npl; ++pl) num += v[pl].size();

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
    if (execute(m,
		money_to_regenerate, health_kits_to_regenerate, poison_kits_to_regenerate, safety_boxes_to_regenerate,
		s_killed, p_killed))
      commands_done.push_back(m);
   }

  os << "commands" << endl;
  Action::print(commands_done, os);

  decrement_rounds_and_regenerate_professors();

  regenerate_killed_soldiers(s_killed);

  regenerate_money(money_to_regenerate);
  
  regenerate_health(health_kits_to_regenerate);

  regenerate_poison(poison_kits_to_regenerate);

  regenerate_safety_boxes(safety_boxes_to_regenerate); // PENDING
    
  update_score(money_to_regenerate,s_killed,p_killed,safety_boxes_to_regenerate); 

  ++rnd;
  
  //   _my_assert(ok(), "Invariants are not satisfied.");

  
}

void Board::regenerate_safety_boxes (const vector<vector<int>>& boxes_to_regen ) {
  vector<Dir> dirs = {Down,Right,Up,Left};

  // First remove all of them
  for (int pl = 0; pl < num_players(); ++pl) {
    for (int box_id : boxes_to_regen[pl]) {
      Pos pos_door = pos_boxes[box_id];
      Pos pos_gold = {-1,1};
      for (auto d : dirs) {
	Pos np = pos_door + d;
	if (grid[np.i][np.j].gold) {
	  pos_gold = np;
	  break;
	}
      }
      _my_assert(pos_gold != Pos(-1,1), "Did not find gold close to the door.");
      grid[pos_gold.i][pos_gold.j].gold = false;
      for (int i = -1; i <= 1; ++i) {
	for (int j = -1; j <= 1; ++j) {
	  Pos np = {pos_gold.i + i, pos_gold.j + j};
	  grid[np.i][np.j].type = Corridor;
	  grid[np.i][np.j].box = -1;
	  grid[np.i][np.j].gold = false;
	}
      }
    }
  }
  
  // Now place them on the board with new position and new hints
  for (int pl = 0; pl < num_players(); ++pl) {
    for (int box_id : boxes_to_regen[pl]) {
      Pos p = get_random_pos_where_regenerate_gold( );
      create_new_safety_box(p,box_id);
    }
  }

}

void Board::regenerate_killed_soldiers (const vector<vector<int>>& s_killed) {// list of units of type soldier killed by each player)
  
  for (int pl = 0; pl < num_players(); ++pl) {
    for (int id : s_killed[pl]) { // Regenerate soldier with identifier id
      Pos p =  get_random_pos_where_regenerate( );
      vector<int> players = {0,1,2,3};
      for (int k = 0; k < 6; ++k) players.push_back(pl); // 7 copies of players, 1 of others
      // Hence 70% to me, 10% to each of the others
      int pl = players[random(0,9)];
      Cell& c = grid[p.i][p.j];
      _my_assert(c.is_empty(),"Pos to regenerate should be empty.");
      
      c.id = id;
      
      Unit& u = units[id];
      u.player = pl;
      u.pos = p;
      u.poison = soldier_ini_poisons();      
      player2soldiers[pl].insert(id);
      _my_assert(u.health == -1 and u.rounds_for_reborn == -1, "Wrong data when regenerating soldier.");
    }
  }
}

void Board::decrement_rounds_and_regenerate_professors() {
    for (int pl = 0; pl < num_players(); ++pl){
    if (units[player2professor[pl]].rounds_for_reborn > 1)
      --units[player2professor[pl]].rounds_for_reborn;
    else if (units[player2professor[pl]].rounds_for_reborn == 1) // recreate
      regenerate_professor(pl);
  }
}

vector<Dir> Board::dir_permutation ( ) {
  vector<Dir> dirs = {Up, Down, Left, Right};
  vector<int> p = random_permutation(4);
  vector<Dir> new_dirs(4);
  for (int i = 0; i < 4; ++i) new_dirs[i] = dirs[p[i]];
  return new_dirs;
}

void Board::regenerate_professor (int pl) {

  Pos p =  get_random_pos_where_regenerate( );
  Cell& c = grid[p.i][p.j];
  _my_assert(c.is_empty(), "Generated professor in already full cell");

  c.id = player2professor[pl];
  
  Unit& u = units[c.id];
  u.pos = p;
  u.health = professor_max_health();
  u.rounds_for_reborn = 0;
  _my_assert(u.poison == -1, "Wrong data when regenerating professor.");
}


//************************************************************
//                     BOARD GENERATION
//************************************************************

int Board::generate_wall (int s_id, int length) {
  int filled = 0;
  vector<Dir> dirs = {Up, Down, Left, Right};
  Dir last_dir = dirs[random(0,dirs.size()-1)];
  Pos p = get_ok_pos_for_initial_wall();
  wall_plan[p.i][p.j] = s_id;
  ++filled;
  while (length > 0) {
    //random_shuffle(dirs.begin(),dirs.end()); // Careful: this does not behave the same in MAC
    dirs = dir_permutation();
    Dir new_possible_dir = Up; // Explore the possibility of turning
    bool dir_found = false;
    for (auto& d:dirs) {
      if (pos_ok_for_wall(s_id,p+d)){
	new_possible_dir = d;
	dir_found = true;
	break;
      }
    }
    
    if (random(1,8) != 1 and pos_ok_for_wall(s_id,p+last_dir)){ // Continue same direction
      p += last_dir;
      wall_plan[p.i][p.j] = s_id;
      --length;
      ++filled;
    }
    else if (dir_found){ // Turn
      last_dir = new_possible_dir;
      _my_assert(cell(p+new_possible_dir).is_empty(), "Cell no empty");
      p += new_possible_dir;
      _my_assert(pos_ok_for_wall(s_id,p), "Pos not ok for street.");
      wall_plan[p.i][p.j] = s_id;
      --length;
      ++filled;
    }
    else // Stop
      return filled;
  }
  return filled;
}

void Board::generate_all_walls (int num_wall_cells, int num_walls){
  wall_plan = vector<vector<int>>(board_rows(),vector<int>(board_cols(),0));
  
  int num_walls_pending = num_walls;
  while (num_walls_pending > 0) {
    int length;
    if (num_walls_pending != 1) length = num_wall_cells/num_walls_pending;
    else length = num_wall_cells;
    num_wall_cells -= generate_wall(num_walls_pending,length);
    --num_walls_pending;
  }

  for (int i = 0; i < board_rows(); ++i) {
    for (int j = 0; j < board_cols(); ++j){
      if (wall_plan[i][j] != 0) {
	grid[i][j].type = Wall;
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

Pos Board::get_ok_pos_for_wall(int s_id) {
  while (true) {
    int i = random(1,board_rows()-2);
    int j = random(1,board_cols()-2);
    if (pos_ok_for_wall(s_id, Pos(i,j))) return Pos(i,j);
  }
}

Pos Board::get_ok_pos_for_initial_wall() {
  while (true) {
    int i = random(1,board_rows()-2);
    int j = random(1,board_cols()-2);
    if (pos_ok_for_initial_wall(Pos(i,j))) return Pos(i,j);
  }
}

bool Board::pos_ok_for_wall(int s_id, const Pos& p){
  int i = p.i, j = p.j;
  if (not pos_ok(p)) return false;
  if (wall_plan[i][j] != 0) return false;
  if (i == 0) return false;
  if (i == board_rows() -1) return false;
  if (j == 0) return false;
  if (j == board_cols() -1) return false;


  
  int num_occupied = 0;
  vector<Dir> dirs = {Up, Down, Left, Right};
  for (auto& d: dirs) {
    Pos newPos = p + d; // Will exists because p is not on a border
    int ni = newPos.i, nj = newPos.j;
    if (wall_plan[ni][nj] != 0 and wall_plan[ni][nj] != s_id) return false;
    else if (wall_plan[ni][nj] == s_id) ++num_occupied;
  }

  vector<pair<int,int>> diags = {{1,1},{1,-1},{-1,1},{-1,-1}};
  for (auto& d : diags) {
    Pos newPos = Pos(p.i + d.first, p.j + d.second);
    int ni = newPos.i, nj = newPos.j;
    if (wall_plan[ni][nj] != 0 and wall_plan[ni][nj] != s_id) return false;
    else if (wall_plan[ni][nj] == s_id) ++num_occupied;
  }


  return num_occupied <= 2;
}

bool Board::pos_ok_for_initial_wall(const Pos& p){
  int i = p.i, j = p.j;
  if (not pos_ok(p)) return false;
  if (wall_plan[i][j] != 0) return false;

  if (i == 0) return false;
  if (i == board_rows() -1) return false;
  if (j == 0) return false;
  if (j == board_cols() -1) return false;

  int num_occupied = 0;
  vector<Dir> dirs = {Up, Down, Left, Right};
  for (auto& d: dirs) {
    Pos newPos = p + d; // Will exists because p is not on a border
    int n_i = newPos.i, n_j = newPos.j;
    if (wall_plan[n_i][n_j] != 0) return false;
  }

  vector<pair<int,int>> diags = {{1,1},{1,-1},{-1,1},{-1,-1}};
  for (auto& d : diags) {
    Pos newPos = Pos(p.i + d.first, p.j + d.second);
    int n_i = newPos.i, n_j = newPos.j;
    if (wall_plan[n_i][n_j] != 0) return false;
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

  for (int i = 0; i < board_rows(); ++i) 
    for (int j = 0; j < board_cols(); ++j)
      if (grid[i][j].type == Wall) G[i][j] = -2;

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

void Board::create_new_professor (Pos& p, int player) {
  int id = fresh_id;
  ++fresh_id;

  units.push_back(Unit(Professor, id, player, p, professor_ini_health(), -1, 0));
  _my_assert((int)units.size() - 1  == id, "Wrong unit id usage for professors");

  player2professor[player] = id;
  
  _my_assert(grid[p.i][p.j].is_empty(),        "Cell is already full.");

  grid[p.i][p.j].id = id;
}

void Board::create_new_soldier (Pos& p, int player) {
  int id = fresh_id;
  ++fresh_id;

  units.push_back(Unit(Soldier, id, player, p, -1, soldier_ini_poisons(), -1));
  _my_assert((int)units.size() - 1  == id, "Wrong unit id usage for soldiers");

  _my_assert(player2soldiers[player].count(id) == 0, "Id already used in soldiers");
  player2soldiers[player].insert(id);
  
  _my_assert(grid[p.i][p.j].is_empty(),        "Cell is already full.");

  grid[p.i][p.j].id = id;
}

// p is the position of the gold (central)
void Board::create_new_safety_box (Pos p, int id) {
  for (int i = p.i - 1; i <= p.i + 1; ++i) {
    for (int j = p.j - 1; j <= p.j + 1; ++j) {
      if (i == p.i and j == p.j) continue;
      grid[i][j].type = Wall;
    }
  }

  Pos pos_door = p;
  int p_door = random(0,3);
  if (p_door == 0)      ++pos_door.i;
  else if (p_door == 1) --pos_door.i;
  else if (p_door == 2) ++pos_door.j;
  else                  --pos_door.j;

  grid[p.i][p.j].gold = true;
  grid[pos_door.i][pos_door.j].type = Corridor;
  grid[pos_door.i][pos_door.j].box = id;
  pos_boxes[id] = pos_door;
}

bool Board::good_triplet (int x, int y, int z) {
  return (x+y == z or x+z == y or y+z == x);
}


bool Board::check_correct_opening (const vector<int>& v,
				   const vector<int>& sol) {
  if (sol.size() != v.size()) return false;
  int n = v.size()/3; // n subsets
  vector<vector<int>> subsets(n,vector<int>());
  for (uint i = 0; i < sol.size(); ++i) {
    if (sol[i] < 0 or sol[i] >= n) return false;
    subsets[sol[i]].push_back(v[i]);
  }
  
  for (auto& s : subsets) {
    if (s.size() != 3) return false;
    if (not good_triplet(s[0],s[1],s[2])) return false;
  }
  return true;
}


vector<int> Board::generate_one_hint ( ){
  _my_assert(size_hints_per_box()%3 == 0, "num_hints_per_box should be a multiple of 3.");
  int n = size_hints_per_box()/3; // we have 3n hints, hence n subsets of size 3
  vector<int> res;
  for (int i = 0; i < 2*n; ++i) res.push_back(random(-10,10));
  for (int i = 0; i < 2*n; i += 2) res.push_back(res[i]+res[i+1]);
  // Random shuffle
  vector<int> aux = random_permutation(res.size());
  vector<int> res_shuff(res.size());
  for (uint i = 0; i < aux.size(); ++i) res_shuff[aux[i]] = res[i];
  return res_shuff;

}
void Board::generate_random_board ( ){
  int rows = board_rows();
  int cols = board_cols();
  
  // Generate corridors (leaving space for citizens)  
  static const int num_wall_cells = 0.5*rows*cols; // goal of 50% walls
  static const int num_walls = 8;

  do {
    // Create grid
    grid = vector<vector<Cell>>(rows, vector<Cell>(cols));
    generate_all_walls(num_wall_cells, num_walls);
  } while (num_connected_components() != 1);

  for (int i = 0; i < board_rows(); ++i) 
    for (int j = 0; j < board_cols(); ++j)
      if (wall_plan[i][j] != 0) 
	grid[i][j].type = Wall;

  // Generate safety boxes
  hints_boxes = vector<vector<int>>(num_safety_boxes(),vector<int>());
  for (auto& v : hints_boxes) {
    v = generate_one_hint();
  }

  // Place them on the board
  pos_boxes = vector<Pos>(num_safety_boxes());
  
  for (int i = 0; i < num_safety_boxes(); ++i) {
    Pos p = get_random_pos_where_regenerate_gold( );
    create_new_safety_box(p,i);
  }
  
  // Generate professors
  for (int pl = 0; pl < num_players(); ++pl) {
    Pos p = get_random_pos_where_regenerate();
    create_new_professor(p,pl);
  }

  // Generate soldiers
  for (int pl = 0; pl < num_players(); ++pl) {
    for (int i = 0; i < num_ini_soldiers_per_clan(); ++i) {
      Pos p = get_random_pos_where_regenerate( );
      create_new_soldier(p,pl);
    }
  }
  
  // Generate money
  for (int i = 0; i < num_money(); ++i) {
    Pos p = get_random_pos_where_regenerate( );
    grid[p.i][p.j].money = random(min_money_value(),max_money_value());
  }

  // Generate poison
  for (int i = 0; i < num_poison_kits(); ++i) {
    Pos p = get_random_pos_where_regenerate( );
    grid[p.i][p.j].poison_kit = true;
  }

  // Generate health
  for (int i = 0; i < num_health_kits(); ++i) {
    Pos p = get_random_pos_where_regenerate( );
    grid[p.i][p.j].health_kit = true;
  }

  
}

