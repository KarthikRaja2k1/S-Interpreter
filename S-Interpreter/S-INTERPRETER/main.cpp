#include <iostream>
#include<cstdlib>
#include<math.h>
#include<string>
#include<cstring>
#include<fstream>
using namespace std;
int lineno=1,scope=0,Loop=0;

ifstream fin;
int countmg=0;
int headers[2]={1,0};
char extra=0;
char z;
//UPto this global



void Tokenizer(char *&Token){
char* result=0;
char b;
int i=2;

int check=0;

if(!fin.eof())
{if(extra!=0){
    b=extra;
extra=0;
}else
fin.get(b);
while(((b>='A' && b<='Z')||(b>='a' && b<='z') ||(b>='0' && b<='9')) && !fin.eof()){
    check=1;
    char *tmp= new char[i];
    if(i==2){
        tmp[1]=0;
        tmp[0]=b;
    }
    else{
        strcpy(tmp,result);

               tmp[i-2]=b;
               tmp[i-1]=0;
        //cout<<tmp<<endl;
        delete[] result;

    }
    result=tmp;

    i++;

    fin.get(b);

extra=b;

}
if(check==0){

    char *tmp= new char[2];
    tmp[1]=0;
    tmp[0]=b;

    result=tmp;
    }
    Token =result;}
    else cout<<"TOkenize rcalled after file ended";
}
void Gettoloop(){
fin.seekg(ios::beg);
int times=0;
char *tmp;
while(times!=Loop){
    Tokenizer(tmp);
    if(strcmp(tmp,"swhile")==0){
        times++;
    }
    delete[] tmp;
}

}
int isnum(char *tmp){
int check=2;
int neg=0;
int dot=0;
int i=0;
while(tmp[i]!=0 &&check==2){
if(tmp[i]>='0' &&tmp[i]<='9')check=2;
else if(tmp[i]=='-')neg++;
else if(tmp[i]=='.')dot++;
else if(neg>1 || dot>1)check=0;
else check=0;
i++;
}

if(dot==1&& neg<=1)check=1;

return check;
}
class Symrow{
double d;
char *n;
char type;
int Scope;
public:
Symrow *next;

    Symrow(){
     type='i';
     d=0;
    }
    void Createvar(int sc,char *tmp,char t=0){
        Scope=sc;
        if(t!=0)
        type=t;
        n=new char[sizeof(tmp)];
        strcpy(n,tmp);
    }

    ~Symrow(){
        delete[] n;
    }
    void Nextrow(Symrow *top){
    next=top;
    }
    void getScope(int &sc){
    sc=Scope;
    }
    void getname(char *&naam){
    naam=new char[strlen(n)+1];
    strcpy(naam,n);
    }
    //below are used access the variable
    void setdata(double a){
        d=a;
    }
    void putdata(){
    if(type!='c'){
    cin>>d;
    if(type=='i')
    d=(int)d;
    }
    else{
        char x;
        cin>>x;
        d=x;
    }
    }
    void getdata(double &a){
    a=d;
    }
    void gettype(char &a){
    a=type;}
    void settype(char a){
    type=a;
    }
};

class Symtable{
Symrow *Top;
public:
    Symtable(){ Top=0;}
    void varPush(int sc,char t,char *name){
        Symrow *tmp= new Symrow();
        if(tmp){
        tmp->Createvar(sc,name,t);
        tmp->Nextrow(Top);
        Top=tmp;
        }
          }
    void varPop(){
        Symrow *tmp;
        tmp=Top;
        Top=tmp->next;
        delete tmp;
        }
    void getTopScope(int &sc){
    if(Top)
    Top->getScope(sc);
    else sc=-1;
    }
    int Searchvar(char *n,Symrow *&var){
    Symrow *tmp=Top;
    char *naam;
    int check=0;
    while(check==0 && tmp!=0){
        tmp->getname(naam);
        if(strcmp(naam,n)==0){
            check=1;
            var=tmp;
        }
        else{
            tmp=tmp->next;
        }
    }
    if(check==0 && tmp==0)
        var=0;
    else check=1;

    return check;}
}MSymtable;
int compop(char *op1,char *op2){
    int check=0;
if((strcmp(op1,"*")==0 || strcmp(op1,"/")==0 || strcmp(op1,"%")==0 || strcmp(op1,"^")==0) &&(strcmp(op2,"+")==0 || strcmp(op2,"-")==0) )
    check=1;
    else if((strcmp(op1,"*")==0 || strcmp(op1,"/")==0 || strcmp(op2,"%")==0 || strcmp(op2,"^")==0)&& (strcmp(op2,"*")==0 || strcmp(op2,"/")==0 || strcmp(op2,"%")==0 || strcmp(op2,"^")==0) )
    check=1;
    else if((strcmp(op2,"+")==0 || strcmp(op2,"-")==0) && (strcmp(op1,"+")==0 || strcmp(op1,"-")==0))
        check=1;
return check;
}
double OPerate(double d1,double d2,const char *tmp){
double check;

if(strcmp(tmp,"+")==0)
    check=d2+d1;
else if(strcmp(tmp,"-")==0)
    check=d2-d1;
else if(strcmp(tmp,"*")==0)
    check=d2*d1;
else if(strcmp(tmp,"/")==0)
    check=d2/d1;
else if(strcmp(tmp,"^")==0)
    check=pow(d2,d1);
else if(strcmp(tmp,"%")==0)
    check = (int)d2%(int)d1;
else {exit(0);cerr<<"UNDEFINED OPERATOR USED";}
    return check;
}
struct Node{
char *str;
Node *next;
Node(){str=0;next=0;}
~Node(){
    delete[] str;
    }
};

struct NumNode{
    double d;
    NumNode *next;
};


class Stck{
Node *Top;
NumNode*nTop;
public:
    Stck(){
    nTop=0;
    Top=0;
    }
    void Push(char *Token){
    Node *tmp =new Node;

    tmp->str =new char[strlen(Token)+1];
    strcpy(tmp->str,Token);
    tmp->next=Top;
    Top=tmp;
        }
    void Pop(char *&Token){
    Node *tmp;
    if(Top!=0){
    tmp=Top;
    Token=new char[strlen(Top->str)+1];
    strcpy(Token,Top->str);

    Top=tmp->next;
    delete tmp;}
    else cerr<<"empty stack";
    }

    void NumPush(double Token){
    NumNode *tmp =new NumNode;
    tmp->d=Token;
    tmp->next=nTop;
    nTop=tmp;
        }
    void NumPop(double &Token){
    NumNode *tmp;
    tmp=nTop;
    Token=tmp->d;
    nTop=tmp->next;

    delete tmp;
    }
    int Empty(){
    return Top==0;
    }
     int nEmpty(){
    return nTop==0;
    }
    ~Stck(){
     Node *tmp;
     while(Top!=0){
    tmp=Top;
    Top=tmp->next;
    delete tmp;
     }
    }
};
class Que{
Node *Front;
Node *Rear;
public:
    Que(){
    Front=0;
    Rear=0;
    }
    void INSERT(char *Token){
        Node *tmp = new Node;
        tmp->str=new char[strlen(Token)+1];
        strcpy(tmp->str,Token);
        if(Front!=0){
            Rear->next=tmp;
            Rear=tmp;
        }
        else {
                Rear=tmp;
            Front=Rear;
        }

    }
    void DELETE(char *&Token){

    if(Front==0){
        cerr<<"Que Empty";
    }else {
    Node*tmp;
    tmp=Front;
    Front=tmp->next;
    delete[] Token;
    Token =new char[strlen(tmp->str)+1];
    strcpy(Token,tmp->str);
    delete tmp;
    }

    }
    int Empty(){

          return Front==0;
    }
    ~Que(){
    Node*tmp;
    while(Front!=0)

    tmp=Front;
    Front=tmp->next;
    delete tmp;
    }
};

void Tostring(double d,char *&tmp){
int neg=0;
if(d<0){neg++; d=d*-1;}
int i;
int nop=0;
int noc=0;//number of characters
i=d;
while(d!=i){
    nop++;
    d=d*10;
    i=d;
}
while(i!=0){
    noc++;
    i=d/pow(10,noc);
}
noc++;//to include '.'
int p=noc;
if(neg)p++;
char *a=new char[p];
for(int m=0;m<noc;m++){
   if(m!=nop){
    a[m]=(int)d%10+ '0';
    d=d/10;
   }
    else   a[m]='.';

    }

    if(neg)a[p-1]='-';

int n;

tmp=new char[p+1];
for(n=0;n<p;n++){
    tmp[n]=a[p-1-n];
}

tmp[p]=0;
}


double strdo(char *Token){
int whole=0,places=0,i,check=0,neg=0;
double d;
for(i=0;Token[i]!=0;i++){

        if(Token[i]!='.' && Token[i]!='-')
    whole=whole*10+(Token[i]-'0');

    if(check!=0)places++;
    if(Token[i]=='.')
        check=1;
        if(Token[i]=='-')
            neg++;
}
d =whole*pow(10,(-1*places));
if(neg>0)d=d*(-1);
return d;
}
class CALC{
double result;
Que Q;
int ob,cb;
Stck opStack,numStack,RPNStack;
public:
CALC(){
result=2;
ob=0;
cb=0;
}
private:
void POST(){
    char *tmp;//for carrying
    char *tmp1;//for checking between the 2 op
    int check=0;
    while(Q.Empty()==0){
    Q.DELETE(tmp);
    if(strcmp(tmp,"+")==0 || strcmp(tmp,"-")==0 || strcmp(tmp,"/")==0 || strcmp(tmp,"*")==0 || strcmp(tmp,"%")==0 ||strcmp(tmp,"^")==0 ||strcmp(tmp,")")==0|| strcmp(tmp,"(")==0 )
        {if(strcmp(tmp,"(")==0)
        {  opStack.Push(tmp);
            }
        else if(strcmp(tmp,")")==0)
            {    delete[] tmp;
                check=0;
                opStack.Pop(tmp);
                while(strcmp(tmp,"(")!=0 && check==0){
                      RPNStack.Push(tmp);
                      delete[] tmp;
                      if(opStack.Empty()==1){
                        check=1;
                      }
                      else
                        opStack.Pop(tmp);
                }

            }
        else {
            if(opStack.Empty()!=1){
        opStack.Pop(tmp1);
        if(compop(tmp1,tmp)){/// UNdefined checks operator priority if tmp1>tmp
        opStack.Push(tmp1);
        delete[] tmp1;
        int check=0;
        while(opStack.Empty()==0 && check==0){
        opStack.Pop(tmp1);
        if(compop(tmp1,tmp))
        RPNStack.Push(tmp1);
        else{opStack.Push(tmp1);check=1;}
        delete[] tmp1;
        }

        }
        else opStack.Push(tmp1);
        }

        opStack.Push(tmp);

        }}

    else if (isnum(tmp)){
        RPNStack.Push(tmp);


    }
    else {cerr<<"some error in POST function  :: "; cerr<<tmp<<"  "; Q.~Que(); exit(0);}//safe case
    delete[] tmp;
    }

    while(opStack.Empty()==0){
    opStack.Pop(tmp);
    RPNStack.Push(tmp);
    delete[] tmp;
        }
    Calculate();
    }
public:
void RPN(){

char *tmp;
int check=1;
while(check==1){
    Tokenizer(tmp);
    if(isnum(tmp)){
        Q.INSERT(tmp);

        }
    else if(strcmp(tmp,"neg")==0){
        Q.INSERT((char*)"-1");///Hav eto update


    }
    else if(strcmp(tmp,"sdouble")==0){
        delete[] tmp;
        int sizenum=0;
        char *num=0,*tmp2=0;
        Tokenizer(tmp);
        if(strcmp(tmp,"<")==0){
            delete[] tmp;
            Tokenizer(tmp);
            while((isnum(tmp)|| strcmp(tmp,"-")==0 || strcmp(tmp,".")==0)&& strcmp(tmp,">")!=0){
                sizenum = sizenum + strlen(tmp);
                num =new char[sizenum+1];
                *num=0;
                if(tmp2){
                strcat(num,tmp2);
                delete[] tmp2;
             }

                strcat(num,tmp);
                tmp2 =num;
                delete[] tmp;
                Tokenizer(tmp);
                }
            if(strcmp(tmp,">")==0){
                delete[] tmp;
                tmp=num;
                Q.INSERT(tmp);
            }else {cerr<<"NO CLOSING"<<endl;exit(0);}
        }else {cerr<<"NO OPEN BRACKETS"<<endl;exit(0);}

    }

    else if(strcmp(tmp,"scompnum")==0){
    double a,b,Chuc=-3;

        Tokenizer(tmp);
        while(Chuc!=0){
       if(strcmp(tmp,"<")==0){
            CALC rpn;
            rpn.RPN();
            a=rpn.getResult();
            delete[] tmp;

            rpn.RPN();
            b=rpn.getResult();
            //cout<<"WHy do i need it here";
            Chuc=0;}
        else {
        delete[] tmp;
         Tokenizer(tmp);}
    }
   if(a<b)Chuc=-1;
    else if(a==b)Chuc=0;
    else if(a>b) Chuc=1;
   Tostring(Chuc,tmp);
   Q.INSERT(tmp);
    }
    else if(strcmp(tmp,"sequi")==0){
      delete[] tmp;
        double d1,d2,Checck=-3;

        Tokenizer(tmp);
        while(Checck!=0){

       if(strcmp(tmp,"<")==0){
            CALC rpn;
            rpn.RPN();
            d1=rpn.getResult();

            delete[] tmp;
            CALC rpn1;
            rpn1.RPN();
            d2=rpn1.getResult();
            Checck=0;
            //cout<<"again why";
            }
        else {
        delete[] tmp;
         Tokenizer(tmp);}
    }
   if(d1==d2)Checck=1;
    else Checck=0;
   Tostring(Checck,tmp);
   Q.INSERT(tmp);
    }



    else if(strcmp(tmp,"and")==0){
        Q.INSERT((char*)"*");

    }
    else if(strcmp(tmp,"or")==0){
        Q.INSERT((char*)"+");


    }

    else if(strcmp(tmp,"+")==0 || strcmp(tmp,"-")==0 || strcmp(tmp,"/")==0 || strcmp(tmp,"*")==0 || strcmp(tmp,"%")==0 ||strcmp(tmp,"^")==0){
       Q.INSERT(tmp);

    }
    else if(strcmp(tmp,"(")==0 || strcmp(tmp,")")==0){
        if(strcmp(tmp,"(")){ob++;Q.INSERT(tmp);}
        if(strcmp(tmp,")")){cb++;
            if(cb>ob)
                check=0; //cerr<<" RPN close brackets exceeded open brackets"<<endl;
            else Q.INSERT(tmp);
        }
    }
    else { Symrow *variable;
             double d;
            if(MSymtable.Searchvar(tmp,variable)){// if it a variable;
                variable->getdata(d);
                delete[] tmp;
            Tostring(d,tmp);//Not done
            Q.INSERT(tmp);
            }
            else if(strcmp(tmp,"]")==0 || strcmp(tmp,"}")==0 || strcmp(tmp,",")==0 || strcmp(tmp,">")==0 ){

                check=0;
            }
            else if(strcmp(tmp," ")==0){
                //do nothing
            }
            else{

                d=tmp[0];//double declared in 2 scope above here
            Tostring(d,tmp);
            Q.INSERT(tmp);
            }
    }
    delete[] tmp;
    }
    POST();
    }
void RPNStackReversal(){
    char *tmp;
while(RPNStack.Empty()==0){
RPNStack.Pop(tmp);
opStack.Push(tmp);

}
while(opStack.Empty()==0){
opStack.Pop(tmp);
numStack.Push(tmp);
}
while(numStack.Empty()==0){
numStack.Pop(tmp);
RPNStack.Push(tmp);
}
}
void Calculate(){
char *tmp;// to get from RPN stack
double d1,d2;
RPNStackReversal();
while(RPNStack.Empty()==0){
RPNStack.Pop(tmp);
if(isnum(tmp)){
    numStack.NumPush(strdo(tmp));
}
else if(strcmp(tmp,"+")==0 || strcmp(tmp,"-")==0 || strcmp(tmp,"/")==0 || strcmp(tmp,"*")==0 || strcmp(tmp,"%")==0 ||strcmp(tmp,"^")==0){
numStack.NumPop(d1);
numStack.NumPop(d2);

numStack.NumPush(OPerate(d1,d2,tmp));///not defined

}
else {cerr<<"Error in Calculating RPN";exit(0);}
}

numStack.NumPop(result);
//cout<<endl<<"result here to? "<<result<<endl;
}
double getResult(){
return result;
}
};


int Execute(char *Token){
    int ret=0;
    if(strcmp(Token,"INTEGER")==0){
        delete[] Token;
                Tokenizer(Token);

        while(strcmp(Token,";")!=0){

            if(strcmp(Token," ")==0 || strcmp(Token,",")==0){
                delete[] Token;
                Tokenizer(Token);

            }else if(strcmp(Token,"\n")==0)lineno++;
            else{
                    Symrow *var=0;
                    if(MSymtable.Searchvar(Token,var)){
                    cerr<<endl<<"ALREADY SAME NAME VARIABLE EXISTING ERROR IN SCOPE and line number"<<scope<<" ,"<<lineno<<" TOKEN "<<Token<<endl;
                    delete [] Token;
                    exit(0);
                    }
                    else{

                MSymtable.varPush(scope,'i',Token);
                delete[] Token;
                Tokenizer(Token);
                    }

            }
        }
                ret=1;
        }
    else if(strcmp(Token,"CHAR")==0){
        delete[] Token;
                Tokenizer(Token);

        while(strcmp(Token,";")!=0){

            if(strcmp(Token," ")==0 || strcmp(Token,",")==0){
                delete[] Token;
                Tokenizer(Token);

            }else if(strcmp(Token,"\n")==0)lineno++;
            else{
                    Symrow *var=0;
                    if(MSymtable.Searchvar(Token,var)){
                    cerr<<endl<<"ALREADY SAME NAME VARIABLE EXISTING ERROR IN SCOPE "<<scope<<" ,"<<lineno<<" TOKEN "<<Token<<endl;
                    delete [] Token;
                    exit(0);
                    }
                    else{

                MSymtable.varPush(scope,'c',Token);
                delete[] Token;
                Tokenizer(Token);
                    }
            }
        }
        ret=1;
    }else if(strcmp(Token,"DOUBLE")==0){
        delete[] Token;
                Tokenizer(Token);

        while(strcmp(Token,";")!=0){

            if(strcmp(Token," ")==0 || strcmp(Token,",")==0){
                delete[] Token;
                Tokenizer(Token);

            }else if(strcmp(Token,"\n")==0)lineno++;
            else{ Symrow *var=0;
                    if(MSymtable.Searchvar(Token,var)){
                    cerr<<endl<<"ALREADY SAME NAME VARIABLE EXISTING ERROR IN SCOPE "<<scope<<" ,"<<lineno<<" TOKEN "<<Token<<endl;
                    delete [] Token;
                    exit(0);
                    Token = new char[1];
                    strcpy(Token,";");
                    }
                    else{
                MSymtable.varPush(scope,'d',Token);
                delete[] Token;
                Tokenizer(Token);}

            }
        }

    }
     else if(strcmp(Token,"VAR")==0){
        delete[] Token;
                Tokenizer(Token);

        while(strcmp(Token,";")!=0){

            if(strcmp(Token," ")==0 || strcmp(Token,",")==0){
                delete[] Token;
                Tokenizer(Token);

            }else if(strcmp(Token,"\n")==0)lineno++;
            else{
                    Symrow *var=0;
                    if(MSymtable.Searchvar(Token,var)){
                    cerr<<endl<<"ALREADY SAME NAME VARIABLE EXISTING ERROR IN SCOPE and line number"<<scope<<" ,"<<lineno<<" TOKEN "<<Token<<endl;
                    delete [] Token;
                    exit(0);
                    }
                    else{

                MSymtable.varPush(scope,'i',Token);
                delete[] Token;
                Tokenizer(Token);
                    }

            }
        }
                ret=1;
        }
    else if(strcmp(Token,"sDisplay")==0){
       delete[] Token;

            Tokenizer(Token);

       while(strcmp(Token,";")!=0){

       if(strcmp(Token,"\"")==0)
        {
            delete[] Token;
            Tokenizer(Token);

            while(strcmp(Token,"\"")!=0){
            cout<<Token; //ToDisplay
            delete[] Token;
            Tokenizer(Token);
            }
            delete[] Token;
            Tokenizer(Token);
        }
        else if(strcmp(Token,",")==0 || strcmp(Token," ")==0){
            delete[] Token;
            Tokenizer(Token);

        }else if(strcmp(Token,"\n")==0)lineno++;
        else if(strcmp(Token,"[")==0){

            CALC rpn;
            rpn.RPN();
            cout<<rpn.getResult();
            delete[] Token;
            Tokenizer(Token);
        }else if(strcmp(Token,"nl")==0){cout<<"\n"; delete[] Token; Tokenizer(Token);}
        else {  double d;char t;Symrow *var=0;
            if(MSymtable.Searchvar(Token,var))
            {  var->getdata(d);
                var->gettype(t);
                if(t=='c')
                cout<<(char)d;
                else if(t=='i')
                    cout<<(int)d;
                    else if(t=='d')
                        cout<<d;
                delete[] Token;
                Tokenizer(Token);
            }//have to give an else here
            else if(strcmp(Token,"\n")==0){
                    lineno++;}
            else {var->getdata(d);cerr<<"SDisplay errror in line  "<<lineno<<"SCOPE"<<scope<<" Token "<<Token;delete[] Token;exit(0);}
        }

       }

       ret=1;

       }
    else if(strcmp(Token,"sPuts")==0){
        delete[] Token;
        Tokenizer(Token);
       Symrow *var=0;
           while(strcmp(Token,";")!=0){

        if(strcmp(Token,",")==0 || strcmp(Token," ")==0){
            delete[] Token;
            Tokenizer(Token);

        }else if(strcmp(Token,"\n")==0)lineno++;
        else {
            if(MSymtable.Searchvar(Token,var))
            {  cout<<"\n";
                var->putdata();
                delete[] Token;
                Tokenizer(Token);
            }
            else if(strcmp(Token,"\n")==0){
                    lineno++;}
            else {cerr<<"\nSDisplay errror in line::Undeclared  variable  "<<lineno<<"SCOPE"<<scope;delete[] Token;exit(0);}
        }
       }
        ret=1;
    }
    else if(strcmp(Token,"sChangeT")==0){
         delete[] Token;
        Tokenizer(Token);
        Symrow *var=0;
           while(strcmp(Token,";")!=0){

        if(strcmp(Token,",")==0 || strcmp(Token," ")==0){
            delete[] Token;
            Tokenizer(Token);

        }else if(strcmp(Token,"\n")==0)lineno++;
        else {

            if(MSymtable.Searchvar(Token,var))
            { delete[] Token;
                Tokenizer(Token);
                if(strcmp(Token,"(")==0){
                  delete[] Token;
                Tokenizer(Token);
                if(strcmp(Token,"i")==0 || strcmp(Token,"c")==0 || strcmp(Token,"d")==0)
                {if(strcmp(Token,"i")==0)
                var->settype('i');
                else if(strcmp(Token,"c")==0)
                var->settype('c');
                else if(strcmp(Token,"d")==0)
                var->settype('d');
                    Tokenizer(Token);
                    if(strcmp(Token,")")!=0){cerr<<"NOT CLOSED"<<" , line number"<<lineno;exit(0);}

                }else {cerr<<"UNDEFINED TYPE IN sChangeT "<<Token<<" scope and line number"<<scope<<" "<<lineno<<endl;exit(0); }
                }else {cerr<<"\nSynatx error in ChangeT in scope "<<scope<<" line number "<<lineno<<endl; exit(0);}
                delete[] Token;
                Tokenizer(Token);
            }
            else if(strcmp(Token,"\n")==0){
                    lineno++;}
            else {cerr<<"\nSChangeT errror in line::Undeclared  variable  in sChangeT "<<lineno<<"SCOPE"<<scope;delete[] Token;exit(0);}
        }
       }
        ret=1;
    }
    else  if(strcmp(Token,"sif")==0){
        delete [] Token;
        int checkif=1;
        Tokenizer(Token);
        while(checkif!=0){
            if(strcmp(Token,"[")==0){

                CALC rpn;
                int currsc=scope;
            rpn.RPN();
            if(rpn.getResult()){
                    while(strcmp(Token,"{")!=0){delete[] Token; Tokenizer(Token);}
                Execute(Token);
                while(currsc!=scope){
                    delete[] Token;
                    Tokenizer(Token);
                    Execute(Token);
                }

            }else {
                    while(strcmp(Token,"{")!=0){delete[] Token; Tokenizer(Token);}
                Execute(Token);
                while(currsc!=scope){
                    delete[] Token;
                    Tokenizer(Token);
                    if(strcmp(Token,"{")==0 || strcmp(Token,"}")==0)
                    Execute(Token);
                }
            }

            checkif=0;
            }
            else if(strcmp(Token,"\n")==0)lineno++;
            else if(strcmp(Token," ")==0){
                delete[] Token;
                Tokenizer(Token);
            }
            else {cerr<<"\nSyntax error in sif in scope"<<scope<<" ,"<<lineno<<" TOKEN "<<Token<<endl;checkif=0;exit(0);}

        }
        ret=1;
    }
    else if(strcmp(Token,"swhile")==0){
            Loop++;
        delete [] Token;
        Tokenizer(Token);
        int checkif=1;
        while(checkif!=0){
            if(strcmp(Token,"[")==0){
                CALC rpn;
                int currsc=scope;
            rpn.RPN();
            if(rpn.getResult()){
                    while(strcmp(Token,"{")!=0){delete[] Token; Tokenizer(Token);}
                Execute(Token);
                while(currsc!=scope){
                    delete[] Token;
                    Tokenizer(Token);
                    Execute(Token);
                }
                           Gettoloop();
                        //cout<<endl<<"why?"<<endl;//WHY?
                        Tokenizer(Token);
                        cout<<"\n";//WHY DO I NEED THIS
            }else {
                    while(strcmp(Token,"{")!=0){delete[] Token; Tokenizer(Token);}
                Execute(Token);
                while(currsc!=scope){
                    delete[] Token;
                    Tokenizer(Token);
                    if(strcmp(Token,"{")==0 || strcmp(Token,"}")==0)
                    Execute(Token);
                }
            checkif=0;
            }


            }
            else if(strcmp(Token,"\n")==0)lineno++;
            else if(strcmp(Token," ")==0){
                delete[] Token;
                Tokenizer(Token);
            }
            else {cerr<<"Syntax error in swhile in scope "<<scope<<" ,"<<lineno<<"by Token "<<"' "<<Token<<" '"<<endl;checkif=0;;
            }

            }
            ret=1;
            }
    else if(strcmp(Token,"sassign")==0){
        delete[] Token;
        Tokenizer(Token);
       Symrow *var=0;
           while(strcmp(Token,";")!=0){

       if(strcmp(Token,"[")==0)
        {  delete[] Token;
        Tokenizer(Token);
            if(MSymtable.Searchvar(Token,var))
            { delete[] Token;
                Tokenizer(Token);
                if(strcmp(Token,",")==0){
                    CALC c;
                    c.RPN();
                    var->setdata(c.getResult());
                }
            }
            else {cerr<<"UNDECLARED VAR IN SCOPE : "<<scope<<" ,"<<lineno<<" TOKEN "<<Token;exit(0);}
            }else if(strcmp(Token,"\n")==0)lineno++;
            else if(strcmp(Token," ")==0 || strcmp(Token,",")==0)
            {delete[] Token;
        Tokenizer(Token);
            }
            else {cerr<<"Syntax error in scope :: "<<scope<<" ,"<<lineno<<" TOKEN "<<Token; delete[] Token;exit(0);}
       }ret=1;
    }
    else if(strcmp(Token,"?")==0){ //FOR COMMENTS
    int x=lineno;
    delete[] Token;
    while(lineno==x){
        Tokenizer(Token);
        if(strcmp(Token,"\n")==0)
        Execute(Token);
    delete[] Token;
    }ret=1;
     }
   else if(strcmp(Token,"\n")==0)lineno++;
    else if(strcmp(Token,"{")==0){scope++;}
    else if(strcmp(Token,"}")==0){
        int sc;
        MSymtable.getTopScope(sc);

        while(sc==scope){
            MSymtable.varPop();
        MSymtable.getTopScope(sc);
        }
        scope--;

    }
    else if(strcmp(Token," ")==0){/*Do nothing*/}
    else {cerr<<"UNDEFINED SYNTAX IN LINE "<<lineno<<" SCOPE "<<scope<<"for token \""<<Token<<"\""<<endl;ret=0;exit(0);}

return ret;
}

int main()
{
    char Address[40];
int i=0;
    cout<<"Enter Address of file to be interpreted\n";
    cin>>Address;
    system("cls");
    char*keywrd;
    fin.open(Address);
    if(fin>>z){
    fin.close();
    fin.open(Address);

   while(!fin.eof()){
       i++;
       Tokenizer(keywrd);


    Execute(keywrd);

       delete[] keywrd;


    }
fin.close();}
else {
    cerr<<"WRONG PATH GIVEN RESTART FROM BEGNINING"<<endl;
}
}

