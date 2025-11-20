#ifndef Structs_hh
#define Structs_hh


#include "Utils.hh"


/**
 * Contains the Dir enumeration,
 * the CommandType enumeration,
 * the Pos struct,
 * the CellType enumeration, the Cell struct,
 * the UnitType enumeration, the Unit struct,
 * and some useful little functions.
 */


/**
 * Enum to encode directions.
 */
enum Dir {
  Down, Right, Up, Left
};
  
inline ostream& operator << (ostream& out, Dir d) {
  switch (d) {
  case Down:   out << "Down";  break;
  case Right:  out << "Right"; break;
  case Up:     out << "Up";    break;
  case Left:   out << "Left";  break;
  default:     out << int(d);  break;
  }
  return out;
}

/**
 * Simple struct to handle positions.
 */
struct Pos {

  int i, j;

  /**
   * Default constructor (0, 0).
   */
  Pos ();
  /**
   * Constructor with all defining fields.
   */
  Pos (int i, int j);

  /**
   * Print operator.
   */
  friend ostream& operator<< (ostream& os, const Pos& p);

  /**
   * Comparison operator.
   */
  friend bool operator== (const Pos& a, const Pos& b);

  /**
   * Comparison operator.
   */
  friend bool operator!= (const Pos& a, const Pos& b);

  /**
   * Comparison operator, mostly needed for sorting.
   */
  friend bool operator< (const Pos& a, const Pos& b);

  /**
   * Increment operator: moves a position according to a direction.
   */
  Pos& operator+= (Dir d);

  /**
   * Addition operator: Returns a position by adding a direction.
   */
  Pos operator+ (Dir d) const;

  /**
   * Increment operator: moves a position according to another position.
   */
  Pos& operator+= (Pos p);

  /**
   * Addition operator: Returns a position by adding another position.
   */
  Pos operator+ (Pos p) const;
};

/**
 * Defines kinds of cells.
 */
enum CellType {
  Corridor,
  Wall
};


inline ostream& operator << (ostream& out, CellType c) {
  switch (c) {
  case Corridor: out << "Corridor"; break;
  case Wall:     out << "Wall";     break;
  default:       out << int(c);     break;    
  }
  return out;
}


/**
 * Describes a cell on the board, and its contents.
 */
struct Cell {

  CellType         type; // The kind of cell (Corridor or Wall).
  int                id; // The id of a unit present, or -1 otherwise.
  bool       health_kit; // Whether it contains a health kit.
  bool       poison_kit; // Whether it contains a poison kit.
  int               box; // The id of a safety box present, or -1 if none.
                         // If != -1, this cell is the door of a safety box
  bool             gold; // Whether this position contains gold
                         // (i.e. the gold inside a safety box)
  int             money; // 0 if no money, > 0 contains the amount of money.
  
  /**
   * Default constructor (Corridor, -1, false, false, -1, false, 0).
   */
  Cell ();

  /**
   * Constructor with all defining fields.
   */
  Cell (CellType t, int i, bool h, bool p, int b, bool g, int m);

  bool is_empty( ) const;
};


/**
 * Defines the type of the unit.
 */
enum UnitType {
  Soldier,
  Professor
};


inline ostream& operator << (ostream& out, UnitType c) {
  switch (c) {
  case   Soldier:  out << "Soldier";    break;
  case Professor:  out << "Professor";  break;
  default:      out << int(c);    break;    
  }
  return out;
}


/**
 * Describes an unit on the board and its properties.
 */
struct Unit {
  UnitType type;             // The type of unit.
  int id;                    // The unique id of this unit during the game.
                             // Ids are consecutive numbers between 0 and 4 + 4*INITIAL_SOLDIERS_PER_CLAN - 1
  int player;                // The player that owns this unit
  Pos pos;                   // The position on the board.                                 
  int health;                // >= 0 for professor: remaning health (-1 if soldier)
  int poison;                // >= 0 for soldier: amount of poison it has (-1 if professor)
  int rounds_for_reborn;     // only for professors (-1 always for soldiers)
                             // 0 if professor is alive
                             // if it is N > 0, and we are at round R, this professor is now dead and will be
                             // reborn at the end of round R + (N-1)
  /**
   * Default constructor (Soldier, -1, -1, (0, 0), -1, 0, -1).
   */
  Unit ();

  /**
   * Constructor with all defining fields.
   */
  Unit (UnitType t, int i, int pl, Pos p, int h, int poi, int r);

  // Returns whether a unit is alive. Note that soldiers are always alive
  bool is_alive();
};



//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////


/**
 * Enum to encode commands.
 */
enum CommandType {
  Move,
  Open,
  Charge,
  Heal
};


inline ostream& operator << (ostream& out, CommandType c) {
  switch (c) {
  case Move:       out << "Move";      break;
  case Open:       out << "Open";      break;
  case Charge:     out << "Charge";    break;
  case Heal:       out << "Heal";      break;   
  default:         out << int(c);      break;
  }
  return out;
}


inline bool dir_ok (Dir dir) {
  return dir >= Down and dir <= Left;
}

inline Pos::Pos (            ) : i(0), j(0) { }
inline Pos::Pos (int i, int j) : i(i), j(j) { }

inline ostream& operator<< (ostream& os, const Pos& p) {
  return os << "(" << p.i << ", " << p.j << ")";
}

inline bool operator== (const Pos& a, const Pos& b) {
  return a.i == b.i and a.j == b.j;
}

inline bool operator!= (const Pos& a, const Pos& b) {
  return not (a == b);
}

inline bool operator< (const Pos& a, const Pos& b) {
  if (a.i != b.i) return a.i < b.i;
  return a.j < b.j;
}

inline Pos& Pos::operator+= (Dir d) {
  switch (d) {
  case Down:   ++i;  break;
  case Right:  ++j;  break;
  case Up:     --i;  break;
  case Left:   --j;  break;
  default: ; // do nothing
  }
  return *this;
}

inline Pos Pos::operator+ (Dir d) const {
  Pos p = *this;
  p += d;
  return p;
}

inline Pos& Pos::operator+= (Pos p) {
    this->i += p.i;
    this->j += p.j;
    return *this;
  }

inline Pos Pos::operator+ (Pos p) const {
    Pos p2 = *this;
    p2 += p;
    return p2;
  }


inline Cell::Cell (                              ) :
  type(Corridor), id(-1), health_kit(false), poison_kit(false), box(-1), gold(false), money(0) { }
inline Cell::Cell (CellType t, int i, bool h, bool p, int b, bool g, int m) :
  type(t), id(i), health_kit(h), poison_kit(p), box(b), gold(g), money(m) { }

inline bool Cell::is_empty ( ) const {
  return
    type == Corridor and
    id == -1 and
    not health_kit and
    not poison_kit and
    box == -1 and
    money == 0;
}

inline bool command_type_ok (CommandType c_type) {
  return (c_type == Move or c_type == Open or c_type == Charge or c_type == Heal);
}

/**
 * Conversion from command type to char.
 */
inline char CommandType2char (int t) {
  switch (t) {
  case Move:      return 'm';
  case Open:      return 'o';
  case Charge:    return 'c';
  case Heal:      return 'h';
  default:        return '_';
  }
}


/**
 * Conversion from char to command type.
 */
inline int char2CommandType (char c) {
  switch (c) {
  case 'm': return Move;
  case 'o': return Open;
  case 'c': return Charge;
  case 'h': return Heal;
  }
  return -1; // Can't abort: if data were corrupted, master would fail.
}


/**
 * Conversion from direction to char.
 * u, d, l, r are obvious
 */
inline char Dir2char (int d) {
  switch (d) {
  case Down:   return 'd';
  case Right:  return 'r';
  case Up:     return 'u';
  case Left:   return 'l';
  default:     return '_';
  }
}

/**
 * Conversion from char to direction.
 */
inline int char2Dir (char c) {
  switch (c) {
  case 'd': return Down;
  case 'r': return Right;
  case 'u': return Up;
  case 'l': return Left;
  }
  return -1; // Can't abort: if data is corrupted, master will fail.
}


/**
 * Conversion from unit type to char.
 */
inline char UnitType2char (int t) {
  switch (t) {
  case Soldier:   return 's';
  case Professor: return 'p';
  default:     return '_';
  }
}


/**
 * Conversion from char to unit type.
 */
inline int char2UnitType (char c) {
  switch (c) {
  case 's': return Soldier;
  case 'p': return Professor;
  }
  return -1; // Can't abort: if data is corrupted, master will fail.
}


inline Unit::Unit ()
  : type(Soldier), id(-1), player(-1), pos(0, 0), health(-1), poison(0), rounds_for_reborn(-1) {}

inline Unit::Unit (UnitType t, int i, int pl, Pos p, int h, int poi, int r)
  : type(t), id(i),  player(pl), pos(p), health(h), poison(poi), rounds_for_reborn(r) { }

inline bool Unit::is_alive ( ) {
  return type == Soldier or rounds_for_reborn == 0;
}

#endif

