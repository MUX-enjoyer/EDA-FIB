//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////  

#ifndef Board_hh
#define Board_hh


#include "Info.hh"
#include "Action.hh"
#include "Random.hh"


/**
 * Contains the Board class, with all the game information,
 * plus the names of the players and a random generator.
 */


/**
 * Manages a random generator and the information of the board.
 */
class Board : public Info, public Random_generator {

  friend class Game;
  friend class SecGame;

  vector<string>     names;
  int             fresh_id;
  bool spell_in_this_round;
  
  /**
   * Checks whether initial fixed board is ok
   */
  void check_is_good_initial_fixed_board() const;  
  
  /**
   * Reads the generator method, and generates or reads the grid.
   */
  void read_generator_and_grid (istream& is) {
    string generator;
    is >> generator;
    if (generator == "FIXED") {
      _my_assert(false,"FIXED generator not allowed");
      //read_grid(is);
      //check_is_good_initial_fixed_board();
    }
    else if (generator == "RANDOM") generate_random_board();
    else                            _my_assert(false,"unknown generator  " + generator);
  }

  /**
   * Prints some information of the unit.
   */
  inline static void print_unit (Unit& u, ostream& os) {
    os << UnitType2char(u.type)  << '\t'
       << u.id                      << '\t'
       << u.player                  << '\t'
       << u.pos.i                   << '\t'
       << u.pos.j                   << '\t'        
       << u.health                  << '\t'
       << u.poison                  << '\t'
       << u.rounds_for_reborn       << '\t'
       << endl;
  }


  /**
   * Tries to apply a move. Returns true if it could.
   */
  bool execute (const Command& m,
		vector<pair<int,int>>& money_to_regenerate,
		int& health_kits_to_regenerate,
		int& poison_kits_to_regenerate,
		vector<vector<int>>& safety_boxes_to_regenerate,
		vector<vector<int>>& s_killed,
		vector<vector<int>>& p_killed);


  /*
   * Returns whether unit u1 wins u2 in an attack
   */
  int result_attack(const Unit& u1, const Unit& u2);

  /* 
   * Perfom an attack from orig_u to dest_u and updates alive_to_dead accordingly
   */
  void perform_attack (Unit& orig_u, Unit& dest_u);
  
  /*
   * Returns whether p is an appropriate position to regenerate an object.
     Should be empty and have no unit/food in the two surrounding squares
     For gold not in the borders
   */
  bool is_good_pos_to_regen ( const Pos& p) const;
  bool is_good_pos_to_regen_gold ( const Pos& p) const;
  bool pos_ok_for_gold (const Pos& p) const;
  
  /*
   * Tries to get a random position which is good_pos_to_regen. If not, return an empty position
   * Note that there should always be empty positions in the board
   */  
  Pos get_random_pos_where_regenerate ( );
  Pos get_random_pos_where_regenerate_gold ( );
  
  /*
    Regenerates money. Vector has size num_players and elements are pairs <num items, total amount>.
  */ 
  void regenerate_money (vector<pair<int,int>>& money_to_regenerate);

  /*
    Regenerates n health_kits.
  */   
  void regenerate_health(int n);

  /*
    Regenerates n poison_kits.
  */   
  void regenerate_poison(int n);

  /*
    Regenerates safety boxes. For each player we know the list of box ids it has opened and hence should
    be regenerated
  */   
  void regenerate_safety_boxes (const vector<vector<int>>& boxes_to_regen);
  
  /*
    Regenerates killed soldiers. s_killed[pl] is the list of soldiers killed by player pl
  */
  void regenerate_killed_soldiers (const vector<vector<int>>& s_killed);
  
  /*
    Decrements rounds_for_reborn of killed professors and regenerates when necessary
   */
  void decrement_rounds_and_regenerate_professors();
  
  /*
    Regenerates the professor of player pl.
   */
  void regenerate_professor (int pl);
  
  /* 
   * Update scr vector in order to contain the score of each player
   */
  void update_score ( const vector<pair<int,int>>& money,
		      const vector<vector<int>>& s_killed,
		      const vector<vector<int>>& p_killed,
		      const vector<vector<int>>& boxes
		      );

  
  /////////////////////// BEGIN BOARD GENERATION ///////////////////////


  /**
   * Generates a board.
   */
  void generate_random_board ( );
  
  /**
   * Randomnly returns an empty pos
   */
  Pos get_empty_pos ( );

  /**
   * s_id identifies the current wall. Position should be not in border. Not already occupied by wall. 
   * Num of adjacent cells of the current wall is <= 2.
   * Num of adjacents cells of other walls is 0.
   */
  bool pos_ok_for_wall(int s_id, const Pos& p);

  /**
   * s_id identifies the current wall. Position should be not in border. Not already occupied by wall. 
   * Num of adjacent cells of some wall is 0.
   */
  bool pos_ok_for_initial_wall(const Pos& p);
    
  /**
   * Randomnly returns a position that fulfills pos_ok_for_wall(s_id)
   */
  Pos get_ok_pos_for_wall (int s_id );

  /**
   * Randomnly returns a position that fulfills pos_ok_for_initial_wall()
   */
  Pos get_ok_pos_for_initial_wall ( );


  /**
   * Tries to randomly generates num_wall_cells cells of type Wall in num_walls disconnected parts
   * Sometime it does not succeed and generates less.
   */
  void generate_all_walls (int num_wall_cells, int num_walls);

  /**
  * Tries to generate a stree of length with identifier s_id
  */
  int generate_wall (int s_id, int length);

  /**
   * Returns the number of connected component grid has (taking into account only streets and buildings)
   */
  int num_connected_components( );

  /** 
   * Auxiliary DFS to count connected components
   */
  void explore_from(vector<vector<int>>& G, int i, int j, int n);

  /** 
   * Creates a professor for player in position p
   */  
  void create_new_professor (Pos& p, int player);
  
  /** 
   * Creates a soldier for player in position p
   */  
  void create_new_soldier (Pos& p, int player);

  /** 
   * Creates a safety box widh identifier 'id' with the gold at position 'p'
   */  
  void create_new_safety_box (Pos p, int id);
  
  /** 
   * Returns whether sol is a correct solution to the ints for opening a safety box
   */  
  bool check_correct_opening (const vector<int>& hints,
				     const vector<int>& sol);
  /** 
   * Returns whether one integer is the sum of the other two integers
   */    
  bool good_triplet (int x, int y, int z);

  /** 
   * Returns the ingredients of a hint to a safety box
   */
  vector<int> generate_one_hint ( );

  /**
   * Aux for generate_spell_ingredients
   */
  void generate_triple(int T, int& x, int& y, int& z);
    
  // Grid that stores the already generated walls. 0 means no wall. Otherwise, contains wall_ids
  vector<vector<int>> wall_plan;

  vector<Dir> dir_permutation ( );
  
  /////////////////////// END BOARD GENERATION ///////////////////////  
  
public:

  /**
   * Construct a board by reading information from a stream.
   */
  Board (istream& is, int seed);

  /**
   * Returns the name of a player.
   */
  inline string name (int pl) const {
    _my_assert(player_ok(pl), "Player is not ok.");
    return names[pl];
  }

  /**
   * Prints the board settings to a stream.
   */
  void print_settings (ostream& os) const;

  /**
   * Prints the name players to a stream.
   */
  void print_names (ostream& os) const;

  /**
   * Prints the state of the board to a stream.
   */
  void print_state (ostream& os);

  /**
   * Prints the results and the names of the winning players.
   */
  void print_results () const;

  /**
   * Computes the next board aplying the given actions to the current board.
   * It also prints to os the actual actions performed.
   */
  void next (const vector<Action>& act, ostream& os);

};

#endif
