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

  vector<string> names;
  int            fresh_id;

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
      read_grid(is);
      check_is_good_initial_fixed_board();
    }
    else if (generator == "RANDOM") generate_random_board();
    else                            _my_assert(false,"unknown generator  " + generator);
  }

  /**
   * Prints some information of the citizen.
   */
  inline static void print_citizen (Citizen& ci, ostream& os) {
    os << CitizenType2char(ci.type)  << '\t'
       << ci.id                      << '\t'
       << ci.player                  << '\t'
       << ci.pos.i                   << '\t'
       << ci.pos.j                   << '\t'        
       << WeaponType2char(ci.weapon) << '\t'
       << ci.life                    << '\t'      
       << endl;
  }

  /**
   * Creates a citizen of type t for player pl at pos p
   */
  void create_new_citizen(Pos p, CitizenType t, int pl);

  /**
   * Tries to apply a move. Returns true if it could.
   */
  bool execute(const Command&    m,
		      set<int>&         killed,
		      vector<pair<BonusType,int>>&   bonus_to_regenerate,
		      vector<pair<WeaponType,int>>&  weapon_to_regenerate,
		      vector<pair<pair<CitizenType,int>,int>>& citizens_to_regenerate
		      );
  /**
   * Kill citizen with id, and add it to killed
   */
  void kill(int id, set<int>& killed);

  /*
   * Returns whether citizen c1 wins c2 in an attack
   */
  bool first_citizen_wins_attack(const Citizen& c1, const Citizen& c2);

  /* 
   * Perfom an attack beween c1 and c2 and updates killed and citizens_to_regenerate (if some of them dies)
   */
  void perform_attack (Citizen& c1, Citizen& c2, set<int>& killed, vector<pair<pair<CitizenType,int>,int>>& citizens_to_regenerate);

  /*
   * Regenerates the citizens in to_regen. Type is {{CitizenType,Player},Rounds}
   */
  void regenerate_citizens (vector<pair<pair<CitizenType,int>,int>>& to_regen);

  /*
   * Regenerates the bonus in to_regen. Type is {BonusType,Rounds}
   */  
  void regenerate_bonus (vector<pair<BonusType,int>>& to_regen);

  /*
   * Regenerates the bonus in to_regen. Type is {WeaponType,Rounds}
   */  
  void regenerate_weapons (vector<pair<WeaponType,int>>& to_regen);

  /*
   * Returns whether p is an appropriate position to regenerate anb object.
     Should be empty and have no citizen in the two surrounding squares
   */
  bool is_good_pos_to_regen ( const Pos& p) const;

  /*
   * Tries to get a random position which is good_pos_to_regen. First argument says whether 
   * it has succeeded in finding it.
   */
  pair<bool,Pos> get_random_pos_where_regenerate ( );

  /*
   * Called at the end of which round. Deteriorates the barricades. Currently, destroys all
   * barricades when passing from night to day.
   */
  void deteriorate_barricades ( );
  
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
   * s_id identifies the current street. Position should be not in border. Not already occupied by street. 
   * Num of adjacent cells of the current street is <= 2.
   * Num of adjacents cells of other streets is 0.
   */
  bool pos_ok_for_street(int s_id, const Pos& p);

  /**
   * s_id identifies the current street. Position should be not in border. Not already occupied by street. 
   * Num of adjacent cells of some street is 0.
   */
  bool pos_ok_for_initial_street(const Pos& p);
  
  /**
   * Randomnly returns a position that fulfills pos_ok_for_street(s_id)
   */
  Pos get_ok_pos_for_street (int s_id );

  /**
   * Randomnly returns a position that fulfills pos_ok_for_initial_street()
   */
  Pos get_ok_pos_for_initial_street ( );


  /**
   * Tries to randomly generates num_building_cells cells of type Building in num_street disconnected parts
   * Sometime it does not succeed and generates less.
   */
  void generate_buildings (int num_building_cells, int num_streets);

  /**
  * Tries to generate a stree of length with identifier s_id
  */
  int generate_street (int s_id, int length);

  /**
   * Returns the number of connected component grid has (taking into account only streets and buildings)
   */
  int num_connected_components( );

  /** 
   * Auxiliary DFS to count connected components
   */
  void explore_from(vector<vector<int>>& G, int i, int j, int n);

  // Grid that stores the already generated streets. 0 means no street. Otherwise, contains street_ids
  vector<vector<int>> street_plan;


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
