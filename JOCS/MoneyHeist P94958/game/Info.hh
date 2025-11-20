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
    //                       owner  id    food
    Cell cell; // (Corridor, -1,    -1,   false) by default
    switch (c) {
    case '.':
      cell = Cell(Corridor,-1,false,false,-1,false,0);
      break; // empty cell
    case 'W':
      cell = Cell(Wall,-1,false,false,-1,false,0);
      break;
    case '0':
      cell = Cell(Corridor,-1,false,false,-1,false,0); // Do not know id yet
      break;
    case '1':
      cell = Cell(Corridor,-1,false,false,-1,false,0); // Do not know id yet
      break;
    case '2':
      cell = Cell(Corridor,-1,false,false,-1,false,0); // Do not know id yet
      break;
    case '3':
      cell = Cell(Corridor,-1,false,false,-1,false,0); // Do not know id yet
      break;
    case 'M':
      cell = Cell(Corridor,-1,false,false,-1,false,0); // Do not know yet amount of money
      break;
    case 'G':
      cell = Cell(Corridor,-1,false,false,-1,true,0);
      break;
    case 'P':
      cell = Cell(Corridor,-1,false,true,-1,false,0);
      break;
    case 'B':
      cell = Cell(Corridor,-1,false,false,-1,false,0); // Do not know yet which box id
      break;
    case 'H':
      cell = Cell(Corridor,-1,true,false,-1,false,0);
      break;
    case 'X':
      cell = Cell(Corridor,-1,false,false,-1,false,0); // Do not know id yet
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
    // Read grid with corridors and walls
    string l;
    is >> l >> l; // Read 1st and 2nd line of column labels.
    
    grid = vector< vector<Cell> >(board_rows(), vector<Cell>(board_cols()));

    for (int i = 0; i < board_rows(); ++i) {
      string s;
      is >> l >> s;      // Read row label in l and row in s.
      _my_assert((int)s.size() == board_cols(),
                 "The read map has a line with incorrect lenght.");
      for (int j = 0; j < board_cols(); ++j) {
	grid[i][j] = char2Cell(s[j]);
      }
    }
    
    // Read units
    is >> l; _my_assert(l == "units", "Expected *units* in grid format");
    int num; is >> num; // Read number of units
    is >> l >> l >> l >> l >> l >> l >> l >> l; // Read type id player row colum health poison rounds
    for (int i = 0; i < num; ++i) {
      char type;
      int id, pl, row, col, health, poison, rounds;
      is >> type >> id >> pl >> row >> col >> health >> poison >> rounds;
      _my_assert(pos_ok(row,col), "Unit placed out of board");
      units.push_back(Unit(UnitType(char2UnitType(type)),id,pl,Pos(row,col),health,poison,rounds));
      _my_assert(id < int(units.size()) and units[id].id == id, "Problem reading units"); // I assume that the units are always fixed, from 0 no N-1 and are written/read in order
      if (type != 'p' or rounds == 0) { // Dead professor do not update the grid
	_my_assert(grid[row][col].is_empty(), "Unit " + to_string(id) + " placed in non-empty cell " + to_string(row) + " " + to_string(col) + " that has " + to_string(grid[row][col].type) + " "  + to_string(grid[row][col].id) + " "  + to_string(grid[row][col].health_kit) + " "  + to_string(grid[row][col].poison_kit) + " "  + to_string(grid[row][col].box) + " "  + to_string(grid[row][col].money));  
	grid[row][col].id = id;
      }

      if      (type == 's') player2soldiers[pl].insert(id);
      else if (type == 'p') player2professor[pl] = id;
      else if (type != 's' and type != 'p') _my_assert(false, "Wrong type of unit " + string(1,type) + " in grid format");
    }
    
    // Read money
    is >> l; _my_assert(l == "money", "Expected 'money' in grid format");
    is >> num; _my_assert(num == num_money(), "Number money in grid should be equal to num_money()");
    is >> l >> l >> l; // Read "row column amount"
    for (int i = 0; i < num; ++i) {
      int row, col, amount;
      is >> row >> col >> amount;
      _my_assert(pos_ok(row,col), "Money placed out of board");
      _my_assert(grid[row][col].is_empty(),
		 "Money placed in non-empty cell");
      grid[row][col].money = amount;
    }

    // Read safety box hints
    is >> l; _my_assert(l == "safety_box_hints", "Expected 'safety_box_hints' in grid format");
    is >> num; _my_assert(num == num_safety_boxes(), "Number safety_box_hints should be equal to num_safety_boxes()");
    hints_boxes.clear();
    pos_boxes.clear();
    for (int i = 0; i < num; ++i) {
      int s, row, col;
      is >> s >> row >> col;
      hints_boxes.push_back({});
      pos_boxes.push_back(Pos(row,col));
      grid[row][col].box = i;
      for (int j = 0; j < s; ++j) {
	int x; is >> x;
	hints_boxes[i].push_back(x);
      }
    }
  }
  
  /**
   * Checks invariants are preserved.
   */
  bool ok() const;
};


#endif
