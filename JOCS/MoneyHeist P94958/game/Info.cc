//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////  

#include "Info.hh"

bool Info::ok() const {

  if (int(grid.size()) != board_rows()) {
    cerr << "error: mismatch in number of rows" << endl;
    return false;
  }

  if (int(grid[0].size()) != board_cols()) {
    cerr << "error: mismatch in number of columns" << endl;
    return false;
  }

  if (int(scr.size()) != num_players()) {
    cerr << "error: size of scr should be number of players" << endl;
    return false;
  }
  
  if (int(player_health.size()) != num_players()) {
    cerr << "error: size of player_health should be number of players" << endl;
    return false;
  }

  for (int pl = 0; pl < num_players(); ++pl) {
    if (player_health[pl] < 0) {
      cerr << "error: player_health of player " << pl << " should be >= 0" << endl;
      return false;
    }
  }
  
  if (int(player_poison.size()) != num_players()) {
    cerr << "error: size of player_poison should be number of players" << endl;
    return false;
  }

  for (int pl = 0; pl < num_players(); ++pl) {
    if (player_poison[pl] < 0) {
      cerr << "error: player_poison of player " << pl << " should be >= 0" << endl;
      return false;
    }
  }

  if (int(stats.size()) != num_players()) {
    cerr << "error: size of stats should be number of players" << endl;
    return false;
  }

  for (int pl = 0; pl < num_players(); ++pl) {
    double st = stats[pl];
    if (st != -1 and not (0 <= st and st <= 1)) {
      cerr << "error: status should be -1 or within [0, 1]" << endl;
      return false;
    }
  }

  if (not (rnd >= 0 and rnd <= num_rounds())) {
    cerr << "error: wrong number of rounds" << endl;
    return false;
  }

  if (int(player2soldiers.size()) != num_players()) {
    cerr << "error: size of player2soldiers should be number of players" << endl;
    return false;
  }

  if (int(player2professor.size()) != num_players()) {
    cerr << "error: size of professors should be number of players" << endl;
    return false;
  }

  uint expected_units = num_players() * (num_ini_soldiers_per_clan() + 1); // also count professors
  if (units.size() != expected_units) {
    cerr << "error: units has a wrong number of units" << endl;
    return false;
  }

  if (int(hints_boxes.size()) != num_safety_boxes()) {
    cerr << "error: size of hint_boxes should be num_safety_boxes()" << endl;
    return false;
  }

  if (hints_boxes.size() != pos_boxes.size()) {
    cerr << "error: size of hint_boxes should be equal to the size of pos_boxes" << endl;
    return false;
  }

  for (auto x : hints_boxes) {
    if (int(x.size()) != size_hints_per_box()) {
      cerr << "error: size of hints per box should be equal to size_hints_per_box()" << endl;
      return false;
    }
  }
  
  vector<int> tmp_nb_cells(num_players(), 0);
  int total_soldiers_in_cells = 0;

  int total_health_kits = 0;
  int total_poison_kits = 0;
  int total_money_units = 0;
  int total_gold_units = 0;
  int total_box_doors = 0;

  vector<Dir> dirs = {Up,Down,Left,Right};
  
  for (int i = 0; i < board_rows(); ++i) {
    for (int j = 0; j < board_cols(); ++j) {
      const Cell& c = grid[i][j];

      if (c.health_kit)  ++total_health_kits;
      if (c.poison_kit)  ++total_poison_kits;
      if (c.money > 0)   ++total_money_units;
      if (c.box != -1) {
        ++total_box_doors;

	// SHOULD CHECK IT IS WELL-CONSTRUCTED AND SOMETHING ELSE BELOW
	// int found_gold = 0;
	// for (auto d : dirs) {
	//   Pos np = Pos(i,j) + d;
	//   if (pos_ok(np) and grid[np.i][np.j].gold) ++found_gold;
	// }
	// if (found_gold != 1) {
	//   cerr << "error: safety box at position (" << i << "," << j << ") not well constructed. Found " << found_gold << "gold items nearby" << endl;
	//   return false;
	// }	
      }
      if (c.gold)        ++total_gold_units;
      
      if (c.type == Wall) {
	if (c.id != -1) {
	  cerr << "error: wall cells cannot have a unit on them" << endl;
	  return false;
	}
	if (c.health_kit) {
	  cerr << "error: wall cells cannot have health_kits on them" << endl;
	  return false;
	}
	if (c.poison_kit) {
	  cerr << "error: wall cells cannot have poison_kits on them" << endl;
	  return false;
	}
	if (c.box != -1) {
	  cerr << "error: wall cells cannot have safety boxes on them" << endl;
	  return false;
	}
	if (c.money > 0) {
	  cerr << "error: wall cells cannot have money" << endl;
	  return false;
	}
      }
      else if (c.type == Corridor) {
	if ((c.health_kit or c.poison_kit or c.box > 0 or c.money > 0 or c.gold) and c.id != -1) {
	  cerr << "error: corridor cells cannot have units and (healths, poison, boxes or money) on them" << endl;
	  return false;
	}

	if (int(c.health_kit) + int(c.poison_kit) + int(c.box != -1) + int(c.money > 0) + int(c.gold) > 1) {
	  cerr << "error: corridor cells can have at most one of: health_kit, poison_kit, box, money, gold on them" << endl;
	  return false;
	}
	
	if (c.id != -1) { // Some unit on the cell
	  if (not unit_ok(c.id)) {
            cerr << "error: could not find unit identifier " << c.id << endl;
            return false;
          }
	  
	  const Unit& u = units[c.id];
          if (u.pos != Pos(i, j)) {
            cerr << "error: mismatch in the position of unit " << c.id << endl;
	    cerr << "unit says it is at " << u.pos << endl;
	    cerr << "but cell " << Pos(i,j) << " says it is there" << endl;
            return false;
          }
	  if (u.id != c.id) {
	    cerr << "error: mismatch in the identifier of unit at pos " << Pos(i,j) << endl;
	    return false;
	  }

	  if (u.type == Professor and u.rounds_for_reborn == 0 and player2professor[u.player] != u.id) {
	    cerr << "error: professor at position " << Pos(i,j) << " is not in professor vector" << endl;
	    return false;
	  }

	  if (u.type == Soldier and player2soldiers[u.player].count(u.id) == 0) {
	    cerr << "error: soldier at posision " << Pos(i,j) << " is not in player2soldiers" << endl;
	    return false;
	  }

	  if (u.type == Soldier) ++total_soldiers_in_cells;
	}       
      }
      else {
        cerr << "error: cells should be either Corridor or Wall" << endl;
        return false;	
      }
    }
  }

  if ( total_soldiers_in_cells != num_players() * num_ini_soldiers_per_clan() ) {
    cerr << "error: wrong number of soldiers in the grid" << endl;
    return false;
  }

  if (total_health_kits != num_health_kits()) {
    cerr << "error: wrong number of health kits on the board" << endl;
    return false;
  }

  if (total_poison_kits != num_poison_kits()) {
    cerr << "error: wrong number of poison kits on the board" << endl;
    return false;
  }

  if (total_money_units != num_money()) {
    cerr << "error: wrong number of money units on the board" << endl;
    return false;
  }

  if (total_gold_units != num_safety_boxes()) {
    cerr << "error: wrong number of gold units on the board" << endl;
    return false;
  }
  
  if (total_box_doors != num_safety_boxes()) {
    cerr << "error: wrong number of box doors on the board" << endl;
    return false;
  }


  uint tmp_units = 0;
  for (auto& x : player2soldiers) tmp_units += x.size();
  tmp_units += player2professor.size();

  if (tmp_units != expected_units) {
    cerr << "error: wrong number of units in player2soldiers + professor" << endl;
    return false;
  }
  
  for (const auto& u : units) {
    if (not player_ok(u.player)) {
      cerr << "error: wrong player identifier" << endl;
      return false;
    }
    
    if (u.id < 0 or u.id >= int(expected_units)){
      cerr << "error: wrong identifier for unit" << endl;
      return false;
    }


    if (u.type == Soldier) {
      if (u.health != -1) {
	cerr << "error: soldiers should have health -1" << endl;
	return false;
      }
      if (u.rounds_for_reborn != -1) {
	cerr << "error: soldiers should have rounds_for_reborn -1" << endl;
	return false;
      }
    }

    if (u.type == Professor) {
      if (u.rounds_for_reborn < 0) {
	cerr << "error: professor cannot have negative rounds_for_reborn" << endl;
	return false;
      }
      if (u.poison != -1) {
	cerr << "error: professors should have poison -1" << endl;
	return false;
      }
    }
  }
  return true;
}
