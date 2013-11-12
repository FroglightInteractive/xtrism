// bricks/data.C

#include "data.H"
#include "../basics/Throw.H"
#include "../basics/Filename.H"
#include <errno.h>

/* data is stored as one byte per line, lsb=leftmost cell. */

const char BDL_EMPTY='-';

// ================================ BDLine ===================================
BDLine::BDLine() {
  for (unsigned int i=0; i<BD_MAXSIZE; i++)
    cell[i]=BDL_EMPTY;
  }

// ================================ BDLines ==================================
inline bool BDLines::safecell(int x, int y) const {
  return (x<0 || y<0 || x>=int(BD_MAXSIZE) || y>=int(BD_MAXSIZE))
    ?false
    :line[y].cell[x]!=BDL_EMPTY;
  }

unsigned int BDLines::count() const {
  int res=0;
  for (unsigned int y=0; y<hei; y++)
    for (unsigned int x=0; x<BD_MAXSIZE; x++)
      res+=(line[y].cell[x]!=BDL_EMPTY);
  return res;
  }

// ======================= BrickData =========================================
inline int rotx(int x, int y, int rot, BrickData::RotStyle rotsty) {
  switch (rotsty)
    {
    case BrickData::ROT_4x4: rot&=3;
      return (rot==0)?x:(rot==3)?y:(rot==2)?(3-x):(3-y);
    case BrickData::ROT_4x4_TWO: return (rot&1)?(3-y):x;
    case BrickData::ROT_3x3: rot&=3;
      return (rot==0)?x:(rot==3)?y:(rot==2)?(2-x):(2-y);
    }
  athrow("BrickData: rotx: illegal rotation style");
  }

inline int roty(int x, int y, int rot, BrickData::RotStyle rotsty) {
  switch (rotsty)
    {
    case BrickData::ROT_4x4: rot&=3;
      return (rot==0)?y:(rot==3)?(3-x):(rot==2)?(3-y):x;
    case BrickData::ROT_4x4_TWO:
      return (rot&1)?x:y;
    case BrickData::ROT_3x3: rot&=3;
      return (rot==0)?y:(rot==3)?(2-x):(rot==2)?(2-y):x;
    }
  athrow("BrickData: roty: illegal rotation style");
  }

/* --------------------------- BrickData::BrickData ----------------------- */
/* Fills the data members.
 * lines is an array of strings describing each of the lines in the brick
 * rot is the angle to rotate by: 0/1/2/3. (1=90 deg clockwise)
 * rotsty is the rotation style
 */
BrickData::BrickData(BDLines const &lines, int rot, RotStyle rotsty):
  CellMatrix(lines.hei), cells_(lines.count()), xx(0), yy(0) {
  xx=new byte[cells_]; yy=new byte[cells_];
  int n=(rotsty==ROT_4x4_TWO && rot&2)?cells_-1:0;
  int dn=n?-1:1;
  for (unsigned int y=0; y<height(); y++)
    for (unsigned int x=0; x<BD_MAXSIZE; x++)
      if (lines.safecell(x,y))
        { int rx=rotx(x,y,rot,rotsty), ry=roty(x,y,rot,rotsty);
          set(rx,ry); xx[n]=rx; yy[n]=ry; n+=dn;
        }
  }

BrickData::~BrickData() {
  if (yy)
    delete [] yy;
  if (xx)
    delete [] xx;
  }

/* -------------------------- BrickData::print ---------------------------- */
/* For debugging purposes: prints the contents of a BrickData. */
void BrickData::print() const {
  int j=0;
  for (unsigned int y=0; y<height(); y++)
    { for (unsigned int x=0; x<BD_MAXSIZE; x++)
        if (line(y) & (1<<x))
          { printf("#");
            j++;
          }
        else
          printf("-");
      printf("\n");
    }
  printf("num of cells = %i\n",j);
  }

// ============================ RBrickData ===================================

RBrickData::RBrickData(FILE *src, int t): bd(0) {
  BDLines lines;
  int n;
  char col[20];
  char buf[80];
  *buf=0;
  errno=0;
  while (*buf<32 && !feof(src))
    fgets(buf,80,src);
  if (sscanf(buf,"%i %s",&n,col)!=2)
    athrow("RBrickData: Bad brick data");
  if (t>=0 && t!=n)
    athrow("RBrickData: Bad brick number");

  if (strchr(col,'/'))
    { // modern colour spec rotstyle/RGB
      switch (col[0])
        {
        case '0': rotsty=BrickData::ROT_4x4; rots=4; lines.hei=4; break;
        case '2': rotsty=BrickData::ROT_4x4_TWO; rots=4; lines.hei=4; break;
        case '3': rotsty=BrickData::ROT_3x3; rots=4; lines.hei=3; break;
        default: athrow("RBrickData: Illegal rotation style");
        }
      bd=new BrickData*[rots];
      for (unsigned int i=0; i<rots; i++)
        bd[i]=0;
      //      int colr=atoi(col+2);
      //#define COLFOO(x) (((x)+.5)/4)      
      //sprintf(col,"%5.3f,%5.3f,%5.3f",
      //COLFOO(colr%10),COLFOO((colr/10)%10),COLFOO((colr/100)%10));
      colour_=col+2;
    }
  
  for (unsigned int y=0; y<BD_MAXSIZE; y++)
    fgets(lines.line[y].cell,BD_MAXSIZE+2,src);
  if (errno)
    athrow(string("RBrickData:: Error reading bricks: ") + strerror(errno));

  for (unsigned int rot=0; rot<rots; rot++)
    bd[rot]=new BrickData(lines,rot,rotsty);
  }

RBrickData::~RBrickData() {
  if (bd)
    { for (unsigned int i=0; i<rots; i++)
        if (bd[i])
          delete bd[i];
      delete [] bd;
    }
  }

void RBrickData::print() const {
  printf("Type = %i; Colour = %s\n",rotsty,colour_.c_str());
  for (unsigned int r=0; r<rots; r++)
    { bd[r]->print(); printf("\n"); }
  }

// ========================= SBrickData ======================================

SBrickData::SBrickData(Filename const &file): rbd(0) {
  FILE *f=fopen(file.name(),"r");
  if (!f)
    athrow(string("SBrickData: Couldn't open `") + file.name() + "': " +
           strerror(errno));
  int r=fscanf(f,"%i\n",&n);
  if (r==0)
    athrow("SBrickData: Couldn't determine number of bricks");
  if (n==0)
    athrow("SBrickData: Zero bricks");
  rbd=new RBrickData*[n];
  for (int i=0; i<n; i++)
    rbd[i]=new RBrickData(f,i);
  fclose(f);
  }

SBrickData::~SBrickData() {
  if (rbd)
    { for (int i=0; i<n; i++)
        if (rbd[i])
          delete rbd[i];
      delete [] rbd;
    }
  }

void SBrickData::print() const {
  printf("Brickset containing %i bricks\n",n);
  for (int i=0; i<n; i++)
    { printf("brick # %i:\n",i);
      rbd[i]->print();
    }
  }

unsigned int SBrickData::maxlines() const {
  int max=0;
  for (int i=0; i<n; i++)
    max=max>?rbd[i]->height();
  return max;
  }
