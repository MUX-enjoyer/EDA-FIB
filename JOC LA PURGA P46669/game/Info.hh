//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////  

#ifndef Info_hh
#define Info_hh


#include "Settings.hh"
#include "State.hh"


/**
 * Contains a class to store most of the information of the game.
 */


/**
 * Stores all the information of the game,
 * except the vector of names and the random generator of the board.
 */
class Info : public Settings, public State {

  friend class Game;
  friend class SecGame;

public:

  /**
   * Returns the cell defined by the char c.
   */
  inline static Cell char2Cell (char c) {
    //                                       resistance, b_owner, id (of citizen)
    Cell cell; // (Street, NoBonus, NoWeapon, -1,        -1,      -1             ) by default
    switch (c) {
    case '.':
      break; // empty cell
    case 'B':
      cell.type = Building;
      break;
    case 'G':
      cell.weapon = Gun;
      break;
    case 'Z':
      cell.weapon = Bazooka;
      break;
    case 'M':
      cell.bonus = Money;
      break;
    case 'F':
      cell.bonus = Food;
      break;
    case 'C':
      break;
    case 'c':
      break;
    case 'W':
      break;
    case 'w':
      break;
    case 'b':
      break;      
    default:
      _my_assert(false, string(1, c) + " in grid definition.");
    }
    return cell;
  }
  
  /**
   * Reads the grid of the board.
   * Should fill the same data structures as a board generator.
   */
  void read_grid (istream& is) {

    // Read grid with streets, buildings, food, money and weapons
    string l;
    is >> l >> l; // Read 1st and 2nd line of column labels.
    grid = vector< vector<Cell> >(board_rows(), vector<Cell>(board_cols()));
    
    for (int i = 0; i < board_rows(); ++i) {
      string s;
      is >> l >> s;      // Read row label in l and row in s.
      _my_assert((int)s.size() == board_cols(),
                 "The read map has a line with incorrect lenght.");
      for (int j = 0; j < board_cols(); ++j)
        grid[i][j] = char2Cell(s[j]);
    }


    // Read citizens
    is >> l; _my_assert(l == "citizens", "Expected citizens in grid format");
    int num; is >> num; // Read number of citizens
    is >> l >> l >> l >> l >> l >> l >> l; // Read "type id player .... life"
    for (int i = 0; i < num; ++i) {
      char type, weapon;
      int id, pl, row, col, life;
      is >> type >> id >> pl >> row >> col >> weapon >> life;
      _my_assert(pos_ok(row,col), "Citizen placed out of board");
      citizens[id] = Citizen(CitizenType(char2CitizenType(type)),id,pl,Pos(row,col),WeaponType(char2WeaponType(weapon)),life);
      _my_assert(grid[row][col].is_empty(), "Citizen placed in non-empty cell");
      grid[row][col].id = id;
      if (type == 'b') player2builders[pl].insert(id);
      else {
	_my_assert(type == 'w', "Wrong type of citizen in grid format");
	player2warriors[pl].insert(id);
      }
    }

    // Read barricades
    is >> l; _my_assert(l == "barricades", "Expected barricades in grid format");
    is >> num;
    is >> l >> l >> l >> l; // Read "player row column resistance"
    for (int i = 0; i < num; ++i) {
      int pl, row, col, resist;
      is >> pl >> row >> col >> resist;
      _my_assert(pos_ok(row,col), "Barricade placed out of board");
      _my_assert(grid[row][col].is_empty() or grid[row][col].id != -1,
		 "Barricade placed in non-empty cell");
      grid[row][col].resistance = resist;
      grid[row][col].b_owner = pl;
      player2barricades[pl].insert(Pos(row,col));
    }
  }
  
  /**
   * Checks invariants are preserved.
   */
  bool ok() const;
};


#endif
