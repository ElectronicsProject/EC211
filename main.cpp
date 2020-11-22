#include <iostream>
#include<string>
using namespace std;

//Compiler version g++ 6.3.0
const int arraymax=400;
const int cubemax=111;


class larray;

void error(string str){
  if(str=="INITTMV"){
    cout<<"INITARRAY:too many variables";
    exit(1);
  }
  else if(str=="COMBUNV"){
    cout<<"COMBINE: unequal number of variables";
    exit(1);
  }
  else if(str=="COMBAOV"){
    cout<<"COMBINE:array overflow";
    exit(1);
  }
  else if(str=="CHNGCOR"){
    cout<<"CHANGECOLUMN: Column out of range";
    exit(1);
  }
  else if(str=="DELCUNC"){
    cout<<"DELETECUBE: nonexistent cube";
    exit(1);
  }
  else if(str=="APCUAOV"){
    cout<<"APPENDCUBE:array overflow";
    exit(1);
  }
  else if(str=="SHRPUNV"){
    cout<<"SHARPS: Unequal number of variable";
    exit(1);
  }
  else if(str=="INTXAOV"){
    cout<<"INTERSECTION: array overflow";
    exit(1);
  }
  else if(str=="INTXUNV"){
    cout<<"INTERSECTION: Unequal number of variable";
    exit(1);
  }
  else if(str=="CONSAOV"){
    cout<<"CONSENSUS: Array overflow";
    exit(1);
  }
} 



class cube{
  public:
    char arr[cubemax];
    
    //initcube
    void initCube(){
      for(int i=0;i<cubemax;i++){
        arr[i]='x';
      }
    }
    
   
    void writeCube(int ni,int no){
      int n=ni+no;
      cout<<"\n";
      for(int i=0;i<n;i++){
        if(i==ni){
          cout<<" ";
        }
        
        if(arr[i]=='0'){
          cout<<"0";
        }
        else if(arr[i]=='1'){
          cout<<"1";
        }
        else if(i>=ni){
          cout<<"-";
        }
       else{
          cout<<"x";
        }
      }
    }
};


bool subsume(cube,cube,int);//declaration
larray splitArray(larray &,cube);
larray intersection (larray &,larray &);
  
  
  
class larray{
  public:
    int noInputs,noOutputs,noCubes;
    cube cubes[arraymax];
    
    void initArray(int arraySize,int ninputs,int noutputs){    
      if(ninputs+noutputs>cubemax){
        error("INITTMV");
      }
      else{
        noInputs=ninputs;
        noOutputs=noutputs;
        noCubes=arraySize;
        for(int i=0;i<noCubes;i++){
          cubes[i].initCube();
        }
      }
    }
    
    void readArray(){
      cin>>noCubes;
      cin>>noInputs;
      cin>>noOutputs;
      this->initArray(noCubes,noInputs,noOutputs);
      
      for(int i=0;i<noCubes;i++){
        bool err=false;
        for(int j=0;j<noInputs+noOutputs;j++){
          if(!err){
            char c;
            cin>>c;
            while(c==' '){
              cin>>c;
            }
            if(c!='0' && c!='1'&& c!='x'&& c!='X'){
              cout<<"Invalid input found\n";
              exit(1);
            }
            else{
              cubes[i].arr[j]=tolower(c);
            }
          }
        }
      }
    }
    void writeArray(){
      if(noCubes==0){
        cout<<"Empty Array\n";
      }
      else{
        for(int i=0;i<noCubes;i++){
          cubes[i].writeCube(noInputs,noOutputs);
        }
      }
    }
    
    //position=0 to delete first element
    void deleteCube(int position){
      if(position>noCubes-1|| position<0){
        error("DELCUNC");
      }
      else{
        for(int i=position;i<noCubes-1;i++){
          cubes[i]=cubes[i+1];
          
        }
        noCubes--;
      }
    }
    
    larray moveCube(int index){
      larray temp;
      temp.initArray(1,noInputs,noOutputs);
      temp.cubes[0]=cubes[index];
      this->deleteCube(index);
      return temp;
    }
    
    void appendCube(cube c){
      
      if(noCubes+1>arraymax){
        error("APCUAOV");
      }
      else{
        cubes[noCubes]=c;
        noCubes++;
      }
    }
    
    void combine(larray &b){
      if(noOutputs+noInputs!=b.noOutputs+b.noInputs){
        error("COMBUNV");
        
      }
      else if(noCubes+b.noCubes>=arraymax){
        error("COMBAOV");
      }
      else{
        for(int i=0;i<b.noCubes;i++){
          cubes[i+noCubes]=b.cubes[i];
        }
        noCubes+=b.noCubes;
      }
    }
    
    void changeColumn(int index,string str){
        if(index>noInputs+noOutputs){
          error("CHNGCOR");
        }
        else if(noCubes>0){
          for(int i=0;i<noCubes;i++){
            char x=cubes[i].arr[index];
            if(x=='0'){
              cubes[i].arr[index]=str[0];
            }
            else if(x=='1'){
              cubes[i].arr[index]=str[1];
            }
            else{
              cubes[i].arr[index]=str[2];
            }
          }
        }
    }
    
    void absorb(){
      if(noCubes>0){
        int nvars=noInputs+noOutputs;
        
        for(int i=0;i<noCubes-1;i++){
          
          for(int j=i+1;j<noCubes;){
            if(subsume(cubes[i],cubes[j],nvars)){
              this->deleteCube(i);
            }
            else if(subsume(cubes[j],cubes[i],nvars)){
              this->deleteCube(j);
            }
            else{
              j++;
            }
          }
        }
      }
    }
    
    void consensus(){
      int nv=noInputs+noOutputs;
      larray b,c,d;
      cube mask,all0mask;
      for(int i=noInputs;i<nv;i++){
        this->changeColumn(i,"01x");
      }
      mask.initCube();
      if(noOutputs>0){
        all0mask.initCube();
        for(int i=noInputs;i<nv;i++){
          all0mask.arr[i]='0';
        }
      }
      for(int i=0;i<noInputs;i++){
        mask.arr[i]='1';
        b=splitArray(*this,mask);
        mask.arr[i]='0';
        c=splitArray(*this,mask);
        this->combine(b);
        this->combine(c);
        if(b.noCubes>0&&c.noCubes>0){
          b.changeColumn(i,"xxx");
          c.changeColumn(i,"xxx");
          d=intersection(b,c);
          mask.initCube();
          if(noOutputs>0&& noCubes>0){
            b=splitArray(d,all0mask);
          }
          this->absorb();
          if(noCubes+d.noCubes>arraymax){
            error("CONSAOV");
          }
          this->combine(d);
          this->absorb();
        }
        mask.arr[i]='x';
      }
      this->absorb();
    }
    
};

bool cubex(cube &a,cube b,cube c,int size){
  
  for(int i=0;i<size;i++){
     if(b.arr[i]=='x'){
       a.arr[i]=c.arr[i];
     }
     else  if(c.arr[i]=='x'){
       a.arr[i]=b.arr[i];
     }
     else  if(b.arr[i]==c.arr[i]){
       a.arr[i]=b.arr[i];
     }
     else {
       return false;
     }
  }
  return true;
}

larray cubeSharp(cube a,cube b,int in,int out){
  cube c;
  larray x;
  x.initArray(0,in,out);
  int size =in+out;
  if(!cubex(c,b,a,size)){
    x.noCubes=1;
    x.cubes[0]=a;
  }
  else{
    x.noCubes=0;
    for(int i=0;i<size;i++){
      if(a.arr[i]=='x'){
        if(c.arr[i]=='0'){
          x.cubes[x.noCubes]=a;
          x.cubes[x.noCubes].arr[i]='1';
          x.noCubes++;
       }else if(c.arr[i]=='1'){
          x.cubes[x.noCubes]=a;
          x.cubes[x.noCubes].arr[i]='0';
          x.noCubes++;
      }
    }
  }
}
return x;
}
//a=b sharp c
larray sharp(larray a,larray &b){
  larray c;
  int nvars=a.noInputs+a.noOutputs;
  if(nvars!=b.noInputs+b.noOutputs){
    error("SHRPUNV");
  }
  else{
    
    larray t;
    t.initArray(0,a.noInputs,a.noOutputs);
    c=a;
    for(int i=0;i<b.noCubes;i++){
      c.noCubes=0;
      for(int k=0;k<a.noCubes;k++){
        t=cubeSharp(a.cubes[k],b.cubes[i],a.noInputs,a.noOutputs);
        c.combine(t);
        if(c.noCubes>arraymax/2){
          c.absorb();
        }
      }
      a=c;
    }
    c.absorb();
  }
  return c; 
}

larray splitArray(larray &a, cube mask){
  larray b;
  b.initArray(0,a.noInputs,a.noOutputs);
  int nvars=a.noInputs+a.noOutputs;
  if(a.noCubes>0){
    int j=0;
    while(j<a.noCubes && a.noCubes>0){
      if(subsume(a.cubes[j],mask,nvars)){
        b.appendCube(a.cubes[j]);
        a.deleteCube(j);
      }
      else{
        j++;
      }
    }
  }
  return b;
}

larray intersection(larray &b,larray &c){
  larray a;
  a.initArray(0,b.noInputs,b.noOutputs);
  if(b.noInputs+b.noOutputs==c.noInputs+c.noOutputs){
    int nv=b.noInputs+b.noOutputs;
    for(int i=0;i<b.noCubes;i++){
      for(int j=0;j<c.noCubes;j++){
        if(a.noCubes+1>arraymax){
          error("INTXAOV");
        }
        else if(cubex(a.cubes[a.noCubes],b.cubes[i],c.cubes[j],nv)){
          a.noCubes++;
        } 
        
        if(a.noCubes+1>arraymax/2){
          a.absorb();
        }
      }
    }
    a.absorb();
  }
  else{
    error("INTXUNV");
  }
  return a;
}
int cubeCost(cube a,int n){
  int count=0;
  for(int i=0;i<n;i++){
    if(a.arr[i]!='x'){
      count++;
    }
  }
  return count;
}

bool subsume(cube a,cube b,int size){
  bool sub=true;
  int i=0;
  while(sub && i<size){
    if(a.arr[i]=='x' && (b.arr[i]=='0' || b.arr[i]=='1') ){
      sub=false;
    }
    else if(a.arr[i]=='0'&&b.arr[i]=='1'){
      sub=false;
    }
    else if(a.arr[i]=='1'&&b.arr[i]=='0'){
      sub=false;
    }
    else{
      i++;
    }
  }
  return sub;
}

// multiple output minimization procedures

void epi(larray &e,larray &f,larray &ON){
  int i;
  larray d1,a,b;
  int npi=f.noCubes;
  i=0;
  cout<<"here is the golmal";
  while(i<npi && ON.noCubes>0){
     d1=f.moveCube(0); 
     a=intersection(ON,d1);
    
     if(a.noCubes>0){
       
       b=sharp(a,f);
       if(b.noCubes==0){
         f.combine(d1);
       }
       else{
         e.combine(d1);
         a=sharp(ON,d1);
         ON=a;
       }
     }
     i++;
  }
}

void lessthan(larray &PI,larray &ON){
  larray a,b,d1,d2;
  bool delcube;
  int npi=PI.noCubes;
  d2.initArray(1,PI.noInputs,PI.noOutputs);
  bool broken=false;
  int i=0;
  while(i<npi && !broken){
    delcube=false;
    d1=PI.moveCube(0);
    a=intersection(ON,d1);
    if(a.noCubes==0){
      delcube=true;
    }
    else{
      int costi=cubeCost(d1.cubes[0],PI.noInputs);
      int j=0;
      while(j<PI.noCubes && !broken){
        if(costi>=cubeCost(PI.cubes[j],PI.noInputs)){
          d2.cubes[0]=PI.cubes[j];
          b=sharp(a,d2);
          if(b.noCubes==0){
            delcube=true;
            broken=true;
          }
        }
        j++;
      }
    }
    if(!delcube){
      PI.combine(d1);
    }
    i++;
  }

}

void pickCube(larray &cover,larray &PI,larray &ON){
  cout<<"pickcube start";
  larray a,b;
  a.initArray(0,PI.noInputs,PI.noOutputs);
  int j=0,cost;
  int mincost=PI.noInputs;
  for(int i=0;i<PI.noCubes;i++){
    cost=cubeCost(PI.cubes[i],PI.noInputs);
    if(cost<mincost){
      mincost=cost;
      j=i;
    }
  }
  a=PI.moveCube(j);
  cover.combine(a);
  b=sharp(ON,a);
  ON=b;
  cout<<"pick cube end";
}

void momin(larray &cover,larray PI,larray ON ){
  bool loop;
  int npi;
  cover.initArray(0,ON.noInputs,ON.noOutputs);
  loop=true;
  do{
   epi(cover,PI,ON);
   if(ON.noCubes>0){
      npi=PI.noCubes;
      lessthan(PI,ON); // important fixed
      
      if(npi==PI.noCubes){
        pickCube(cover,PI,ON);
        
        if(ON.noCubes==0){
          loop=false;
        }
      }
    }
    else{
      loop=false;    
    }
    
  }while(!loop);
  for(int i=cover.noInputs;i<cover.noInputs+cover.noOutputs;i++){
    cover.changeColumn(i,"011");
  }
}


//connection minization techniques

void conmin(larray &E,larray ON){
  int ni,nvars,i,j,EINcubes;
  cube Mask,eij;
  larray Ei,EINi,Etemp,ONi,d;
  
  if(E.noCubes>0 && ON.noCubes>0){
    ni=E.noInputs;
    nvars=ni+E.noOutputs;
    Mask.initCube();
    for(i=ni;i<nvars;i++){
      Mask.arr[i]='1';
      E.changeColumn(i,"011");
      Ei=splitArray(E,Mask);
      if(Ei.noCubes>0){
        EINi=Ei;
        EINcubes=Ei.noCubes;
        EINi.noOutputs=0;
        ONi=splitArray(ON,Mask);
        ON.combine(ONi);
        ONi.noOutputs=0;
        for(j=0;j<EINcubes;j++){
          eij=Ei.cubes[0];
          Ei.deleteCube(0);
          Etemp=EINi.moveCube(0);
          d=intersection(Etemp,ONi);
          d=sharp(d,EINi);
          if(d.noCubes>0){
            EINi.appendCube(Etemp.cubes[0]);
            Ei.appendCube(eij);
          }
          else{
            eij.arr[i]='0';
            E.appendCube(eij);
          }
        }
        E.combine(Ei);
      }
      Mask.arr[i]='x';
    }
    for(i=ni;i<nvars;i++){
      Mask.arr[i]='0';
    }
    Ei=splitArray(E,Mask);
  }
}










class Gatetype{
  public:
    int inputs;
    int count;
};

int nvars,i,k,fanin,fanout;
int nandgate=0;

int outfanin[cubemax];
Gatetype gate[6];


  void pgate(char g,int gateno,char x){
    cout<<" ";
    if(x=='0'){
      cout<<"-";
    }
    else{
      cout<<" ";
    }
    cout<<g;
    if(gateno>9){
      cout<<gateno/10;
      gateno%=10;
    }
    else{
      cout<<"0";
    }
    cout<<gateno;  
  }


  void updategate(int incount){
    if(incount>8){
      incount=9;
    }
    switch(incount){
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

 void writedigit(int k){
    if(k<10){
      cout<<" ";
    }
    else{
      cout<< k/10;
    }
  }
  
 int packcount;
   
 void ICcount(int i){
      int k;
      switch(i){
        case 1:
          k=(gate[i].count+3)/4;
          break;
        case 2:
          k=(gate[i].count+2)/3;
          break;
        case 3:
          k=(gate[i].count+1)/2;
          break;
        case 4:
        case 5:
          k=1;
          break;
      }
      packcount+=k;
      cout<<"     "<<k<<" IC packs \n";
    }
    
   void writegate(int i){
      cout<<"  "<<gate[i].count<<"   ";
      nandgate+=gate[i].count;
      if(i<5){
        cout<<gate[i].inputs<<"-input NAND gates";
      }
      else{
        cout<<" special NAND gate";
      }
    }
    
  void statistics(int ne,int nout){
    
    
    
    gate[1].inputs=2;
    gate[2].inputs=3;
    gate[3].inputs=4;
    gate[4].inputs=8;
    cout<<"\n\n";
    packcount=0;
    nandgate=0;
    for(int i=1;i<6;i++){
      if(gate[i].count>0){
        writegate(i);
        ICcount(i);
      }
    }
    i=ne+nout;
    cout<<"  ----------------------    -------------    \n";
    cout<<"      "<<i<<" NAND gates     ";
    cout<<"      "<<packcount<<" IC packs\n";
  }
  
   
void publish(larray f){

  
  for(int i=1;i<6;i++){
    gate[i].count=0;
  }
  nvars=f.noInputs+f.noOutputs;
  cout<<"\n\n";
  for(k=1;k<f.noInputs+1;k++){
    cout<<"X";
  }
  cout<<" ";
  for(k=1;k<f.noOutputs+1;k++){
    cout<<"Z";
  }
  cout<<"\n";
  
  for(k=1;k<f.noInputs+1;k++){
    cout<<k;
  }
  cout<<" ";
  for(k=1;k<f.noOutputs+1;k++){
    cout<<k;
  }
  cout<<"\n";  
  f.writeArray();
  cout<<"\n\n";
  cout<<"           NAND Gate Realization\n";
  cout<<"\n\n";
  
  cout<<"  Gate    Fan    Fan        Driving Signal\n";
  cout<<"  No.     In     Out\n\n";
  
  
  
  for(i=0;i<f.noCubes;i++){
    if(cubeCost(f.cubes[i],f.noInputs)>1){
      pgate('G',i+1,'1');
      fanout=0;
      for(k=f.noInputs;k<nvars;k++){
        if(f.cubes[i].arr[k]=='1'){
          fanout++;
        }
      }
      fanin=cubeCost(f.cubes[i],f.noInputs);
      cout<<"      "<<fanin<<"       "<<fanout<<"    ";
      updategate(fanin);
      for(k=0;k<f.noInputs;k++){
        if(f.cubes[i].arr[k]!='x'){
          pgate('X',k+1,f.cubes[i].arr[k]);
        }
      }
      cout<<"\n";
     
    }
  }
  for(k=0;k<f.noOutputs;k++){
    outfanin[k]=0;
  }
  for(i=0;i<f.noCubes;i++){
    for(k=f.noInputs;k<nvars;k++){
      if(f.cubes[i].arr[k]=='1'){
        outfanin[k-f.noInputs]++;
      }
    }
  }
  for(k=f.noInputs;k<nvars;k++){
    pgate('Z',k-f.noInputs+1,'1');
    cout<<"      "<<outfanin[k-f.noInputs]<<"       1    ";
    updategate(outfanin[k-f.noInputs]);
    
    for(i=0;i<f.noCubes;i++){
      if(cubeCost(f.cubes[i],f.noInputs)==1){
        if(f.cubes[i].arr[k]=='1'){
          pgate('X',i,'1'); // doubt i or 1 may be
        }
      }
      else if(f.cubes[i].arr[k]=='1'){
        pgate('G',i,'1');
        
      }
    }
    cout<<"\n";
  }
  statistics(f.noCubes,f.noOutputs);
}
/*

9
4
3
0001001
0010101
0011001
0100110
0101010
1010111
1011111
1100101
1101110

9
4
1
00011
00101
00111
01001
01011
10101
10111
11001
11011

6
4
1
00101
01001
10101
10111
11001
11011

9
4
1
00000
00010
01000
01100
01110
11000
11100
0010X
0101x


7
4
1
00101
00111
01001
10111
11001
11011
11111

8
3
2
00011
00111
01011
01111
10011
10111
11011
11111
*/
int main() 
{
    cout << "Hello, Dcoder!\n";
    
    larray f;
    f.readArray();
    cout<<"\nArray f\n";
    f.writeArray();
    
    larray a,ON;
    int nin=f.noInputs;
    int nout=f.noOutputs;
    cube mask;
    mask.initCube();
   
    ON.initArray(0,nin,nout);
    for(int i=nin;i<nin+nout;i++){
      mask.arr[i]='1';
    
      a=splitArray(f,mask);
      f.combine(a);
      for(int j=nin;j<nin+nout;j++){
        if(i!=j){
          a.changeColumn(j,"000");
          
        }
      }
      ON.combine(a);
      mask.arr[i]='x';
      
    }   
     
    if(f.noOutputs>1){
   //   publish(f);
      f.consensus();
      momin(a,f,ON);
      conmin(a,ON);
      publish(a);
    }
    else{
  //  publish(f);
    f.consensus();
    publish(f);
    }
}



