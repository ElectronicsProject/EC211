#include <iostream>
#include<string>
using namespace std;

//Compiler version g++ 6.3.0
const int arraymax=40;
const int cubemax=11;






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
larray splitArray(larray,cube);
larray intersection (larray,larray);
  
  
  
class larray{
  public:
    int NoInputs,NoOutputs,NoCubes;
    cube cubes[arraymax];
    
    void initArray(int arraySize,int ninputs,int noutputs){    
      if(ninputs+noutputs>cubemax){
        error("INITTMV");
      }
      else{
        NoInputs=ninputs;
        NoOutputs=noutputs;
        NoCubes=arraySize;
        for(int i=0;i<NoCubes;i++){
          cubes[i].initCube();
        }
      }
    }
    
    void readArray(){
      cin>>NoCubes;
      cin>>NoInputs;
      cin>>NoOutputs;
      this->initArray(NoCubes,NoInputs,NoOutputs);
      
      for(int i=0;i<NoCubes;i++){
        bool err=false;
        for(int j=0;j<NoInputs+NoOutputs;j++){
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
      if(NoCubes==0){
        cout<<"Empty Array\n";
      }
      else{
        for(int i=0;i<NoCubes;i++){
          cubes[i].writeCube(NoInputs,NoOutputs);
        }
      }
    }
    
    //position=0 to delete first element
    void deleteCube(int position){
      if(position>NoCubes-1|| position<0){
        error("DELCUNC");
      }
      else{
        for(int i=position;i<NoCubes-1;i++){
          cubes[i]=cubes[i+1];
          
        }
        NoCubes--;
      }
    }
    
    larray moveCube(int index){
      larray temp;
      temp.initArray(1,NoInputs,NoOutputs);
      temp.cubes[0]=cubes[index];
      this->deleteCube(index);
      return temp;
    }
    
    void appendCube(cube c){
      
      if(NoCubes+1>arraymax){
        error("APCUAOV");
      }
      else{
        cubes[NoCubes]=c;
        NoCubes++;
      }
    }
    
    void combine(larray b){
      if(NoOutputs+NoInputs!=b.NoOutputs+b.NoInputs){
        error("COMBUNV");
      }
      else if(NoCubes+b.NoCubes>=arraymax){
        error("COMBAOV");
      }
      else{
        for(int i=0;i<b.NoCubes;i++){
          cubes[i+NoCubes]=b.cubes[i];
        }
        NoCubes+=b.NoCubes;
      }
    }
    
    void changeColumn(int index,string str){
        if(index>NoInputs+NoOutputs){
          error("CHNGCOR");
        }
        else if(NoCubes>0){
          for(int i=0;i<NoCubes;i++){
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
      if(NoCubes>0){
        int nvars=NoInputs+NoOutputs;
        
        for(int i=0;i<NoCubes-1;i++){
          
          for(int j=i+1;j<NoCubes;){
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
      int nv=NoInputs+NoOutputs;
      larray b,c,d;
      cube mask,all0mask;
      for(int i=NoInputs;i<nv;i++){
        this->changeColumn(i,"01x");
      }
      mask.initCube();
      if(NoOutputs>0){
        all0mask.initCube();
        for(int i=NoInputs;i<nv;i++){
          all0mask.arr[i]='0';
        }
      }
      for(int i=0;i<NoInputs;i++){
        mask.arr[i]='1';
        b=splitArray(*this,mask);
        mask.arr[i]='0';
        c=splitArray(*this,mask);
        this->combine(b);
        this->combine(c);
        if(b.NoCubes>0&&c.NoCubes>0){
          b.changeColumn(i,"xxx");
          c.changeColumn(i,"xxx");
          d=intersection(b,c);
          mask.initCube();
          if(NoOutputs>0&& NoCubes>0){
            b=splitArray(d,all0mask);
          }
          this->absorb();
          if(NoCubes+d.NoCubes>arraymax){
            error("CONSAOV");
          }
          this->combine(d);
          this->absorb();
        }
        mask.arr[i]='x';
      }
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

larray cubeSharp(cube a,cube b,int size){
  cube c;
  larray x;
  if(!cubex(c,b,a,size)){
    x.NoCubes=1;
    x.cubes[0]=a;
  }
  else{
    x.NoCubes=0;
    for(int i=0;i<size;i++){
      if(a.arr[i]=='x'){
        if(c.arr[i]=='0'){
          x.cubes[x.NoCubes]=a;
          x.cubes[x.NoCubes].arr[i]='1';
          x.NoCubes++;
       }else if(c.arr[i]=='1'){
          x.cubes[x.NoCubes]=a;
          x.cubes[x.NoCubes].arr[i]='0';
          x.NoCubes++;
      }
    }
  }
}
return x;
}
//a=b sharp c
larray sharp(larray &a,larray &b){
  larray c;
  int nvars=a.NoInputs+a.NoOutputs;
  if(nvars!=b.NoInputs+b.NoOutputs){
    error("SHRPUNV");
  }
  else{
    
    larray t;
    c=a;
    for(int i=0;i<b.NoCubes;i++){
      c.NoCubes=0;
      for(int k=0;k<a.NoCubes;k++){
        t=cubeSharp(a.cubes[k],b.cubes[i],nvars);
        c.combine(t);
        if(c.NoCubes>arraymax/2){
          c.absorb();
        }
      }
      a=c;
    }
    c.absorb();
  }
  return c;
}

larray splitArray(larray a, cube mask){
  larray b;
  b.initArray(0,a.NoInputs,a.NoOutputs);
  int nvars=a.NoInputs+a.NoOutputs;
  if(a.NoCubes>0){
    int j=0;
    while(j<a.NoCubes && a.NoCubes>0){
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

larray intersection (larray b,larray c){
  larray a;
  a.initArray(0,b.NoInputs,b.NoOutputs);
  if(b.NoInputs+b.NoOutputs==c.NoInputs+c.NoOutputs){
    int nv=b.NoInputs+b.NoOutputs;
    for(int i=0;i<b.NoCubes;i++){
      for(int j=0;j<c.NoCubes;j++){
        if(a.NoCubes+1>arraymax){
          error("INTXAOV");
        }
        else if(cubex(a.cubes[a.NoCubes],b.cubes[i],c.cubes[j],nv)){
          a.NoCubes++;
        } 
        
        if(a.NoCubes+1>arraymax/2){
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
00010
00101
00110
01001
01010
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
*/
int main() 
{
    cout << "Hello, Dcoder!\n";
    
    larray f;
    f.readArray();
    cout<<"\nArray f\n";
    f.writeArray();
    
    f.consensus();
    cout<<"\nReduced Array f\n";
    f.writeArray();
    
    
}
