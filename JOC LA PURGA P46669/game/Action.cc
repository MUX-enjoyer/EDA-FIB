//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////

#include "Action.hh"

Action::Action (istream& is) {
  u.clear();
  v.clear();

  // warning: all read operations must be checked for SecGame.
  int l;
  is >> l;
  for (int k = 0; k < l; ++k) {
    int i;
    is >> i;
    char c, d;
    if (is >> c >> d) {
      u.insert(i);
      v.push_back(Command(i, char2CommandType(c), char2Dir(d)));
    }
    else {
      //cerr << "warning: only partially read command for citizen " << i << endl;
      return;
    }
  }
}

void Action::print (const vector<Command>& commands, ostream& os) {
  os << commands.size() << endl;
  for (const Command& com : commands)
    os <<                  com.id       << '\t'
       << CommandType2char(com.c_type)  << '\t'
       <<         Dir2char(com.dir   )  << '\t'
       << endl;
}
