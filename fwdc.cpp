#include <iostream>
#include <stdlib.h> /* to use exit */
#include <string>
#include <vector>
using namespace std;

// FALL22
// Update this file for HW2 FWDC (Yoshii)
// Read the main() first and write down all the functions it calls!
// Then, look for *** using control-S and add code.
// ** always marks where you need to change!
//--------------------------------------------------
// YOUR NAME: **Joshua White
// Tested on Empress using g++
//--------------------------------------------------

// State description class.
// Could have used struct.
// Items here is a string of L or R for FWDC
//    e.g. RRRR for the initial state
class state {
public:
  string items;    // made up of L and R going from RRRR to LLLL
  string camefrom; // the parent on the solution path
  int g;           // cost so far
  int h;           // estimated cost to the goal
  int f;           // g+h
};

// Major data structures:
vector<state> frontier;  // Frontier nodes - will pick one of these to expand
vector<state> beenThere; // already expanded nodes

// ========== Utility Functions ===================

// utility to add x to the beenthere
void addtoBeenThere(state x) { beenThere.push_back(x); }

// utility to add x to the frontier
void addtoFrontier(state x) { frontier.push_back(x); }

// to remove state x from the frontier when it has been expanded
void removefromFrontier(state x) {
  vector<state> newfrontier;
  for (int k = 0; k < frontier.size(); k++)
    if (frontier[k].items != x.items)
      newfrontier.push_back(frontier[k]);

  frontier = newfrontier;
}
//=================== Helper Function Declarations ===============
bool safetyCheck(state next);

bool checkBT(state next);

//============= At the Very End ====================

// Trace and display the solution path from goal to initial.
// Note that camefrom of each state should be used. (called from generate)
void tracePath(state goal) {
  // *** add code here to display the path from goal to initial
  // using cameFrom's  -- recursion is useful to not to depend on a particular
  // order of state in Been There!
  cout << goal.items << endl;
  if (goal.camefrom != "") {
    int index = 0;
    for (int i = 0; i < beenThere.size(); i++) {
      if (goal.camefrom == beenThere.at(i).items) {
        index = i;
        break;
      }
    }
    tracePath(beenThere.at(index));
  } else {
    return;
  }

} // end of tracePath

//======= For Generating a new state to add to Frontier  =============

// Check to see if next's items is already in frontier and return true or false.
//   If it is already in frontier,
//   and if the next's f is better,
//   update the frontier node to next.  (called from generate)
bool inFrontier(state next) {
  // **** add code here, possibly updating
  // the same frontier node with next if f is better.
  // Please cout a message in that case.
  // Return true or false.
  for (int i = 0; i < frontier.size(); i++) {
    if (next.items == frontier.at(i).items) {
      if (next.f < frontier.at(i).f) {
        cout << "Improvement to a node in Frontier" << endl;
        removefromFrontier(next); // take advantage of oversight in code, since
                                  // remove function only checks items
        addtoFrontier(next);
      }
      return true;
    }
  }
  return false;
} // end of inFrontier

// Try to add next to frontier but stop if LLLL (called from generateAll)
void generate(state next) {
  cout << "Trying to add: " << next.items;

  if (next.items == "LLLL") // the goal is reached
  {
    cout << ">>Reached: " << next.items << endl;
    tracePath(next); // display the solution path
    exit(0);
  } // done

  // *** if been there before, do not add to frontier.
  // You may write a utility function check been-there and call it
  bool youBeenThere = checkBT(next);
  // *** if unsafe, do not add to frontier.
  // You may write a function to check unsafe and call it
  bool youSafe = safetyCheck(next);
  // else compute h and then f for next
  int h = 0; // start point for h; g is already set
  // ** update h and then f
  for (int i = 1; i < 4; i++) {
    if (next.items[i] == 'R') {
      h++;
    }
  }
  next.h = h;
  next.f = next.g + next.h;

  if (!inFrontier(next) && youBeenThere == false && youSafe == true)
    addtoFrontier(next); // add next to Frontier

} // end of generate

// Generate all new states from current (called from main)
void generateAll(state current) {
  state next; // a generated one

  // each next will be modified from current for ease
  current.g = current.g + 1; // to update g to be used for next
  // storing the parent so that we can produce the solution path
  current.camefrom = current.items;

  // check all possibilities as follows..

  // move F alone to L
  next = current; // starting point of next is current
  if (current.items[0] == 'R') {
    next.items[0] = 'L';
    generate(next);
  };

  next = current; // starting point of next
  // move F alone to R
  if (current.items[0] == 'L') {
    next.items[0] = 'R';
    generate(next);
  };

  next = current; // starting point of next
  // move FW to L
  if (current.items[0] == 'R' && current.items[1] == 'R') {
    next.items[0] = 'L';
    next.items[1] = 'L';
    generate(next);
  };

  next = current; // starting point of next
  // move FW to R
  if (current.items[0] == 'L' && current.items[1] == 'L') {
    next.items[0] = 'R';
    next.items[1] = 'R';
    generate(next);
  };

  next = current; // starting point of next
  // move FD to L
  if (current.items[0] == 'R' && current.items[2] == 'R') {
    next.items[0] = 'L';
    next.items[2] = 'L';
    generate(next);
  };

  next = current; // starting point of next
  // move FD to R
  if (current.items[0] == 'L' && current.items[2] == 'L') {
    next.items[0] = 'R';
    next.items[2] = 'R';
    generate(next);
  };

  next = current; // starting point of next
  // move FC to L
  if (current.items[0] == 'R' && current.items[3] == 'R') {
    next.items[0] = 'L';
    next.items[3] = 'L';
    generate(next);
  };

  next = current; // starting point of next
  // move FC to R
  if (current.items[0] == 'L' && current.items[3] == 'L') { //**
    next.items[0] = 'R';
    next.items[3] = 'R';
    generate(next);
  };

} // end of generateAll

// Find the best f state out of the frontier and return it (called from main)
state bestofFrontier() {
  // **** add code here
  int currentMax = frontier.at(0).f;
  int indexTracker = 0;
  for (int i = 1; i < frontier.size(); i++) {
    if (frontier.at(i).f > currentMax) {
      currentMax = frontier.at(i).f;
      indexTracker = i;
    }
  }
  return frontier.at(indexTracker);
} // end of bestofFrontier

// Display the states in the frontier  (called from main)
void displayFrontier() {
  for (int k = 0; k < frontier.size(); k++) {
    cout << frontier[k].items << " ";
    cout << "g = " << frontier[k].g << " ";
    cout << "h = " << frontier[k].h << " ";
    cout << "f = " << frontier[k].f << endl;
  }
} // end of displayFrontier

//==================Helper Functions ===============
// checks if state exists within beenthere vector
bool checkBT(state next) {
  bool bt = false;

  for (int i = 0; i < beenThere.size(); i++) {
    if (next.items == beenThere.at(i).items) {
      bt = true;
      break;
    }
  }
  return bt;
}

// checks if state is unsafe
bool safetyCheck(state next) {
  bool isSafe = true;

  if (next.items[1] == next.items[2] && next.items[0] == next.items[3] &&
      next.items[1] != next.items[0]) {
    isSafe = false;
  }
  if (next.items[2] == next.items[3] && next.items[0] == next.items[1] &&
      next.items[2] != next.items[0]) {
    isSafe = false;
  }
  return isSafe;
}

//================= Main ===========================

int main() {

  state current = {"RRRR", "", 0, 3, 3}; // initial
  addtoFrontier(current);

  char ans;
  while (ans != 'n') {
    removefromFrontier(current);
    addtoBeenThere(current);

    cout << ">>>Expand:" << current.items << endl;
    generateAll(current); // new states are added to frontier

    cout << "Frontier is:" << endl;
    displayFrontier();

    current = bestofFrontier(); // pick the best state out of frontier
    cin >> ans;

  } // end while

} // end of main
