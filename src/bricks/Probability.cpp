// Probability.cpp

#include "Probability.h"
#include <QMap>
#include "MapIterator.h"

Probability::Probability(QMap<int, float> probs) {
  float norm = 0;
  for (auto el: maprange(probs)) {
    norm += el.value();
    thrs << norm;
    res << el.key();
  }
  for (int k=0; k<thrs.size(); k++)
    thrs[k] /= norm;
}

int Probability::select(float x) const {
  int N = thrs.size();
  for (int k=0; k<N; k++)
    if (x<=thrs[k])
      return res[k];
  return -1;
}

Probability const &probabilities(int bset) {
  static QList<Probability> probs{
  // brickset 0
    QMap<int,float>{{0, .174},
    {1, .092},
    {2, .154},
    {3, .133},
    {4, .154},
    {5, .184},
    {6, .133}},

  // brickset 1
   QMap<int,float>{{ 0, .261    },
    { 1, .138    },
    { 2, .231    },
    { 3, .1995   },
    { 4, .231    },
    { 5, .276    },
    { 6, .133    },
    { 7, .040    },
    { 8, .040    },
    { 9, .020    },
    {10, .023   },
    {11, .023   },
    {12, .0215  },
    {13, .0215  },
    {14, .016   },
    {15, .016   },
    {16, .015   }},

  // brickset 2
   QMap<int,float>{{ 0, .261  },
    { 1, .138  },
    { 2, .231  },
    { 3, .1995 },
    { 4, .231  },
    { 5, .276  },
    { 6, .133  },
    { 7, .040  },
    { 8, .040  },
    { 9, .020  },
    {10, .023  },
    {11, .023  },
    {12, .0215 },
    {13, .0215 },
    {14, .016  },
    {15, .016  },
    {16, .015  },
    {24, .016  },
    {25, .016  },
    {26, .017  },
    {27, .012  },
    {28, .012  },
    {29, .014  },
    {30, .014  },
    {31, .015  },
    {32, .015  },
    {33, .008  }},

  // brickset 3
   QMap<int,float>{{  0, .261   },
    {  1, .138   },
    {  2, .231   },
    {  3, .1995  },
    {  4, .231   },
    {  5, .276   },
    {  6, .133   },
    {  7, .040   },
    {  8, .040   },
    {  9, .020   },
    { 10, .023   },
    { 11, .023   },
    { 12, .0215  },
    { 13, .0215  },
    { 14, .016   },
    { 15, .016   },
    { 16, .015   },
    { 17, .007   },
    { 18, .001   },
    { 19, .007   },
    { 20, .007   },
    { 21, .007   },
    { 22, .007   },
    { 23, .003   },
    { 24, .016   },
    { 25, .016   },
    { 26, .017   },
    { 27, .012   },
    { 28, .012   },
    { 29, .014   },
    { 30, .014   },
    { 31, .015   },
    { 32, .015   },
    { 33, .008   }}
  };
    
  if (bset<0 || bset>=probs.size())
    throw "Illegal probability set";

  return probs[bset];
}  
