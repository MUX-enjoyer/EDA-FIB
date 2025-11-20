//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////

#include "Action.hh"

Action::Action (istream& is) {
  u.clear();
  v.clear();

  // warning: all read operations must be checked for SecGame.

  // Syntax is:
  // id commandType dir     [for Move]
  // id commandType dir sol [for Open] where sol is: size elem_1 elem_2 ... elem_size
  // id commandType         [for Heal/Charge]

  int l;
  if (not(is >> l)) {
      cerr << "warning: cannot read number commands " << endl;
      return;    
  }
  for (int k = 0; k < l; ++k) {
    int i;
    if (not(is >> i)) { // id
      cerr << "warning: cannot read unit of next command" << endl;
      return;    
    }
    char c;
    if (not(is >> c)) {
      cerr << "warning: cannot read command type for unit " << i << endl;
      return;
    }

    if (char2CommandType(c) == Move) {
      char d;
      if (is >> d) {
	u.insert(i);
	v.push_back(Command(i, char2CommandType(c), char2Dir(d),{}));
      }
      else {
	cerr << "warning: only partially read move command for unit " << i << endl;
	return;
      }
    }
    else if (char2CommandType(c) == Heal or char2CommandType(c) == Charge) {
      u.insert(i);
      v.push_back(Command(i, char2CommandType(c), -1, {}));
    }
    else {

      if (char2CommandType(c) != Open) {
	cerr << "warning: found unexpected " + string(1,c) + " read command type for unit " << i << endl;
	return;
      }
      
      char d;
      if (not(is >> d)) {
	cerr << "warning: only partially read (type 1) open command for unit " << i << endl;
	return;
      }

      int n;
      if (not (is >> n)) {
	cerr << "warning: only partially read (type 2) open command for unit " << i << endl;
	return;
      }
       
      vector<int> s; int x;     
      for (int k = 0; k < n; ++k) {
	if (not(is >> x)) {
	  cerr << "warning: only partially read (type 3) open command for unit " << i << endl;
	  return;
	}
	s.push_back(x);
      }

      u.insert(i);
      v.push_back(Command(i, char2CommandType(c), char2Dir(d), s));
    }
  }
}

void Action::print (const vector<Command>& commands, ostream& os) {
  os << commands.size() << endl;
  for (const Command& com : commands) {
    if (com.c_type == Move) {
      os <<                  com.id       << '\t'
	 << CommandType2char(com.c_type)  << '\t'
	 <<         Dir2char(com.dir   )  << '\t'
	 << endl;
    }
    else if (com.c_type == Charge or com.c_type == Heal) {
      os <<                  com.id       << '\t'
	 << CommandType2char(com.c_type)  << '\t'
	 << endl;
    }
    else {
      if (com.c_type != Open) {cerr << "warning: unexpected command type" << endl;}
      os <<                       com.id << '\t'
	 << CommandType2char(com.c_type) << '\t'
	 <<         Dir2char(com.dir   )  << '\t'
	 <<               com.sol.size() << '\t'; // First write size so that it can be easily read
      for (int x : com.sol) os << " " << x;
      os << endl;	
    }
  }
}
