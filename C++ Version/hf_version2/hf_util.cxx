#include "ptoc.h"

#define __hf_util_implementation__
                      /* Unit used by HoleFlow.PAS */
                      /* A.A. Dreyer */
                      /* 27/11/1996 + 11/10/2000 */
#include "hf_util.h"

//Added INCLUDE files
#include <fstream>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <conio.h>
#include <sstream>

using namespace std;

//Added Global variables
SYSTEMTIME st;

/*$L WIN*/
/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

//Integrated functions
void gettime1(){
    h1 = st.wHour;
    m1 = st.wMinute;
    s1 = st.wSecond;
    s1x = st.wMilliseconds;
}

void gettime2(){
    h2 = st.wHour;
    m2 = st.wMinute;
    s2 = st.wSecond;
    s2x = st.wMilliseconds;
}

//Where x from the window
BYTE wherex()
{
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  COORD  result;
  if (!GetConsoleScreenBufferInfo(GetStdHandle( STD_OUTPUT_HANDLE ),&csbi))
    return -1;
  return result.X;
}

//Where y from the window
BYTE wherey()
{
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  COORD  result;
  if (!GetConsoleScreenBufferInfo(GetStdHandle( STD_OUTPUT_HANDLE ),&csbi))
    return -1;
  return result.Y;
}

//LO and HI bytes
BYTE lo(WORD w){
    return w & 0xFF;
}

BYTE hi(WORD w){
    return (w>>8) & 0xff;
}

//Keypressed scans the keyboard buffer and sees if a key has been pressed.
//If this is the case, True is returned. If not, False is returned.
//The Shift, Alt, Ctrl keys are not reported. The key is not removed from the buffer,
//and can hence still be read after the KeyPressed function has been called.
bool keypressed(){
    while(!kbhit()){}
    return true;
}

//Delay waits a specified number of milliseconds. The number of specified seconds is an approximation, and may be off a lot, if system load is high.
void delay(int ms){
    Sleep(ms);
}

//Sound sounds the speaker at a frequency of hz. Under Windows, a system sound is played and the frequency parameter is ignored. On other operating systems, this routine may not be implemented.
void sound(int hz){
    Beep(hz,1000); // 523 hertz (C5) for 500 milliseconds
    //cout << "\a";
}

//void window(BYTE x1, BYTE y1, BYTE x2, BYTE y2){
//    HWND console = GetConsoleWindow();
//    RECT r;
//    GetWindowRect(console, &r); //stores the console's current dimensions
//    MoveWindow(console, x1, y1, x2, y2, TRUE); // 800 width, 100 height
//}


/* Procedure to display polynomial correlations for CD for cooling openings */
void cdeditmenu(double& a,double& b,double& c,double& d)
{
  fstream fn;
  strarr item;
  strarr2 item3;
  double min,max;
  double coeff[8][4];
  int i,imin,imax,ncorr,selected,lastpos,error;

  /* Read current correlation data */
  fn.open("CDDATA.txt", ios::in);
  /*$I-*/
  //reset(fn);
  /*$I+*/
  error=*strerror(errno);
  if (error==0)
  {
    for( i=1; i <= 8; i ++)
      fn >> coeff[i][1] >> coeff[i][2] >> coeff[i][3] >> coeff[i][4] ;
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
    maintitle="Hole CD Correlations";
    heading(maintitle,true);
    item[ 1]=' ';
    item[ 2]=" CD = a + b((Pan-pft)/qan) + c((Pan-pft)/qan)^2 + d((Pan-pft)/qan)^3";
    item[ 3]=' ';
    item[ 4]="  where Pan - Stagnation pressure in annulus";
    item[ 5]="        pft - Static pressure in flame tube";
    item[ 6]="        qan - Velocity pressure in annulus";
    item[ 7]=' ';
    item[ 8]="                      a         b         c         d";
    item[ 9]=string(" Correlation 1 - ")+rts(coeff[1][1],9,3)+' '+rts(coeff[1][2],9,3)+' '+rts(coeff[1][3],9,3)+' '+rts(coeff[1][4],9,3);
    item[10]=string(" Correlation 2 - ")+rts(coeff[2][1],9,3)+' '+rts(coeff[2][2],9,3)+' '+rts(coeff[2][3],9,3)+' '+rts(coeff[2][4],9,3);
    item[11]=string(" Correlation 3 - ")+rts(coeff[3][1],9,3)+' '+rts(coeff[3][2],9,3)+' '+rts(coeff[3][3],9,3)+' '+rts(coeff[3][4],9,3);
    item[12]=string(" Correlation 4 - ")+rts(coeff[4][1],9,3)+' '+rts(coeff[4][2],9,3)+' '+rts(coeff[4][3],9,3)+' '+rts(coeff[4][4],9,3);
    item[13]=string(" Correlation 5 - ")+rts(coeff[5][1],9,3)+' '+rts(coeff[5][2],9,3)+' '+rts(coeff[5][3],9,3)+' '+rts(coeff[5][4],9,3);
    item[14]=string(" Correlation 6 - ")+rts(coeff[6][1],9,3)+' '+rts(coeff[6][2],9,3)+' '+rts(coeff[6][3],9,3)+' '+rts(coeff[6][4],9,3);
    item[15]=string(" Correlation 7 - ")+rts(coeff[7][1],9,3)+' '+rts(coeff[7][2],9,3)+' '+rts(coeff[7][3],9,3)+' '+rts(coeff[7][4],9,3);
    item[16]=string(" Correlation 8 - ")+rts(coeff[8][1],9,3)+' '+rts(coeff[8][2],9,3)+' '+rts(coeff[8][3],9,3)+' '+rts(coeff[8][4],9,3);
    item[17]=' ';
    item[18]="E - Edit correlations ";
    item[19]="R - Select a correlation and return to previous menu";
    cout << endl;
    for( i=1; i <= 19; i ++) cout << setw(5) << i << setw(5) << (item[i]).length() << item[i] << endl;
    displaymenu(19,lastpos,item,"Hole CD correlation Menu",selected);
    lastpos=selected+17;
    switch (selected) {
      case 1 : {
            imin=1;
            imax=8;
            item3[1]="Give the number of correlation to edit (1) ? ";
            item3[2]=string("Limits on valid correlation numbers : ")+its(imin,1)+" - "+its(imax,1)+" [-]";
            ncorr=intprompt(1,imin,imax,4,0,23,item3);
            min=-1000;
            max=1000;
            /* a */
            item3[1]=string("Give the constant a(")+its(ncorr,1)+") ("+rts(coeff[ncorr][1],8,5)+") ? ";
            item3[2]=string("Limits on valid correlation numbers : ")+its(imin,1)+" - "+its(imax,1)+" [-]";
            coeff[ncorr][1]=realprompt(coeff[ncorr][1],min,max,4,0,23,item3);
            /* b */
            item3[1]=string("Give the constant b(")+its(ncorr,1)+") ("+rts(coeff[ncorr][2],8,5)+") ? ";
            item3[2]=string("Limits on valid correlation numbers : ")+its(imin,1)+" - "+its(imax,1)+" [-]";
            coeff[ncorr][2]=realprompt(coeff[ncorr][2],min,max,4,0,23,item3);
            /* c */
            item3[1]=string("Give the constant c(")+its(ncorr,1)+") ("+rts(coeff[ncorr][3],8,5)+") ? ";
            item3[2]=string("Limits on valid correlation numbers : ")+its(imin,1)+" - "+its(imax,1)+" [-]";
            coeff[ncorr][3]=realprompt(coeff[ncorr][3],min,max,4,0,23,item3);
            /* d */
            item3[1]=string("Give the constant d(")+its(ncorr,1)+") ("+rts(coeff[ncorr][4],8,5)+") ? ";
            item3[2]=string("Limits on valid correlation numbers : ")+its(imin,1)+" - "+its(imax,1)+" [-]";
            coeff[ncorr][4]=realprompt(coeff[ncorr][4],min,max,4,0,23,item3);
          }
          break;
      case 2 : {
            imin=1;
            imax=8;
            item3[1]="Give the number of the correlation to select (1) ? ";
            item3[2]=string("Limits on valid correlation numbers : ")+its(imin,1)+" - "+its(imax,1)+" [-]";
            ncorr=intprompt(1,imin,imax,4,0,23,item3);
            a=coeff[ncorr][1];
            b=coeff[ncorr][2];
            c=coeff[ncorr][3];
            d=coeff[ncorr][4];
          }
          break;
    }    /*case*/
  } while (!(selected==2));
  /* Write current correlation data to file */
  fn.open("CDDATA.txt", ios::out);
  //rewrite(fn);
  error=*strerror(errno);
  for( i=1; i <= 8; i ++)
    fn << coeff[i][1] << coeff[i][2] << coeff[i][3] << coeff[i][4] << endl;
  fn.close();
}    /* of procedure CDEditMenu */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to calculate hyperbolic tangents */
double tanh(double a)
{
   double tanh_result;
   tanh_result=(exp(a)-exp(-a))/(exp(a)+exp(-a));
   return tanh_result;
}    /* of function TANH */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to evalute a to the power b */
double p(double aa,double bb)
{
  double p_result;
  p_result=exp(bb*log(aa));
  return p_result;
}    /* of function P */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to evaluate log10 */
double log10(double aa)
{
  double log10_result;
  log10_result=(log(aa))/log(10.0);
  return log10_result;
}    /* of function LOG10 */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Use Taylor series to calculate asin */
real asin(real a)
{
   int i;
   double total,subtotal;

   real asin_result;
   if (abs(a)>1.0)
   {
/*    writeln('invalid argument to arc sin : ',a);
     repeat until KeyPressed;
     Halt(1); */
     a=1.0;
   }
   total=a;
   subtotal=a;
   for( i=1; i <= 100; i ++)
   {
      subtotal=subtotal*a*a*(2.0*i-1.0)*(2.0*i-1.0)/((2.0*i)*(2.0*i+1.0));
      total=total+subtotal;
   }
   asin_result=total;
   return asin_result;
}    /* of function asin */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to calculate the temperature rise of a combusting air fuel mixture*/
/* T3 in K, P3 in bar */
double dt(double phi,double t3,double p3);


typedef double arraytype[23][6];



double t3ref[5] = {200.0 , 400.0 , 600.0 , 800.0 , 1000.0};
     /* phi      200      400      600      800     1000 */
arraytype dt001 =                                   /*0.1 bar*/
     {{0.02 ,   60.0 ,   57.0 ,   54.0 ,   51.0 ,   48.0},
      {0.03 ,   88.0 ,   85.5 ,   81.0 ,   76.5 ,   72.0},
      {0.04 ,  116.0 ,  114.0 ,  108.0 ,  102.0 ,   96.0},
      {0.05 ,  145.0 ,  141.5 ,  134.0 ,  127.0 ,  120.0},
      {0.06 ,  174.0 ,  169.0 ,  160.0 ,  152.0 ,  144.0},
      {0.07 ,  201.5 ,  196.0 ,  185.5 ,  176.0 ,  167.5},
      {0.08 ,  229.0 ,  223.0 ,  211.0 ,  200.0 ,  191.0},
      {0.10 ,  288.0 ,  278.0 ,  261.0 ,  250.0 ,  235.0},
      {0.15 ,  421.0 ,  407.0 ,  384.0 ,  365.0 ,  348.0},
      {0.20 ,  549.0 ,  527.0 ,  501.0 ,  478.0 ,  458.0},
      {0.30 ,  794.0 ,  758.0 ,  723.0 ,  695.0 ,  664.0},
      {0.40 , 1017.0 ,  978.0 ,  934.0 ,  896.0 ,  862.0},
      {0.50 , 1226.0 , 1183.0 , 1126.0 , 1074.0 , 1030.0},
      {0.60 , 1450.0 , 1365.0 , 1301.0 , 1232.0 , 1150.0},
      {0.70 , 1608.0 , 1534.0 , 1452.0 , 1362.0 , 1255.0},
      {0.80 , 1766.0 , 1674.0 , 1570.0 , 1454.0 , 1328.0},
      {0.90 , 1894.0 , 1778.0 , 1651.0 , 1515.0 , 1379.0},
      {1.00 , 1968.0 , 1820.0 , 1696.0 , 1552.0 , 1412.0},
      {1.10 , 1973.0 , 1843.0 , 1708.0 , 1569.0 , 1428.0},
      {1.20 , 1922.0 , 1815.0 , 1693.0 , 1567.0 , 1430.0},
      {1.30 , 1853.0 , 1758.0 , 1656.0 , 1538.0 , 1418.0},
      {1.40 , 1779.0 , 1690.0 , 1595.0 , 1490.0 , 1372.0},
      {1.50 , 1713.0 , 1623.0 , 1531.0 , 1436.0 , 1327.0}};
     /* phi      200      400      600      800     1000 */
arraytype dt003 =                                   /*0.3 bar*/
     {{0.02 ,   60.0 ,   57.0 ,   54.0 ,   51.0 ,   48.0},
      {0.03 ,   88.0 ,   85.5 ,   81.0 ,   76.5 ,   72.0},
      {0.04 ,  116.0 ,  114.0 ,  108.0 ,  102.0 ,   96.0},
      {0.05 ,  145.0 ,  141.5 ,  134.0 ,  127.0 ,  120.0},
      {0.06 ,  174.0 ,  169.0 ,  160.0 ,  152.0 ,  144.0},
      {0.07 ,  201.5 ,  196.0 ,  185.5 ,  176.0 ,  167.5},
      {0.08 ,  229.0 ,  223.0 ,  211.0 ,  200.0 ,  191.0},
      {0.10 ,  288.0 ,  278.0 ,  261.0 ,  250.0 ,  235.0},
      {0.15 ,  421.0 ,  407.0 ,  384.0 ,  365.0 ,  348.0},
      {0.20 ,  549.0 ,  527.0 ,  501.0 ,  478.0 ,  458.0},
      {0.30 ,  794.0 ,  758.0 ,  723.0 ,  695.0 ,  664.0},
      {0.40 , 1017.0 ,  978.0 ,  934.0 ,  896.0 ,  862.0},
      {0.50 , 1235.0 , 1184.0 , 1126.0 , 1077.0 , 1022.0},
      {0.60 , 1432.0 , 1370.0 , 1309.0 , 1244.0 , 1169.0},
      {0.70 , 1614.0 , 1540.0 , 1465.0 , 1382.0 , 1285.0},
      {0.80 , 1774.0 , 1687.0 , 1596.0 , 1488.0 , 1373.0},
      {0.90 , 1911.0 , 1805.0 , 1684.0 , 1562.0 , 1433.0},
      {1.00 , 1993.0 , 1871.0 , 1738.0 , 1608.0 , 1468.0},
      {1.10 , 1998.0 , 1882.0 , 1756.0 , 1622.0 , 1484.0},
      {1.20 , 1936.0 , 1840.0 , 1732.0 , 1609.0 , 1481.0},
      {1.30 , 1861.0 , 1773.0 , 1678.0 , 1573.0 , 1453.0},
      {1.40 , 1784.0 , 1702.0 , 1612.0 , 1518.0 , 1414.0},
      {1.50 , 1715.0 , 1629.0 , 1543.0 , 1455.0 , 1360.0}};
     /* phi      200      400      600      800     1000 */
arraytype dt01 =                                    /*1 bar*/
     {{0.02 ,   60.0 ,   57.0 ,   54.0 ,   51.0 ,   48.0},
      {0.03 ,   88.0 ,   85.5 ,   81.0 ,   76.5 ,   72.0},
      {0.04 ,  116.0 ,  114.0 ,  108.0 ,  102.0 ,   96.0},
      {0.05 ,  145.0 ,  141.5 ,  134.0 ,  127.0 ,  120.0},
      {0.06 ,  174.0 ,  169.0 ,  160.0 ,  152.0 ,  144.0},
      {0.07 ,  201.5 ,  196.0 ,  185.5 ,  176.0 ,  167.5},
      {0.08 ,  229.0 ,  223.0 ,  211.0 ,  200.0 ,  191.0},
      {0.10 ,  288.0 ,  278.0 ,  261.0 ,  250.0 ,  235.0},
      {0.15 ,  421.0 ,  407.0 ,  384.0 ,  365.0 ,  348.0},
      {0.20 ,  549.0 ,  527.0 ,  501.0 ,  478.0 ,  458.0},
      {0.30 ,  794.0 ,  758.0 ,  723.0 ,  695.0 ,  664.0},
      {0.40 , 1017.0 ,  978.0 ,  934.0 ,  896.0 ,  862.0},
      {0.50 , 1234.0 , 1183.0 , 1126.0 , 1083.0 , 1030.0},
      {0.60 , 1432.0 , 1374.0 , 1310.0 , 1251.0 , 1189.0},
      {0.70 , 1614.0 , 1548.0 , 1472.0 , 1399.0 , 1319.0},
      {0.80 , 1784.0 , 1700.0 , 1610.0 , 1520.0 , 1419.0},
      {0.90 , 1930.0 , 1830.0 , 1723.0 , 1602.0 , 1489.0},
      {1.00 , 2025.0 , 1912.0 , 1789.0 , 1660.0 , 1530.0},
      {1.10 , 2012.0 , 1911.0 , 1802.0 , 1678.0 , 1550.0},
      {1.20 , 1944.0 , 1855.0 , 1762.0 , 1652.0 , 1538.0},
      {1.30 , 1868.0 , 1785.0 , 1695.0 , 1603.0 , 1502.0},
      {1.40 , 1789.0 , 1707.0 , 1625.0 , 1540.0 , 1449.0},
      {1.50 , 1713.0 , 1633.0 , 1552.0 , 1471.0 , 1387.0}};
     /* phi      200      400      600      800     1000 */
arraytype dt03 =                                    /*3 bar*/
     {{0.02 ,   60.0 ,   57.0 ,   54.0 ,   51.0 ,   48.0},
      {0.03 ,   88.0 ,   85.5 ,   81.0 ,   76.5 ,   72.0},
      {0.04 ,  116.0 ,  114.0 ,  108.0 ,  102.0 ,   96.0},
      {0.05 ,  145.0 ,  141.5 ,  134.0 ,  127.0 ,  120.0},
      {0.06 ,  174.0 ,  169.0 ,  160.0 ,  152.0 ,  144.0},
      {0.07 ,  201.5 ,  196.0 ,  185.5 ,  176.0 ,  167.5},
      {0.08 ,  229.0 ,  223.0 ,  211.0 ,  200.0 ,  191.0},
      {0.10 ,  288.0 ,  278.0 ,  261.0 ,  250.0 ,  235.0},
      {0.15 ,  421.0 ,  407.0 ,  384.0 ,  365.0 ,  348.0},
      {0.20 ,  549.0 ,  527.0 ,  501.0 ,  478.0 ,  458.0},
      {0.30 ,  794.0 ,  758.0 ,  723.0 ,  695.0 ,  664.0},
      {0.40 , 1017.0 ,  978.0 ,  934.0 ,  896.0 ,  862.0},
      {0.50 , 1239.0 , 1139.0 , 1145.0 , 1085.0 , 1033.0},
      {0.60 , 1438.0 , 1373.0 , 1323.0 , 1252.0 , 1192.0},
      {0.70 , 1609.0 , 1550.0 , 1484.0 , 1409.0 , 1345.0},
      {0.80 , 1789.0 , 1710.0 , 1639.0 , 1590.0 , 1450.0},
      {0.90 , 1940.0 , 1850.0 , 1750.0 , 1640.0 , 1530.0},
      {1.00 , 2043.0 , 1940.0 , 1820.0 , 1707.0 , 1580.0},
      {1.10 , 2025.0 , 1930.0 , 1838.0 , 1718.0 , 1611.0},
      {1.20 , 1942.0 , 1861.0 , 1769.0 , 1672.0 , 1579.0},
      {1.30 , 1868.0 , 1787.0 , 1705.0 , 1618.0 , 1529.0},
      {1.40 , 1790.0 , 1709.0 , 1630.0 , 1550.0 , 1469.0},
      {1.50 , 1720.0 , 1640.0 , 1550.0 , 1480.0 , 1399.0}};
     /* phi      200      400      600      800     1000 */
arraytype dt10 =                                    /*10 bar*/
     {{0.02 ,   60.0 ,   57.0 ,   54.0 ,   51.0 ,   48.0},
      {0.03 ,   88.0 ,   85.5 ,   81.0 ,   76.5 ,   72.0},
      {0.04 ,  116.0 ,  114.0 ,  108.0 ,  102.0 ,   96.0},
      {0.05 ,  145.0 ,  141.5 ,  134.0 ,  127.0 ,  120.0},
      {0.06 ,  174.0 ,  169.0 ,  160.0 ,  152.0 ,  144.0},
      {0.07 ,  201.5 ,  196.0 ,  185.5 ,  176.0 ,  167.5},
      {0.08 ,  229.0 ,  223.0 ,  211.0 ,  200.0 ,  191.0},
      {0.10 ,  288.0 ,  278.0 ,  261.0 ,  250.0 ,  235.0},
      {0.15 ,  421.0 ,  407.0 ,  384.0 ,  365.0 ,  348.0},
      {0.20 ,  549.0 ,  527.0 ,  501.0 ,  478.0 ,  458.0},
      {0.30 ,  794.0 ,  758.0 ,  723.0 ,  695.0 ,  664.0},
      {0.40 , 1017.0 ,  978.0 ,  934.0 ,  896.0 ,  862.0},
      {0.50 , 1239.0 , 1185.0 , 1129.0 , 1089.0 , 1033.0},
      {0.60 , 1433.0 , 1377.0 , 1313.0 , 1257.0 , 1200.0},
      {0.70 , 1615.0 , 1552.0 , 1482.0 , 1420.0 , 1350.0},
      {0.80 , 1789.0 , 1718.0 , 1630.0 , 1560.0 , 1475.0},
      {0.90 , 1949.0 , 1861.0 , 1770.0 , 1673.0 , 1560.0},
      {1.00 , 2059.0 , 1968.0 , 1859.0 , 1749.0 , 1635.0},
      {1.10 , 2030.0 , 1944.0 , 1853.0 , 1753.0 , 1649.0},
      {1.20 , 1949.0 , 1870.0 , 1779.0 , 1705.0 , 1613.0},
      {1.30 , 1869.0 , 1790.0 , 1710.0 , 1640.0 , 1550.0},
      {1.40 , 1789.0 , 1711.0 , 1630.0 , 1555.0 , 1479.0},
      {1.50 , 1710.0 , 1635.0 , 1552.0 , 1480.0 , 1407.0}};
     /* phi      200      400      600      800     1000 */
arraytype dt296 =                                   /*29.6 bar*/
     {{0.02 ,   60.0 ,   57.0 ,   54.0 ,   51.0 ,   48.0},
      {0.03 ,   88.0 ,   85.5 ,   81.0 ,   76.5 ,   72.0},
      {0.04 ,  116.0 ,  114.0 ,  108.0 ,  102.0 ,   96.0},
      {0.05 ,  145.0 ,  141.5 ,  134.0 ,  127.0 ,  120.0},
      {0.06 ,  174.0 ,  169.0 ,  160.0 ,  152.0 ,  144.0},
      {0.07 ,  201.5 ,  196.0 ,  185.5 ,  176.0 ,  167.5},
      {0.08 ,  229.0 ,  223.0 ,  211.0 ,  200.0 ,  191.0},
      {0.10 ,  288.0 ,  278.0 ,  261.0 ,  250.0 ,  235.0},
      {0.15 ,  421.0 ,  407.0 ,  384.0 ,  365.0 ,  348.0},
      {0.20 ,  549.0 ,  527.0 ,  501.0 ,  478.0 ,  458.0},
      {0.30 ,  794.0 ,  758.0 ,  723.0 ,  695.0 ,  664.0},
      {0.40 , 1017.0 ,  978.0 ,  934.0 ,  896.0 ,  862.0},
      {0.50 , 1230.0 , 1180.0 , 1130.0 , 1083.0 , 1035.0},
      {0.60 , 1430.0 , 1375.0 , 1315.0 , 1260.0 , 1205.0},
      {0.70 , 1617.0 , 1552.0 , 1489.0 , 1421.0 , 1359.0},
      {0.80 , 1790.0 , 1720.0 , 1649.0 , 1520.0 , 1498.0},
      {0.90 , 1950.0 , 1872.0 , 1789.0 , 1700.0 , 1605.0},
      {1.00 , 2079.0 , 1989.0 , 1887.0 , 1785.0 , 1675.0},
      {1.10 , 2039.0 , 1951.0 , 1870.0 , 1780.0 , 1685.0},
      {1.20 , 1950.0 , 1870.0 , 1795.0 , 1717.0 , 1635.0},
      {1.30 , 1869.0 , 1790.0 , 1715.0 , 1640.0 , 1563.0},
      {1.40 , 1790.0 , 1711.0 , 1637.0 , 1561.0 , 1489.0},
      {1.50 , 1715.0 , 1639.0 , 1550.0 , 1485.0 , 1410.0}};



static void arrangearrays(double a,double b,arraytype dta,arraytype dtb,arraytype& dtl,arraytype& dtr,double& pl,double& pr)
{
    BYTE i,j;

  for( i=0; i < 23; i ++)
  {
    for( j=0; j <= 5; j ++)
    {
      dtl[i][j]=dta[i][j];
      dtr[i][j]=dtb[i][j];
    }
  }
  pl=a;
  pr=b;
}

double dt(double phi,double t3,double p3)

/* main function */
{
  double dta,dtb,dt1,dt2,pl,pr;
  BYTE i,j,il,ir,jl,jr;
  arraytype dtr,dtl;

  /* Find pressure range */
  double dt_result;
  if (p3<=0.1)                   arrangearrays( 0.1, 0.1,dt001,dt001,dtl,dtr,pl,pr);
  if ((p3> 0.1) && (p3<= 0.3))  arrangearrays( 0.1, 0.3,dt001,dt003,dtl,dtr,pl,pr);
  if ((p3> 0.3) && (p3<= 1.0))  arrangearrays( 0.3, 1.0,dt003,dt01 ,dtl,dtr,pl,pr);
  if ((p3> 1.0) && (p3<= 3.0))  arrangearrays( 1.0, 3.0,dt01 ,dt03 ,dtl,dtr,pl,pr);
  if ((p3> 3.0) && (p3<=10.0))  arrangearrays( 3.0,10.0,dt03 ,dt10 ,dtl,dtr,pl,pr);
  if ((p3>10.0) && (p3<=29.6))  arrangearrays(10.0,29.6,dt10 ,dt296,dtl,dtr,pl,pr);
  if (p3>29.6)                   arrangearrays(29.6,29.6,dt296,dt296,dtl,dtr,pl,pr);
  /* Find temperature range */
  j=0;
  do {
    j += 1;
  } while (!((t3<t3ref[j]) || (j==5)));
  if (t3<t3ref[j])  jl=j-1; else jl=j;
  jr=j;
  if (jl<1)  jl=1;
  if (jr>5)  jr=5;
  /* Find fuel ratio range */
  i=0;
  do {
    i += 1;
  } while (!((phi<dt001[i][0]) || (i==23)));
  if (phi<dt001[i][0])  il=i-1; else il=i;
  ir=i;
  if (il<1)  il=1;
  if (ir>23)  ir=23;
  /* Find correct dT at lower pressure */
  if (jl==jr)
  {
    dt1=dtl[il][jl];
    dt2=dtl[ir][jl];
  }
  else
  {
    dt1=dtl[il][jl]+(dtl[il][jr]-dtl[il][jl])*((t3-t3ref[jl])/(t3ref[jr]-t3ref[jl]));
    dt2=dtl[ir][jl]+(dtl[ir][jr]-dtl[ir][jl])*((t3-t3ref[jl])/(t3ref[jr]-t3ref[jl]));;
  }
  if (il==ir)
    dta=dt1;
  else
    dta=dt1+(dt2-dt1)*((phi-dt001[il][0])/(dt001[ir][0]-dt001[il][0]));
  /* Find correct dT at higher pressure */
  if (jl==jr)
  {
    dt1=dtr[il][jl];
    dt2=dtr[ir][jl];
  }
  else
  {
    dt1=dtr[il][jl]+(dtr[il][jr]-dtr[il][jl])*((t3-t3ref[jl])/(t3ref[jr]-t3ref[jl]));
    dt2=dtr[ir][jl]+(dtr[ir][jr]-dtr[ir][jl])*((t3-t3ref[jl])/(t3ref[jr]-t3ref[jl]));;
  }
  if (il==ir)
    dtb=dt1;
  else
    dtb=dt1+(dt2-dt1)*((phi-dt001[il][0])/(dt001[ir][0]-dt001[il][0]));
  /* Calculate the final dT */
  if ((p3>pr) || (p3<pl))
    dt_result=dta;    /*or dTb*/
  else
    dt_result=dta+(dtb-dta)*((p3-pl)/(pr-pl));
  return dt_result;
}    /*of function dT*/

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to convert a string to uppercase */
string ucstring(string s)
{
  string ucstring_result;
  std::locale loc;
  ucstring_result= toupper(s, loc);
  return ucstring_result;
}    /* of UCString */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to initialize timer routine */
void starttimer()
{
  gettime1();
}    /* of procedure StartTimer */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to read the time difference from initialization */
double endtimer()
{
  double endtimer_result;
  gettime2();
  endtimer_result=(h2*3600.0+m2*60.0+s2*1.0+s2x/100.0)-(h1*3600.0+m1*60.0+s1*1.0+s1x/100.0);
  return endtimer_result;
}    /* of function EndTimer */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to check if the disk drive is ready */
bool accessdisk(string defdir)
{
  fstream fn;
  string message;
  int error,counter,maxtries;

  bool accessdisk_result;
  maxtries=3;
  fn.open(defdir+"TEST.HFI", ios::out);
  counter=0;
  do {
    counter += 1;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    cout<< "\n\n                        " << string("Checking if drive ")+"\n  "+defdir+"\n                           is ready\n" << endl;
    //displaymessage(0,23,string("Checking if drive ")+defdir+" is ready",false);
    /*$I-*/
    //rewrite(fn);
    /*$I+*/
    //closewindow();
    error=*strerror(errno);
    if (error==78)
    {
      fn.close();
      //erase(fn);
    }
    else
    {
      if (counter!=maxtries)
      {
        switch (error) {
          case 101 : message="Disk full - Replace disk with empty disk and press RETURN"; break;
          case 150 : message="Disk write protected - Unprotect disk and press RETURN"; break;
          case 152 : message="Drive not ready - Close drive door and press RETURN"; break;
          default: message=string("Disk/Drive error no ")+its(error,1)+" - Press RETURN";
        }    /*case*/
        cout << message;
        //displaymessage(0,23,message,false);
        do {;  } while (!(readkey()=='\15'));
        //closewindow();
      }
    }
  } while (!((counter==maxtries) || (error==78)));

  if (error==78)
    accessdisk_result=true;
  else
    accessdisk_result=false;
  return accessdisk_result;
}    /* of function ACCESSDISK */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to check whether printer is ready */
/*
bool accessprinter()
{
  registers regs;
  int counter;
  int bit3,bit4;
  int bit5,bit7;
  int maxtries;
  int xrem,yrem;
  bool printerready;
  string message;

  bool accessprinter_result;
  xrem=wherex();
  yrem=wherey();
  maxtries=3;
  counter=0;
  do {
    counter += 1;
    // Determine printer status
    regs.s2.ah=2;
    regs.s1.dx=0;                   // 0 --> LPT1
    //intr(0x17,regs);
    bit3=((cardinal)regs.s2.ah >> 3) & 1;  // 0 = no error
    bit4=((cardinal)regs.s2.ah >> 4) & 1;  // 1 = on line
    bit5=((cardinal)regs.s2.ah >> 5) & 1;  // 1 = out of paper
    bit7=((cardinal)regs.s2.ah >> 7) & 1;  // 1 = not busy
    printerready=((bit3==0) && (bit4==1) && (bit5==0) && (bit7==1));
    if ((! printerready) && (counter!=maxtries))
    {
      if ((bit3==1) && (bit5==1))
        message="Printer out of paper - Fit paper & press RETURN";
      else
        message="Printer not ready - Correct and press RETURN";
      displaymessage(0,23,message,false);
      do {; } while (!(readkey()=='\15'));
      //closewindow();
    }
  } while (!(printerready || (counter==maxtries)));
  accessprinter_result=printerready;
  return accessprinter_result;
}    // of function ACCESSPRINTER */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to read a string from the user - only valid characters are accepted */
string readstring(BYTE n,string default_,bool displaychars)
{
  int i,j,num;
  BYTE x,y;
  char t[255];
  string name;
  bool test,return_;

  string readstring_result;
  i=1;
  name="";
  x=wherex();
  y=wherey();
  //clreol();
  do {
    do {
      t[i]=toupper(readkey());
      num=ord(t[i]);
/*     if (i>1) then
        test:=((num>=65) and (num<=90)) or ((num>=48) and (num<=57)) or (num=8)
      else
        test:=((num>=65) and (num<=90)); */
      if (i>1)
        test=(set::of(8,range(48,57),range(65,90), eos).has(num));
      else
        test=(set::of(range(48,57),range(65,90), eos).has(num));
      return_=(num==13);
    } while (!(test || return_));
    if (num!=8)
      {
        if (displaychars)
          cout << t[i];
        else
          cout << '*';
        if (num!=13)  name=(name+t[i]);
        i=i+1;
      }
    else
      {
        i=i-1;
        name.erase(0,(name).length());
        gotoxy(x,y);
        //clreol();
        if (displaychars)
          cout << name;
        else
        {
          for( j=1; j <= (int)(name).length(); j ++)
            cout << '*';
        }
      }
  } while (!(return_ || (i==n)));
/* ClrEol; */
  if (return_ && ((name).length()==0))  name=default_;
  readstring_result=name;
  return readstring_result;
}    /* of function READSTRING */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to read a file name - only valid characters are accepted */
string readfilename(string default_)
{
      const int n = 9;
  int i,num;
  BYTE x,y;
  char t[n];
  string name;
  bool test,return_;

  string readfilename_result;
  i=1;
  name="";
  x=wherex();
  y=wherey();
  //clreol();
  do {
    do {
      t[i]=toupper(readkey());
      num=ord(t[i]);
/*     if (i>1) then
        test:=((num>=65) and (num<=90)) or ((num>=48) and (num<=57)) or (num=8)
      else test:=((num>=65) and (num<=90)); */
      if (i>1)
        test=(set::of(8,range(48,57),range(65,90), eos).has(num));
      else
        test=(set::of(range(48,57),range(65,90), eos).has(num));
      return_=(num==13);
    } while (!(test || return_));
    if (num!=8)
      {
        cout << t[i];
        if (num!=13)  name=(name+t[i]);
        i=i+1;
      }
    else
      {
        i=i-1;
        name.erase(0,(name).length());
        gotoxy(x,y);
        //clreol();
        cout << name;
      }
  } while (!(return_ || (i==n)));
/* ClrEol; */
  if (return_ && ((name).length()==0))  name=default_;
  readfilename_result=name;
  return readfilename_result;
}    /* of function READFILENAME */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to read int values */
int readint(int default_)
{
      const int n = 5;
  BYTE x,y;
  string value;
  char t[n];
  int i,num,int_;
  bool test1,test2,return_;

  int readint_result;
  i=1;
  value="";
  x=wherex();
  y=wherey();
  //clreol();
  do {
    do {
      t[i]=toupper(readkey());
      num=ord(t[i]);
      test1=((num>=48) && (num<=57)) || ((num==8) && ((value).length()>0));
      test2=((num==45) && (i==1));
      return_=(num==13);
    } while (!(test1 || test2 || return_));
    if (num!=8)
      {
        cout << t[i];
        if (num!=13)  value=(value+t[i]);
        i=i+1;
      }
    else
      {
        i=i-1;
        value.erase(0,(value).length());
        gotoxy(x,y);
        //clreol();
        cout << value;
      }
  } while (!(return_ || (i==n)));
/* ClrEol;*/
  if (return_ && (i==2))
    int_=default_;
  else
    //val(value,int_,code);
    int_ = stoi(value);
  readint_result=int_;
  return readint_result;
}    /* of function READINT */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to read real values */
double readreal(double default_)
{
  const int n = 20;
  bool test1,test2;
  bool return_;
  char dot;
  char t[n];
  string valuestring;
  BYTE x,y;
  int i,num;
  double realvalue;

  double readreal_result;
  i=1;
  dot='N';
  valuestring="";
  x=wherex();
  y=wherey();
  //clreol();
  do {
    do {
      t[i]=toupper(readkey());
      num=ord(t[i]);
      test1=((num>=48) && (num<=57)) || ((num==8) && ((valuestring).length()>0));
      test2=((num==45) && (i==1)) || ((num==46) && (dot=='N'));
      return_=(num==13);
    } while (!(test1 || test2 || return_));
    if (num==46)  dot='Y';
    if (num!=8)
      {
        cout << t[i];
        if (num!=13)  valuestring=(valuestring+t[i]);
        i=i+1;
      }
    else
      {
        i=i-1;
        if (valuestring.substr(0,(valuestring).length())==""+'.')  dot='N';
        valuestring.erase(0,(valuestring).length());
        //gotoxy(x,y);
        //clreol();
        cout << valuestring;
      }
  } while (!(return_ || (i==n)));
/* ClrEol; */
  if (return_ && (i==2))
    realvalue=default_;
  else
    //val(valuestring,realvalue,code);
    realvalue = stof(valuestring);
  readreal_result=realvalue;
  return readreal_result;
}    /* of function READREAL */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to open a window and draw a titled box around it */

struct boxrec {
  char ul,    ur;
  char ll,    lr;
  char horiz, vert;
};


const boxrec singlebox = {'к', 'П',
                              'Р', 'й',
                              'Ф', 'Г'};
const boxrec doublebox1 = {'Щ', 'Л',
                              'Ш', 'М',
                              'Э', 'К'};
/*
void windowbox(int x1,int y1,int width,int height,string title)
{
  int i,oldattr,x2,y2;
  boxrec usebox;

  //window(1,1,80,25);
  x2=x1+width;
  y2=y1+height+1;
  //OldAttr:=TextAttr;
  textattr=menuattr;
  if (width==78)  usebox=singlebox; else usebox=doublebox1;
  {
    gotoxy(x1,y1);
    cout << usebox.ul;
    for( i=x1+1; i <= x2-1; i ++) cout << usebox.horiz;
    cout << usebox.ur;
    for( i=y1+1; i <= y2-1; i ++)
    {
      gotoxy(x1,i);cout << usebox.vert;
      gotoxy(x2,i);cout << usebox.vert;
    }
    gotoxy(x1,y2);
    cout << usebox.ll;
    for( i=x1+1; i <= x2-1; i ++) cout << usebox.horiz;
    cout << usebox.lr;
  }     //with
  // Center title on top of box
  if (title != "")
  {
    gotoxy(x1 + ((x2-x1)-(title).length()) / 2, y1);
    textattr=titleattr;
    cout << ' ' << title << ' ';
  }
// TextAttr:=OldAttr;
  textattr=menuattr;
  //window(x1+1,y1+1,x2-1,y2-1);
  clrscr();
}    // of procedure WINDOWBOX*/

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to open a window and draw a titled box around it */

struct boxrec1 {
  char ul,    ur;
  char ll,    lr;
  char horiz, vert;
};


const boxrec1 singlebox1 = {'к', 'П',
                              'Р', 'й',
                              'Ф', 'Г'};
const boxrec1 doublebox2 = {'Щ', 'Л',
                              'Ш', 'М',
                              'Э', 'К'};
/*
void windowbox2(int x1,int y1,int width,int height,string title,string title2)
{
  int i,oldattr,x2,y2;
  boxrec1 usebox;

  //window(1,1,80,25);
  x2=x1+width;
  y2=y1+height+1;
/ OldAttr:=TextAttr;
  textattr=menuattr;
  if (width==78)  usebox=singlebox1; else usebox=doublebox2;
  {
    gotoxy(x1,y1);
    cout << usebox.ul;
    for( i=x1+1; i <= x2-1; i ++) cout << usebox.horiz;
    cout << usebox.ur;
    for( i=y1+1; i <= y2-1; i ++)
    {
      gotoxy(x1,i);cout << usebox.vert;
      gotoxy(x2,i);cout << usebox.vert;
    }
    gotoxy(x1,y2);
    cout << usebox.ll;
    for( i=x1+1; i <= x2-1; i ++) cout << usebox.horiz;
    cout << usebox.lr;
  }    // with
  // Center title on top of box
  if (title != "")
  {
    gotoxy(x1 + ((x2-x1)-(title).length()) / 2, y1);
    textattr=titleattr;
    cout << ' ' << title << ' ';
  }
  // Center title2 at the bottom of box
  if (title2!="")
  {
    gotoxy(x1 + ((x2-x1)-(title2).length()) / 2, y2);
    textattr=titleattr;
    cout << ' ' << title2 << ' ';
  }
// TextAttr:=OldAttr;
  textattr=menuattr;
  //window(x1+1,y1+1,x2-1,y2-1);
  clrscr();
}    // of procedure WINDOWBOX2 */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to write a centred heading at the top of the screen */
void heading(string title,bool clrscreen)
{
    int remattr,x1,x2;

  //window(1,1,80,25);
  textattr=normalattr;
  if (clrscreen)  clrscr();
  if (title!="")
  {
    remattr=textattr;
    x1=0;
    x2=80;
    gotoxy(x1 + ((x2-x1)-((title).length()+2)) / 2, 1);
    textattr=firstcharattr;
    cout << ' ' << title << ' ';
    textattr=remattr;
  }
}    /* of procedure HEADING */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to convert a real number into its string representation */
string rts(double x,int width,int decimals)
{
    stringstream stream;
    stream << fixed << setprecision(decimals) << setw(width) << x;
    string s = stream.str();
    return s;
}    /* of function RTS */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to convert an int number into its string representation */
string its(int x,int width)
{

    stringstream stream;
    stream << setw(width) << x;
    string s = stream.str();
    return s;
}    /* of function ITS */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to display a standard menu */
void displaymenu(int numentr,int firstpos,strarr item,string title,int& selected);

struct boxrec2 {
   char ul,    ur;
   char ll,    lr;
   char horiz, vert;
};


const boxrec2 singlebox2 = {'к', 'П',
                              'Р', 'й',
                              'Ф', 'Г'};
const boxrec2 doublebox3 = {'Щ', 'Л',
                              'Ш', 'М',
                              'Э', 'К'};



  /* Procedure to determine which options are not valid */
static void searchdeadoptions(int& numdead, int& numentr, strarr item, int deadoption[23], int& maxlength, string blankstring[23])
{
    int i,j,numblanks;

  numdead=0;
  for( i=1; i <= numentr; i ++)
  {
    if (item[i][1]==' ')
    {
      numdead += 1;
      deadoption[numdead]=i;
    }
    numblanks=(maxlength-(item[i]).length())-2;
    blankstring[i]="";
    for( j=1; j <= numblanks; j ++) blankstring[i]=blankstring[i]+' ';
  }
}      /* of procedure SearchDeadOptions */



/* Procedure to write the firstcharacter of a string in a bright colour */
static void writestring(string mainstr)
{
  string first;
  string rest;

  first=mainstr.substr(0,1);
  rest=mainstr.substr(1, (mainstr).length());
  if (textattr==highlightattr)
  {
    textattr=firstcharhighlightattr; cout << first;
    textattr=highlightattr;          cout << rest;
  }
  else
  {
    textattr=firstcharattr; cout << first;
    textattr=menuattr;      cout << rest;
  }
}      /* of procedure WriteString */



  /* Procedure to determine highlight each selected option */
static void highlight(int& x1, int& y1, int& oldposition, strarr item, string blankstring[23], int& position)
{
  /* Clear previous position */
  gotoxy(x1+2,y1+oldposition);
  writestring(item[oldposition]+blankstring[oldposition]);
  /* Highlight new position */
  gotoxy(x1+2,y1+position);
  textattr=highlightattr;    /*2*16+15*/
  writestring(item[position]+blankstring[position]);
  textattr=menuattr;
  gotoxy(x1+2,y1+position);
}      /* of procedure HighLight */



  /* Function to determine if a given position is valid or not */
static bool in_dead_array(int i, int& numdead, int deadoption[23])
{
    int j;

  bool in_dead_array_result;
  in_dead_array_result=false;
  for( j=1; j <= numdead; j ++)
    if (i == deadoption[j])  in_dead_array_result=true;
  return in_dead_array_result;
}      /* of function In_Dead_Array */



  /* Function to determine if a given character is valid */
static bool in_valid_array(char cc, int& numentr, strarr item)
{
    int i;

  bool in_valid_array_result;
  in_valid_array_result=false;
  for( i=1; i <= numentr; i ++)
    if ((toupper(item[i][1])==cc) && (cc!=' '))  in_valid_array_result=true;
  return in_valid_array_result;
}      /* of function In_Valid_Array */

void displaymenu(int numentr,int firstpos,strarr item,string title,int& selected)
{
    string title2;

    /* main procedure */
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    if(title.length() < 10)
        cout << "\n         ************************  " << title << "  ************************\n";
    else if(title.length() > 10 && title.length() < 20)
        cout << "\n         **********************  " << title << "  ************************\n";
    else if(title.length() > 20 && title.length() < 30){
        cout << "\n         ****************  " << title << "  ***************\n";
    }

    for (int i = 0; i <= numentr; i++)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        cout << "          " << item[i] << endl;
    }
    bool current_keypress = false;
    do{
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        cout << "\n                        KEY PRESS YOUR OPERATION CHOICE...";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

        current_keypress = false;

        switch(toupper(readkey()))
        {
        case 'A':
            selected = 'A';
            break;
        case 'B':
            selected = 'B';
            break;
        case 'C':
            selected = 'C';
            break;
        case 'D':
            selected = 'D';
            break;
        case 'E':
            selected = 'E';
            break;
        case 'F':
            selected = 'F';
            break;
        case 'G':
            selected = 'G';
            break;
        case 'H':
            selected = 'H';
            break;
        case 'I':
            selected = 'I';
            break;
        case 'J':
            selected = 'J';
            break;
        case 'K':
            selected = 'K';
            break;
        case 'L':
            selected = 'L';
            break;
        case 'M':
            selected = 'M';
            break;
        case 'N':
            selected = 'N';
            break;
        case 'O':
            selected = 'O';
            break;
        case 'P':
            selected = 'P';
            break;
        case 'Q':
            selected = 'Q';
            break;
        case 'R':
            selected = 'R';
            break;
        case 'S':
            selected = 'S';
            break;
        case 'T':
            selected = 'T';
            break;
        case 'U':
            selected = 'U';
            break;
        case 'V':
            selected = 'V';
            break;
        case 'W':
            selected = 'W';
            break;
        case 'X':
            selected = 'X';
            break;
        case 'Y':
            selected = 'Y';
            break;
        case 'Z':
            selected = 'Z';
            break;
        default:
            current_keypress = true;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            cout << "\n\n               ERROR! You have selected an invalid choice.\n"; Sleep(3000);
            break;
        }
    }while(current_keypress);

}    /* of procedure DISPLAYMENU */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to choose a file from a given directory - 22 November 1993 */
void choosefile(string& dir,string mask,string heading,string& chosen);

const bool sortdata = true;      /* Sort the file names alphabetically */

const bool showdrives = true;    /* Display the valid drive names in the file list */
                        /* Maximum number of files */
const int maxcol = 4;           /* Number of columns (max = 5)*/
                        /* Maximum number of lines in the window */
const int maxrowdisplay = 4;
const int maxperpage = maxrowdisplay*maxcol;

const int maxnumdrives = 10;
const int maxnumfiles = 10;

typedef bool drivearr[maxnumdrives];
typedef string filenamearray[maxnumfiles];



  /* Procedure to find the available drives */
static void finddrives(drivearr& drive)
{
  BYTE i;
  registers regs;

  for( i=1; i <= maxnumdrives; i ++) drive[i]=false;
  /* Search for valid drives */
  {
    for( i=1; i <= maxnumdrives; i ++)
    {
      regs.s2.dl=i-1;
      if (i>2)  regs.s2.dl=0x80+i-3;
      regs.s2.ah=21;   /* $15 */
      //intr(0x13,regs);  /* ROM BIOS disk service */
      if (set::of(range(1,3), eos).has(regs.s2.ah))  drive[i]=true;
      /*
        write(i:4,'   ',Drive[i]);
        case AH of
           0 : writeln('  No drive present');
           1 : writeln('   Floppy disk (non sensing)');
           2 : writeln('   Floppy disk (sensing)');
           3 : writeln('   Hard disk');
          else writeln;
        end; {case}
        */
    }
  }    /*with*/
}      /* of procedure FINDDRIVES */

static void sortlist(int lo,int hi,filenamearray& a);

static void sort(int l,int r, filenamearray& a)
{
  int i,j;
  string x,y;

  i=l;
  j=r;
  x=a[(l+r) / 2];
  do {
    while (a[i]<x)  i=i+1;
    while (x<a[j])  j=j-1;
    if (i<=j)
    {
      y=a[i];
      a[i]=a[j];
      a[j]=y;
      i=i+1;
      j=j-1;
    }
  } while (!(i>j));
  if (l<j)  sort(l,j, a);
  if (i<r)  sort(i,r, a);
}        /* of procedure Sort */



  /* Procedure to sort a list of strings alphabetically - Quick sort */
static void sortlist(int lo,int hi,filenamearray& a)
{      /* SortList */
  if (hi<lo)  return;
  sort(lo,hi, a);
  cout << "Sort";
  Sleep(2000);
}      /* of procedure SortList */



  /* Procedure to read all the filenames/directories in a directory */
static void readdir(string dir,string mask,drivearr drive, int& numfiles, filenamearray& filename)
{
  string blank;
  bool lowerdir;
  searchrec dirinfo;
  int i,numfiles0;

  /* Find all the normal files in the specified directory */
  //findfirst(dir+mask,0,dirinfo);
  numfiles=0;

  /*while (doserror==0)
  {

    numfiles += 1;
    if (dirinfo.name.find(".")!=0)
    {
      blank="            ";
      cout << dirinfo.name << endl;
      Sleep(2000);
      blank.erase(0, (dirinfo.name).length());
      size_t found = dirinfo.name.find(".");
      if(found!=string::npos){
        cout << "\n1: Code check, code check, 1 2, 1 2";
        Sleep(2000);
        cout << dirinfo.name << endl;
        blank.insert(found, dirinfo.name);
      }


    }
    else
    {
      while ((dirinfo.name).length()<8)  dirinfo.name=dirinfo.name+' ';
      dirinfo.name=dirinfo.name+".   ";
    }
    filename[numfiles]=dirinfo.name;
    //findnext(dirinfo);

  }*/


  if (sortdata)  sortlist(1,numfiles,filename);

  numfiles0=numfiles;
  /* Find all the subdirectories in the specified directory */
  lowerdir=false;
  //findfirst(dir+"*.*",directory,dirinfo);
  while (doserror==0)
  {
    if ((dirinfo.name!=""+'.') && (dirinfo.attr==16))
    {
      if (dirinfo.name[1]=='.')  lowerdir=true;
      else
      {
        numfiles += 1;
        blank="            ";
        blank.erase(0,(dirinfo.name).length());
        filename[numfiles]=dirinfo.name+'\\'+blank;
      }
    }
    //findnext(dirinfo);
  }    /* while */
  if (sortdata)  sortlist(numfiles0+1,numfiles,filename);
  /* Find the special entries ie. drives and symbol for lower directories */
  if (lowerdir)
  {
    numfiles += 1;
    filename[numfiles]="..\\         ";
  }
  else
  {
    if (showdrives)
    {
      for( i=1; i <= maxnumdrives; i ++)
      {
        if (drive[i] && ((dir.substr(0,1)!=""+chr(64+i))))
        {
          numfiles += 1;
          filename[numfiles]=string("[")+chr(64+i)+":]        ";
        }
      }
    }
  }
  cout << "Going out";
  Sleep(2000);
}      /* of procedure READDIR */



  /* Procedure to determine the blank string to fill a file name to 12 chars */
static string blankstring(string tempfile)
{
  string blank;
  int j,numblanks;

    string blankstring_result;
    numblanks=(12-(tempfile).length())+1;
    blank="";
    for( j=1; j <= numblanks; j ++) blank=blank+"  ";
    blankstring_result=blank;
    return blankstring_result;
}      /* of function BLANKSTRING */



  /* Procedure to determine highlight the currently selected file */
  /*
static void highlight1(int& page_, int& oldrow, int& oldcol, filenamearray& filename, int& row, int& col)
{
    int oldnumber,number;

  // Clear previous position
  oldnumber=(page_-1)*maxperpage+maxcol*(oldrow-1)+oldcol;
  //writestr(1+(oldcol-1)*16,oldrow,string("  ")+filename[oldnumber]+blankstring(filename[oldnumber]),menuattr);
  // Highlight new position
  number=(page_-1)*maxperpage+maxcol*(row-1)+col;
  //writestr(1+(col-1)*16,row,string("  ")+filename[number]+blankstring(filename[number]),highlightattr);
}      // of procedure HIGHLIGHT */

//void choosefile(string& dir,string mask,string heading,string& chosen)
//{
//
//  framechars frame;
//
//
//const bool fixedvertsize = true; /* Fixed of floating maximum number of rows */
//const int maxnumfiles = 400;
//const int firstline = 2;        /* Position of window from top of page */
//const int maxrowdisplay = 15;
//const int maxpages = (maxnumfiles / maxperpage)+1;
//filenamearray filename;
//int oldrow,oldcol,row,col,numfiles,page_;
//
///* Main procedure */
//BYTE remattr,x1,x2;
//drivearr drive;
//string title,message;
//char character,dummy;
//bool specialkey,changepage;
//string filepage[maxpages][maxperpage];
//int i,j,k,x,y,height,width,numpages,numfileslastpage,maxlength,
//currentnumfiles,numrowslastpage,maxrow,numcollastrow;
//
//
//
//finddrives(drive);
//
//if (dir.substr(0, (dir).length())!=""+'\\')  dir=dir+'\\';
//
////window(1,1,80,25);
//topwindow=nil;
//cursoroff();
//message=string(" Use ")+chr(27)+chr(24)+chr(25)+chr(26)+" and "+chr(60)+chr(217)+" (or Esc) for selection ";
//do {
//  textattr=menuattr;
//  readdir(dir,mask,drive, numfiles, filename);
//    cout << "\nCode check, code check, 1 2, 1 2";
//    Sleep(2000);
//  if (numfiles==0)            /* empty directory */
//  {
//    currentnumfiles=1;
//    if (doserror==3)  message="    Invalid directory !    "; else message=" No matching files found ";
//    maxlength=(message).length();
//    x=(((lo(windmax)+lo(windmin))-maxlength) / 2);
//    y=firstline;
//    width=maxlength+2;
//    height=3;
//    //openwindow(x,y,x+width-1,y+height-1,string(" ")+dir+mask+' ',message,titleattr,menuattr);
//    //writestr(1,1,message,menuattr);
//    chosen="";
//    dummy=readkey();
//  }
//  else
//  {
//    page_=1;
//    numpages=(numfiles / maxperpage)+1;
//    if ((numpages-1)*maxperpage==numfiles)  numpages=numpages-1;
//    /* Copy files into the two dimensional array for display */
//    i=1;
//    j=0;
//    for( k=1; k <= numfiles; k ++)
//    {
//      j += 1;
//      filepage[i][j]= filename[k];
//      if (k==(i*maxperpage))
//      {
//        i += 1;
//        j=0;
//      }
//    }
//    if (numfiles==numpages*maxperpage)  numfileslastpage=maxperpage; else numfileslastpage=j;
//    oldrow=1;
//    oldcol=1;
//    do {
//      /* Write applications heading */
//      if (heading!="")
//      {
//        remattr=textattr;
//        x1=0;
//        x2=80;
//        gotoxy(x1 + ((x2-x1)-((heading).length()+2)) / 2, 1);
//        textattr=firstcharattr;
//        cout << ' ' << heading << ' ';
//        textattr=remattr;
//      }
//      if (page_==numpages)  currentnumfiles=numfileslastpage;
//        else currentnumfiles=maxperpage;
//      /* Display files in window */
//      maxlength=maxcol*12+(maxcol-1)*4+4;
//      x=(((lo(windmax)+lo(windmin))-maxlength) / 2);
//      y=firstline;
//      width=maxlength+2;
//      if (fixedvertsize)
//        height=maxrowdisplay+2;
//      else
//      {
//        height=(currentnumfiles / maxcol)+3;
//        if ((currentnumfiles % maxcol)==0)  height -= 1;
//      }
//      if (numpages>1)
//        title=dir+mask+"   (Page "+its(page_,1)+" of "+its(numpages,1)+')';
//      else
//        title=dir+mask;
//      //openwindow(x,y,x+width-1,y+height-1,string(" ")+title+' ',message,titleattr,menuattr);
//      /* Write the entries inside the block */
//      row=1;
//      col=1;
//      for( i=1; i <= currentnumfiles; i ++)
//      {
//        //writestr(3+(col-1)*16,row,filepage[page_][i]+blankstring(filepage[page_][i]),menuattr);
//        col += 1;
//        if ((col>maxcol) && (i!=currentnumfiles))
//        {
//          row += 1;
//          col=1;
//        }
//      }
//      maxrow=row;
//      /* Place highlight on first file */
//      row=oldrow;
//      col=oldcol;
//      highlight1(page_, oldrow, oldcol, filename, row, col);
//      /* Move cursor to required position */
//      do {
//        changepage=false;
//        oldrow=row;
//        oldcol=col;
//        specialkey=false;
//        do {
//          character=toupper(readkey());
//        } while (!(set::of('\0','\15','\33', eos).has(character)));
//        if (character=='\0')
//        {
//          specialkey=true;
//          character=toupper(readkey());
//          switch (character) {
//            case '\110' : row=row-1; break;   /* Up arrow */
//            case '\113' : col=col-1; break;   /* Left arrow */
//            case '\115' : col=col+1; break;   /* Right arrow */
//            case '\120' : row=row+1; break;   /* Down arrow */
//            case '\111' : {                  /* Page up */
//                    if (page_>1)
//                    {
//                      changepage=true;
//                      page_=page_-1;
//                      oldrow=row;
//                      oldcol=col;
//                    }
//                    else row=1;
//                  }
//                  break;
//            case '\121' : {                  /* Page down */
//                    if (page_==numpages)
//                    {
//                      numcollastrow=(numfileslastpage % maxcol);
//                      if ((numcollastrow)==0)  numcollastrow=maxcol;
//                      numrowslastpage=((numfileslastpage-numcollastrow) / maxcol)+1;
//                      row=numrowslastpage;
//                      if (((row-1)*maxcol+col)>numfileslastpage)  row=row-1;
//                    }
//                    else
//                    {
//                      changepage=true;
//                      page_=page_+1;
//                      oldrow=row;
//                      oldcol=col;
//                      if (page_==numpages)
//                      {
//                        numcollastrow=(numfileslastpage % maxcol);
//                        if ((numcollastrow)==0)  numcollastrow=maxcol;
//                        numrowslastpage=((numfileslastpage-numcollastrow) / maxcol)+1;
//                        if (((row-1)*maxcol+col)>numfileslastpage)
//                        {
//                          oldcol=numcollastrow;
//                          oldrow=numrowslastpage;
//                        }
//                      }
//                    }
//                  }
//                  break;
//            case '\107' : {                   /* Home */
//                    if (page_==1)
//                    {
//                      row=1;
//                      col=1;
//                    }
//                    else
//                    {
//                      changepage=true;
//                      page_=1;
//                      oldcol=1;
//                      oldrow=1;
//                    }
//                  }
//                  break;
//            case '\117' : {                   /* End */
//                    if (page_==numpages)
//                    {
//                      row=maxrow;
//                      col=(maxcol-((maxcol*maxrow)-currentnumfiles));
//                    }
//                    else
//                    {
//                      changepage=true;
//                      page_=numpages;
//                      numcollastrow=(numfileslastpage % maxcol);
//                      if ((numcollastrow)==0)  numcollastrow=maxcol;
//                      numrowslastpage=((numfileslastpage-numcollastrow) / maxcol)+1;
//                      oldcol=numcollastrow;
//                      oldrow=numrowslastpage;
//                    }
//                  }
//                  break;
//          }    /* case */
//        }
//        if (row>maxrow)               /* scolling past the bottom */
//        {
//          if (page_<numpages)
//          {
//            changepage=true;
//            page_=page_+1;
//            oldrow=1;
//            oldcol=col;
//            if (page_==numpages)
//            {
//              numcollastrow=(numfileslastpage % maxcol);
//              if ((numcollastrow)==0)  numcollastrow=maxcol;
//              numrowslastpage=((numfileslastpage-numcollastrow) / maxcol)+1;
//              if ((col)>numfileslastpage)
//              {
//                oldcol=numcollastrow;
//                oldrow=numrowslastpage;
//              }
//            }
//          }
//          else row=row-1;
//        }
//        if (row<1)                    /* scolling past the top */
//        {
//          if (page_==1)  row=1;
//          else
//          {
//            changepage=true;
//            page_=page_-1;
//            oldrow=maxrowdisplay;
//            oldcol=col;
//          }
//        }
//        if (col>maxcol)               /* scolling past the right */
//        {
//          row=row+1;
//          col=1;
//          if (row>maxrow)
//          {
//            if (page_==numpages)
//            {
//              col=maxcol;
//              row=row-1;
//            }
//            else
//            {
//              changepage=true;
//              page_=page_+1;
//              oldrow=1;
//              oldcol=1;
//            }
//          }
//        }
//        if (col<1)                    /* scolling past the left */
//        {
//          row=row-1;
//          col=maxcol;
//          if (row<1)
//          {
//            if (page_==1)
//            {
//              row=1;
//              col=1;
//            }
//            else
//            {
//              changepage=true;
//              page_=page_-1;
//              oldrow=maxrowdisplay;
//              oldcol=maxcol;
//            }
//          }
//        }
//        if ((row==maxrow) && ((maxcol*(row-1)+col)>currentnumfiles))
//        {
//          if (page_==numpages)
//          {
//            if (character=='\120')  row=row-1;
//            if (character=='\115')  col=col-1;
//          }
//          else              /* Page down */
//          {
//            changepage=true;
//            page_=page_+1;
//            oldrow=1;
//            oldcol=col;
//          }
//        }
//        if (! changepage)  highlight1(page_, oldrow, oldcol, filename, row, col);
//      } while (!((set::of('\15','\33', eos).has(character)) || changepage));
//      //if (changepage)  closewindow();
//    } while (!(! changepage));
//    if (character=='\33')  chosen="";
//    else
//    {
//      chosen=filepage[page_][(maxcol*(row-1)+col)];
//      while (chosen.find(" ")>0)  chosen.erase(0, chosen.find(" ")-1);
//      if (chosen.substr(0, (chosen).length()-1)==""+'\\')     /* Change directory */
//      {
//        if (chosen.substr(0,1)==""+'.')
//        {
//          do {
//            dir.erase(0, (dir).length());
//          } while (!(dir.substr(0, (dir).length()-1)==""+'\\'));
//        }
//        else dir=dir+chosen;
//        textattr=normalattr;
//        //closewindow();
//      }
//      if (chosen.substr(0, (chosen).length()-1)==""+']')      /* Change drive */
//      {
//        dir=chosen.substr(1,2)+'\\';
//        chosen='\\';
//        //closewindow();
//      }
//    }
//  }
//} while (!(chosen.substr(0, (chosen).length()-1)!=""+'\\'));
////closewindow();
//cursoron();
//textattr=normalattr;
////window(1,1,80,25);
//}  /* of procedure CHOOSEFILE */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to choose a file from a given directory */
void choosefile0(string& dir,string mask,string& chosen);

struct boxrec3 {
   char ul,    ur;
   char ll,    lr;
   char horiz, vert;
};

                        /* Maximum number of files */
const int maxcol1 = 3;           /* Number of columns */
                        /* Maximum number of lines in the window */
const int maxrowdisplay1 = 3;
const int maxperpage1 = maxrowdisplay1*maxcol1;

const boxrec3 doublebox = {'Щ', 'Л',
                              'Ш', 'М',
                              'Э', 'К'};

const int maxnumdrives1 = 10;


typedef bool drivearr1[maxnumdrives1];



  /* Procedure to turn cursor off */
//void cursoroff()
//{
//    registers regs;
//
//  {
//    regs.s2.ah=0x1;
//    regs.s2.ch=0x20;
//    //intr(0x10,regs);
//  }
//}      /* of procedure CURSOROFF */



  /* Procedure to turn cursor on */
//void cursoron()
//{
//    registers regs;
//
//  {
//    regs.s2.ah=0x1;
//    regs.s2.ch=6;
//    regs.s2.cl=7;
//    //intr(0x10,regs);
//  }
//}      /* of procedure CURSORON */



  /* Procedure to find the available drives */
void finddrives1(drivearr1& drive)
{
  BYTE i;
  registers regs;

  for( i=1; i <= maxnumdrives1; i ++) drive[i]=false;
  /* Search for valid drives */
  {
    for( i=1; i <= maxnumdrives1; i ++)
    {
      regs.s2.dl=i-1;
      if (i>2)  regs.s2.dl=0x80+i-3;
      regs.s2.ah=21;   /* $15 */
      //intr(0x13,regs);  /* ROM BIOS disk service */
      if (set::of(range(1,3), eos).has(regs.s2.ah))  drive[i]=true;
    }
  }    /*with*/
}      /* of procedure FINDDRIVES */



  /* Procedure to read all the filenames/directories in a directory */
void readdir1(string dir,string mask,drivearr1 drive, int& numfiles, string filename[400])
{
  int i;
  searchrec dirinfo;
  bool lowerdir;

  /* Find all the normal files in the specified directory */
  //findfirst(dir+mask,0,dirinfo);
  numfiles=0;
  while (doserror==0)
  {
    numfiles += 1;
    filename[numfiles]=dirinfo.name;
    //findnext(dirinfo);
  }
  /* Find all the subdirectories in the specified directory */
  lowerdir=false;
  //findfirst(dir+"*.*",directory,dirinfo);
  while (doserror==0)
  {
    if ((dirinfo.name!=""+'.') && (dirinfo.attr==16))
    {
      if (dirinfo.name[1]=='.')  lowerdir=true;
      else
      {
        numfiles += 1;
        filename[numfiles]=dirinfo.name+'\\';
      }
    }
    //findnext(dirinfo);
  }    /* while */
  if (lowerdir)
  {
    numfiles += 1;
    filename[numfiles]="..\\";
  }
  else
  {
    for( i=1; i <= maxnumdrives1; i ++)
    {
      if (drive[i] && (dir.substr(0,1)!=""+chr(64+i)))
      {
        numfiles += 1;
        filename[numfiles]=string("[")+chr(64+i)+":]";
      }
    }
  }
}      /* of procedure READDIR */



  /* Procedure to determine the blank string to fill a file name to 12 chars */
string blankstring1(string tempfile)
{
  int j,numblanks;
  string blank;

    string blankstring1_result;
    numblanks=(12-(tempfile).length())+1;
    blank="";
    for( j=1; j <= numblanks; j ++) blank=blank+' ';
    blankstring1_result=blank;
    return blankstring1_result;
}      /* of function BLANKSTRING */



  /* Procedure to determine highlight the currently selected file */
void highlight2(int& page_, int& oldrow, int& oldcol, int& x1, int& y1, string filename[400], int& row, int& col)
{
    int oldnumber,number;

  /* Clear previous position */
  oldnumber=(page_-1)*maxperpage1+maxcol1*(oldrow-1)+oldcol;
  gotoxy(x1+1+(oldcol-1)*14,y1+oldrow);
  textattr=menuattr;
  cout << ' ' << filename[oldnumber]+blankstring1(filename[oldnumber]);
  /* Highlight new position */
  number=(page_-1)*maxperpage1+maxcol1*(row-1)+col;
  gotoxy(x1+1+(col-1)*14,y1+row);
  textattr=highlightattr;         /*2*16+7*/
  cout << ' ' << filename[number]+blankstring1(filename[number]);
  textattr=menuattr;
}      /* of procedure HIGHLIGHT */



  /* Procedure to draw a frame around the file entries */
void frame(int maxlength,int y1,int y2,int xsize,string title,string message, int& x1)
{
  int i,x2;

  x1=(xsize-maxlength) / 2;
  x2=x1+maxlength+1;
  {
    gotoxy(x1,y1);
    cout << doublebox.ul;
    for( i=x1+1; i <= x2-1; i ++) cout << doublebox.horiz;
    cout << doublebox.ur;
    for( i=y1+1; i <= y2-1; i ++)
    {
      gotoxy(x1,i);cout << doublebox.vert;
      gotoxy(x2,i);cout << doublebox.vert;
    }
    gotoxy(x1,y2);
    cout << doublebox.ll;
    for( i=x1+1; i <= x2-1; i ++) cout << doublebox.horiz;
      cout << doublebox.lr;
  }    /* with */
  textattr = titleattr;
  if ((title).length()>maxlength-2)  title[0]=chr(maxlength-2);
  gotoxy(x1 + ((x2-x1)-(title).length()) / 2 + 1, y1);
  cout << title;
  if ((message).length()>maxlength-2)  message[0]=chr(maxlength-2);
  gotoxy(x1 + ((x2-x1)-(message).length()) / 2 + 1, y2);
  cout << message;
  textattr=menuattr;
}      /* of procedure FRAME */

void choosefile0(string& dir,string mask,string& chosen){


const bool fixedvertsize1 = true; /* Fixed of floating maximum number of rows */
const int maxnumfiles1 = 400;
const int firstline1 = 2;        /* Position of window from top of page */
const int maxrowdisplay1 = 10;
const int maxpages1 = (maxnumfiles1 / maxperpage1)+1;
int x1,y1,oldrow,oldcol,row,col,numfiles,page_;
string filename[maxnumfiles1];

/* Main procedure */
drivearr1 drive;
char character;
string title,pagestr,message;
bool specialkey,changepage;
int i,j,k,numfileslastpage,xsize,ysize,y2,maxlength,numpages,
currentnumfiles,numrowslastpage,maxrow,numcollastrow;
string filepage[maxpages1][maxperpage1];

finddrives1(drive);
do {
  textattr=normalattr;
  readdir1(dir,mask,drive, numfiles, filename);
  if (numfiles==0)            /* empty directory */
  {
    currentnumfiles=1;
    textattr=menuattr;
    /* draw a box around the entries */
    ysize=hi(windmax)-hi(windmin);
    y1=firstline1;
    y2=y1+2;
    xsize=lo(windmax)-lo(windmin);
    maxlength=36;
    title=string(" ")+dir+mask+' ';
    frame(maxlength,y1,y2,xsize,title," Press any key to continue ", x1);
    gotoxy(x1+1,y1+1);
    cout << "       No matching files found      ";
    chosen="";
    do {; } while (!keypressed());
  }
  else
  {
    page_=1;
    numpages=(numfiles / maxperpage1)+1;
    if ((numpages-1)*maxperpage1==numfiles)  numpages=numpages-1;
    /* Copy files into the two dimensional array for display */
    i=1;
    j=0;
    for( k=1; k <= numfiles; k ++)
    {
      j += 1;
      filepage[i][j]= filename[k];
      if (k==(i*maxperpage1))
      {
        i += 1;
        j=0;
      }
    }
    if (numfiles==numpages*maxperpage1)  numfileslastpage=maxperpage1; else numfileslastpage=j;
    oldrow=1;
    oldcol=1;
    do {
      if (page_==numpages)  currentnumfiles=numfileslastpage;
        else currentnumfiles=maxperpage1;
      /* Display files in window */
      textattr=menuattr;
      /* draw a box around the entries */
      ysize=hi(windmax)-hi(windmin);
      y1=firstline1;
      if (fixedvertsize1)
        y2=y1+maxrowdisplay1+1;
      else
      {
        y2=y1+(currentnumfiles / maxcol1)+2;
        if ((currentnumfiles % maxcol1)==0)  y2=y2-1;
      }
      xsize=lo(windmax)-lo(windmin);
      maxlength=maxcol1*12+(maxcol1-1)*2+2;
      if (numpages>1)
        title=string(" ")+dir+mask+"   (Page "+its(page_,1)+" of "+its(numpages,1)+") ";
      else
        title=string(" ")+dir+mask+' ';
      message=string(" Use ")+chr(27)+chr(24)+chr(25)+chr(26)+" and "+chr(60)+chr(217)+" (or Esc) for selection ";
      frame(maxlength,y1,y2,xsize,title,message, x1);
      /* Write the entries inside the block */
      row=1;
      col=1;
      textattr=menuattr;
      for( i=1; i <= currentnumfiles; i ++)
      {
        gotoxy(x1+1+(col-1)*14,y1+row);
        cout << ' ' << filepage[page_][i]+blankstring1(filepage[page_][i]);
        col += 1;
        if ((col>maxcol1) && (i!=currentnumfiles))
        {
          row += 1;
          col=1;
        }
      }
      maxrow=row;
      /* Rewrite the blank entries in the last row */
      if ((maxcol1*maxrow)>currentnumfiles)
      {
        for( i=1; i <= ((maxcol1*maxrow)-currentnumfiles); i ++)
        {
          row=maxrow;
          col=(maxcol1-((maxcol1*maxrow)-currentnumfiles))+i;
          gotoxy(x1+1+(col-1)*14,y1+row);
          cout << "              ";
        }
      }
      /* Rewrite the blank entries in the lines below the last entry */
      if ((maxrow<maxrowdisplay1) && fixedvertsize1)
      {
        for( i=maxrow+1; i <= maxrowdisplay1; i ++)
        {
          gotoxy(x1+1,y1+i);
          for( j=1; j <= maxcol1; j ++) cout << "              ";
          cout << endl;
        }
      }
      /* Place highlight on first file */
      row=oldrow;
      col=oldcol;
      highlight2(page_, oldrow, oldcol, x1, y1, filename, row, col);
      /* Move cursor to required position */
      do {
        changepage=false;
        oldrow=row;
        oldcol=col;
        specialkey=false;
        do {
          character=toupper(readkey());
        } while (!(set::of('\0','\15','\33', eos).has(character)));
        if (character=='\0')
        {
          specialkey=true;
          character=toupper(readkey());
          switch (character) {
            case '\110' : row=row-1; break;   /* Up arrow */
            case '\113' : col=col-1; break;   /* Left arrow */
            case '\115' : col=col+1; break;   /* Right arrow */
            case '\120' : row=row+1; break;   /* Down arrow */
            case '\111' : {                  /* Page up */
                    if (page_>1)
                    {
                      changepage=true;
                      page_=page_-1;
                      oldrow=row;
                      oldcol=col;
                    }
                    else row=1;
                  }
                  break;
            case '\121' : {                  /* Page down */
                    if (page_==numpages)
                    {
                      numcollastrow=(numfileslastpage % maxcol1);
                      if ((numcollastrow)==0)  numcollastrow=maxcol1;
                      numrowslastpage=((numfileslastpage-numcollastrow) / maxcol1)+1;
                      row=numrowslastpage;
                      if (((row-1)*maxcol1+col)>numfileslastpage)  row=row-1;
                    }
                    else
                    {
                      changepage=true;
                      page_=page_+1;
                      oldrow=row;
                      oldcol=col;
                      if (page_==numpages)
                      {
                        numcollastrow=(numfileslastpage % maxcol1);
                        if ((numcollastrow)==0)  numcollastrow=maxcol1;
                        numrowslastpage=((numfileslastpage-numcollastrow) / maxcol1)+1;
                        if (((row-1)*maxcol1+col)>numfileslastpage)
                        {
                          oldcol=numcollastrow;
                          oldrow=numrowslastpage;
                        }
                      }
                    }
                  }
                  break;
            case '\107' : {                   /* Home */
                    if (page_==1)
                    {
                      row=1;
                      col=1;
                    }
                    else
                    {
                      changepage=true;
                      page_=1;
                      oldcol=1;
                      oldrow=1;
                    }
                  }
                  break;
            case '\117' : {                   /* End */
                    if (page_==numpages)
                    {
                      row=maxrow;
                      col=(maxcol1-((maxcol1*maxrow)-currentnumfiles));
                    }
                    else
                    {
                      changepage=true;
                      page_=numpages;
                      numcollastrow=(numfileslastpage % maxcol1);
                      if ((numcollastrow)==0)  numcollastrow=maxcol1;
                      numrowslastpage=((numfileslastpage-numcollastrow) / maxcol1)+1;
                      oldcol=numcollastrow;
                      oldrow=numrowslastpage;
                    }
                  }
                  break;
          }    /* case */
        }
        if (row>maxrow)               /* scolling past the bottom */
        {
          if (page_<numpages)
          {
            changepage=true;
            page_=page_+1;
            oldrow=1;
            oldcol=col;
            if (page_==numpages)
            {
              numcollastrow=(numfileslastpage % maxcol1);
              if ((numcollastrow)==0)  numcollastrow=maxcol1;
              numrowslastpage=((numfileslastpage-numcollastrow) / maxcol1)+1;
              if ((col)>numfileslastpage)
              {
                oldcol=numcollastrow;
                oldrow=numrowslastpage;
              }
            }
          }
          else row=row-1;
        }
        if (row<1)                    /* scolling past the top */
        {
          if (page_==1)  row=1;
          else
          {
            changepage=true;
            page_=page_-1;
            oldrow=maxrowdisplay1;
            oldcol=col;
          }
        }
        if (col>maxcol1)              /* scolling past the right */
        {
          row=row+1;
          col=1;
          if (row>maxrow)
          {
            if (page_==numpages)
            {
              col=maxcol1;
              row=row-1;
            }
            else
            {
              changepage=true;
              page_=page_+1;
              oldrow=1;
              oldcol=1;
            }
          }
        }
        if (col<1)                    /* scolling past the left */
        {
          row=row-1;
          col=maxcol1;
          if (row<1)
          {
            if (page_==1)
            {
              row=1;
              col=1;
            }
            else
            {
              changepage=true;
              page_=page_-1;
              oldrow=maxrowdisplay1;
              oldcol=maxcol1;
            }
          }
        }
        if ((row==maxrow) && ((maxcol1*(row-1)+col)>currentnumfiles))
        {
          if (page_==numpages)
          {
            if (character=='\120')  row=row-1;
            if (character=='\115')  col=col-1;
          }
          else              /* Page down */
          {
            changepage=true;
            page_=page_+1;
            oldrow=1;
            oldcol=col;
          }
        }
        if (changepage)
        {
          textattr=normalattr;
          clrscr();
        }
        else highlight2(page_, oldrow, oldcol, x1, y1, filename, row, col);
      } while (!((set::of('\15','\33', eos).has(character)) || changepage));
    } while (!(! changepage));
    if (character=='\33')  chosen="";
    else
    {
      chosen=filepage[page_][(maxcol1*(row-1)+col)];
      if (chosen.substr(0,(chosen).length())==""+'\\')     /* Change directory */
      {
        if (chosen.substr(0,1)==""+'.')
        {
          do {
            dir.erase(0, (dir).length());
          } while (!(dir.substr(0, (dir).length())==""+'\\'));
        }
        else dir=dir+chosen;
        textattr=normalattr;
        clrscr();
      }
      if (chosen.substr(0,(chosen).length())==""+']')      /* Change drive */
      {
        dir=chosen.substr(1,2)+'\\';
        chosen='\\';
      }
    }
  }
} while (!(chosen.substr(0,(chosen).length())!=""+'\\'));
textattr=normalattr;
clrscr();
} /* of procedure CHOOSEFILE */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to display a entry message box on the screen */
void entrybox(int numentr,int xpos,int ypos,int excess,strarr2 item,string title)
{
//int currentattr,width,height,i,x,y,maxlength;

/* excess=0 --> for comments */
//if (excess==0)  currentattr=firstcharattr; else currentattr=menuattr;
//textattr=currentattr;
//cursoroff();
/* Set up a framed window */
//maxlength=0;
//for( i=1; i <= numentr; i ++)
//  if ((item[i]).length()>maxlength)  maxlength=(item[i]).length();
//if (maxlength<((item[numentr]).length()+excess))  maxlength=(item[numentr]).length()+excess;
//if (xpos==0)  x=(((lo(windmax)+lo(windmin))-maxlength-2) / 2); else x=xpos;
//y=ypos;
//width=maxlength+4;
//height=numentr+2;ar
if (title!="")  title=string(" ")+title+' ';
cout << title;

}  /* of procedure ENTRYBOX */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to read a real value in an entry box */
/* Codes : 0--> no limits,
          1--> only lower limit,
          2--> only upper limit
          3--> upper and lower limit,
          4--> single entry for both entries */
double realprompt(double default_,double min,double max,int code,int xpos,int ypos,strarr2 item)
{
    cout << "Inside ***realprompt functiom***" << endl;
strarr2 item2;
double tempvalue;
char dummy;
bool test0,test1,test2,test3,test4;

double realprompt_result;
if ((code>4) || (code<0))  code=0;
do {
  entrybox(1,xpos,ypos,12,item,"Enter data at cursor");

  tempvalue=readreal(default_);
  /* Display error messages if required */
  if (code!=0)
  {
    switch (code) {
    case 1:case 4 : {
            if ((tempvalue<min) || ((code==4) && (tempvalue>max)))
            {
              sound(500);
              item2[1]=item[2];
              entrybox(1,xpos,ypos,0,item2,"Press any key");
              delay(200);
              dummy=readkey();
            }
          }
          break;
      case 2 : {
            if (tempvalue>max)
            {
              sound(200);
              item2[1]=item[3];
              entrybox(1,xpos,ypos,0,item2,"Press any key");
              delay(200);
              dummy=readkey();
            }
          }
          break;
      case 3 : {
            if ((tempvalue<min) || (tempvalue>max))
            {
              sound(200);
              item2[1]=item[2];
              item2[2]=item[3];
              entrybox(2,xpos,ypos-1,0,item2,"Press any key");
              delay(200);
              dummy=readkey();
            }
          }
          break;
    }    /* case */
  }    /* if */
  test0=(code==0);
  test1=((code==1) && (tempvalue>=min));
  test2=((code==2) && (tempvalue<=max));
  test3=((code==3) && (tempvalue>=min) && (tempvalue<=max));
  test4=((code==4) && (tempvalue>=min) && (tempvalue<=max));
} while (!(test0 || test1 || test2 || test3 || test4));
realprompt_result=tempvalue;
return realprompt_result;
}  /* of function RealPROMPT */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to read a int value in an entry box */
/* Codes : 0--> no limits,
          1--> only lower limit,
          2--> only upper limit
          3--> upper and lower limit,
          4--> single entry for both entries */
int intprompt(int default_,int min,int max,int code,int xpos,int ypos,strarr2 item)
{
char dummy;
strarr2 item2;
int tempvalue;
bool test0,test1,test2,test3,test4;

int intprompt_result;
if ((code>4) || (code<0))  code=0;
do {
  entrybox(1,xpos,ypos,12,item,"Enter data at cursor");
  tempvalue=readint(default_);
  /* Display error messages if required */
  if (code!=0)
  {
    switch (code) {
    case 1:case 4 : {
            if ((tempvalue<min) || ((code==4) && (tempvalue>max)))
            {
              item2[1]=item[2];
              entrybox(1,xpos,ypos,0,item2,"Press any key");
              delay(200);
              dummy=readkey();
            }
          }
          break;
      case 2 : {
            if (tempvalue>max)
            {
              sound(200);
              item2[1]=item[3];
              entrybox(1,xpos,ypos,0,item2,"Press any key");
              delay(200);
              dummy=readkey();
            }
          }
          break;
      case 3 : {
            if ((tempvalue<min) || (tempvalue>max))
            {
              sound(200);
              item2[1]=item[2];
              item2[2]=item[3];
              entrybox(2,xpos,ypos-1,0,item2,"Press any key");
              delay(200);
              dummy=readkey();
            }
          }
          break;
    }    /* case */
  }    /* if */
  test0=(code==0);
  test1=((code==1) && (tempvalue>=min));
  test2=((code==2) && (tempvalue<=max));
  test3=((code==3) && (tempvalue>=min) && (tempvalue<=max));
  test4=((code==4) && (tempvalue>=min) && (tempvalue<=max));
} while (!(test0 || test1 || test2 || test3 || test4));
intprompt_result=tempvalue;
return intprompt_result;
}  /* of function IntPROMPT */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to read a file name in an entry box */
string filenameprompt(string default_,int xpos,int ypos,string question)
{
strarr2 item;
string tempvalue;

string filenameprompt_result;
item[1]=question;
entrybox(1,xpos,ypos,12,item,"Enter data at cursor");
tempvalue=readfilename(default_);
filenameprompt_result=tempvalue;
return filenameprompt_result;
}  /* of function FilenamePROMPT */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to read a string in an entry box */
string stringprompt(string default_,int xpos,int ypos,string question)
{
strarr2 item;
string tempvalue;

string stringprompt_result;
item[1]=question;
entrybox(1,xpos,ypos,30,item,"Enter data at cursor");
cin >> tempvalue ;
if (tempvalue=="")  tempvalue=default_;
stringprompt_result=tempvalue;
return stringprompt_result;
}  /* of function StringPROMPT */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to read a single digit in an entry box */
int digitprompt(int default_,int min,int max,int xpos,int ypos,string question)
{
strarr2 item;
char tempvalue;
int i,code;

int digitprompt_result;
item[1]=question;
entrybox(1,xpos,ypos,12,item,"Enter data at cursor");
do {
  tempvalue=toupper(readkey());
  if (tempvalue=='\15')
    i=default_;
  else
    i = tempvalue;
} while (!(set::of(range(min,max), eos).has(i)));
digitprompt_result=i;
return digitprompt_result;
}  /* of function DigitPROMPT */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Function to read character prompt in an entry box */
char charprompt(char default_,string choices,int xpos,int ypos,string textstring)
{
int i;
strarr2 item;
char tempvalue;
bool validchar;
string test;

char charprompt_result;
item[1]=textstring;
entrybox(1,xpos,ypos,0,item,"Select option");
do {
  tempvalue=toupper(readkey());
  if (tempvalue=='\0')
  {
    tempvalue=toupper(readkey());
    tempvalue='\0';
  }
  if (tempvalue=='\15')  tempvalue=default_;
  validchar=false;
  for( i=1; i <= (choices).length(); i ++)
  {
    test=choices.substr(0,i);
    if (test==""+tempvalue)  validchar=true;
  }
} while (!validchar);
//closewindow();
charprompt_result=tempvalue;
return charprompt_result;
}  /* of function CharPROMPT */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to display a message in an entry box - remember to call CloseWindow */
//void displaymessage(int xpos,int ypos,string question,bool readkeyboard)
//{
//char dummy;
//strarr2 item;
//
//item[1]=question;
//entrybox(1,xpos,ypos,0,item,"");
//if (readkeyboard)
//{
//  dummy=readkey();
//  if (dummy=='\0')  dummy=readkey();
//}
//}  /* of procedure DisplayMESSAGE */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to display a message in an entry box */
//void displaymessage2(int xpos,int ypos,string question)
//{
//char dummy;
//strarr2 item;
//
//item[1]=question;
//entrybox(1,xpos,ypos,0,item,"Press any key");
//dummy=readkey();
//if (dummy=='\0')  dummy=readkey();
////closewindow();
//}  /* of procedure DisplayMESSAGE2 */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

   /* of unit HF_Util */
