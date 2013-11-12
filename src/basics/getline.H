// getline.H

#ifndef _getline_H
#define _getline_H

#include <string>
#include <iostream>
using namespace std;

string getline_nocmt(istream &is);
void skipcmt(istream &is);
void skiptoeol(istream &is);
istream &skipwhite(istream &is);

#endif
