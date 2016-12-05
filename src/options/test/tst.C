#include "Player.H"
#include <iostream.h>
#include "../basics/vecio.H"

main() {
  vector<Player> pv;
  Player p;
  while (cin.good())
    { cin >> p;
      if (!cin.fail())
        { pv.push_back(p);
          cout << "Assigned player `" << p.name() << "' OK" << endl;
        }
      else
        cout << "Didn't assign player" << endl;
    }

  cout << "output" << endl; 
  cout << pv;
  cout << "end" << endl;
  }
