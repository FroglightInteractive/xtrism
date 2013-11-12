// Throw.H - trivial function for easier throwing of errors

/* fthrow throws an error if its first argument is false,
 * tthrow throws an error if its first argument is true,
 * athrow always throws an error.
 ** Note that athrow has been added to facilitate changing the error
    handling mechanism later: please DO use it instead of just throwing
    string errors directly (being an inline function the overhead is nil).
 */

#ifndef _Throw_H
#define _Throw_H

#include <string>
using namespace std;
#include <stdio.h>

void athrow(const string &text) __attribute__ ((noreturn));

inline void athrow(const string &text) { fprintf(stderr,"Error: %s\n",
                                                 text.c_str());
                                         throw text; 
                                       }
inline void fthrow(bool dont,const string &text) { if (!dont) athrow(text); }
inline void tthrow(bool doit,const string &text) { if (doit) athrow(text); }

#endif
