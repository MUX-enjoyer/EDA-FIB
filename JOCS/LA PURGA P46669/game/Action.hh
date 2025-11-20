#ifndef Action_hh
#define Action_hh

#include "Structs.hh"

struct Command;


/**
 * Class that stores the commands requested by a player in a round.
 */
class Action {

public:

  /**
   * The following functions add a command to the action (= list of commands).
   * They fail if a command is already present for the commanded citizen.
   */

  /**
   * Commands citizen with identifier id to move following direction dir.
   */
  void move(int id, Dir dir);

  /**
   * Commands builder with identifier id build a barricade in direction dir.
   */
  void build(int id, Dir dir);


//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////

  /**
   * Empty constructor.
   */
  Action () : q(0) { }
 
 
private:
  
  friend class Game;
  friend class SecGame;
  friend class Board;

  /**
   * Maximum number of commands allowed for a player during one round.
   */
  static const int MAX_COMMANDS = 1000;

  /**
   * Number of commands tried so far.
   */
  int q;
  
  /**
   * Set of citizens that have already performed a command.
   */
  set<int> u;

  /**
   * List of commands to be performed during this round.
   */
  vector<Command> v;

  /**
   * Read/write commands to/from a stream.
   */
  Action (istream& is);
  static void print (const vector<Command>& commands, ostream& os);

  void execute(const Command& m);
  
};


/**
 * Class for commands.
 */
struct Command {

  int id;      // Identifier of the commanded citizen.
  int c_type;  // Type of command.
  int dir;     // Direction of the command

  /**
   * Constructor with all defining fields.
   */
  Command (int id, int c_type, int dir) :
    id(id), c_type(c_type), dir(dir) { }
};


inline void Action::move(int id, Dir dir) {
  execute(Command(id, Move, int(dir)));
}

inline void Action::build(int id, Dir dir) {
  execute(Command(id, Build, int(dir)));
}

inline void Action::execute(const Command& m) {

  ++q;
  _my_assert(q <= MAX_COMMANDS, "Too many commands.");  
  
  if (u.find(m.id) != u.end()) {
    //cerr << "warning: command already requested for citizen " << m.id << endl;
    return;
  }
  u.insert(m.id);
  v.push_back(m);
}

#endif
