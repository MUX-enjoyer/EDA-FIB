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

  for (int i = 0; i < board_rows(); ++i) {
    for (int j = 0; j < board_cols(); ++j) {
      const Cell& c = grid[i][j];
      if (c.type == Building) {
        if (c.bonus != NoBonus) {
          cerr << "error: building cells cannot have bonus" << endl;
          return false;
        }
        if (c.weapon != NoWeapon) {
          cerr << "error: building cells cannot have weapons" << endl;
          return false;
        }
        if (c.resistance != -1) {
          cerr << "error: building cells cannot have barricades" << endl;
          return false;
        }
        if (c.id != -1) {
          cerr << "error: building cells cannot have citizens" << endl;
          return false;
        }
      }
      else if (c.type == Street) {
        if (c.id != -1) { // Cell contains citizen
          auto it = citizens.find(c.id);
          if (it == citizens.end()) {
            cerr << "error: could not find citizen identifier" << endl;
            return false;
          }
	  if (c.resistance != -1 and it->second.player != c.b_owner) { // barricade and citizen
	    cerr << "error: citizen cannot stand in a rival barricade" << endl;
	    return false;
	  }

          const Citizen& ci = it->second;
          if (ci.pos != Pos(i, j)) {
            cerr << "error: mismatch in idenfiers in the grid" << endl;
            return false;
          }
	  if (c.bonus != NoBonus) {
	    cerr << "error: cell should not contain citizen and bonus" << endl;
            return false;
	  }
	  if (c.weapon != NoWeapon) {
	    cerr << "error: cell should not contain citizen and weapon" << endl;
            return false;
	  }	  
        }
	if (c.bonus != NoBonus and c.weapon != NoWeapon) {
	  cerr << "error: cell cannot have bonus and weapon" << endl;
	  return false;
	}
	if (c.resistance != -1 and c.bonus != NoBonus) {
	  cerr << "error: cell cannot have bonus and barricade" << endl;
	  return false;	  
	}
	if (c.resistance != -1 and c.weapon != NoWeapon) {
	  cerr << "error: cell cannot have weapon and barricade" << endl;
	  return false;	  
	}
        if (c.bonus < Money or c.bonus > NoBonus) {
          cerr << "error: cell contains unknown bonus" << endl;
          return false;
        }
	if (c.weapon < Hammer or c.weapon > NoWeapon) {
          cerr << "error: cell contains unknown weapon" << endl;
          return false;
        }

      }
      else {
        cerr << "error: cells should be either building or street" << endl;
        return false;
      }
    }
  }
  
  if (not (rnd >= 0 and rnd <= num_rounds())) {
    cerr << "error: wrong number of rounds" << endl;
    return false;
  }

  for (int pl = 0; pl < num_players(); ++pl) {
    double st = stats[pl];
    if (st != -1 and not (0 <= st and st <= 1)) {
      cerr << "error: status should be -1 or within [0, 1]" << endl;
      return false;
    }
  }

  vector<vector<int>> player2citizens(num_players(), vector<int>(2)); // builders and warriors
  for (const auto& p : citizens) {
    int id = p.first;
    const auto& ci = p.second;

    if (ci.type != Builder and ci.type != Warrior) {
      cerr << "error: wrong type for citizen" << endl;
      return false;
    }

    if (ci.id != id) {
      cerr << "error: mismatch in identifiers" << endl;
      return false;
    }

    if (not (player_ok(ci.player))) {
      cerr << "error: wrong player identifier" << endl;
      return false;
    }

    if (not (pos_ok(ci.pos))) {
      cerr << "error: wrong position" << endl;
      return false;
    }

    if (ci.weapon < Hammer or ci.weapon > NoWeapon) {
      cerr << "error: wrong weapon for citizen" << endl;
      return false;
    }
    
    if (ci.type == Warrior and ci.weapon == NoWeapon) {
      cerr << "error: all warriors should have a weapon" << endl;
      return false;	
    }

    if (ci.type == Builder and ci.weapon != NoWeapon) {
      cerr << "error: builders cannot have a weapon" << endl;
      return false;	
    }
    
    if (ci.life <= 0) {
      cerr << "error: citizen cannot have negative life" << endl;
      return false;
    }    
    else if (ci.life > citizen_ini_life(ci.type)) {
      cerr << "error: citizen has too large a life" << endl;
      return false;
    }

    ++player2citizens[ci.player][ci.type];
  }

  if (int(player2builders.size()) != num_players()) {
    cerr << "error: size of player2builders should be number of players" << endl;
    return false;
  }

  if (int(player2warriors.size()) != num_players()) {
    cerr << "error: size of player2warriors should be number of players" << endl;
    return false;
  }
  
  for (int pl = 0; pl < num_players(); ++pl) {

    const auto& builders = player2builders[pl];
    for (int id : builders) {
      auto it = citizens.find(id);
      if (it == citizens.end()) {
        cerr << "error: could not find identifier of builder" << endl;
        return false;
      }
      const Citizen& ci = it->second;
      if (ci.type != Builder) {
        cerr << "error: mismatch in type of builder" << endl;
        return false;
      }
      if (ci.player != pl) {
        cerr << "error: mismatch in player of builder" << endl;
        return false;
      }
    }

    if (player2citizens[pl][Builder] != int(builders.size())) {
      cerr << "error: mismatch in number of buiders" << endl;
      return false;
    }


    const auto& warriors = player2warriors[pl];
    for (int id : warriors) {
      auto it = citizens.find(id);
      if (it == citizens.end()) {
        cerr << "error: could not find identifier of warrior" << endl;
        return false;
      }
      const Citizen& ci = it->second;
      if (ci.type != Warrior) {
        cerr << "error: mismatch in type of warrior" << endl;
        return false;
      }
      if (ci.player != pl) {
        cerr << "error: mismatch in player of warrior" << endl;
        return false;
      }
    }

    if (player2citizens[pl][Warrior] != int(warriors.size())) {
      cerr << "error: mismatch in number of warriors" << endl;
      return false;
    }
  }

  // Redudancy grid and citizens
  for (const auto& p : citizens) {
    if (grid[p.second.pos.i][p.second.pos.j].id != p.first) {
      cerr << "error: citizen " << p.first << " in 'citizens' should be at position " << p.second.pos << " but is not in 'grid'" << endl;
      return false;
    }
  }

  // Redundancy grid and barricades
  for (int pl = 0; pl < num_players(); ++pl) {
    for (auto& posBar : player2barricades[pl]) {
      if (grid[posBar.i][posBar.j].resistance == -1 or
	  grid[posBar.i][posBar.j].b_owner != pl) {
	cerr << "error: position " << posBar << " is a barricade of player " << pl << " according to players2barricades but grid does say so, resistance is " << grid[posBar.i][posBar.j].resistance << " and b_owner " << grid[posBar.i][posBar.j].b_owner << endl;
	return false;
      }
    }
  }
  
  return true;
}
