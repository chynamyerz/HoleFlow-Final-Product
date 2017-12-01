#include "ptoc.h"

#define __hf_menu_implementation__
                      /* Unit used by HoleFlow.PAS */
                      /* A.A. Dreyer */
                      /* 25/10/2000 */

#include "hf_menu.h"

//Added INCLUDE files
#include <fstream>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <string>
#include <conio.h>
#include <stdlib.h>

using namespace std;

/*$L WIN*/

//Added functions
//Accept only numeric values
int is_num(string prompt){
    int check;
    string num_str;
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 2 );
    cout << "\n               " << prompt;
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 11 );
    while(getline(cin, num_str))
    {
        stringstream ss(num_str);
        if (ss >> check)
        {
            if (ss.eof())
            {   // Success
                break;
            }
        }else{
            SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 4 );
            cout << "\n               "<< "Only numerical values allowed! Please re-enter!" << endl;
            SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 2 );
            cout << "               " << prompt;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        }
    }
    return check;
}


//Check if the prompted value is numeric and it in the range
double limit(double minimum, double maximum, string alert, string prompt){
    double check;
    string num_str;
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 2 );
    cout << "\n               " << prompt;
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 11 );
    while(getline(cin, num_str))
    {
        stringstream ss(num_str);
        if (ss >> check)
        {
            if (ss.eof())
            {   // Success
                if(check < minimum || check > maximum){

                    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 4 );
                    cout << "\n               " << alert << endl;
                    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 2 );
                    cout << "               "<< prompt;
                    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 11 );
                }else{
                    break;
                }
            }
        }else{
            SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 4 );
            cout << "\n               "<< "Only numerical values allowed! Please re-enter!" << endl;
            SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 2 );
            cout << "               " << prompt;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

        }
    }

    return check;
}

//Get the directory where the *.exe file is located.
string getdir(){
    char buffer[MAX_PATH];

    GetModuleFileName( NULL, buffer, MAX_PATH );

    string::size_type pos = string( buffer ).find_last_of( "\\/" );

    return string( buffer ).substr( 0, pos);
}

//Key mappings under Linux can cause the wrong key to be reported by ReadKey, so caution is needed when using ReadKey.
char readkey(){
    return _getch();
}

//GotoXY function
void gotoxy(BYTE column, BYTE line)
{
  COORD coord;
  coord.X = column;
  coord.Y = line;
  SetConsoleCursorPosition(
    GetStdHandle( STD_OUTPUT_HANDLE ),
    coord
    );
}

//Clear screen function
void clrscr() {
  system("cls");
}

//ClrEol clears the current line, starting from the cursor position, to the end of the window. The cursor doesn't move
void clreol(){
    cout << "\r";
}

const double pi = 3.1415926535897;

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to write input data to a file */
void writedata(string filename)
{
  int i;
  fstream fn;
  fn.open(filename, ios::out);
  //rewrite(fn);
  fn << version << endl;
  fn << setw(1) << cctype << endl; /*1-Can, 2-Annular, 3-Can-Annular*/
  fn << mdot << endl;
  fn << afr << endl;
  fn << afrst << endl;
  fn << p0 << endl;
  fn << t0 << endl;
  fn << cp << endl;
  fn << gamma << endl;
  fn << ch << endl;
  fn << lcv << endl;
  fn << lrz << endl;
  fn << lpz << endl;
  fn << lsz << endl;
  fn << lcomb << endl;
  if (filmcooling)  fn << setw(1) << 1 << endl; else fn << setw(1) << 0 << endl;
  if (simplified)  fn << setw(1) << 1 << endl; else fn << setw(1) << 0 << endl;
  fn << tambient << endl;
  fn << eli << endl;
  fn << elo << endl;
  fn << eci << endl;
  fn << eco << endl;
  fn << kl << endl;
  fn << kc << endl;
  fn << tl << endl;
  fn << tc << endl;
  fn << ncans << endl;
  fn << nholesets << endl;
  for( i=1; i <= nholesets; i ++)
  {
    fn << "Hole set #" << setw(1) << i << endl;
    fn << cdspecification[i] << endl;
    fn << acd[i] << endl;
    fn << bcd[i] << endl;
    fn << ccd[i] << endl;
    fn << dcd[i] << endl;
    fn << holetype[i] << endl;
    fn << nholes[i] << endl;
    fn << holeposition[i] << endl;
    fn << holecd[i] << endl;
    fn << dhole[i] << endl;
    fn << slotdepth[i] << endl;
    fn << xhole[i] << endl;
    fn << dliner[i] << endl;
    fn << dlineri[i] << endl;
    fn << dcasing[i] << endl;
    fn << dcasingi[i] << endl;
    fn << primaryrzy[i] << endl;
    fn << primaryrpy[i] << endl;
    fn << secondaryy[i] << endl;
    fn << dilutiony[i] << endl;
  }
  fn.close();
}    /* of procedure WRITEDATA */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to read input data from file */
void readdata(string filename,bool readerror)
{
  fstream fn;
  int i,error;
  string fileversion,dummy;
  string fileversionroot;

  fn.open(filename, ios::in);
  fn >> fileversionroot ;
  if (fileversionroot!=version)
  {
    fn.close();
    error=*strerror(errno);
    readerror=true;
    return;
  }
  fn >> cctype ;
  fn >> mdot ;
  fn >> afr ;
  fn >> afrst ;
  fn >> p0 ;
  fn >> t0 ;
  fn >> cp ;
  fn >> gamma ;
  fn >> ch ;
  fn >> lcv ;
  fn >> lrz ;
  fn >> lpz ;
  fn >> lsz ;
  fn >> lcomb ;
  fn >> i ; if (i==1)  filmcooling=true; else filmcooling=false;
  fn >> i ; if (i==1)  simplified=true; else simplified=false;
  fn >> tambient ;
  fn >> eli ;
  fn >> elo ;
  fn >> eci ;
  fn >> eco ;
  fn >> kl ;
  fn >> kc ;
  fn >> tl ;
  fn >> tc ;
  fn >> ncans ;
  fn >> nholesets ;
  if ((nholesets>maxnumsets) || (nholesets==0))
  {
    fn.close();
    error=*strerror(errno);
    readerror=true;
    return;
  }
  for( i=1; i <= nholesets; i ++)
  {
    fn >> dummy ;
    fn >> cdspecification[i] ;
    fn >> acd[i] ;
    fn >> bcd[i] ;
    fn >> ccd[i] ;
    fn >> dcd[i] ;
    fn >> holetype[i] ;
    fn >> nholes[i] ;
    fn >> holeposition[i] ;
    fn >> holecd[i] ;
    fn >> dhole[i] ;
    fn >> slotdepth[i] ;
    fn >> xhole[i] ;
    fn >> dliner[i] ;
    fn >> dlineri[i] ;
    fn >> dcasing[i] ;
    fn >> dcasingi[i] ;
    fn >> primaryrzy[i] ;
    fn >> primaryrpy[i] ;
    fn >> secondaryy[i] ;
    fn >> dilutiony[i] ;
  }
  fn.close();
  error=*strerror(errno);
  readerror=true;
  if (error==78)  readerror=false;
}    /* of procedure READDATA */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to define the initial values */
void initialise()
{
  fstream fn;
  int error;
  bool readerror;

  rootdir = getdir();
  if (rootdir.substr(0, (rootdir).length())!=""+'\\')  rootdir=rootdir+'\\';
  fn.open(rootdir+"DEFAULT.txt", ios::out | ios::in);

  error=*strerror(errno);

  if (error==0)
  {
    fn.close();
    readerror=false;
    readdata(rootdir+"DEFAULT.txt",readerror);
  }
  else readerror=true;
  /* ReadError:=True; */
  if (! readerror)  casename="DEFAULT.txt";
  if (readerror)
  {
    casename="Built-in default data";
    cctype=1;              /* Combustor type : 1: CAN, 2: ANNULAR, 3: CAN-ANNULAR */
    mdot=0.5;              /* Air mass flow rate, [kg/s] */
    afr=50.0;              /* Overall air/fuel ratio, [-] */
    afrst=14.7;            /* Stoichoimetric AFR [-] */
    p0=1.0E6;              /* Stagnation pressure, [Pa] */
    t0=500.0;              /* Stagnation temperature, [K] */
    cp=1006.0;             /* Specific heat of gas, [J/kgK] */
    gamma=1.4;             /* Specific heat ratio, [-] */
    ch=15.0;               /* Fuel atomic C/H ratio, [-] */
    lcv=42.0E6;            /* Fuel lower calorific value, [J/kg] */
    lrz=0.05;              /* Reciculation zone boundary [m] */
    lpz=0.08;              /* Primary zone boundary [m] */
    lsz=0.15;              /* Secondary zone boundary [m] */
    lcomb=0.2;             /* Overall combustor length [m] */
    filmcooling=true;      /* Modeling with/without film cooling of liner*/
    simplified=true;       /* Simple/accurate model for flame radiation */
    tambient=25.0;         /* Ambient temperature, јC */
    eli=0.4;               /* Liner inside emissivity =0.4 if ceramic coating is used, - */
    elo=0.8;               /* Liner outside emissivity, - */
    eci=0.8;               /* Casing inside emissivity, - */
    eco=0.8;               /* Casing outside emissivity, - */
    kl=15.0;               /* Liner conductivity, W/mK */
    kc=15.0;               /* Casing conductivity, W/mK - set to 0.000001 for insulated outer wall */
    tl=0.001;              /* Liner thickness, m */
    tc=0.001;              /* Casing thickness, m */
    /* Specify default layout */
    ncans=1;               /* Number of cans (CCtype in [1,3]) */
    nholesets=2;
    /* Holeset 1 */
    cdspecification[1]=1;
    acd[1]=0.111;
    bcd[1]=0.0;
    ccd[1]=0.0;
    dcd[1]=0.0;
    holetype[1]=1;
    nholes[1]=1;
    holeposition[1]=1;
    holecd[1]=1.0;
    dhole[1]=0.015;
    slotdepth[1]=0.002;
    xhole[1]=0.0;
    dliner[1]=0.2;
    dlineri[1]=0.1;
    dcasing[1]=0.25;
    dcasingi[1]=0.05;
    primaryrzy[1]=0.5;
    primaryrpy[1]=0.25;
    secondaryy[1]=0.25;
    dilutiony[1]=0.0;
    /* Holeset 2 */
    cdspecification[2]=1;
    acd[2]=0.222;
    bcd[2]=0.0;
    ccd[2]=0.0;
    dcd[2]=0.0;
    holetype[2]=3;
    nholes[2]=10;
    holeposition[2]=2;  /*Annular only*/
    holecd[2]=1.0;
    dhole[2]=0.005;
    slotdepth[2]=0.002;
    xhole[2]=lcomb/2.0;
    dliner[2]=0.2;
    dlineri[2]=0.1;     /*Annular only*/
    dcasing[2]=0.25;
    dcasingi[2]=0.05;   /*Annular only*/
    primaryrzy[2]=0.5;
    primaryrpy[2]=0.25;
    secondaryy[2]=0.25;
    dilutiony[2]=0.0;
  }
}    /* of procedure INIT */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to display polynomial correlations for CD for cooling openings */
void cdcorrelationeditmenu(double& a,double& b,double& c,double& d)
{
  fstream fn;
  string item[23];
  strarr2 item3;
  double min,max;
  double coeff[9][5];
  int i,ncorr,selected,lastpos,error;

  /* Read current correlation data */
  fn.open(rootdir+"CDDATA.txt", ios::in);

  error=*strerror(errno);
  if (error==0)
  {
    for( i=1; i <= 8; i ++)
    {
      fn >> coeff[i][1] ;
      fn >> coeff[i][2] ;
      fn >> coeff[i][3] ;
      fn >> coeff[i][4] ;
    }
    fn.close();
  }
  else
  {
    for( i=1; i <= 8; i ++)
    {
      coeff[i][1]=0.62;
      coeff[i][2]=0.0;
      coeff[i][3]=0.0;
      coeff[i][4]=0.0;
    }
  }
  /* Display menu */
  lastpos=19;
  do {
    maintitle="Hole CD Correlation Input/Selection";
    heading(maintitle,true);
    item[ 1]=' ';
    item[ 2]=" CD = a + b((Pan-pft)/qan) + c((Pan-pft)/qan)^2 + d((Pan-pft)/qan)^3";
    item[ 3]=' ';
    item[ 4]="  where Pan - Stagnation pressure in annulus";
    item[ 5]="        pft - Static pressure in flame tube";
    item[ 6]="        qan - Velocity pressure in annulus";
    item[ 7]=' ';
    item[ 8]="                a            b            c            d";
    item[ 9]=string("  Corr. 1 - ")+rts(coeff[1][1],12,7)+' '+rts(coeff[1][2],12,7)+' '+rts(coeff[1][3],12,7)+' '+rts(coeff[1][4],12,7);
    item[10]=string("  Corr. 2 - ")+rts(coeff[2][1],12,7)+' '+rts(coeff[2][2],12,7)+' '+rts(coeff[2][3],12,7)+' '+rts(coeff[2][4],12,7);
    item[11]=string("  Corr. 3 - ")+rts(coeff[3][1],12,7)+' '+rts(coeff[3][2],12,7)+' '+rts(coeff[3][3],12,7)+' '+rts(coeff[3][4],12,7);
    item[12]=string("  Corr. 4 - ")+rts(coeff[4][1],12,7)+' '+rts(coeff[4][2],12,7)+' '+rts(coeff[4][3],12,7)+' '+rts(coeff[4][4],12,7);
    item[13]=string("  Corr. 5 - ")+rts(coeff[5][1],12,7)+' '+rts(coeff[5][2],12,7)+' '+rts(coeff[5][3],12,7)+' '+rts(coeff[5][4],12,7);
    item[14]=string("  Corr. 6 - ")+rts(coeff[6][1],12,7)+' '+rts(coeff[6][2],12,7)+' '+rts(coeff[6][3],12,7)+' '+rts(coeff[6][4],12,7);
    item[15]=string("  Corr. 7 - ")+rts(coeff[7][1],12,7)+' '+rts(coeff[7][2],12,7)+' '+rts(coeff[7][3],12,7)+' '+rts(coeff[7][4],12,7);
    item[16]=string("  Corr. 8 - ")+rts(coeff[8][1],12,7)+' '+rts(coeff[8][2],12,7)+' '+rts(coeff[8][3],12,7)+' '+rts(coeff[8][4],12,7);
    item[17]=' ';
    item[18]="E - Edit correlations ";
    item[19]="R - Select a correlation and return to previous menu";
    displaymenu(19,lastpos,item,"Hole CD Correlation Menu",selected);
    lastpos=selected+17;

    switch (selected) {
      case 'E' : {
            ncorr = is_num("\n           Enter the number corresponding to the entry to edit : ");
            min=-1000.0;
            max=+1000.0;
            /* a */
            item3[1]=string("Give the constant a(")+its(ncorr,1)+") ("+rts(coeff[ncorr][1],7,5)+") ? ";
            item3[2]=string("Limits on valid correlation numbers : ")+its(min,1)+" - "+its(max,1)+" [-]";
            coeff[ncorr][1] = limit(min, max, item3[2], item3[1]);
            /* b */
            item3[1]=string("Give the constant b(")+its(ncorr,1)+") ("+rts(coeff[ncorr][2],7,5)+") ? ";
            item3[2]=string("Limits on valid correlation numbers : ")+its(min,1)+" - "+its(max,1)+" [-]";
            coeff[ncorr][2] = limit(min, max, item3[2], item3[1]);
            /* c */
            item3[1]=string("Give the constant c(")+its(ncorr,1)+") ("+rts(coeff[ncorr][3],7,5)+") ? ";
            item3[2]=string("Limits on valid correlation numbers : ")+its(min,1)+" - "+its(max,1)+" [-]";
            coeff[ncorr][3] = limit(min, max, item3[2], item3[1]);
            /* d */
            item3[1]=string("Give the constant d(")+its(ncorr,1)+") ("+rts(coeff[ncorr][4],7,5)+") ? ";
            item3[2]=string("Limits on valid correlation numbers : ")+its(min,1)+" - "+its(max,1)+" [-]";
            coeff[ncorr][4] = limit(min, max, item3[2], item3[1]);
          }
          break;
      case 'R' : {
            ncorr = (int)limit(1,8, "Enter int between [1 - 8]", "\n           Enter the number corresponding to the entry to edit :");
            a=coeff[ncorr][1];
            b=coeff[ncorr][2];
            c=coeff[ncorr][3];
            d=coeff[ncorr][4];
          }
          break;
    }    /*case*/
  } while (!(selected=='R'));
  /* Write current correlation data to file */

  fn.open(rootdir+"CDDATA.txt", ios::out);

  for( i=1; i <= 8; i ++)
  {
    fn << coeff[i][1] << endl;
    fn << coeff[i][2] << endl;
    fn << coeff[i][3] << endl;
    fn << coeff[i][4] << endl;
  }
  fn.close();
}    /* of procedure CDEditMenu */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to check that the validity of the liner and casing diameters */
void checkcasingandlinerdiameters(int currentset,int& selected,int& lastpos)
{
  double dcl;
  int maxnumcans;
  bool test1,test2,test3,test4,test5,test6;

  test1=true;
  test2=true;
  test3=true;
  test4=true;
  test5=true;
  test6=true;
  if ((cctype==1) && (dcasing[currentset]<=dliner[currentset]))
  {
    test1=false;
    cout << "\n          " << string("Set #")+its(currentset,1)+" : Liner diameter must be smaller than casing diameter !!!";
    cout << "\n          Press any key...";
    readkey();
    lastpos=14;
  }
  if ((cctype==2) && (dcasing[currentset]<=dliner[currentset]))
  {
    test2=false;
    cout << "\n          " << string("Set #")+its(currentset,1)+" : Liner outer diameter must be smaller than casing outer diameter !!!";
    cout << "\n          Press any key...";
    readkey();
    lastpos=14;
  }
  if ((cctype==2) && (dliner[currentset]<=dlineri[currentset]))
  {
    test3=false;
    cout << "\n          " << string("Set #")+its(currentset,1)+" : Liner outer diameter must be larger than liner inner diameter !!!";
    cout << "\n          Press any key...";
    readkey();
    lastpos=14;
  }
  if ((cctype==2) && (dlineri[currentset]<=dcasingi[currentset]))
  {
    test4=false;
    cout << "\n          " << string("Set #")+its(currentset,1)+" : Liner inner diameter must be larger than casing inner diameter !!!";
    cout << "\n          Press any key...";
    readkey();
    lastpos=16;
  }
  if ((cctype==3) && (dliner[currentset]>=(0.5*(dcasing[currentset]-dcasingi[currentset]))))
  {
    test5=false;
    cout << "\n          " << string("Set #")+its(currentset,1)+" : Liner diameter must fit into casing !!!";
    cout << "\n          Press any key...";
    readkey();
    lastpos=16;
  }
  maxnumcans=1;
  if (cctype==3)
  {
    dcl=(dcasing[currentset]+dcasingi[currentset])/2.0;
    maxnumcans=trunc(pi*dcl/(dliner[currentset]+0.0001));
  }
  if ((cctype==3) && (ncans>maxnumcans))
  {
    test6=false;
    cout << "\n          " << string("Set #")+its(currentset,1)+" : Too many cans inside casing (max="+its(maxnumcans,1)+") !!!";
    cout << "\n          Press any key...";
    readkey();
    lastpos=2;
  }
  if (! (test1 && test2 && test3 && test4 && test5 && test6))  selected=0;
}    /* of procedure CheckCasingAndLinerDiameters */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to display hole specification input menu */
void holespecinputmenu()
{
  fstream fn;
  strarr item;
  strarr2 item3;
  double min,max;
  string filename,dummy;
  int i,irem,imin,imax,currentset,selected,
  lastpos,nholesetsrem,currentsetrem,ncansrem,error;

  currentset=1;
  lastpos=5;
  do {
    if ((! (set::of(3,4, eos).has(holetype[currentset]))) && (cdspecification[currentset]==2))
        cdspecification[currentset]=3;
    if (set::of(1,2, eos).has(cctype))
        ncans=1;
    maintitle="Hole Set Parameter Specification";
    heading(maintitle,true);
    switch (cctype) {
      case 1 : item[ 1]="A - Combustor type ...................... (CCtype) :  CAN"; break;
      case 2 : item[ 1]="A - Combustor type ...................... (CCtype) :  ANNULAR"; break;
      case 3 : item[ 1]="A - Combustor type ...................... (CCtype) : CAN-ANNULAR"; break;
    }    /*case*/
    if (set::of(1,2, eos).has(cctype))
      item[ 2]=' ';
    else
      item[ 2]=string("B - Total number of cans ................. (nCans) = ")+its(ncans,5)+"    [-]";
    item[ 3]=string("C - Total number of hole sets ........ (nHoleSets) = ")+its(nholesets,5)+"    [-]";
    item[ 4]=' ';
    item[ 5]=string("D - Current hole set number ......... (CurrentSet) = ")+its(currentset,5)+"    [-]";
    item[ 6]=string("E - Hole type ......................... (HoleType) =    ")+holetypes[holetype[currentset]-1];
    switch (cdspecification[currentset]) {
      case 1 : { /*user input*/
            item[ 7]="F - CD : user input/built-in function/user corr. . :    User Input    ";
            item[ 8]=string("G - Hole discharge coefficient .......... (HoleCD) = ")+rts(holecd[currentset],8,2)+" [-]";
          }
          break;
      case 2 : { /*built-in function*/
            item[ 7]="F - CD : user input/built-in function/user corr. . : Built-in Function";
            item[ 8]="    Hole discharge coefficient .......... (HoleCD) : Built-in Function";
          }
          break;
      case 3 : { /*user correlation*/
            item[ 7]="F - CD : user input/built-in function/user corr. . : User Correlation ";
            item[ 8]="G - Hole discharge coefficient .......... (HoleCD) : Edit/Select Corr.";
          }
          break;
    }    /*case*/
    if (cctype==2)
      item[ 9]=string("H - Hole set position .......... (HoleSetPosition) :    ")+holepositions[holeposition[currentset]-1];
    else
      item[ 9]=' ';
    item[10]=string("I - Number of holes in set .............. (nHoles) = ")+its(nholes[currentset],5)+"    [-]";
    if (holetypes[holetype[currentset]-1]=="Wiggle strip")
      item[11]=string("J - Wiggle strip width ................... (dHole) = ")+rts(dhole[currentset]*1000.0,8,2)+" [mm]";
    else
      item[11]=string("J - Physical hole diameter ............... (dHole) = ")+rts(dhole[currentset]*1000.0,8,2)+" [mm]";
    if (set::of(5,6,7, eos).has(holetype[currentset]))
      item[12]=string("K - Cooling slot depth (s) ........... (SlotDepth) = ")+rts(slotdepth[currentset]*1000.0,8,2)+" [mm]";
    else
      item[12]=' ';
    item[13]=string("L - Hole position (x-coord rel. to dome) . (xHole) = ")+rts(xhole[currentset]*1000.0,8,2)+" [mm]";
    switch (cctype) {
     case 1 : { /*CAN*/
           item[14]=string("M - Liner diameter at hole .............. (dLiner) = ")+rts(dliner[currentset]*1000.0,8,2)+" [mm]";
           item[15]=' ';
           item[16]=string("O - Casing diameter at hole ............ (dCasing) = ")+rts(dcasing[currentset]*1000.0,8,2)+" [mm]";
           item[17]=' ';
         }
         break;
     case 2 : { /*ANNULAR*/
           item[14]=string("M - Liner outer diameter at hole ........ (dLiner) = ")+rts(dliner[currentset]*1000.0,8,2)+" [mm]";
           item[15]=string("N - Liner inner diameter at hole ....... (dLinerI) = ")+rts(dlineri[currentset]*1000.0,8,2)+" [mm]";
           item[16]=string("O - Casing outer diameter at hole ...... (dCasing) = ")+rts(dcasing[currentset]*1000.0,8,2)+" [mm]";
           item[17]=string("P - Casing inner diameter at hole ..... (dCasingI) = ")+rts(dcasingi[currentset]*1000.0,8,2)+" [mm]";
         }
         break;
     case 3 : { /*CAN-ANNULAR*/
           item[14]=string("M - Liner outer diameter at hole ........ (dLiner) = ")+rts(dliner[currentset]*1000.0,8,2)+" [mm]";
           item[15]=' ';
           item[16]=string("O - Casing outer diameter at hole ...... (dCasing) = ")+rts(dcasing[currentset]*1000.0,8,2)+" [mm]";
           item[17]=string("P - Casing inner diameter at hole ..... (dCasingI) = ")+rts(dcasingi[currentset]*1000.0,8,2)+" [mm]";
         }
         break;
    }    /*case*/
    item[18]=string("S - Mass fraction into PRIMARY zone .... (PrimRZY) = ")+rts(primaryrzy[currentset]*100,8,2)+" [%]";
    item[19]=string("T - Mass fraction into PRIMARY zone .... (PrimRPY) = ")+rts(primaryrpy[currentset]*100,8,2)+" [%]";
    item[20]=string("U - Mass fraction into SECONDARY zone (SecondaryY) = ")+rts(secondaryy[currentset]*100,8,2)+" [%]";
    item[21]=string("    Mass fraction into DILUTION zone . (DilutionY) = ")+rts(dilutiony[currentset]*100,8,2)+" [%]";;
    item[22]="R - Return to MAIN MENU";

    displaymenu(22,lastpos,item,"Hole Layout Menu",selected);

    switch (selected) {
      case 'A' : {

            string out = "";
            /* Write current geometry data for current combustor type to file */
            switch (cctype) {
             case 1 : filename=rootdir+"CAN.txt"; break;
             case 2 : filename=rootdir+"ANNULAR.txt"; break;
             case 3 : filename=rootdir+"CAN_ANNU.txt"; break;
            }
            fn.open(filename, ios::out);
            fn << ncans << endl;
            fn << nholesets << endl;
            for( i=1; i <= nholesets; i ++)
            {
              fn << "Hole set #" << setw(1) << i << endl;
              fn << cdspecification[i] << endl;
              fn << acd[i] << endl;
              fn << bcd[i] << endl;
              fn << ccd[i] << endl;
              fn << dcd[i] << endl;
              fn << holetype[i] << endl;
              fn << nholes[i] << endl;
              fn << holeposition[i] << endl;
              fn << holecd[i] << endl;
              fn << dhole[i] << endl;
              fn << slotdepth[i] << endl;
              fn << xhole[i] << endl;
              fn << dliner[i] << endl;
              fn << dlineri[i] << endl;
              fn << dcasing[i] << endl;
              fn << dcasingi[i] << endl;
              fn << primaryrzy[i] << endl;
              fn << primaryrpy[i] << endl;
              fn << secondaryy[i] << endl;
              fn << dilutiony[i] << endl;
            }
            fn.close();
            /* Select next combustor type */
            cctype += 1;
            if (cctype>3)  cctype=1;
            currentset=1;
            /* Read previous geometry data file for the new combustor type */
            switch (cctype) {
             case 1 : filename=rootdir+"CAN.txt"; break;
             case 2 : filename=rootdir+"ANNULAR.txt"; break;
             case 3 : filename=rootdir+"CAN_ANNU.txt"; break;
            }
            fn.open(filename, ios::in);
            error=*strerror(errno);

            if (error==0)
            {
              fn >> ncans ;
              fn >> nholesets ;
              for( i=1; i <= nholesets; i ++)
              {
                fn >> dummy ;
                fn >> cdspecification[i] ;
                fn >> acd[i] ;
                fn >> bcd[i] ;
                fn >> ccd[i] ;
                fn >> dcd[i] ;
                fn >> holetype[i] ;
                fn >> nholes[i] ;
                fn >> holeposition[i] ;
                fn >> holecd[i] ;
                fn >> dhole[i] ;
                fn >> slotdepth[i] ;
                fn >> xhole[i] ;
                fn >> dliner[i] ;
                fn >> dlineri[i] ;
                fn >> dcasing[i] ;
                fn >> dcasingi[i] ;
                fn >> primaryrzy[i] ;
                fn >> primaryrpy[i] ;
                fn >> secondaryy[i] ;
                fn >> dilutiony[i] ;
              }
              fn.close();

              error=0;
              error=*strerror(errno);
              if ((error)!=78)
              {
                cout << "\n          " << string("Error reading data file ")+filename+" !!!";
                /* Re-initialise variables with default data */
                ncans=1;               /* Number of cans (CCtype in [1,3]) */
                nholesets=2;
                cdspecification[1]=1;
                acd[1]=0.111;
                bcd[1]=0.0;
                ccd[1]=0.0;
                dcd[1]=0.0;
                holetype[1]=1;
                nholes[1]=1;
                holeposition[1]=1;
                holecd[1]=1.0;
                dhole[1]=0.015;
                slotdepth[1]=0.002;
                xhole[1]=0.0;
                dliner[1]=0.2;
                dlineri[1]=0.1;
                dcasing[1]=0.25;
                dcasingi[1]=0.05;
                primaryrzy[1]=0.5;
                primaryrpy[1]=0.25;
                secondaryy[1]=0.25;
                dilutiony[1]=0.0;
                /*HoleSet 2*/
                cdspecification[2]=1;
                acd[2]=0.222;
                bcd[2]=0.0;
                ccd[2]=0.0;
                dcd[2]=0.0;
                holetype[2]=2;
                nholes[2]=5;
                holeposition[2]=2;
                holecd[2]=1.0;
                dhole[2]=0.005;
                slotdepth[2]=0.002;
                xhole[2]=lcomb/2.0;
                dliner[2]=0.2;
                dlineri[2]=0.1;
                dcasing[2]=0.25;
                dcasingi[2]=0.05;
                primaryrzy[2]=0.5;
                primaryrpy[2]=0.25;
                secondaryy[2]=0.25;
                dilutiony[2]=0.0;
              }
            }
          }
          break;
      case 'B' : {    /* number of cans (nCans) */
            if(item[ 2] != " "){
                ncansrem=ncans;
                imin=1;
                imax=50;
                item3[1]=string("Give the number of cans (")+its(ncans,1)+") ? ";
                item3[2]=string("Limits on the number of cans : ")+its(imin,1)+" - "+its(imax,1)+" [-]";
                ncans = limit(imin, imax, item3[2],item3[1]);
            }
          }
          break;
      case 'C' : {    /* number of hole sets (nHoleSets) */
            nholesetsrem=nholesets;
            imin=2;
            imax=maxnumsets;
            item3[1]=string("Give the number of hole sets (")+its(nholesets,1)+") ? ";
            item3[2]=string("Limits on the number of hole sets : ")+its(imin,1)+" - "+its(imax,1)+" [-]";
            nholesets = limit(imin, imax, item3[2],item3[1]);
            if (nholesets>nholesetsrem)
            {
              for( i=(nholesetsrem+1); i <= nholesets; i ++)
              {
                cdspecification[i]=1;
                acd[i]=0.333;
                bcd[i]=0.0;
                ccd[i]=0.0;
                dcd[i]=0.0;
                holetype[i]=3;
                nholes[i]=10;
                holeposition[i]=2;
                holecd[i]=1.0;
                dhole[i]=0.005;
                slotdepth[i]=0.002;
                xhole[i]=0.1;
                dliner[i]=0.15;
                dlineri[i]=0.1;
                dcasing[i]=0.25;
                dcasingi[i]=0.05;
                primaryrzy[i]=0.5;
                primaryrpy[i]=0.5;
                secondaryy[i]=0.0;
                dilutiony[i]=0.0;
              }
            }
          }
          break;
      case 'D' : {    /* current set (CurrentSet) */
            currentsetrem=currentset;
            imin=1;
            imax=nholesets;
            item3[1]=string("Give the number of the current hole set (")+its(currentset,1)+") ? ";
            item3[2]=string("Current set should be in range : ")+its(imin,1)+" - "+its(imax,1)+" [-]";
            currentset = limit(imin, imax, item3[2],item3[1]);
            if (currentset!=currentsetrem)
              checkcasingandlinerdiameters(currentset,selected,lastpos);
          }
          break;
      case 'E' : {    /* hole type */
            holetype[currentset] += 1;
            if (holetype[currentset]>nholetypes)  holetype[currentset]=1;
          }
          break;
      case 'F' : {
            cdspecification[currentset] += 1;
            if (cdspecification[currentset]>3)  cdspecification[currentset]=1;
          }
          break;
      case 'G' : {    /* hole discharge coefficient (CD) */
            if (cdspecification[currentset]==1)
            {
              min=0.0;
              max=1.0;
              item3[1]=string("Give the hole discharge coefficient [-] (")+rts(holecd[currentset],4,2)+") ? ";
              item3[2]=string("Limits on the hole discharge coefficient : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [-]";
              holecd[currentset] = limit(min, max, item3[2],item3[1]);
            }
            else
            {
              cdcorrelationeditmenu(acd[currentset],bcd[currentset],ccd[currentset],dcd[currentset]);
            }
          }
          break;
      case 'H' : {    /* hole position (ANNULAR) */
            if(item[ 9] != " "){
                holeposition[currentset] += 1;
                if (holeposition[currentset]>3)  holeposition[currentset]=1;
            }
          }
          break;
      case 'I' : {    /* number of holes in current set (nHoles) */
            imin=1;
            imax=255;
            item3[1]=string("Give the number of holes in current set (")+its(nholes[currentset],1)+") ? ";
            item3[2]=string("Number of holes in current set should be in range : ")+its(imin,1)+" - "+its(imax,1)+" [-]";
            nholes[currentset] = limit(imin, imax, item3[2],item3[1]);
          }
          break;
     case 'J' : {    /* Physical hole diameter (dHole) */
            if (holetypes[holetype[currentset]-1]=="Wiggle strip")
            {
              min=0.0;
              max=0.10*1000.0;
              item3[1]=string("Give the wiggle strip width [mm] (")+rts(dhole[currentset]*1000.0,4,2)+") ? ";
              item3[2]=string("Limits on wiggle strip width : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [mm]";
              dhole[currentset] = limit(min, max, item3[2],item3[1]);
              dhole[currentset] = dhole[currentset]/1000.0;
            }
            else
            {
              min=0.0;
              max=0.050*1000.0;
              item3[1]=string("Give the physical hole diameter [mm] (")+rts(dhole[currentset]*1000.0,4,2)+") ? ";
              item3[2]=string("Limits on the hole diameter : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [mm]";
              dhole[currentset] = limit(min, max, item3[2],item3[1]);
              dhole[currentset] = dhole[currentset]/1000.0;
            }
          }
          break;
     case 'K' : {    /* Depth of cooling slot (SlotDepth) */
            if(item[12] != " "){
                min=0.0;
                max=0.050*1000.0;
                item3[1]=string("Give the cooling slot depth [mm] (")+rts(slotdepth[currentset]*1000.0,4,2)+") ? ";
                item3[2]=string("Limits on the cooling slot depth : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [mm]";
                slotdepth[currentset] = limit(min, max, item3[2],item3[1]);
                slotdepth[currentset] = slotdepth[currentset]/1000.0;
            }
          }
          break;
     case 'L' : {    /* Hole position from dome (xHole) */
            min=0.0;
            max=0.50*1000.0;
            item3[1]=string("Give the hole position from dome [mm] (")+rts(xhole[currentset]*1000.0,4,2)+") ? ";
            item3[2]=string("Limits on the hole position from dome : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [mm]";
            xhole[currentset] = limit(min, max, item3[2],item3[1]);
            xhole[currentset] = xhole[currentset]/1000.0;
          }
          break;
     case 'M' : {    /* Liner diameter at hole position (dLiner) */
            min=0.0;
            max=0.50*1000.0;
            item3[1]=string("Give the liner diameter at the hole position [mm] (")+rts(dliner[currentset]*1000.0,4,2)+") ? ";
            item3[2]=string("Limits on the liner diameter at the hole position : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [mm]";
            dliner[currentset] = limit(min, max, item3[2],item3[1]);
            dliner[currentset] = dliner[currentset]/1000.0;
            checkcasingandlinerdiameters(currentset,selected,lastpos);
          }
          break;
     case 'N' : {    /* Liner inner diameter at hole position (dLinerI) */
            if(item[15] != " "){
                min=0.0;
                max=0.50*1000.0;
                item3[1]=string("Give the liner ID at the hole position [mm] (")+rts(dlineri[currentset]*1000.0,4,2)+") ? ";
                item3[2]=string("Limits on the liner ID at the hole position : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [mm]";
                dlineri[currentset] = limit(min, max, item3[2],item3[1]);
                dlineri[currentset] = dlineri[currentset]/1000.0;
                checkcasingandlinerdiameters(currentset,selected,lastpos);
            }
          }
          break;
     case 'O' : {    /* Casing diameter at hole position (dCasing) */
            min=0.0;
            max=0.50*1000.0;
            item3[1]=string("Give the casing diameter at the hole position [mm] (")+rts(dcasing[currentset]*1000.0,4,2)+") ? ";
            item3[2]=string("Limits on the casing diameter at the hole position : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [mm]";
            dcasing[currentset] = limit(min, max, item3[2],item3[1]);
            dcasing[currentset] = dcasing[currentset] /1000.0;
            checkcasingandlinerdiameters(currentset,selected,lastpos);
          }
          break;
     case 'P' : {    /* Casing inner diameter at hole position (dCasingI) */
            if(item[17] != " "){
                min=0.0;
                max=0.50*1000.0;
                item3[1]=string("Give the casing ID at the hole position [mm] (")+rts(dcasingi[currentset]*1000.0,4,2)+") ? ";
                item3[2]=string("Limits on the casing ID at the hole position : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [mm]";
                dcasingi[currentset] = limit(min, max, item3[2],item3[1]);
                dcasingi[currentset] = dcasingi[currentset]/1000.0;
                checkcasingandlinerdiameters(currentset,selected,lastpos);
            }
          }
          break;
     case 'S' : {    /* Primary zone mass flow fraction (PrimaryRZY) */
            min=0.0;
            max=100.0;
            item3[1]=string("Give the flow fraction to primary recirc. zone [%] (")+rts(primaryrzy[currentset]*100.0,4,2)+") ? ";
            item3[2]=string("Limits on flow fraction to the primary recirc. zone : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [%]";
            primaryrzy[currentset] = limit(min, max, item3[2],item3[1]);
            primaryrzy[currentset] = primaryrzy[currentset]/100.0;
            if ((primaryrzy[currentset]+secondaryy[currentset]+dilutiony[currentset])<1.0)
              primaryrpy[currentset]=1.0-primaryrzy[currentset]-secondaryy[currentset]-dilutiony[currentset];
            else
            {
              if ((primaryrzy[currentset]+primaryrpy[currentset])<1.0)
              {
                secondaryy[currentset]=1.0-primaryrzy[currentset]-primaryrpy[currentset];
                dilutiony[currentset]=0.0;
              }
              else
              {
                primaryrpy[currentset]=1.0-primaryrzy[currentset];
                secondaryy[currentset]=0.0;
                dilutiony[currentset]=0.0;
              }
            }
          }
          break;
     case 'T' : {    /* Primary zone mass flow fraction (HolePrimaryRPY) */
            min=0.0;
            max=100.0;
            item3[1]=string("Give the flow fraction to rem. of primary zone [%] (")+rts(primaryrpy[currentset]*100.0,4,2)+") ? ";
            item3[2]=string("Limits on the flow fraction to the rem. of primary zone : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [%]";
            primaryrpy[currentset] = limit(min, max, item3[2],item3[1]);
            primaryrpy[currentset] = primaryrpy[currentset] /100.0;
            if ((primaryrzy[currentset]+primaryrpy[currentset]+dilutiony[currentset])<1.0)
              secondaryy[currentset]=1.0-primaryrzy[currentset]-primaryrpy[currentset]-dilutiony[currentset];
            else
            {
              if ((primaryrzy[currentset]+primaryrpy[currentset])<1.0)
              {
                secondaryy[currentset]=1.0-primaryrzy[currentset]-primaryrpy[currentset];
                dilutiony[currentset]=0.0;
              }
              else
              {
                primaryrzy[currentset]=1.0-primaryrpy[currentset];
                secondaryy[currentset]=0.0;
                dilutiony[currentset]=0.0;
              }
            }
          }
          break;
     case 'U' : {    /* Secondary zone mass flow fraction (HoleSecY) */
            min=0.0;
            max=100.0;
            item3[1]=string("Give the flow fraction to secondary zone [%] (")+rts(secondaryy[currentset]*100.0,4,2)+") ? ";
            item3[2]=string("Limits on the flow fraction to the secondary zone : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [%]";
            secondaryy[currentset] = limit(min, max, item3[2],item3[1]);
            secondaryy[currentset] = secondaryy[currentset] /100.0;
            if ((primaryrzy[currentset]+primaryrpy[currentset]+secondaryy[currentset])<1.0)
              dilutiony[currentset]=1.0-primaryrzy[currentset]-primaryrpy[currentset]-secondaryy[currentset];
            else
            {
              if ((primaryrpy[currentset]+secondaryy[currentset])<1.0)
              {
                dilutiony[currentset]=1.0-primaryrpy[currentset]-secondaryy[currentset];
                primaryrzy[currentset]=0.0;
              }
              else
              {
                primaryrpy[currentset]=1.0-secondaryy[currentset];
                primaryrzy[currentset]=0.0;
                dilutiony[currentset]=0.0;
              }
            }
          }
          break;
     case 'R' : {
            irem=maxnumsets;
            for( i=1; i <= nholesets; i ++)
            {
              checkcasingandlinerdiameters(i,selected,lastpos);
              if ((selected==0) && (irem>i))
              {
                irem=i;
                currentset=i;
              }
            }
          }
          break;
    }    /*case*/
  } while (!(selected=='R'));
  /* Write current geometry data for current combustor type to file */
  switch (cctype) {
   case 1 : filename=rootdir+"CAN.txt"; break;
   case 2 : filename=rootdir+"ANNULAR.txt"; break;
   case 3 : filename=rootdir+"CAN_ANNU.txt"; break;
  }
  fn.open(filename, ios::out);
  fn << ncans << endl;
  fn << nholesets << endl;
  for( i=1; i <= nholesets; i ++)
  {
    fn << "Hole set #" << setw(1) << i << endl;
    fn << cdspecification[i] << endl;
    fn << acd[i] << endl;
    fn << bcd[i] << endl;
    fn << ccd[i] << endl;
    fn << dcd[i] << endl;
    fn << holetype[i] << endl;
    fn << nholes[i] << endl;
    fn << holeposition[i] << endl;
    fn << holecd[i] << endl;
    fn << dhole[i] << endl;
    fn << slotdepth[i] << endl;
    fn << xhole[i] << endl;
    fn << dliner[i] << endl;
    fn << dlineri[i] << endl;
    fn << dcasing[i] << endl;
    fn << dcasingi[i] << endl;
    fn << primaryrzy[i] << endl;
    fn << primaryrpy[i] << endl;
    fn << secondaryy[i] << endl;
    fn << dilutiony[i] << endl;
  }
  fn.close();
}    /* of procedure HoleSpecInputMenu */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to display heat transfer model parameter input menu */
void heattransferparametermenu()
{
  fstream fn;
  strarr item;
  strarr2 item3;
  double min,max;
  string filename,dummy;
  int selected,lastpos;

  lastpos=12;
  do {
    maintitle="Heat Transfer Parameter Specification";
    heading(maintitle,true);
    if (filmcooling)
      item[ 1]="A - Heat transfer/liner cooling model     : FILM-COOLED LINER";
    else
      item[ 1]="A - Heat transfer/liner cooling model     : UNCOOLED LINER   ";
    if (simplified)
      item[ 2]="B - Flame radiation model                 : SIMPLIFIED";
    else
      item[ 2]="B - Flame radiation model                 : ACCURATE  ";
    item[ 3]=string("C - Ambient temperature ...... (Tambient) = ")+rts(tambient,6,2)+"    [јC]";
    item[ 4]=string("D - Liner inner emissivity ........ (eLi) = ")+rts(eli,6,2)+"    [-]";
    item[ 5]=string("E - Liner outer emissivity ........ (eLo) = ")+rts(elo,6,2)+"    [-]";
    item[ 6]=string("F - Casing inner emissivity ....... (eCi) = ")+rts(eci,6,2)+"    [-]";
    item[ 7]=string("G - Casing outer emissivity ....... (eCo) = ")+rts(eco,6,2)+"    [-]";
    item[ 8]=string("H - Liner thermal conductivity ..... (kL) = ")+rts(kl,6,2)+"    [W/mK]";
    item[ 9]=string("I - Casing thermal conductivity .... (kC) = ")+rts(kc,6,2)+"    [W/mK]";
    item[10]=string("J - Liner wall thickness ........... (tL) = ")+rts(tl*1000.0,6,2)+"    [mm]";
    item[11]=string("K - Casing wall thickness .......... (tC) = ")+rts(tc*1000.0,6,2)+"    [mm]";
    item[12]="R - Return to MAIN MENU";
    displaymenu(12,lastpos,item,"Heat Transfer Parameter Menu",selected);
    lastpos=selected;
    switch (selected) {
      case 'A' : filmcooling=! filmcooling; break;
      case 'B' : simplified=! simplified; break;
      case 'C' : {    /* Ambient temperature */
            min=0.0;
            max=300.0;
            item3[1]=string("Give the ambient temperature [јC] (")+rts(tambient,4,2)+") ? ";
            item3[2]=string("Limits on the ambient temperature : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [јC]";
            tambient = limit(min, max, item3[2],item3[1]);
          }
          break;
      case 'D' : {    /* Liner inner emissivity, - */
            min=0.0;
            max=1.0;
            item3[1]=string("Give the liner inner emissivity [-] (")+rts(eli,4,2)+") ? ";
            item3[2]=string("Limits on the liner inner emissivity : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [-]";
            eli = limit(min, max, item3[2],item3[1]);
          }
          break;
      case 'E' : {    /* Liner outer emissivity, - */
            min=0.0;
            max=1.0;
            item3[1]=string("Give the liner outer emissivity [-] (")+rts(elo,4,2)+") ? ";
            item3[2]=string("Limits on the liner outer emissivity : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [-]";
            elo = limit(min, max, item3[2],item3[1]);
          }
          break;
      case 'F' : {    /* Casing inner emissivity, - */
            min=0.0;
            max=1.0;
            item3[1]=string("Give the casing inner emissivity [-] (")+rts(eci,4,2)+") ? ";
            item3[2]=string("Limits on the casing inner emissivity : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [-]";
            eci = limit(min, max, item3[2],item3[1]);
          }
          break;
      case 'G' : {    /* Casing outer emissivity, - */
            min=0.0;
            max=1.0;
            item3[1]=string("Give the casing outer emissivity [-] (")+rts(eco,4,2)+") ? ";
            item3[2]=string("Limits on the casing outer emissivity : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [-]";
            eco = limit(min, max, item3[2],item3[1]);
          }
          break;
      case 'H' : {    /* Liner conductivity, W/mK */
            min=1.0;
            max=400.0;
            item3[1]=string("Give the liner conductivity [W/mK] (")+rts(kl,4,2)+") ? ";
            item3[2]=string("Limits on the liner conductivity : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [W/mK]";
            kl = limit(min, max, item3[2],item3[1]);
          }
          break;
      case 'I' : {    /* Casing conductivity, - */
            min=1.0;
            max=400.0;
            item3[1]=string("Give the casing conductivity [W/mK] (")+rts(kc,4,2)+") ? ";
            item3[2]=string("Limits on the casing conductivity : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [W/mK]";
            kc = limit(min, max, item3[2],item3[1]);
          }
          break;
     case 'J' : {    /* Liner thickness, m */
            min=0.0001;
            max=0.01;
            item3[1]=string("Give the liner thickness [mm] (")+rts(tl*1000.0,4,2)+") ? ";
            item3[2]=string("Limits on the liner thickness : ")+rts(min*1000.0,3,1)+" - "+rts(max*1000.0,3,1)+" [mm]";
            tl = limit(min, max, item3[2],item3[1]);
            tl = tl/1000.0;
          }
          break;
     case 'K' : {    /* Casing thickness, m */
            min=0.0001;
            max=0.01;
            item3[1]=string("Give the casing thickness [mm] (")+rts(tc*1000.0,4,2)+") ? ";
            item3[2]=string("Limits on the casing thickness : ")+rts(min*1000.0,3,1)+" - "+rts(max*1000.0,3,1)+" [mm]";
            tc = limit(min, max, item3[2],item3[1]);
            tc = tc/1000.0;
          }
          break;
    }    /*case*/
  } while (!(selected=='R'));
}    /* of procedure HeatTransferParameterMenu */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to display main menu */
void mainmenu()
{
  int arrayi[(3+4*maxnumsets)];
  bool arrayb[(2+maxnumsets)];
  double arrayr[(22+maxnumsets*17)];
  fstream fn;
  strarr item;
  strarr2 item3;
  double min,max;
  int ni,nr,nb;
  boolean readerror = false,validfile;
  char answer,dummy;
  string dir,filename,tempstr,datestr;
  int selected,i,error,lastpos;

  lastpos=19;
  do {
    if (cctype==2)  ncans=1;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
    maintitle="Combustion Chamber Flow Distribution/Heat Transfer Simulation";
    heading(maintitle,true);
    item[ 1]=string("A - Total air mass flow rate ..... (mdot) = ")+rts(mdot,8,3)+" [kg/s]";
    item[ 2]=string("B - Overall air/fuel ratio ........ (AFR) = ")+rts(afr,8,2)+" [-]";
    item[ 3]=string("C - Stoichiometric air/fuel ratio (AFRst) = ")+rts(afrst,8,2)+" [-]";
    item[ 4]=string("D - Stagnation pressure  ........... (P0) = ")+rts(p0/1000.0,8,2)+" [kPa]";
    item[ 5]=string("E - Stagnation temperature ......... (T0) = ")+rts(t0,8,2)+" [K]";
    item[ 6]=string("F - Stagnation temperature ......... (T0) = ")+rts(t0-273.15,8,2)+" [јC]";
    item[ 7]=string("G - Specific heat of gas ........... (Cp) = ")+rts(cp,8,2)+" [J/kgK]";
    item[ 8]=string("H - Specific heat ratio ......... (gamma) = ")+rts(gamma,8,2)+" [-]";
    item[ 9]=string("I - Fuel atomic C/H ratio .......... (CH) = ")+rts(ch,8,2)+" [-]";
    item[10]=string("J - Fuel lower calorific value .... (LCV) = ")+rts(lcv/1000000.0,8,2)+" [MJ/kg]";
    item[11]=string("K - Recirculation zone boundary ... (Lrz) = ")+rts(lrz*1000.0,8,2)+" [mm]";
    item[12]=string("L - Primary zone (total) boundary . (Lpz) = ")+rts(lpz*1000.0,8,2)+" [mm]";
    item[13]=string("M - Secondary zone total boundary . (Lsz) = ")+rts(lsz*1000.0,8,2)+" [mm]";
    item[14]=string("N - Overall combustor length .... (Lcomb) = ")+rts(lcomb*1000.0,8,2)+" [mm]";
    item[15]="O - Go to HOLE LAYOUT MENU to edit hole size, number etc.";
    item[16]="P - Go to HEAT TRANSFER PARAMETER MENU";
    item[17]="S - Write input data to a data file ";
    item[18]="T - Read input data from an existing data file ";
    item[19]="R - Run program using current input data";
    item[20]="Q - Quit and return to DOS";
    displaymenu(20,lastpos,item,"Main Menu",selected);

    lastpos=selected;
    switch (selected) {
      case 'A' : {    /* air mass flow rate (mdot) */
            min=0.0;
            max=100.0;
            item3[1]=string("Give the air mass flow rate [kg/s] (")+rts(mdot,4,2)+") ? ";
            item3[2]=string("Limits on the air mass flow rate : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [kg/s]";
            mdot = limit(min, max, item3[2],item3[1]);
          }
          break;
      case 'B' : {    /* Air fuel ratio (AFR) */
            min=5.0;
            max=120.0;
            item3[1]=string("Give the air fuel ratio [-] (")+rts(afr,4,2)+") ? ";
            item3[2]=string("Limits on the air fuel ratio : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [-]";
            afr = limit(min, max, item3[2],item3[1]);
          }
          break;
      case 'C' : {    /* Stoiciometric air fuel ratio (AFRst) */
            min=5.0;
            max=120.0;
            item3[1]=string("Give the stoichiometric air fuel ratio [-] (")+rts(afrst,4,2)+") ? ";
            item3[2]=string("Limits on the stoichiometric air fuel ratio : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [-]";
            afrst = limit(min, max, item3[2],item3[1]);
          }
          break;
      case 'D' : {    /* stagnation pressure */
            min=100.0;
            max=3000.0;
            item3[1]=string("Give the stagnation pressure [kPa] (")+rts(p0/1000.0,4,2)+") ? ";
            item3[2]=string("Limits on the stagnation pressure : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [kPa]";
            p0 = limit(min, max, item3[2],item3[1]) * 1000.0;
          }
          break;
      case 'E' : {    /* stagnation temperature [K] */
            min=10.0;
            max=2000.0;
            item3[1]=string("Give the stagnation temperature [K] (")+rts(t0,4,2)+") ? ";
            item3[2]=string("Limits on the stagnation temperature : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [K]";
            t0 = limit(min, max, item3[2],item3[1]);
          }
          break;
      case 'F' : {    /* stagnation temperature [јC] */
            min=10.0;
            max=2000.0;
            item3[1]=string("Give the stagnation temperature [јC] (")+rts(t0-273.15,4,2)+") ? ";
            item3[2]=string("Limits on the stagnation temperature : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [јC]";
            t0 = limit(min, max, item3[2],item3[1]) + 273.15;
          }
          break;
      case 'G' : {    /* specific heat of gas [J/kgK] */
            min=500.0;
            max=5000.0;
            item3[1]=string("Give the specific heat of the gas [J/kgK] (")+rts(cp,4,2)+") ? ";
            item3[2]=string("Limits on the specific heat of gas : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [J/kgK]";
            cp = limit(min, max, item3[2],item3[1]);
          }
          break;
      case 'H' : {    /* specific heat ratio [-] */
            min=1.0;
            max=2.0;
            item3[1]=string("Give the specific heat ratio [-] (")+rts(gamma,4,2)+") ? ";
            item3[2]=string("Limits on the specific heat ratio : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [-]";
            gamma = limit(min, max, item3[2],item3[1]);
          }
          break;
      case 'I' : {    /* fuel C/H ratio [-] */
            min=10.0;
            max=2000.0;
            item3[1]=string("Give the fuel C/H ratio [-] (")+rts(ch,4,2)+") ? ";
            item3[2]=string("Limits on the fuel C/H ratio : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [-]";
            ch = limit(min, max, item3[2],item3[1]);
          }
          break;
     case 'J' : {    /* fuel lower calorific value [J/kg] */
            min= 10.0;
            max=100.0;
            item3[1]=string("Give the lower calorific value of the fuel [MJ/kg] (")+rts(lcv/1.0E6,4,2)+") ? ";
            item3[2]=string("Limits on the calorific value of the fuel : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [MJ/kg]";
            lcv = limit(min, max, item3[2],item3[1]) *1.0E6;
          }
          break;
     case 'K' : {    /* Recirculation zone boundary from dome (Lrz) */
            min=0.0;
            max=0.50*1000.0;
            item3[1]=string("Give the recirculation zone boundary [mm] (")+rts(lrz*1000.0,4,2)+") ? ";
            item3[2]=string("Limits on the recirculation zone boundary : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [mm]";
            lrz = limit(min, max, item3[2],item3[1]) /1000.0;
          }
          break;
     case 'L' : {    /* Primary zone boundary from dome (Lpz) */
            min=0.0;
            max=0.50*1000.0;
            item3[1]=string("Give the primary zone boundary [mm] (")+rts(lpz*1000.0,4,2)+") ? ";
            item3[2]=string("Limits on the primary zone boundary : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [mm]";
            lpz = limit(min, max, item3[2],item3[1]) /1000.0;
          }
          break;
     case 'M' : {    /* Secondary zone boundary from dome (Lsz) */
            min=0.0;
            max=0.750*1000.0;
            item3[1]=string("Give the secondary zone boundary [mm] (")+rts(lsz*1000.0,4,2)+") ? ";
            item3[2]=string("Limits on the secondary zone boundary : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [mm]";
            lsz = limit(min, max, item3[2],item3[1]) /1000.0;
          }
          break;
     case 'N' : {    /* Total combustor length (Lcomb) */
            min=0.0;
            max=1.0*1000.0;
            item3[1]=string("Give the total combustor length [mm] (")+rts(lcomb*1000.0,4,2)+") ? ";
            item3[2]=string("Limits on the total combustor length : ")+rts(min,3,1)+" - "+rts(max,3,1)+" [mm]";
            lcomb = limit(min, max, item3[2],item3[1]) /1000.0;
          }
          break;
     case 'O' : holespecinputmenu(); break;    /* set hole types, sizes etc. */
     case 'P' : heattransferparametermenu(); break;    /* edit heat transfer modeling parameters */
     case 'S' : {                 /* write input data file */
            if (accessdisk(rootdir))
            {
              do {
                answer='Y';
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
                cout << "  Give the new filename for the input data file (DEFAULT) ? ";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                cin >> filename;
                cout << "\n          " << string("Checking if input data file ")+filename+".txt exists ...\n" << endl;
                fn.open(rootdir+filename+".txt", ios::in);

                error=*strerror(errno);
                if (fn.is_open())
                {
                  fn.close();
                  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
                  cout << "\n          " << string("Input data file ")+filename+".txt already exists. Rewrite (Y/N) ? ";
                  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                  cin >> answer;
                  answer = toupper(answer);
                }
              } while (!(answer=='Y'));
              SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
              cout << "\n          " << string("Writing input data to ")+"\n  "+rootdir+filename+".txt";
              Sleep(3000);

              writedata(rootdir+filename+".txt");
            }
            else
            {
              SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 4 );
              cout << "\n          Saving of result data unsuccessful - Press RETURN";
              SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 11 );
              do {; } while (!(readkey()=='\15'));
            }
          }
          break;
     case 'T' : {    /* read new input data file */
            /* save current data for backup purposes */
            arrayi[1]=cctype;
            arrayr[1]=mdot;
            arrayr[2]=afr;
            arrayr[3]=afrst;
            arrayr[4]=p0;
            arrayr[5]=t0;
            arrayr[6]=cp;
            arrayr[7]=gamma;
            arrayr[8]=ch;
            arrayr[9]=lcv;
            arrayr[10]=lrz;
            arrayr[11]=lpz;
            arrayr[12]=lsz;
            arrayr[13]=lcomb;
            arrayb[1]=filmcooling;
            arrayb[2]=simplified;
            arrayr[14]=tambient;
            arrayr[15]=eli;
            arrayr[16]=elo;
            arrayr[17]=eci;
            arrayr[18]=eco;
            arrayr[19]=kl;
            arrayr[20]=kc;
            arrayr[21]=tl;
            arrayr[22]=tc;
            /* Combustion chamber geometry */
            arrayi[2]=ncans;
            arrayi[3]=nholesets;
            ni=3;
            nr=22;
            nb=2;
            for( i=1; i <= nholesets; i ++)
            {
              ni += 1; arrayi[ni]=cdspecification[i];
              nr += 1; arrayr[nr]=acd[i];
              nr += 1; arrayr[nr]=bcd[i];
              nr += 1; arrayr[nr]=ccd[i];
              nr += 1; arrayr[nr]=dcd[i];
              ni += 1; arrayi[ni]=holetype[i];
              ni += 1; arrayi[ni]=nholes[i];
              ni += 1; arrayi[ni]=holeposition[i];
              nr += 1; arrayr[nr]=holecd[i];
              nr += 1; arrayr[nr]=dhole[i];
              nr += 1; arrayr[nr]=slotdepth[i];
              nr += 1; arrayr[nr]=xhole[i];
              nr += 1; arrayr[nr]=dliner[i];
              nr += 1; arrayr[nr]=dlineri[i];
              nr += 1; arrayr[nr]=dcasing[i];
              nr += 1; arrayr[nr]=dcasingi[i];
              nr += 1; arrayr[nr]=primaryrzy[i];
              nr += 1; arrayr[nr]=primaryrpy[i];
              nr += 1; arrayr[nr]=secondaryy[i];
              nr += 1; arrayr[nr]=dilutiony[i];
            }
            /* read input file from disk */
            dir=rootdir;
            do {
              validfile=true;
              SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
              heading(maintitle,true);
              SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
              /*
                choosefile function is causing the program to crash. Need to look at it in deeper details.
              */

              char filename[ MAX_PATH ];

              OPENFILENAME ofn;
              ZeroMemory( &filename, sizeof( filename ) );
              ZeroMemory( &ofn,      sizeof( ofn ) );
              ofn.lStructSize  = sizeof( ofn );
              ofn.hwndOwner    = NULL;  // If you have a window to center over, put its HANDLE here
              ofn.lpstrFilter  = "Text Files\0*.txt\0Any File\0*.*\0";
              ofn.lpstrFile    = filename;
              ofn.nMaxFile     = MAX_PATH;
              ofn.lpstrTitle   = "Select a File!";
              ofn.Flags        = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

              if (GetOpenFileNameA( &ofn ))
              {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                cout << "\n\n                    You chose the file: \n\n" << filename << "\n\n                    ";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                system("pause");
              }

              if (filename!=NULL)
              {
                readdata(dir+filename,readerror);
                if (readerror)
                {
                  validfile=false;
                  heading(maintitle,true);
                  cout << endl;
                  cout << endl;
                  cout << "       Invalid input data file !!!" << endl;
                  cout << endl;
                  cout << "       Note that data files of previous versions of this" << endl;
                  cout << "       program is incompatible with version " << version << endl;
                  gotoxy(1,25);
                  cout << "               Press any key to continue";
                  dummy=readkey();
                  if (dummy=='\0')  dummy=readkey();
                }
              }
            } while (!((filename=="") || validfile));
            if (validfile)  casename=filename;
            /* set current data to backup data if required */
            if (filename=="")
            {
              cctype=arrayi[1];
              mdot=arrayr[1];
              afr=arrayr[2];
              afrst=arrayr[3];
              p0=arrayr[4];
              t0=arrayr[5];
              cp=arrayr[6];
              gamma=arrayr[7];
              ch=arrayr[8];
              lcv=arrayr[9];
              lrz=arrayr[10];
              lpz=arrayr[11];
              lsz=arrayr[12];
              lcomb=arrayr[13];
              filmcooling=arrayb[1];
              simplified=arrayb[2];
              tambient=arrayr[14];
              eli=arrayr[15];
              elo=arrayr[16];
              eci=arrayr[17];
              eco=arrayr[18];
              kl=arrayr[19];
              kc=arrayr[20];
              tl=arrayr[21];
              tc=arrayr[22];
              /* Combustion chamber geometry */
              ncans=arrayi[2];
              nholesets=arrayi[3];
              ni=3;
              nr=22;
              nb=2;
              for( i=1; i <= nholesets; i ++)
              {
                ni += 1; cdspecification[i]=arrayi[ni];
                nr += 1; acd[i]=arrayr[nr];
                nr += 1; bcd[i]=arrayr[nr];
                nr += 1; ccd[i]=arrayr[nr];
                nr += 1; dcd[i]=arrayr[nr];
                ni += 1; holetype[i]=arrayi[ni];
                ni += 1; nholes[i]=arrayi[ni];
                ni += 1; holeposition[i]=arrayi[ni];
                nr += 1; holecd[i]=arrayr[nr];
                nr += 1; dhole[i]=arrayr[nr];
                nr += 1; slotdepth[i]=arrayr[nr];
                nr += 1; xhole[i]=arrayr[nr];
                nr += 1; dliner[i]=arrayr[nr];
                nr += 1; dlineri[i]=arrayr[nr];
                nr += 1; dcasing[i]=arrayr[nr];
                nr += 1; dcasingi[i]=arrayr[nr];
                nr += 1; primaryrzy[i]=arrayr[nr];
                nr += 1; primaryrpy[i]=arrayr[nr];
                nr += 1; secondaryy[i]=arrayr[nr];
                nr += 1; dilutiony[i]=arrayr[nr];
              }
            }
          }
          break;
     case 'R' : {    /* check if hole dimensions agree with overall dimensions */
            lmax=0.0;
            for( i=1; i <= nholesets; i ++)
            {
              if (xhole[i]>lmax)  lmax=xhole[i];
            }
            if (lmax>lcomb)
            {
              SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 4 );
              cout << "\n          " << string("Combustor length smaller than last holeset x-position (")+rts(lmax*1000.0,3,1)+"mm) !!!";
              SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 11 );
              lastpos=14;
              selected=0;
            }
          }
          break;
     case 'Q' : {    /* exit to DOS */
            SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 4 );
            cout << "\n           Are you sure you want to return to DOS (Y/N) ? ";
            answer = toupper(readkey());
            if (answer=='Y')
            {
              clrscr();

              SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 2 );
              cout << "\n           Saving input data as DEFAULT.txt " << endl;
              cout << "\n  "<< rootdir << endl;
              writedata(rootdir+"\\DEFAULT.txt");

              SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 4 );
              cout << " \nProgram terminated" << endl;
              SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 11 );
              exit(1);
            }
            lastpos=19;
          }
          break;
    }    /*case*/
  } while (!(selected=='R'));
}    /* of procedure MainMenu */

     /*of unit HF_Menu*/
