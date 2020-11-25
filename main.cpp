#include<iostream>
#include<string>
#include<set>
#include<algorithm>
using namespace std;

//Compiler version g++ 6.3.0
//Global constants
const int arraymax = 400;
const int cubemax = 11;

//class definitions
class cube {
  public:
    set < int > arr[cubemax];
};

class larray {
  public:
    int noInputs;
  int noOutputs;
  int noCubes;
  cube cubes[arraymax];
};

//function declarations
void error(string);
void writeCube(cube & , int, int);
void initCube(cube & );
void initArray(larray & , int, int, int);
void readArray(larray & );
void writeArray(larray & );
void deleteCube(larray & , int);
void moveCube(larray & , larray & , int);
void appendCube(larray & , cube);
void combine(larray & , larray & );
void changeColumn(larray & , int, string);
void absorb(larray & );
bool subsume(cube, cube, int);
bool cubex(cube & , cube, cube, int);
void cubeSharp(larray & , cube, cube, int, int);
void sharp(larray & , larray, larray & );
void splitArray(larray & , larray & , cube);
void intersection(larray & , larray & , larray & );
int cubeCost(cube, int);
void consensus(larray & );

// multiple output minimization procedures
void epi(larray & , larray & , larray & );
void lessthan(larray & , larray & );
void pickCube(larray & , larray & , larray & );
void momin(larray & , larray, larray);

//connection minization technique procedures
void conmin(larray & , larray);

//Gate Realization class
class Gatetype {
  public:
    int inputs;
  int count;
};

//global variables for gate Realization
int nvars, fanin, fanout;
int nandgate = 0;
int outfanin[cubemax];
Gatetype gate[6];
int packcount;
// Gate Realization procedures
void pgate(char, int, set < int > );
void updategate(int);
void writedigit(int);
void ICcount(int);
void writegate(int);
void statistics(int, int);
void publish(larray);

/*
 Here are the example to use for program

structure:

no. of cube
no. of inputs
no. of outputs
0101... 0110...
0101... 0101...
upto total cubes

Example:

9
4
3
0001 001
0010 101
0011 001
0100 110
0101 010
1010 111
1011 111
1100 101
1101 110


9
4
1
0001 1
0010 1
0011 1
0100 1
0101 1
1010 1
1011 1
1100 1
1101 1


6
4
1
0010 1
0100 1
1010 1
1011 1
1100 1
1101 1


7
4
1
0010 1
0011 1
0100 1
1011 1
1100 1
1101 1
1111 1


8
3
2
000 11
001 11
010 11
011 11
100 11
101 11
110 11
111 11

8
4
3
0000 x0x
0001 xxx
01x1 x0x
0110 xx0
1001 xxx
1101 0xx
1110 xxx
1111 xx0


9
4
1
0000 1
0001 1
0010 1
0011 1
0100 1
0110 1
1000 1
1001 1
1100 1

*/

int main() {
  cout << "Hello, Please enter the required details!\n";

  larray f;
  readArray(f);
  cout << "\nArray f\n";
  writeArray(f);

  //creating ON array
  larray a, ON;
  int nin = f.noInputs;
  int nout = f.noOutputs;
  cube mask;
  initCube(mask);

  initArray(ON, 0, nin, nout);
  for (int i = nin; i < nin + nout; i++) {
    mask.arr[i] = (set < int > ) {
      1
    };

    splitArray(a, f, mask);
    combine(f, a);
    for (int j = nin; j < nin + nout; j++) {
      if (i != j) {
        changeColumn(a, j, "000");

      }
    }
    combine(ON, a);
    mask.arr[i] = (set < int > ) {
      0,
      1
    };

  }

  if (f.noOutputs > 1) {
    //case of multiple output system
    consensus(f);
    momin(a, f, ON);
    conmin(a, ON);
    publish(a);
  } else {
    //case of single output system
    consensus(f);
    publish(f);
  }
}

// procedures Definitions 

void error(string str) {
  if (str == "INITTMV") {
    cout << "INITARRAY:too many variables";
    exit(1);
  } else if (str == "COMBUNV") {
    cout << "COMBINE: unequal number of variables";
    exit(1);
  } else if (str == "COMBAOV") {
    cout << "COMBINE:array overflow";
    exit(1);
  } else if (str == "CHNGCOR") {
    cout << "CHANGECOLUMN: Column out of range";
    exit(1);
  } else if (str == "DELCUNC") {
    cout << "DELETECUBE: nonexistent cube";
    exit(1);
  } else if (str == "APCUAOV") {
    cout << "APPENDCUBE:array overflow";
    exit(1);
  } else if (str == "SHRPUNV") {
    cout << "SHARPS: Unequal number of variable";
    exit(1);
  } else if (str == "INTXAOV") {
    cout << "INTERSECTION: array overflow";
    exit(1);
  } else if (str == "INTXUNV") {
    cout << "INTERSECTION: Unequal number of variable";
    exit(1);
  } else if (str == "CONSAOV") {
    cout << "CONSENSUS: Array overflow";
    exit(1);
  }
}

void writeCube(cube & c, int ni, int no) {
  int n = ni + no;
  cout << "\n";
  for (int i = 0; i < n; i++) {
    if (i == ni) {
      cout << " ";
    }

    if (c.arr[i] == (set < int > ) {
        0
      }) {
      cout << "0";
    } else if (c.arr[i] == (set < int > ) {
        1
      }) {
      cout << "1";
    } else if (i >= ni) {
      cout << "-";
    } else {
      cout << "x";
    }
  }
}
void initCube(cube & a) {
  for (int i = 0; i < cubemax; i++) {
    a.arr[i] = (set < int > ) {
      0,
      1
    };
  }
}

void initArray(larray & a, int arraySize, int ninputs, int noutputs) {
  if (ninputs + noutputs > cubemax) {
    error("INITTMV");
  } else {
    a.noInputs = ninputs;
    a.noOutputs = noutputs;
    a.noCubes = arraySize;
    for (int i = 0; i < a.noCubes; i++) {
      initCube(a.cubes[i]);
    }
  }
}

void readArray(larray & a) {
  cout<<"\nPlease Enter no. of Cubes(min terms): ";
  cin >> a.noCubes;
  cout<<"\nPlease Enter no. of Inputs: ";
  cin >> a.noInputs;
  cout<<"\nPlease Enter no. of Outputs: ";
  cin >> a.noOutputs;
  initArray(a, a.noCubes, a.noInputs, a.noOutputs);
  cout<<"\nPlease Enter the Cubes\nOne cube(input+output) in each line:\n";

  for (int i = 0; i < a.noCubes; i++) {

    bool err = false;
    for (int j = 0; j < a.noInputs + a.noOutputs; j++) {
      if (!err) {
        char c;
        cin >> c;
        while (c == ' ') {
          cin >> c;
        }
        if (c != '0' && c != '1' && c != 'x' && c != 'X') {
          cout << "Invalid input found\n";
          exit(1);
        } else {
          if (c == '0') {
            a.cubes[i].arr[j] = (set < int > ) {
              0
            };
          } else if (c == '1') {
            a.cubes[i].arr[j] = (set < int > ) {
              1
            };
          } else {
            a.cubes[i].arr[j] = (set < int > ) {
              0,
              1
            };
          }
        }
      }
    }
  }

}

void writeArray(larray & a) {
  if (a.noCubes == 0) {
    cout << "Empty Array\n";
  } else {
    for (int i = 0; i < a.noCubes; i++) {
      writeCube(a.cubes[i], a.noInputs, a.noOutputs);
    }
  }
}

//position=0 to delete first element
void deleteCube(larray & a, int position) {
  if (position > a.noCubes - 1 || position < 0) {
    error("DELCUNC");
  } else {
    for (int i = position; i < a.noCubes - 1; i++) {
      a.cubes[i] = a.cubes[i + 1];

    }
    a.noCubes--;
  }
}

void moveCube(larray & a, larray & b, int index) {
  initArray(a, 1, b.noInputs, b.noOutputs);
  a.cubes[0] = b.cubes[index];
  deleteCube(b, index);
}

void appendCube(larray & a, cube c) {

  if (a.noCubes + 1 > arraymax) {
    error("APCUAOV");
  } else {
    a.cubes[a.noCubes] = c;
    a.noCubes++;
  }
}

void combine(larray & a, larray & b) {
  if (a.noOutputs + a.noInputs != b.noOutputs + b.noInputs) {
    error("COMBUNV");

  } else if (a.noCubes + b.noCubes >= arraymax) {
    error("COMBAOV");
  } else {
    for (int i = 0; i < b.noCubes; i++) {
      a.cubes[i + a.noCubes] = b.cubes[i];
    }
    a.noCubes += b.noCubes;
  }
}

void changeColumn(larray & a, int index, string str) {
  if (index > a.noInputs + a.noOutputs) {
    error("CHNGCOR");
  } else if (a.noCubes > 0) {
    set < int > ary[3];
    for (int i = 0; i < 3; i++) {
      if (str[i] == '0') {
        ary[i] = (set < int > ) {
          0
        };
      } else if (str[i] == '1') {
        ary[i] = (set < int > ) {
          1
        };
      } else if (str[i] == 'x') {
        ary[i] = (set < int > ) {
          0,
          1
        };
      } else {
        cout << "CHANGECOLUN: unknown character found";
        ary[i] = (set < int > ) {
          0,
          1
        };
      }

    }
    for (int i = 0; i < a.noCubes; i++) {
      set < int > x = a.cubes[i].arr[index];
      if (x == (set < int > ) {
          0
        }) {
        a.cubes[i].arr[index] = ary[0];
      } else if (x == (set < int > ) {
          1
        }) {
        a.cubes[i].arr[index] = ary[1];
      } else {
        a.cubes[i].arr[index] = ary[2];
      }
    }
  }
}

void absorb(larray & a) {
  if (a.noCubes > 0) {
    int nvars = a.noInputs + a.noOutputs;

    for (int i = 0; i < a.noCubes - 1; i++) {

      for (int j = i + 1; j < a.noCubes;) {
        if (subsume(a.cubes[i], a.cubes[j], nvars)) {
          deleteCube(a, i);
        } else if (subsume(a.cubes[j], a.cubes[i], nvars)) {
          deleteCube(a, j);
        } else {
          j++;
        }
      }
    }
  }
}

void consensus(larray & a) {
  int nv = a.noInputs + a.noOutputs;
  larray b, c, d;
  cube mask, all0mask;
  for (int i = a.noInputs; i < nv; i++) {
    changeColumn(a, i, "0xx"); //may be here is error.
  }
  initCube(mask);
  if (a.noOutputs > 0) {
    initCube(all0mask);
    for (int i = a.noInputs; i < nv; i++) {
      all0mask.arr[i] = (set < int > ) {
        0
      };
    }
  }
  for (int i = 0; i < a.noInputs; i++) {
    mask.arr[i] = (set < int > ) {
      1
    };
    splitArray(b, a, mask);
    mask.arr[i] = (set < int > ) {
      0
    };
    splitArray(c, a, mask);
    combine(a, b);
    combine(a, c);
    if (b.noCubes > 0 && c.noCubes > 0) {
      changeColumn(b, i, "xxx");
      changeColumn(c, i, "xxx");
      intersection(d, b, c);
      initCube(mask);
      if (a.noOutputs > 0 && d.noCubes > 0) {
        splitArray(b, d, all0mask);
      }
      absorb(a);
      if (a.noCubes + d.noCubes > arraymax) {
        error("CONSAOV");
      }
      combine(a, d);
      absorb(a);
    }
    mask.arr[i] = (set < int > ) {
      0,
      1
    };
  }
  absorb(a);
}

//this cubex is not working
/*
bool cubex(cube &a,cube b,cube c,int size){
  bool cubexresult=true;
  for(int i=0;i<size;i++){
     
    set_intersection(b.arr[i].begin(),
    b.arr[i].end(),
    c.arr[i].begin(),
    c.arr[i].end(),
    inserter(a.arr[i],a.arr[i].begin()));
    
    if(a.arr[i].empty()){
      cubexresult=false;
      break;
    } 
  }
  return cubexresult;
}

*/

bool cubex(cube & a, cube b, cube c, int size) {
  for (int i = 0; i < size; i++) {

    if (b.arr[i] == (set < int > ) {
        0,
        1
      }) {
      a.arr[i] = c.arr[i];
    } else if (c.arr[i] == (set < int > ) {
        0,
        1
      }) {
      a.arr[i] = b.arr[i];
    } else if (b.arr[i] == c.arr[i]) {
      a.arr[i] = b.arr[i];
    } else {
      a.arr[i] = {};
      return false;
    }
  }
  return true;
}

void cubeSharp(larray & x, cube a, cube b, int in , int out) {
  cube c;
  int size = in +out;
  if (!cubex(c, b, a, size)) {
    x.noCubes = 1;
    x.cubes[0] = a;
  } else {
    x.noCubes = 0;
    for (int i = 0; i < size; i++) {

      set < int > y;
      set_difference(
        a.arr[i].begin(),
        a.arr[i].end(),
        c.arr[i].begin(),
        c.arr[i].end(),
        inserter(y, y.begin()));

      if (!y.empty()) {
        x.cubes[x.noCubes] = a;
        x.cubes[x.noCubes].arr[i] = y;
        x.noCubes++;
      }
    }
  }
}

//c = a sharp b
void sharp(larray & c, larray a, larray & b) {
  larray t;
  int nvars = a.noInputs + a.noOutputs;
  if (nvars != b.noInputs + b.noOutputs) {
    error("SHRPUNV");
  } else {

    t = a;
    c = a;
    for (int i = 0; i < b.noCubes; i++) {
      c.noCubes = 0;
      for (int k = 0; k < a.noCubes; k++) {
        cubeSharp(t, a.cubes[k], b.cubes[i], a.noInputs, a.noOutputs);
        combine(c, t);
        if (c.noCubes > arraymax / 2) {
          absorb(c);
        }
      }
      a = c;
    }
    absorb(c);
  }

}

void splitArray(larray & b, larray & a, cube mask) {
  initArray(b, 0, a.noInputs, a.noOutputs);
  int nvars = a.noInputs + a.noOutputs;
  if (a.noCubes > 0) {
    int j = 0;
    while (j < a.noCubes && a.noCubes > 0) {
      if (subsume(a.cubes[j], mask, nvars)) {
        appendCube(b, a.cubes[j]);
        deleteCube(a, j);
      } else {
        j++;
      }
    }
  }
}

void intersection(larray & a, larray & b, larray & c) {
  initArray(a, 0, b.noInputs, b.noOutputs);
  if (b.noInputs + b.noOutputs == c.noInputs + c.noOutputs) {
    int nv = b.noInputs + b.noOutputs;
    for (int i = 0; i < b.noCubes; i++) {
      for (int j = 0; j < c.noCubes; j++) {
        if (a.noCubes + 1 > arraymax) {
          error("INTXAOV");
        } else if (cubex(a.cubes[a.noCubes], b.cubes[i], c.cubes[j], nv)) {
          a.noCubes++;
        }

        if (a.noCubes + 1 > arraymax / 2) {
          absorb(a);
        }
      }
    }
    absorb(a);
  } else {
    error("INTXUNV");
  }
}

int cubeCost(cube a, int n) {
  int count = 0;
  for (int i = 0; i < n; i++) {
    if (a.arr[i] != (set < int > ) {
        0,
        1
      }) {
      count++;
    }
  }
  return count;
}

bool subsume(cube a, cube b, int size) {
  bool sub = true;
  int i = 0;
  while (sub && i < size) {

    set < int > temp;

    set_difference(a.arr[i].begin(),
      a.arr[i].end(),
      b.arr[i].begin(),
      b.arr[i].end(),
      inserter(temp, temp.begin()));

    if (!temp.empty()) {
      sub = false;
    } else {
      i++;
    }
  }
  return sub;
}

// multiple output minimization procedures
void epi(larray & e, larray & f, larray & ON) {
  int i;
  larray d1, a, b;
  int npi = f.noCubes;
  i = 0;
  while (i < npi && ON.noCubes > 0) {

    moveCube(d1, f, 0);
    intersection(a, ON, d1);

    if (a.noCubes > 0) {
      sharp(b, a, f);
      if (b.noCubes == 0) {
        combine(f, d1);
      } else {
        combine(e, d1);
        sharp(a, ON, d1);
        ON = a;
      }
    }
    i++;
  }
}

void lessthan(larray & PI, larray & ON) {
  larray a, b, d1, d2;
  bool delcube;
  int npi = PI.noCubes;
  initArray(d2, 1, PI.noInputs, PI.noOutputs);
  bool broken = false;
  int i = 0;
  while (i < npi && !broken) {
    delcube = false;
    moveCube(d1, PI, 0);
    intersection(a, ON, d1);
    if (a.noCubes == 0) {
      delcube = true;
    } else {
      int costi = cubeCost(d1.cubes[0], PI.noInputs);
      int j = 0;
      while (j < PI.noCubes && !broken) {
        if (costi >= cubeCost(PI.cubes[j], PI.noInputs)) {
          d2.cubes[0] = PI.cubes[j];
          sharp(b, a, d2);
          if (b.noCubes == 0) {
            delcube = true;
            broken = true;
          }
        }
        j++;
      }
    }
    if (!delcube) {
      combine(PI, d1);
    }
    i++;
  }

}

void pickCube(larray & cover, larray & PI, larray & ON) {
  larray a, b;
  initArray(a, 0, PI.noInputs, PI.noOutputs);
  int j = 0, cost;
  int mincost = PI.noInputs;
  for (int i = 0; i < PI.noCubes; i++) {
    cost = cubeCost(PI.cubes[i], PI.noInputs);
    if (cost < mincost) {
      mincost = cost;
      j = i;
    }
  }
  moveCube(a, PI, j);
  combine(cover, a);
  sharp(b, ON, a);
  ON = b;
}

void momin(larray & cover, larray PI, larray ON) {
  bool loop;
  int npi;
  initArray(cover, 0, ON.noInputs, ON.noOutputs);
  loop = true;
  do {
    epi(cover, PI, ON);
    if (ON.noCubes > 0) {
      npi = PI.noCubes;
      lessthan(PI, ON);

      if (npi == PI.noCubes) {
        pickCube(cover, PI, ON);

        if (ON.noCubes == 0) {
          loop = false;
        }
      }
    } else {
      loop = false;
    }

  } while (!loop);
  for (int i = cover.noInputs; i < cover.noInputs + cover.noOutputs; i++) {
    changeColumn(cover, i, "011");
  }
}

//connection minimization techniques
void conmin(larray & E, larray ON) {
  int ni, nvars, i, j, EINcubes;
  cube Mask, eij;
  larray Ei, EINi, Etemp, ONi, d;

  if (E.noCubes > 0 && ON.noCubes > 0) {
    ni = E.noInputs;
    nvars = ni + E.noOutputs;
    initCube(Mask);
    for (i = ni; i < nvars; i++) {
      Mask.arr[i] = (set < int > ) {
        1
      };
      changeColumn(E, i, "011");
      splitArray(Ei, E, Mask);
      if (Ei.noCubes > 0) {
        EINi = Ei;
        EINcubes = Ei.noCubes;
        EINi.noOutputs = 0;
        splitArray(ONi, ON, Mask);
        combine(ON, ONi);
        ONi.noOutputs = 0;
        for (j = 0; j < EINcubes; j++) {
          eij = Ei.cubes[0];
          deleteCube(Ei, 0);
          moveCube(Etemp, EINi, 0);
          intersection(d, Etemp, ONi);
          sharp(d, d, EINi);
          if (d.noCubes > 0) {
            appendCube(EINi, Etemp.cubes[0]);
            appendCube(Ei, eij);
          } else {
            eij.arr[i] = (set < int > ) {
              0
            };
            appendCube(E, eij);
          }
        }
        combine(E, Ei);
      }
      Mask.arr[i] = (set < int > ) {
        0,
        1
      };
    }
    for (i = ni; i < nvars; i++) {
      Mask.arr[i] = (set < int > ) {
        0
      };
    }
    splitArray(Ei, E, Mask);
  }
}

// Gate minimization procedures

void pgate(char g, int gateno, set < int > x) {
  cout << " ";
  if (x == (set < int > ) {
      0
    }) {
    cout << "-";
  } else {
    cout << " ";
  }
  cout << g;
  if (gateno > 9) {
    cout << gateno / 10;
    gateno %= 10;
  } else {
    cout << "0";
  }
  cout << gateno;
}

void updategate(int incount) {
  if (incount > 8) {
    incount = 9;
  }
  switch (incount) {
  case 1:
  case 2:
    gate[1].count++;
    break;
  case 3:
    gate[2].count++;
    break;
  case 4:
    gate[3].count++;
    break;
  case 5:
  case 6:
  case 7:
  case 8:
    gate[4].count++;
    break;
  case 9:
    gate[5].count++;
    break;
  }
}

void writedigit(int k) {
  if (k < 10) {
    cout << " ";
  } else {
    cout << k / 10;
  }
}

void ICcount(int i) {
  int k;
  switch (i) {
  case 1:
    k = (gate[i].count + 3) / 4;
    break;
  case 2:
    k = (gate[i].count + 2) / 3;
    break;
  case 3:
    k = (gate[i].count + 1) / 2;
    break;
  case 4:
  case 5:
    k = 1;
    break;
  }
  packcount += k;
  cout << "     " << k << " IC packs \n";
}

void writegate(int i) {
  cout << "  " << gate[i].count << "   ";
  nandgate += gate[i].count;
  if (i < 5) {
    cout << gate[i].inputs << "-input NAND gates";
  } else {
    cout << " special NAND gate";
  }
}

void statistics(int ne, int nout) {

  gate[1].inputs = 2;
  gate[2].inputs = 3;
  gate[3].inputs = 4;
  gate[4].inputs = 8;
  cout << "\n\n";
  packcount = 0;
  nandgate = 0;
  for (int i = 1; i < 6; i++) {
    if (gate[i].count > 0) {
      writegate(i);
      ICcount(i);
    }
  }
  int i = ne + nout;
  cout << "  ----------------------    -------------    \n";
  cout << "      " << i << " NAND gates     ";
  cout << "      " << packcount << " IC packs\n";
}

void publish(larray f) {

  for (int i = 1; i < 6; i++) {
    gate[i].count = 0;
  }
  nvars = f.noInputs + f.noOutputs;
  cout << "\n\n";
  for (int k = 1; k < f.noInputs + 1; k++) {
    cout << "X";
  }
  cout << " ";
  for (int k = 1; k < f.noOutputs + 1; k++) {
    cout << "Z";
  }
  cout << "\n";

  for (int k = 1; k < f.noInputs + 1; k++) {
    cout << k;
  }
  cout << " ";
  for (int k = 1; k < f.noOutputs + 1; k++) {
    cout << k;
  }
  cout << "\n";
  writeArray(f);
  cout << "\n\n";
  cout << "           NAND Gate Realization\n";
  cout << "\n\n";

  cout << "  Gate    Fan    Fan        Driving Signal\n";
  cout << "  No.     In     Out\n\n";

  for (int i = 0; i < f.noCubes; i++) {
    if (cubeCost(f.cubes[i], f.noInputs) > 1) {
      pgate('G', i + 1, {
        1
      });
      fanout = 0;
      for (int k = f.noInputs; k < nvars; k++) {
        if (f.cubes[i].arr[k] == (set < int > ) {
            1
          }) {
          fanout++;
        }
      }
      fanin = cubeCost(f.cubes[i], f.noInputs);
      cout << "      " << fanin << "       " << fanout << "    ";
      updategate(fanin);
      for (int k = 0; k < f.noInputs; k++) {
        if (f.cubes[i].arr[k] != (set < int > ) {
            0,
            1
          }) {
          pgate('X', k + 1, f.cubes[i].arr[k]);
        }
      }
      cout << "\n";

    }
  }
  for (int k = 0; k < f.noOutputs; k++) {
    outfanin[k] = 0;
  }
  for (int i = 0; i < f.noCubes; i++) {
    for (int k = f.noInputs; k < nvars; k++) {
      if (f.cubes[i].arr[k] == (set < int > ) {
          1
        }) {
        outfanin[k - f.noInputs]++;
      }
    }
  }
  for (int k = f.noInputs; k < nvars; k++) {
    pgate('Z', k - f.noInputs + 1, {
      1
    });
    cout << "      " << outfanin[k - f.noInputs] << "       1    ";
    updategate(outfanin[k - f.noInputs]);

    for (int i = 0; i < f.noCubes; i++) {
      if (cubeCost(f.cubes[i], f.noInputs) == 1) {
        if (f.cubes[i].arr[k] == (set < int > ) {
            1
          }) {
          pgate('X', i, {
            1
          }); // doubt i or 1 may be
        }
      } else if (f.cubes[i].arr[k] == (set < int > ) {
          1
        }) {
        pgate('G', i, {
          1
        });

      }
    }
    cout << "\n";
  }
  statistics(f.noCubes, f.noOutputs);
}
