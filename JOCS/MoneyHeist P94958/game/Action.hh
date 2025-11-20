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
   * They fail if a command is already present for the commanded unit.
   */

  /**
   * Commands unit with identifier id to move following direction dir.
   */
  void move(int id, Dir dir);

  /**
   * Commands unit with identifier id (a professor) to provide password 'sol' to open the safety box whose door is
   * at direction 'dir'.
   */
  void open(int id, const vector<int>& sol, Dir d);

  /**
   * Commands unit with identifier id (a soldier) to fully charge its poison.
   */
  void charge(int id);

  /**
   * Commands unit with identifier id (a professor) to fully heal.
   */
  void heal(int id);


  
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
   * Set of units that have already performed a command.
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

  int id;           // Identifier of the commanded unit.
  int c_type;       // Type of command.
  int dir;          // Direction of the command
  vector<int> sol;  // Sol to the password
  /**
   * Constructor with all defining fields.
   */
  Command (int id, int c_type, int dir, const vector<int>& s) :
    id(id), c_type(c_type), dir(dir), sol(s) { }
};


inline void Action::move(int id, Dir dir) {
  execute(Command(id, Move, int(dir), {}));
}

inline void Action::open(int id, const vector<int>& s, Dir dir) {
  execute(Command(id, Open, int(dir), s));
}

inline void Action::charge(int id) {
  execute(Command(id, Charge, -1, {}));
}

inline void Action::heal(int id) {
  execute(Command(id, Heal, -1, {}));
}


inline void Action::execute(const Command& m) {

  ++q;
  _my_assert(q <= MAX_COMMANDS, "Too many commands.");  
  
  if (u.find(m.id) != u.end()) {
    cerr << "warning: command already requested for unit " << m.id << endl;
    return;
  }
  u.insert(m.id);
  v.push_back(m);
}

#endif
