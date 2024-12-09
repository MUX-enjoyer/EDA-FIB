#ifndef Structs_hh
#define Structs_hh


#include "Utils.hh"


/**
 * Contains the Dir enumeration,
 * the CommandType enumeration,
 * the Pos struct,
 * the BonusType enumeration,
 * the WeaponType enumeration,
 * the CellType enumeration, the Cell struct,
 * the CitizenType enumeration, the Citizen struct,
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
 * Defines kinds of bonuses.
 */
enum BonusType {
  Money,
  Food,
  NoBonus  // No bonus.
};


inline ostream& operator << (ostream& out, BonusType b) {
  switch (b) {
  case Money:    out << "Money"; break;
  case Food:     out << "Food";  break;
  case NoBonus:  out << "NoBonus";  break;
  default:      out << int(b);  break;    
  }
  return out;
}


/**
 * Defines kinds of weapons.
 */
enum WeaponType {
  Hammer,
  Gun,
  Bazooka,
  NoWeapon
};

inline WeaponType strongestWeapon (WeaponType w1, WeaponType w2) {
  if (w1 == Bazooka or w2 == Bazooka) return Bazooka;
  else if (w1 == Gun or w2 == Gun) return Gun;
  else if (w1 == Hammer or w2 == Hammer) return Hammer;
  else return NoWeapon;
}

inline ostream& operator << (ostream& out, WeaponType w) {
  switch (w) {
  case Hammer:   out << "Hammer";   break;
  case Gun:      out << "Gun";      break;
  case Bazooka:  out << "Bazooka";  break;
  case NoWeapon: out << "NoWeapon"; break;
  default:       out << int(w);     break;
  }
  return out;
}


/**
 * Defines kinds of cells.
 */
enum CellType {
  Street,
  Building
};


inline ostream& operator << (ostream& out, CellType c) {
  switch (c) {
  case Street:   out << "Street";   break;
  case Building: out << "Building"; break;
  default:       out << int(c);     break;    
  }
  return out;
}


/**
 * Describes a cell on the board, and its contents.
 */
struct Cell {

  CellType         type; // The kind of cell (street or building).
  BonusType       bonus; // The kind of bonus (if any)
  WeaponType     weapon; // The kind of weapon (if any)
  int        resistance; // The resistence of the barricade present there, or -1 if no barricade.
  int           b_owner; // Player that owns this barricade, or -1 if no barricade.
  int                id; // The id of a citizen if present, or -1 otherwise.

  /**
   * Default constructor (Street, NoBonus, NoWeapon, -1, -1, -1).
   */
  Cell ();

  /**
   * Constructor with all defining fields.
   */
  Cell (CellType t, BonusType b, WeaponType w, int r, int o, int i);

  bool is_empty( ) const;
};


/**
 * Defines the type of the citizen.
 */
enum CitizenType {
  Builder,
  Warrior
};


inline ostream& operator << (ostream& out, CitizenType c) {
  switch (c) {
  case Builder: out << "Builder";  break;
  case Warrior: out << "Warrior";  break;
  default:      out << int(c);    break;    
  }
  return out;
}


/**
 * Describes an citizen on the board and its properties.
 */
struct Citizen {

  CitizenType type;  // The type of citizen.
  int id;            // The unique id of this citizen during the game.
  int player;        // The player that owns this citizen.
  Pos pos;           // The position on the board.    
  WeaponType weapon; // The weapon the citizen is carrying.
                     // NoWeapon if the citizen is not a warrior.
  int life;          // Life points (>= 0). Zero means dead.
  
  /**
   * Default constructor (Builder, -1, -1, (0, 0), NoWeapon, INT_MAX).
   */
  Citizen ();

  /**
   * Constructor with all defining fields.
   */
  Citizen (CitizenType t, int i, int pl,  Pos p, WeaponType w, int l);

};


//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////


/**
 * Enum to encode commands.
 */
enum CommandType {
  Move,
  Build,
};


inline ostream& operator << (ostream& out, CommandType c) {
  switch (c) {
  case Move:       out << "Move";      break;
  case Build:      out << "Build";     break;
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
  type(Street), bonus(NoBonus), weapon(NoWeapon), resistance(-1), b_owner(-1), id(-1) { }
inline Cell::Cell (CellType t, BonusType b, WeaponType w, int r, int o, int i) :
  type(t),      bonus(b),       weapon(w),  	  resistance(r),  b_owner(o),  id(i)  { }

inline bool Cell::is_empty ( ) const {
  return
    type == Street and
    bonus == NoBonus and
    weapon == NoWeapon and
    resistance == -1 and
    b_owner == -1 and
    id == -1;
}

inline bool command_type_ok (CommandType c_type) {
  return c_type >= Move and c_type <= Build;
}


/**
 * Conversion from command type to char.
 */
inline char CommandType2char (int t) {
  switch (t) {
  case Move:      return 'm';
  case Build:     return 'b';
  default:        return '_';
  }
}


/**
 * Conversion from char to command type.
 */
inline int char2CommandType (char c) {
  switch (c) {
  case 'm': return Move;
  case 'b': return Build;
  }
  return -1; // Can't abort: if data were corrupted, master would fail.
}


/**
 * Conversion from direction to char.
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
 * Conversion from bonus to char.
 */
inline char BonusType2char (int f) {
  switch (f) {
  case Money: return 'm';
  case Food : return 'f';
  default:    return 'n';
  }
}


/**
 * Conversion from char to bonus.
 */
inline int char2BonusType (char c) {
  switch (c) {
  case 'm': return Money;
  case 'f': return Food;
  case 'n': return NoBonus;
  }
  return -1; // Can't abort: if data is corrupted, master will fail.
}

/**
 * Conversion from weapon to char.
 */
inline char WeaponType2char (int f) {
  switch (f) {
  case Hammer:   return 'h';
  case Gun:      return 'g';
  case Bazooka:  return 'b';
  default:       return 'n';
  }
}


/**
 * Conversion from char to weapon.
 */
inline int char2WeaponType (char c) {
  switch (c) {
  case 'h': return Hammer;
  case 'g': return Gun;
  case 'b': return Bazooka;
  case 'n': return NoWeapon;
  }
  return -1; // Can't abort: if data is corrupted, master will fail.
}


/**
 * Conversion from citizen type to char.
 */
inline char CitizenType2char (int t) {
  switch (t) {
  case Builder: return 'b';
  case Warrior: return 'w';
  default:      return '_';
  }
}


/**
 * Conversion from char to citizen type.
 */
inline int char2CitizenType (char c) {
  switch (c) {
  case 'b': return Builder;
  case 'w': return Warrior;
  }
  return -1; // Can't abort: if data is corrupted, master will fail.
}


inline Citizen::Citizen ()
  : type(Builder), id(-1), player(-1), pos(0, 0), weapon(NoWeapon), life(INT_MAX) { }

inline Citizen::Citizen (CitizenType t, int i, int pl, Pos p, WeaponType w, int l)
  : type(t),       id(i),  player(pl), pos(p),    weapon(w),    life(l)    { }


#endif
