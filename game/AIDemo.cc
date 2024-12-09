#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Demo

// DISCLAIMER: The following Demo player is *not* meant to do anything
// sensible. It is provided just to illustrate how to use the API.
// Please use AINull.cc as a template for your player.

struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }


  /**
   * Types and attributes for your player can be defined here.
   */
  const vector<Dir> dirs = {Up,Down,Left,Right};



  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {

    
    // If nearly out of time, do nothing.
    double st = status(me());
    if (st >= 0.9) return;

    // If more than halfway through, do nothing.
    if (round() > num_rounds()/2) return;



    
    // Write debugging info about barricades
    vector<Pos> bs = barricades(me());
    cerr << "At round " << round() << " player " << me() << " has " << bs.size() << " barricades:" << endl;
    for (Pos& p : bs) {
      Cell c = cell(p);
      cerr << "Pos " << p << " with resistance " << c.resistance << endl;
    }

    
    if (is_day()) {
      
      // At day take care of builders
      vector<int> b = builders(me());
      for (int id : b) { // Iterate over all builders
	Pos p = citizen(id).pos;


	
	// If there is food nearby, move there
	bool food_nearby = false;
	Dir food_dir;
	for (Dir d : dirs) {
	  if (pos_ok(p+d) and cell(p+d).bonus == Food) {
	    food_nearby = true;
	    food_dir = d;
	  }
	}
	if (food_nearby) move(id,food_dir);	
	else { // No food nearby
	  if (random(0,3) <= 1) {// Do this with 25% probability [try to build in random position]
	    bool some_dir = false;
	    for (uint i = 0; not some_dir and i < dirs.size(); ++i){
	      Dir d = dirs[i];
	      Pos np = p + d;
	      if (pos_ok(np)) { // pos_ok checks np is in the board, but here something else would be needed to make   
		build(id,d);    // sure we can construct
		cerr << "build " << id << " dir " << d << endl;
		some_dir = true;
	      }
	    }
	  }
	  else { // Do this with 75% probability [move to money if possible, otherwise randomly]
	    Dir random_dir = dirs[random(0,3)];
	    if (pos_ok(p+random_dir) and (cell(p+random_dir).id == -1 or 
					  citizen(cell(p+random_dir).id).type == Builder)) { // Move there if no citizen or builder	     
	      move(id,random_dir);
	      cerr << "move " << id << " dir " << random_dir << endl;
	    }
	  }
	}
      }
    }
    else {
      // At night take care of warriors
      vector<int> w = warriors(me());
      for (int id : w) { // Iterate over all warriors
	if (random(0,9) < 6) {// 60 % probability move randomly, otherwise do nothing
	  Dir random_dir = dirs[random(0,3)];
	  Pos p = citizen(id).pos;
	  if (pos_ok(p+random_dir)) {
	    cerr << "move " << id << " dir " << random_dir << endl;
	    move(id,random_dir);
	  }
	}	
      }
    }
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
