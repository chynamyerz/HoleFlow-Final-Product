#include "ptoc.h"

                      /* Program to calculate the flow through a combustion */
                      /* chamber lining, the local gas temperatures and the */
                      /* heat transfer to and from the liner */
                      /* A.A. Dreyer */
                      /* 25/10/2000 */
/*$N+,E+*/              /*use 8087 if present*/
/*$M 65000,0,655360*/   /*increase stack size*/
#include "crt.h"
#include "hf_menu.h"
#include "HF_Util.h"
#include "Printer.h"
#include "Win.h"

//Added INCLUDE files
#include <fstream>
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <stdio.h>
#include <windows.h>

using namespace std;

double srterm1,srterm2,srterm3,aft,aref,dref,phipz,theta,combeta,ei_co,ei_hc,ei_h2,
combeta2,pf,phirz,phirp,phisz,phidz,uref,rhoaref;

const double pi = 3.1415926535897932385;

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to calculate air properties */
/* t in јC, P in Pa */
void airproperties(double t,double pressure,double& rho,double& cp,double& mu,double& k,double& pr)
{
  if (((t+273.15)>1500.0) || ((t+273.15)<-200.0))
  {
/*   writeln('Invalid air temperature');
    Halt(1); */
    cp=1006.5;
    mu=7.05e-5;
    k=0.157;
  }
  else
  {
    cp=1002.106+0.089329*t+2.332e-4*t*t-1.2831e-7*t*t*t;         /*J/kgK*/
    mu=1.733e-5+4.5543e-8*t-2.2106e-11*t*t+7.7219e-15*t*t*t;     /*kg/ms*/
    k=2.4186e-2+7.8957e-5*t-3.7873e-8*t*t+1.6685e-11*t*t*t;      /*W/mK*/
  }
  rho=pressure/(287.08*(273.15+t));                              /*kg/m^3*/
  pr=cp*mu/k;                                                    /*-*/
}    /* of procedure AirProperties */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to calculate the heat transfer per liner element - ANNULAR */
void evaluateelement2(double p0,double t0,double tgas,double dl,double dli,double dc,double dci,double lcomb,double dx,double mail,double maoloa,double maolia,
                           double localafr,double xcurrent,double xprevoa,double sprevoa,double mprevoa,double xprevia,double sprevia,double mprevia,
                           boolean filmcooledelementoa,boolean filmcooledelementia,
                               double& twli,double& twlo,double& twci,double& twco,double& etafc,double& twlii,double& twloi,double& twcii,double& twcoi,double& etafci)
{
  int i;
  double aflowliner,dhi,dhannulus,mua,rei,nuli,hali,ttr,ttl,qradli,qfcli,qwalll,
  ali,alo,aci,aco,pra,aflowannulus,reo,nu1o,ka,halo,qfclo,
  qradlo,nuci,haci,qfcci,qradci,qwallc,dqa,qncco,qradco,qlost,
  hradlo,tgaso,dqgas,cpa,tao,f12,f21,term1,term2,term3,term,q,eg,
  lb,l,rhoa,rhoai,cpai,muai,kai,prai,m,twad,res,rex,s,t,x,mailrem;

  mailrem=mail;
  t0=t0-273.15;        /* Stagnation temperature, јC */
  tgas=tgas-273.15;    /* Gas temperature, јC */
  q=1.0/localafr;      /* fuel/air ratio by mass, - */
  /* Perform heat transfer calculations from liner to outside */
  aflowliner=(pi/4.0)*(dl*dl-dli*dli);
  dhi=4.0*aflowliner/(pi*(dl+dli));
  aflowannulus=(pi/4.0)*(dc*dc-(dl+2.0*tl)*(dl+2.0*tl));
  dhannulus=4.0*aflowannulus/(pi*((dl+2.0*tl)+dc));
  ali=pi*dl*dx;
  alo=pi*(dl+2.0*tl)*dx;
  aci=pi*dc*dx;
  aco=pi*(dc+2.0*tc)*dx;
  /* Calculate the heat gain through the walls of the combustion chamber */
  ttr=tgas;
  ttl=tambient;
  do {
    twli=(ttr+ttl)/2.0;
    /* inner wall of combustion chamber */
    airproperties(t0,p0,rhoai,cpai,muai,kai,prai);
    airproperties(tgas,p0,rhoa,cpa,mua,ka,pra);
    if (filmcooledelementoa && filmcooling)
    {
      if (maoloa<=0.0)  maoloa=mdot/1000000.0;
      if (mail<=0.0)  mail=mdot/1000000.0;
      s=sprevoa;
      x=xcurrent-xprevoa;
      if (x<s)  x=s;
/*     if xCurrent>0.048 then
        writeln; */
      t=tl;    /* assumption */
      m=(mprevoa/(pi*dl*s))/(mail/(0.25*pi*dl*dl));
      rex=(mprevoa/(pi*dl*s))*x/muai;
      res=(mprevoa/(pi*dl*s))*s/muai;
      if (m<1.3)
      {
        etafc=1.10*p(m,0.65)*p(muai/mua,0.15)*p(x/s,-0.2)*p(t/s,-0.2);
        if (etafc>1.0)  etafc=1.0;
        nuli=0.069*p(res*x/s,0.7);
      }
      else
      {
        etafc=1.28*p(muai/mua,0.15)*p(x/s,-0.2)*p(t/s,-0.2);
        if (etafc>1.0)  etafc=1.0;
        nuli=0.10*p(res,0.8)*p(x/s,0.44);
      }
      twad=tgas-etafc*(tgas-t0);
      hali=nuli*kai/x;
      qfcli=hali*ali*(twad-twli);
    }
    else /*Uncooled liner*/
    {
      etafc=0.0;
      rei=(mail/((pi/4.0)*dl*dl))*dhi/mua;
      nuli=0.027*p(rei,0.8)*p(pra,0.3333);
      hali=nuli*(ka/dhi);
      qfcli=hali*ali*(tgas-twli);
    }
    /* Gas radiation to inner wall */
    if (simplified)
      qradli=eli*5.669e-8*ali*(p((tgas+273.15),4.0)-p((twli+273.15),4.0));
    else
    {
      lb=3.4*lcomb*0.25*pi*dl*dl/(pi*dl*lcomb);
      l=3.0*p((ch-5.2),0.75);
      eg=1.0-exp(-290.0*(p0/1000.0)*l*sqrt(q*lb)*p((tgas+273.15),-1.5));
      qradli=0.5*(1.0+eli)*5.669e-8*ali*eg*p(tgas+273.15,1.5)*(p((tgas+273.15),2.5)-p((twli+273.15),2.5));
    }
    qwalll=qfcli+qradli;
    dqgas=qwalll;
    tgaso=tgas-dqgas/(cpa*mail);
    twlo=twli-(qwalll/(0.5*(ali+alo)*kl/tl));
    /* Outer wall of the combustion chamber */
    airproperties(t0,p0,rhoa,cpa,mua,ka,pra);
    reo=(maoloa/aflowannulus)*dhannulus/mua;
    if (reo<=2300.0)
      nu1o=3.66;
    else
      nu1o=0.027*p(reo,0.8)*p(pra,0.3333);
    halo=nu1o*(ka/dhannulus);
    qfclo=halo*alo*(twlo-t0);
    qradlo=qwalll-qfclo;
    twci=t0+10.0;
    for( i=1; i <= 20; i ++)
    {
      /*Holman*/
      f21=0.35;     /* see figure 8-15, p.322 - Heat transfer (J.P. Holman) */
      f12=f21*(aci/alo);
      term1=(1.0-elo)/elo;
      term2=1.0/f12;
      term3=(1.0-eci)*alo/(eci*aci);
      term=1.0/(term1+term2+term3);
      /*Lefebvre*/
      term=elo*eci/(eci+elo*((1.0-eci)*dl/dc));
      hradlo=term*5.669e-8*(p((twlo+273.15),2.0)+p((twci+273.15),2.0))*((twlo+273.15)+(twci+273.15));
      twci=twlo-qradlo/(hradlo*alo);
/*     writeln(TwCi:8:2);  */
      if (twci<(tambient+0.00001))  twci=tambient+0.00001;
    }
/*   QradLo:=eLo*5.669E-8*ALo*(P((TwLo+273.15),4.0)-P((TwCi+273.15),4.0)); */
    /* Inner wall of supply line */
    airproperties(t0,p0,rhoa,cpa,mua,ka,pra);
    reo=(maoloa/aflowannulus)*dhannulus/mua;
    if (reo<=2300.0)
      nuci=3.66;
    else
      nuci=(0.027*p(reo,0.8)*p(pra,0.3333));
    haci=nuci*(ka/dhannulus);
    qfcci=haci*aci*(t0-twci);
    dqa=qfclo-qfcci;    /* heating of the air flowing in the annulus */
    if (maoloa<=0.0)
      tao=t0;
    else
      tao=t0+dqa/(cpa*maoloa);
    qradci=qradlo;
    qwallc=qradci+qfcci;
/*   if (QwallC<0.0) then QwallC:=0.0; */
    /* outer wall of supply line */
    twco=twci-(qwallc/(0.5*(aci+aco)*kc/tc));
    if (twco<(tambient+0.00001))  twco=tambient+0.00001;
    qncco=aco*1.24*p((twco-tambient),1.3333);
    qradco=eco*5.669e-8*aco*(p((twco+273.15),4.0)-p((tambient+273.15),4.0));
    qlost=qncco+qradco;
    if (qwallc<qlost)  ttr=twli; else ttl=twli;
/*   writeln(QwallL:8:2,QwallC:8:2,TwLi:8:2,TwLo:8:2,TwCi:8:2,TwCo:8:2); */
/*   dummy:=ReadKey; */
  } while (!(abs(100.0*(qwallc-qlost)/qwalll)<0.05));
  /* Convert temperatures to Kelvin */
  twli=twli+273.15;  /* Convert to Kelvin */
  twlo=twlo+273.15;  /* Convert to Kelvin */
  twci=twci+273.15;  /* Convert to Kelvin */
  twco=twco+273.15;  /* Convert to Kelvin */
  /* Perform heat transfer calculations from liner to centreline */
  mail=mailrem;
  aflowliner=(pi/4.0)*(dl*dl-dli*dli);
  dhi=4.0*aflowliner/(pi*(dl+dli));
  aflowannulus=(pi/4.0)*((dli-2.0*tl)*(dli-2.0*tl)-dci*dci);
  dhannulus=4.0*aflowannulus/(pi*((dli-2.0*tl)+dci));
  ali=pi*dli*dx;
  alo=pi*(dli-2.0*tl)*dx;
  aci=pi*dci*dx;
  aco=pi*(dci-2.0*tc)*dx;
  /* Calculate the heat gain through the walls of the combustion chamber */
  ttr=tgas;
  ttl=tambient;
  do {
    twlii=(ttr+ttl)/2.0;
    /* inner wall of combustion chamber */
    airproperties(t0,p0,rhoai,cpai,muai,kai,prai);
    airproperties(tgas,p0,rhoa,cpa,mua,ka,pra);
    if (filmcooledelementia && filmcooling)
    {
      if (maolia<=0.0)  maolia=mdot/1000000.0;
      if (mail<=0.0)  mail=mdot/1000000.0;
      s=sprevia;
      x=xcurrent-xprevia;
      if (x<s)  x=s;
/*     if xCurrent>0.048 then
        writeln; */
      t=tl;    /* assumption */
      m=(mprevia/(pi*dli*s))/(mail/(0.25*pi*dli*dli));
      rex=(mprevia/(pi*dli*s))*x/muai;
      res=(mprevia/(pi*dli*s))*s/muai;
      if (m<1.3)
      {
        etafci=1.10*p(m,0.65)*p(muai/mua,0.15)*p(x/s,-0.2)*p(t/s,-0.2);
        if (etafci>1.0)  etafci=1.0;
        nuli=0.069*p(res*x/s,0.7);
      }
      else
      {
        etafci=1.28*p(muai/mua,0.15)*p(x/s,-0.2)*p(t/s,-0.2);
        if (etafci>1.0)  etafci=1.0;
        nuli=0.10*p(res,0.8)*p(x/s,0.44);
      }
      twad=tgas-etafci*(tgas-t0);
      hali=nuli*kai/x;
      qfcli=hali*ali*(twad-twlii);
    }
    else /*Uncooled liner*/
    {
      etafci=0.0;
      rei=(mail/((pi/4.0)*dli*dli))*dhi/mua;
      nuli=0.027*p(rei,0.8)*p(pra,0.3333);
      hali=nuli*(ka/dhi);
      qfcli=hali*ali*(tgas-twlii);
    }
    /* Gas radiation to inner wall */
    if (simplified)
      qradli=eli*5.669e-8*ali*(p((tgas+273.15),4.0)-p((twlii+273.15),4.0));
    else
    {
      lb=3.4*lcomb*0.25*pi*dli*dli/(pi*dli*lcomb);
      l=3.0*p((ch-5.2),0.75);
      eg=1.0-exp(-290.0*(p0/1000.0)*l*sqrt(q*lb)*p((tgas+273.15),-1.5));
      qradli=0.5*(1.0+eli)*5.669e-8*ali*eg*p(tgas+273.15,1.5)*(p((tgas+273.15),2.5)-p((twlii+273.15),2.5));
    }
    qwalll=qfcli+qradli;
    dqgas=qwalll;
    tgaso=tgas-dqgas/(cpa*mail);
    twloi=twlii-(qwalll/(0.5*(ali+alo)*kl/tl));
    /* Outer wall of the combustion chamber */
    airproperties(t0,p0,rhoa,cpa,mua,ka,pra);
    reo=(maoloa/aflowannulus)*dhannulus/mua;
    if (reo<=2300.0)
      nu1o=3.66;
    else
      nu1o=0.027*p(reo,0.8)*p(pra,0.3333);
    halo=nu1o*(ka/dhannulus);
    qfclo=halo*alo*(twloi-t0);
    qradlo=qwalll-qfclo;
    twcii=t0+10.0;
    for( i=1; i <= 20; i ++)
    {
      /*Holman*/
      f21=0.35;     /* see figure 8-15, p.322 - Heat transfer (J.P. Holman) */
      f12=f21*(aci/alo);
      term1=(1.0-elo)/elo;
      term2=1.0/f12;
      term3=(1.0-eci)*alo/(eci*aci);
      term=1.0/(term1+term2+term3);
      /*Lefebvre*/
      term=elo*eci/(eci+elo*((1.0-eci)*dl/dc));
      hradlo=term*5.669e-8*(p((twloi+273.15),2.0)+p((twcii+273.15),2.0))*((twloi+273.15)+(twcii+273.15));
      twcii=twloi-qradlo/(hradlo*alo);
/*     writeln(TwCiI:8:2);  */
      if (twcii<(tambient+0.00001))  twcii=tambient+0.00001;
    }
    /* Inner wall of supply line */
    airproperties(t0,p0,rhoa,cpa,mua,ka,pra);
    reo=(maolia/aflowannulus)*dhannulus/mua;
    if (reo<=2300.0)
      nuci=3.66;
    else
      nuci=(0.027*p(reo,0.8)*p(pra,0.3333));
    haci=nuci*(ka/dhannulus);
    qfcci=haci*aci*(t0-twcii);
    dqa=qfclo-qfcci;    /* heating of the air flowing in the annulus */
    if (maolia<=0.0)
      tao=t0;
    else
      tao=t0+dqa/(cpa*maolia);
    qradci=qradlo;
    qwallc=qradci+qfcci;
/*   if (QwallC<0.0) then QwallC:=0.0; */
    /* outer wall of supply line */
    twcoi=twcii-(qwallc/(0.5*(aci+aco)*kc/tc));
    if (twcoi<(tambient+0.00001))  twcoi=tambient+0.00001;
    qncco=aco*1.24*p((twcoi-tambient),1.3333);
    qradco=eco*5.669e-8*aco*(p((twcoi+273.15),4.0)-p((tambient+273.15),4.0));
    qlost=qncco+qradco;
    if (qwallc<qlost)  ttr=twlii; else ttl=twlii;
/*   writeln(QwallL:8:2,QwallC:8:2,TwLiI:8:2,TwLoI:8:2,TwCiI:8:2,TwCoI:8:2); */
/*   dummy:=ReadKey; */
  } while (!(abs(100.0*(qwallc-qlost)/qwalll)<0.05));
  /* Convert temperatures to Kelvin */
  twlii=twlii+273.15;  /* Convert to Kelvin */
  twloi=twloi+273.15;  /* Convert to Kelvin */
  twcii=twcii+273.15;  /* Convert to Kelvin */
  twcoi=twcoi+273.15;  /* Convert to Kelvin */
}    /* of procedure EvaluateElement2 */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to calculate the heat transfer per liner element - CAN + CAN-ANNULAR */
void evaluateelement13(double p0,double t0,double tgas,double dl,double dc,double lcomb,double dx,double mai,double mao,double localafr,
                            double xcurrent,double xprev,double sprev,double mprev,
                            boolean filmcooledelement,
                                double& twli,double& twlo,double& twci,double& twco,double& etafc)
{
  int i;
  double dhi,dhannulus,mua,rei,nuli,hali,ttr,ttl,qradli,qfcli,qwalll,
  ali,alo,aci,aco,pra,aflowannulus,reo,nu1o,ka,halo,qfclo,
  qradlo,nuci,haci,qfcci,qradci,qwallc,dqa,qncco,qradco,qlost,
  hradlo,tgaso,dqgas,cpa,tao,f12,f21,term1,term2,term3,term,q,eg,
  lb,l,rhoa,rhoai,cpai,muai,kai,prai,m,twad,res,rex,s,t,x;

  t0=t0-273.15;        /* Stagnation temperature, јC */
  tgas=tgas-273.15;    /* Gas temperature, јC */
  q=1.0/localafr;      /* fuel/air ratio by mass, - */
  aflowannulus=(pi/4.0)*(dc*dc-(dl+2.0*tl)*(dl+2.0*tl));
  dhi=dl;
  dhannulus=4.0*aflowannulus/(pi*(dl+2.0*tl+dc));
  ali=pi*dl*dx;
  alo=pi*(dl+2.0*tl)*dx;
  aci=pi*dc*dx;
  aco=pi*(dc+2.0*tc)*dx;
  /* Calculate the heat gain through the walls of the combustion chamber */
  ttr=tgas;
  ttl=tambient;
  do {
    twli=(ttr+ttl)/2.0;
    /* inner wall of combustion chamber */
    airproperties(t0,p0,rhoai,cpai,muai,kai,prai);
    airproperties(tgas,p0,rhoa,cpa,mua,ka,pra);
    if (filmcooledelement && filmcooling)
    {
      if (mao<=0.0)  mao=mdot/1000000.0;
      if (mai<=0.0)  mai=mdot/1000000.0;
      s=sprev;
      x=xcurrent-xprev;
      if (x<s)  x=s;
/*     if xCurrent>0.048 then
        writeln; */
/*     t:=0.4*s; */
      t=tl;    /* assumption */
      m=(mprev/(pi*dl*s))/(mai/(0.25*pi*dl*dl));
      rex=(mprev/(pi*dl*s))*x/muai;
      res=(mprev/(pi*dl*s))*s/muai;
      if (m<1.3)
      {
        etafc=1.10*p(m,0.65)*p(muai/mua,0.15)*p(x/s,-0.2)*p(t/s,-0.2);
        if (etafc>1.0)  etafc=1.0;
        nuli=0.069*p(res*x/s,0.7);
      }
      else
      {
        etafc=1.28*p(muai/mua,0.15)*p(x/s,-0.2)*p(t/s,-0.2);
        if (etafc>1.0)  etafc=1.0;
        nuli=0.10*p(res,0.8)*p(x/s,0.44);
      }
      twad=tgas-etafc*(tgas-t0);
      hali=nuli*kai/x;
      qfcli=hali*ali*(twad-twli);
    }
    else /*Uncooled liner*/
    {
      etafc=0.0;
      rei=(mai/((pi/4.0)*dl*dl))*dhi/mua;
      nuli=0.027*p(rei,0.8)*p(pra,0.3333);
      hali=nuli*(ka/dhi);
      qfcli=hali*ali*(tgas-twli);
    }
    /* Gas radiation to inner wall */
    if (simplified)
      qradli=eli*5.669e-8*ali*(p((tgas+273.15),4.0)-p((twli+273.15),4.0));
    else
    {
      lb=3.4*lcomb*0.25*pi*dl*dl/(pi*dl*lcomb);
      l=3.0*p((ch-5.2),0.75);
      eg=1.0-exp(-290.0*(p0/1000.0)*l*sqrt(q*lb)*p((tgas+273.15),-1.5));
      qradli=0.5*(1.0+eli)*5.669e-8*ali*eg*p(tgas+273.15,1.5)*(p((tgas+273.15),2.5)-p((twli+273.15),2.5));
    }
    qwalll=qfcli+qradli;
    dqgas=qwalll;
    tgaso=tgas-dqgas/(cpa*mai);
    twlo=twli-(qwalll/(0.5*(ali+alo)*kl/tl));
    /* Outer wall of the combustion chamber */
    airproperties(t0,p0,rhoa,cpa,mua,ka,pra);
    reo=(mao/aflowannulus)*dhannulus/mua;
    if (reo<=2300.0)
      nu1o=3.66;
    else
      nu1o=0.027*p(reo,0.8)*p(pra,0.3333);
    halo=nu1o*(ka/dhannulus);
    qfclo=halo*alo*(twlo-t0);
    qradlo=qwalll-qfclo;
    twci=t0+10.0;
    for( i=1; i <= 20; i ++)
    {
      /*Holman*/
      f21=0.35;     /* see figure 8-15, p.322 - Heat transfer (J.P. Holman) */
      f12=f21*(aci/alo);
      term1=(1.0-elo)/elo;
      term2=1.0/f12;
      term3=(1.0-eci)*alo/(eci*aci);
      term=1.0/(term1+term2+term3);
      /*Lefebvre*/
      term=elo*eci/(eci+elo*((1.0-eci)*dl/dc));
      hradlo=term*5.669e-8*(p((twlo+273.15),2.0)+p((twci+273.15),2.0))*((twlo+273.15)+(twci+273.15));
      twci=twlo-qradlo/(hradlo*alo);
/*     writeln(TwCi:8:2);  */
      if (twci<(tambient+0.00001))  twci=tambient+0.00001;
    }
/*   QradLo:=eLo*5.669E-8*ALo*(P((TwLo+273.15),4.0)-P((TwCi+273.15),4.0)); */
    /* Inner wall of supply line */
    airproperties(t0,p0,rhoa,cpa,mua,ka,pra);
    reo=(mao/aflowannulus)*dhannulus/mua;
    if (reo<=2300.0)
      nuci=3.66;
    else
      nuci=(0.027*p(reo,0.8)*p(pra,0.3333));
    haci=nuci*(ka/dhannulus);
    qfcci=haci*aci*(t0-twci);
    dqa=qfclo-qfcci;    /* heating of the air flowing in the annulus */
    if (mao<=0.0)
      tao=t0;
    else
      tao=t0+dqa/(cpa*mao);
    qradci=qradlo;
    qwallc=qradci+qfcci;
/*   if (QwallC<0.0) then QwallC:=0.0; */
    /* outer wall of supply line */
    twco=twci-(qwallc/(0.5*(aci+aco)*kc/tc));
    if (twco<(tambient+0.00001))  twco=tambient+0.00001;
    qncco=aco*1.24*p((twco-tambient),1.3333);
    qradco=eco*5.669e-8*aco*(p((twco+273.15),4.0)-p((tambient+273.15),4.0));
    qlost=qncco+qradco;
    if (qwallc<qlost)  ttr=twli; else ttl=twli;
/*   writeln(QwallL:8:2,QwallC:8:2,TwLi:8:2,TwLo:8:2,TwCi:8:2,TwCo:8:2); */
/*   dummy:=ReadKey; */
  } while (!(abs(100.0*(qwallc-qlost)/qwalll)<0.05));
/* writeln(QradLi:8:2,QfcLi:8:2,(QradLi/ALi):10:1);
  writeln(QradLo:8:2,QfcLo:8:2,(QradLo/ALo):10:1);
  writeln(QradCi:8:2,QfcCi:8:2);
  writeln(QradCo:8:2,QncCo:8:2);
  writeln(Tgas:8:2,Tgaso:8:2);
  writeln(T0:8:2,Tao:8:2);
  writeln((TwLi+273.15):8:2,(TwLo+273.15):8:2,(TwCi+273.15):8:2,(TwCo+273.15):8:2);
  repeat until KeyPressed; */
  twli=twli+273.15;  /* Convert to Kelvin */
  twlo=twlo+273.15;  /* Convert to Kelvin */
  twci=twci+273.15;  /* Convert to Kelvin */
  twco=twco+273.15;  /* Convert to Kelvin */
}    /* of procedure EvaluateElement13 */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to calculate the flow distribution through the combustion chamber */
void calculate()
{
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
  fstream linert;
  //char dummy = '';
  int i,j,k,irem,jrem,currentset,counter,counter2,counter3,zone,nelements;
  double min,dpl,dpr,dp,ma,msum,aflow,vflow,q,pstat,t,ahole,a,b,cd,
  mhole,mu,kk,splitl,splitr,msumi,msumo,mai,mao,trzi,trzo,
  etarz,trpi,trpo,etarp,tszi,tszo,etasz,tdzi,tdzo,etadz,mf,afrrz,
  afrrp,afrsz,afrdz,xprev,sprev,mprev,y,zeta,zeta300,dstar,term,
  term1,term2,dft,n,volumepz,lzone,tin,tout,x,dx,tgas,x0,dc,dl,
  localafr,x1,x2,dc1,dc2,dl1,dl2,twli,twlo,twci,twco,volumesz,
  mail,maol,aaa,maoloa,maolia,dci1,dci2,dli1,dli2,xprevoa,
  sprevoa,mprevoa,xprevia,sprevia,mprevia,dci,dli,twlii,twloi,
  twcii,twcoi,etafc,etafci,dlm,dlim,dx1,rhoa;
  boolean test1,test2,test3,test4,currentsetnottaken,errorflag,noinnerholes,
  noouterholes,filmcooledelement,filmcooledelementoa,filmcooledelementia;
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
  heading(maintitle+" - RESULTS",true);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
  cout << endl;
  /* Sort hole sets by increasing x-value */
  for( i=1; i <= maxnumsets; i ++) holesetorder[i]=0;
  for( j=1; j <= nholesets; j ++)
  {
    min=1000000.0;
    for( i=1; i <= nholesets; i ++)
    {
      currentsetnottaken=true;
      for( k=1; k <= j; k ++)
        if (holesetorder[k]==i)  currentsetnottaken=false;
      if (currentsetnottaken)
      {
        if (xhole[i]<min)
        {
          min=xhole[i];
          irem=i;
        }
      }
    }    /*for i*/
    holesetorder[j]=irem;
  }    /*for j*/
  /* Perform iterative calculations to find air flow per hole set */
  if (cctype==2)          /*ANNULAR*/
  {
    noinnerholes=true;
    noouterholes=true;
    for( i=1; i <= nholesets; i ++)
    {
      if (holeposition[i]==2)  noinnerholes=false;
      if (holeposition[i]==3)  noouterholes=false;
    }
    /* Assume flow split */
    splitl=0.0;
    splitr=1.0;
    counter2=0;
    counter3=0;
    do {
      split=(splitl+splitr)/2.0;
      if (noinnerholes)  split=0.0;
      if (noouterholes)  split=1.0;
      counter2 += 1;
      /* Assume pressure drop through combustion chamber */
      dpl=0.0;
      dpr=p0;
      exitflag=false;
      counter=0;
      do {
        counter += 1;
        dp=(dpl+dpr)/2.0;
        pstatcan=p0-dp;
        msumi=0.0;
        msumo=0.0;
        mai=mdot*split;         /* Inner */
        mao=mdot*(1.0-split);   /* Outer */
        errorflag=false;
        maod=0.0;
        /* Loop to evaluate each hole set */
        i=0;
        do {
          i += 1;
          counter3 += 1;
          currentset=holesetorder[i];
          switch (holeposition[currentset]) {
            case 1 : {  /* On-Dome */
                  ma=mai+mao;
                  aflow=(pi/4.0)*(sqr(dcasing[currentset])-sqr(dcasingi[currentset]));
                }
                break;
            case 2 : {  /* Inner */
                  ma=mai;
                  aflow=(pi/4.0)*(sqr(dlineri[currentset])-sqr(dcasingi[currentset]));
                }
                break;
            case 3 : {  /* Outer */
                  ma=mao;
                  aflow=(pi/4.0)*(sqr(dcasing[currentset])-sqr(dliner[currentset]));
                }
                break;
          }
          /* Calculate air density */
          rhoa=p0/(287.08*t0);
          for( j=1; j <= 5; j ++)
          {
            /* Calculate annulus velocity */
            vflow=ma/(aflow*rhoa);
            /* Calculate the static pressure in annulus */
            q=0.5*rhoa*vflow*vflow;
            pstat=p0-q;
            t=t0-(vflow*vflow/(2.0*cp));
            rhoa=pstat/(287.08*t);
          }
          if (holetypes[holetype[currentset]-1]=="Wiggle strip")
            ahole=slotdepth[currentset]*dhole[currentset];
          else
            ahole=(pi/4.0)*sqr(dhole[currentset]);
          /* Determine CD if not specified */
          switch (cdspecification[currentset]) {
            case 1 : holecd[currentset]=holecd[currentset]; break; /*user input*/
            case 2 : { /*built-in functions*/
                  a=ahole*nholes[currentset]/aflow;
                  cd=1.0;
                  for( j=1; j <= 5; j ++)
                  {
                    if ((pstat-pstatcan)<0.0)
                      mhole=-ahole*cd*sqrt(2.0*rhoa*(pstatcan-pstat));
                    else
                      mhole=ahole*cd*sqrt(2.0*rhoa*(pstat-pstatcan));
                    if (ma<0.0)
                      b=1.0E20;
                    else
                      b=abs(mhole)*nholes[currentset]/ma;
                    mu=(b/a);
      /*        if counter3=98 then
                  write;          */
                    switch (holetype[currentset]) {
                      case 3 : {  /* Plain hole */
                            kk=1.0+0.64*(2.0*mu*mu+sqrt(4.0*sqr(sqr(mu))+1.56*mu*mu*(4.0*b-b*b)));
                            cd=(kk-1.0)/(0.8*sqrt(4.0*kk*kk-kk*sqr(2.0-b)));
                          }
                          break;
                      case 4 : {  /* Plunged hole */
                            kk=1.0+0.36*(2.0*mu*mu+sqrt(4.0*sqr(sqr(mu))+2.77*(4.0*b-b*b)));
                            cd=(kk-1.0)/(0.6*sqrt(4.0*kk*kk-kk*sqr(2.0-b)));
                          }
                          break;
                    }    /*case*/
                  }    /*for j*/
                  holecd[currentset]=cd;
                }
                break;
            case 3 : { /*user correlation*/
                  term=(p0-pstatcan)/q;
                  holecd[currentset]=acd[currentset]+
                                      bcd[currentset]*term+
                                      bcd[currentset]*term*term+
                                      bcd[currentset]*term*term*term;
                }
                break;
          }    /*case*/
          if (holecd[currentset]<=0.0)  holecd[currentset]=0.1;
          if (holecd[currentset]>=1.0)  holecd[currentset]=1.0;
          /* Calculate the flow angles */
          holeflowangle[currentset]=0.0;
          a=ahole*nholes[currentset]/aflow;
          if ((pstat-pstatcan)<0.0)
            mhole=-ahole*holecd[currentset]*sqrt(2.0*rhoa*(pstatcan-pstat));
          else
            mhole=ahole*holecd[currentset]*sqrt(2.0*rhoa*(pstat-pstatcan));
          if (ma<0.0)
            b=1.0E20;
          else
            b=abs(mhole)*nholes[currentset]/ma;
          mu=abs(b/a);
          switch (holetype[currentset]) {
            case 3 : {   /* Plain hole */
                  kk=1.0+0.64*(2.0*mu*mu+sqrt(4.0*sqr(sqr(mu))+1.56*mu*mu*(4.0*b-b*b)));
                  holeflowangle[currentset]=(180.0/pi)*asin((1.0/(1.6*holecd[currentset]))*((kk-1.0)/kk));
                }
                break;
            case 4 : {   /* Plunged hole */
                  kk=1.0+0.36*(2.0*mu*mu+sqrt(4.0*sqr(sqr(mu))+2.77*(4.0*b-b*b)));
                  holeflowangle[currentset]=(180.0/pi)*asin((1.0/(1.2*holecd[currentset]))*((kk-1.0)/kk));
                }
                break;
          }    /*case*/
          /* Calculate the mass flow rate through the current set of holes */
          if ((pstat-pstatcan)<0.0)
            holesetmassflow[currentset]=-nholes[currentset]*ahole*holecd[currentset]*sqrt(2.0*rhoa*(pstatcan-pstat));
          else
            holesetmassflow[currentset]=nholes[currentset]*ahole*holecd[currentset]*sqrt(2.0*rhoa*(pstat-pstatcan));
          holesetvelocity[currentset]=holesetmassflow[currentset]/(rhoa*nholes[currentset]*ahole);
          switch (holeposition[currentset]) {
            case 1 : {
                  mai=mai-0.5*holesetmassflow[currentset];
                  mao=mao-0.5*holesetmassflow[currentset];
                  msumi=msumi+0.5*holesetmassflow[currentset];
                  msumo=msumo+0.5*holesetmassflow[currentset];
                  maod=maod+holesetmassflow[currentset];
                }
                break;
            case 2 : {
                  mai=mai-holesetmassflow[currentset];    /* Inner */
                  msumi=msumi+holesetmassflow[currentset];
                }
                break;
            case 3 : {
                  mao=mao-holesetmassflow[currentset];    /* Outer */
                  msumo=msumo+holesetmassflow[currentset];
                }
                break;
          }
/*         writeln(counter3:3,i:3,mdot:8:4,msumI:8:4,msumO:8:4,(pstat-pstatCAN):10:2,dPL:8:2,dPR:10:2); */
        } while (!((i==nholesets) || errorflag));
        msum=msumi+msumo;
/*       writeln;
        writeln(counter:5,i:5,maI:8:2,maO:8:2,dPL:10:1,dP:10:1,dPR:10:1,mdot:7:2,msum:7:2); */
/*      if counter=12 then
          write; */
        if (errorflag)
          dpr=dp;
        else
        {
          if (mdot>msum)  dpl=dp; else dpr=dp;
        }
        if (counter>30)
        {
          SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
          if (dp>(p0/2.0))
            {cout << "\n                   Decrease the air mass flow rate !!!!"; Sleep(3000);}
          else
            {cout << "\n                   Increase the air mass flow rate !!!!"; Sleep(3000);}
          SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
          exitflag=true;
          return;
        }
      } while (!(((abs(mdot-msum)/mdot)<0.000001) || exitflag));
/*     writeln(counter2:5,Split:8:2,msum:8:2,maI:8:3,maO:8:3,dP:10:1,pstat:10:1,pstatCAN:10:1,(pstat-pstatCAN):10:1); */
/*     XXX:=ReadKey; */
      if (mai>mao)  splitr=split; else splitl=split;
      if (counter2>30)
      {
        exitflag=true;
        return;
      }
    } while (!(((abs(mai)<=0.0001) && (abs(mao)<=0.0001)) || noinnerholes || noouterholes));
/*   XXX:=ReadKey; */
    maod=0.0;
    for( i=1; i <= nholesets; i ++)
    {
      currentset=holesetorder[i];
      if (holeposition[currentset]==1)  maod=maod+holesetmassflow[currentset];
    }
  }
  else    /* CAN, CAN-ANNULAR */
  {
    /* Assume pressure drop through combustion chamber */
    dpl=0.0;
    dpr=p0;
    exitflag=false;
    counter=0;
    do {
      counter += 1;
      dp=(dpl+dpr)/2.0;
      pstatcan=p0-dp;
      ma=mdot;
      msum=0.0;
      errorflag=false;
      i=0;
      do {
        i += 1;
        currentset=holesetorder[i];
        /* Calculate air density */
        rhoa=p0/(287.08*t0);
        for( j=1; j <= 5; j ++)
        {
          /* Calculate annulus velocity */
          if (cctype==1)
            aflow=0.25*pi*(sqr(dcasing[currentset])-sqr(dliner[currentset]))*ncans;
          else
            aflow=0.25*pi*(sqr(dcasing[currentset])-sqr(dcasingi[currentset]))
                   -0.25*pi*sqr(dliner[currentset])*ncans;
          vflow=ma/(aflow*rhoa);
          /* Calculate the static pressure in annulus */
          q=0.5*rhoa*vflow*vflow;
          pstat=p0-q;
          t=t0-(vflow*vflow/(2.0*cp));
          rhoa=pstat/(287.08*t);
        }
        if (holetypes[holetype[currentset]-1]=="Wiggle strip")
          ahole=slotdepth[currentset]*dhole[currentset];
        else
          ahole=(pi/4.0)*sqr(dhole[currentset]);
        /* Determine CD if not specified */
        switch (cdspecification[currentset]) {
          case 1 : holecd[currentset]=holecd[currentset]; break; /*user input*/
          case 2 : { /*built-in functions*/
                a=ahole*nholes[currentset]/(aflow/ncans);
                cd=1.0;
                for( j=1; j <= 5; j ++)
                {
                  if ((pstat-pstatcan)<0.0)
                    mhole=-ahole*cd*sqrt(2.0*rhoa*(pstatcan-pstat));
                  else
                    mhole=ahole*cd*sqrt(2.0*rhoa*(pstat-pstatcan));
                  b=abs(mhole)*nholes[currentset]/(ma/ncans);
                  mu=(b/a);
                  switch (holetype[currentset]) {
                    case 3 : { /* Plain hole */
                          kk=1.0+0.64*(2.0*mu*mu+sqrt(4.0*sqr(sqr(mu))+1.56*mu*mu*(4.0*b-b*b)));
                          cd=(kk-1.0)/(0.8*sqrt(4.0*kk*kk-kk*sqr(2.0-b)));
                        }
                        break;
                    case 4 : { /* Plunged hole */
                          kk=1.0+0.36*(2.0*mu*mu+sqrt(4.0*sqr(sqr(mu))+2.77*(4.0*b-b*b)));
                          cd=(kk-1.0)/(0.6*sqrt(4.0*kk*kk-kk*sqr(2.0-b)));
                        }
                        break;
                  }    /*case*/
                }    /*for j*/
                holecd[currentset]=cd;
              }
              break;
            case 3 : { /*user correlation*/
                  term=(p0-pstatcan)/q;
                  holecd[currentset]=acd[currentset]+
                                      bcd[currentset]*term+
                                      bcd[currentset]*term*term+
                                      bcd[currentset]*term*term*term;
                }
                break;
        }    /*case*/
        if (holecd[currentset]<=0.0)  holecd[currentset]=0.1;
        if (holecd[currentset]>=1.0)  holecd[currentset]=1.0;
        /* Calculate the flow angles */
        holeflowangle[currentset]=0.0;
        a=ahole*nholes[currentset]/(aflow/ncans);
        if ((pstat-pstatcan)<0.0)
          mhole=-ahole*holecd[currentset]*sqrt(2.0*rhoa*(pstatcan-pstat));
        else
          mhole=ahole*holecd[currentset]*sqrt(2.0*rhoa*(pstat-pstatcan));
        if (ma<0.0)
          b=1.0E20;
        else
          b=abs(mhole)*nholes[currentset]/(ma/ncans);
        mu=(b/a);
        switch (holetype[currentset]) {
          case 3 : {   /* Plain hole */
                kk=1.0+0.64*(2.0*mu*mu+sqrt(4.0*sqr(sqr(mu))+1.56*mu*mu*(4.0*b-b*b)));
                holeflowangle[currentset]=(180.0/pi)*asin((1.0/(1.6*holecd[currentset]))*((kk-1.0)/kk));
              }
              break;
          case 4 : {   /* Plunged hole */
                kk=1.0+0.36*(2.0*mu*mu+sqrt(4.0*sqr(sqr(mu))+2.77*(4.0*b-b*b)));
                holeflowangle[currentset]=(180.0/pi)*asin((1.0/(1.2*holecd[currentset]))*((kk-1.0)/kk));
              }
              break;
        }    /*case*/
        /* Calculate the mass flow rate through the current set of holes */
        if ((pstat-pstatcan)<0.0)
          holesetmassflow[currentset]=-nholes[currentset]*ahole*ncans*holecd[currentset]*sqrt(2.0*rhoa*(pstatcan-pstat));
        else
          holesetmassflow[currentset]=nholes[currentset]*ahole*ncans*holecd[currentset]*sqrt(2.0*rhoa*(pstat-pstatcan));
        holesetvelocity[currentset]=holesetmassflow[currentset]/(rhoa*nholes[currentset]*ahole*ncans);
        msum=msum+holesetmassflow[currentset];
        ma=ma-holesetmassflow[currentset];
        if (ma<1.0e-20)
        {
          ma=1.0e-20;
          errorflag=true;
        }
/*       writeln(i:3,ma:8:4,msum:8:4,(pstat-pstatCAN):10:2,dPL:8:2,dPR:10:2); */
      } while (!((i==nholesets) || errorflag));
/*     writeln;
      writeln(counter:4,mdot:8:2,msum:8:2,ma:8:2,dP:10:2,pstat:10:2,pstatCAN:10:2,(pstat-pstatCAN):10:2);
      writeln; */
      if (errorflag)
        dpr=dp;
      else
      {
        if (mdot>msum)  dpl=dp; else dpr=dp;
      }
      if (counter>50)
      {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
        if (dp>(p0/2.0))
          {cout << "\n                   Decrease the air mass flow rate !!!!"; Sleep(3000);}
        else
          {cout << "\n                   Increase the air mass flow rate !!!!"; Sleep(3000);}
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        exitflag=true;
        return;
      }
    } while (!(((abs(mdot-msum)/mdot)<0.000000001) || exitflag));
  }
  /* Calculate the flow per zone */
  mprz=0.0;
  mprp=0.0;
  ms=0.0;
  md=0.0;
  for( i=1; i <= nholesets; i ++)
  {
    mprz=mprz+primaryrzy[i]*holesetmassflow[i];
    mprp=mprp+primaryrpy[i]*holesetmassflow[i];
    ms=ms+secondaryy[i]*holesetmassflow[i];
    md=md+dilutiony[i]*holesetmassflow[i];
  }
  /* Perform the heat transfer calculations to determine liner temperatures */
  /* Calculate the gas temperatures in each zone */
  mf=mdot/afr;  /*overall*/
  /* Calculate representative pressure loss terms - See table 2.5 p. 2.53 */
  /*
  case CCtype of
    1 : Aref:=0.25*PI*sqr(dCasing[1]); {CAN}
    2 : Aref:=PI*(sqr(dCasing[1]-dCasingI[1])+(dCasing[1]-dCasingI[1])*dCasingI[1]); {ANNULAR}
    3 : Aref:=PI*(sqr(dCasing[1]-dCasingI[1])+(dCasing[1]-dCasingI[1])*dCasingI[1]); {CAN-ANNULAR}
  end; {case}
  */
  aref=0.0;
  for( i=1; i <= nholesets; i ++)
  {
    switch (cctype) {
      case 1 : aref=aref+0.25*pi*sqr(dcasing[i]); break; /*CAN*/
      case 2 : aref=aref+0.25*pi*(sqr(dcasing[i])-sqr(dcasingi[i])); break; /*ANNULAR*/
      case 3 : aref=aref+0.25*pi*(sqr(dcasing[i])-sqr(dcasingi[i])); break; /*CAN-ANNULAR*/
    }    /*case*/
  }
  aref=aref/nholesets;
  dlm=0.0;
  dlim=0.0;
  for( i=1; i <= nholesets; i ++)
  {
    dlm=dlm+dliner[i];
    dlim=dlim+dlineri[i];
  }
  dlm=dlm/nholesets;
  dlim=dlim/nholesets;
  switch (cctype) {
    case 1 : aft=0.25*pi*sqr(dlm); break;             /*CAN*/
    case 2 : aft=0.25*pi*(sqr(dlm)-sqr(dlim)); break; /*ANNULAR*/
    case 3 : aft=0.25*pi*sqr(dlm)*ncans; break;       /*CAN-ANNULAR*/
  }    /*case*/
  rhoaref=rhoa;
  uref=287.0*mdot*t0/(aref*p0);
  qref=0.5*rhoaref*uref*uref;
  srterm2=dp/qref;
  srterm3=mdot*sqrt(t0)/(aref*p0);
  srterm1=143.5*srterm2*srterm3*srterm3*100.0;
  /* Recirculation zone (primary zone) */
  trzi=t0;
  afrrz=(mprz/mf);
  phirz=afrst/afrrz;
  etarz=0.56+0.44*tanh(1.5475e-3*(t0+108.0*log(p0)-1863.0)); /*combustion efficiency*/
  trzo=t0+etarz*dt(phirz,t0,p0);  /*maximum possible temperature*/
  trzo=0.3333*trzi+0.6667*trzo;
  /* Remainder of primary zone (primary zone) */
  trpi= /*T0*/trzo;
  afrrp=((mprp+mprz)/mf);
  phirp=afrst/afrrp;
  etarp=0.71+0.29*tanh(1.5475e-3*(t0+108.0*log(p0)-1863.0));
  trpo=t0+etarp*dt(phirp,t0,p0);
  /* Secondary zone */
  tszi=trpo;
  afrsz=((mprz+mprp+ms)/mf);
  phisz=afrst/afrsz;
  if (phisz<1.0)      /*phi<1 ---> lean (air rich)*/
  {
    n=2.0*phisz;
    dft=0.0;       /*diameter of flame tube*/
    for( i=1; i <= nholesets; i ++)
    {
      currentset=holesetorder[i];
      dft=dft+dliner[currentset];
    }
    if (nholesets<=1)
      dft=dliner[1];
    else
      dft=dft/nholesets;
    if (cctype==2)        /*ANNULAR*/
      volumepz=(pi*(dliner[1]+dlineri[1])/2.0)*0.5*(dliner[1]-dlineri[1])*lpz; /*approximate*/
    else
      volumepz=(pi/4.0)*dft*dft*lpz*ncans;   /*approximate*/
    zeta300=mf/((volumepz/ncans)*p(p0,n));  /*nCans=1 if ANNULAR type*/
    y=phisz;
    if (phisz>1.0)  y=1.0;
    term=p(y,-1.205);
    zeta=zeta300*(p(10.0,-3.054*term)*p(t0,1.2327*term));
    dstar=0.736-0.0173*(p0/dp);
    term1=0.911*log10(zeta)+8.02*phisz-1.097+dstar;
    term2=p(10.0,term1);
    etasz=1.0/(p(10.0,term2));
  }
  else   /*not lean*/
    etasz=(1.0/phisz);
  etasz=0.9;   /*approximation, problem with Eq 2.37*/
  tszo=t0+etasz*dt(phisz,t0,p0);
  /* Dilution zone */
  tdzi=tszo;
  afrdz=(mdot/mf);
  phidz=afrst/afrdz;
  n=2.0*phidz;
  dft=0.0;       /*diameter of flame tube*/
  for( i=1; i <= nholesets; i ++)
  {
    currentset=holesetorder[i];
    dft=dft+dliner[currentset];
  }
  if (nholesets<=1)
    dft=dliner[1];
  else
    dft=dft/nholesets;
  if (cctype==2)        /*ANNULAR*/
    volumesz=(pi*(dliner[1]+dlineri[1])/2.0)*0.5*(dliner[1]-dlineri[1])*(lsz-lpz); /*approximate*/
  else
    volumesz=(pi/4.0)*dft*dft*(lsz-lpz)*ncans;   /*approximate*/
  zeta300=mf/((volumesz/ncans)*p(p0,n));
  y=phidz;
  if (phidz>1.0)  y=1.0;
  term=p(y,-1.205);
  zeta=zeta300*(p(10.0,-3.054*term)*p(t0,1.2327*term));
  dstar=0.736-0.0173*(p0/dp);
  term1=0.911*log10(zeta)+8.02*phidz-1.097+dstar;
  term2=p(10.0,term1);
  etadz=1.0/(p(10.0,term2));
  etadz=0.9;   /*approximation, problem with Eq 2.37*/
  tdzo=t0+etadz*dt(phidz,t0,p0);
  /* Calculate theta parameter and combustion efficiency */
  dref=0.0;
  for( i=1; i <= nholesets; i ++)
  {
    switch (cctype) {
      case 1 : dref=dref+dcasing[i]; break; /*CAN*/
      case 2 : dref=dref+(dcasing[i]-dcasingi[i])/2.0; break; /*ANNULAR*/
      case 3 : dref=dref+(dcasing[i]-dcasingi[i])/2.0; break; /*CAN-ANNULAR*/
    }    /*case*/
  }
  dref=dref/nholesets;
  phipz=(phirz+phirp)/2.0;
  if (phipz<1.0)
    b=245.0*(1.39+log(phipz));
  else
    b=170.0*(2.00-log(phipz));
  theta=(p(p0,1.75)*aref*p(dref,0.75)*exp(t0/b)/mdot)/1000000.0;
  if (cctype==2)      /* see Figure 2.2 */
  {
    if (theta<17.0)
      combeta=(2.994-0.042779*17.0+0.000295*17.0*17.0-32.829/17.0)*100.0;
    else if (theta>70.0)
      combeta=(2.994-0.042779*70.0+0.000295*70.0*70.0-32.829/70.0)*100.0;
    else
      combeta=(2.994-0.042779*theta+0.000295*theta*theta-32.829/theta)*100.0;
  }
  else
  {
    if (theta<17.0)
      combeta=(1.8467-0.01634*15.0+0.000104*15.0*15.0-18.296/15.0)*100.0;
    else if (theta>70.0)
      combeta=(1.8467-0.01634*70.0+0.000104*70.0*70.0-18.296/70.0)*100.0;
    else
      combeta=(1.8467-0.01634*theta+0.000104*theta*theta-18.296/theta)*100.0;
  }
  /* Calculate combustion efficiencies/emmision indices */
  ei_co=p(10.0,13.477-4.5270*log10(t0));
  ei_hc=p(10.0,19.730-7.1915*log10(t0));
  ei_h2=0.0275*ei_co;
  combeta2=(1.0-(0.24*ei_co+ei_hc+2.82*ei_h2)/1000.0)*100.0;
  /* Calculate 'pattern factor' */
  if (cctype==2)
    aaa=0.05;
  else
    aaa=0.07;
  pf=1.0-exp(-(dref*qref)/(aaa*lcomb*dp));
  /* Calculate local liner and casing temperatures */
  if (cctype==2)        /*ANNULAR*/
  {
    currentset=1;
    mail=0.0;                         /* inside liner*/
    maoloa=(1.0-split)*(mdot-maod);   /* outside liner - outer annulus */
    maolia=split*(mdot-maod);         /* outside liner - inner annulus*/
    linert.open(rootdir+"LinerT.txt", ios::out);
    cout << "This is the one\n\n";
    cout <<        "Zone  x      Tgas  mIL   mOLOA mOLIA dCO   dCI   dLO   dLI   AFR   TwLi   TwLiI" << endl;
    cout <<        " -    mm      K    kg/s  kg/s  kg/s   m     m     m     m     -     K      K" << endl;
    linert << string(" Zone   x     Tgas  mIL   mOLOA mOLIA dCO   dCI   dLO   dLI    AFR   ")+
                   " TwLi    TwLo    TwCi    TwCo    eta    TwLiI   TwLoI   TwCiI   TwCoI   etaI" << endl;
    linert << string("  -     mm     K    kg/s  kg/s  kg/s   m     m     m     m      -    ")+
                   "  K       K       K       K       %       K       K       K       K       %  " << endl;
    /* Initial linear positions, casing diameters and lining diameters */
    dx1=0.0000001;
    x1=xhole[holesetorder[1]];
    x2=xhole[holesetorder[2]];
    dc1=dcasing[holesetorder[1]];
    dc2=dcasing[holesetorder[2]];
    dci1=dcasingi[holesetorder[1]];  /*Annular*/
    dci2=dcasingi[holesetorder[2]];  /*Annular*/
    dl1=dliner[holesetorder[1]];
    dl2=dliner[holesetorder[2]];
    dli1=dlineri[holesetorder[1]];   /*Annular*/
    dli2=dlineri[holesetorder[2]];   /*Annular*/
    for( zone=1; zone <= 4; zone ++)
    {
      switch (zone) {
        case 1 : {
              lzone=lrz;
              tin=trzi;
              tout=trzo;
              x=0.0;
            }
            break;
        case 2 : {
              lzone=lpz-lrz;
              tin=trpi;
              tout=trpo;
              x=lrz;
            }
            break;
        case 3 : {
              lzone=lsz-lpz;
              tin=tszi;
              tout=tszo;
              x=lpz;
            }
            break;
        case 4 : {
              lzone=lcomb-lsz;
              tin=tdzi;
              tout=tdzo;
              x=lsz;
            }
            break;
      }    /*case*/
      nelements=25;
      if (zone==1)
        dx=((lzone-x1-dx1)/(nelements-1));
      else
        dx=(lzone/nelements);
      x0=x;
      i=0;
      do {
        i += 1;
        if ((zone==1) && (i==1))
          x=x+x1+dx1;
        else
          x=x+dx;
        /* Determine the distance from previous slot - outer annulus */
        filmcooledelementoa=false;
        xprevoa=0.0;
        sprevoa=0.0;
        mprevoa=0.0;
        jrem=0;
        for( j=1; j <= nholesets; j ++)
        {
          test1=((holetypes[holetype[holesetorder[j]]]=="Splash hole ")
                 && (holepositions[holeposition[holesetorder[j]]]=="Outer "));
          test2=((holetypes[holetype[holesetorder[j]]]=="Wiggle strip")
                 && (holepositions[holeposition[holesetorder[j]]]=="Outer "));
          test3=((holetypes[holetype[holesetorder[j]]]=="Machnd. ring")
                 && (holepositions[holeposition[holesetorder[j]]]=="Outer "));
          test4=(xhole[holesetorder[j]]<x);
          if ((test1 || test2 || test3) && test4)  jrem=j;       /*Cooling device exists upstream */
        }
        if (jrem>0)
        {
          filmcooledelementoa=true;
          xprevoa=xhole[holesetorder[jrem]];
          sprevoa=slotdepth[holesetorder[jrem]];
          mprevoa=holesetmassflow[holesetorder[jrem]];
        }
        /* Determine the distance from previous slot - inner annulus */
        filmcooledelementia=false;
        xprevia=0.0;
        sprevia=0.0;
        mprevia=0.0;
        jrem=0;
        for( j=1; j <= nholesets; j ++)
        {
          test1=((holetypes[holetype[holesetorder[j]]]=="Splash hole ")
                 && (holepositions[holeposition[holesetorder[j]]]=="Inner "));
          test2=((holetypes[holetype[holesetorder[j]]]=="Wiggle strip")
                 && (holepositions[holeposition[holesetorder[j]]]=="Inner "));
          test3=((holetypes[holetype[holesetorder[j]]]=="Machnd. ring")
                 && (holepositions[holeposition[holesetorder[j]]]=="Inner "));
          test4=(xhole[holesetorder[j]]<x);
          if ((test1 || test2 || test3) && test4)  jrem=j;       /*Cooling device exists upstream */
        }
        if (jrem>0)
        {
          filmcooledelementia=true;
          xprevia=xhole[holesetorder[jrem]];
          sprevia=slotdepth[holesetorder[jrem]];
          mprevia=holesetmassflow[holesetorder[jrem]];
        }
        /* Calculate the mean gas temperature in current element */
        tgas=tin-(tin-tout)*((x-x0)/lzone);
        if (currentset<=nholesets)
        {
          for( j=currentset; j <= nholesets; j ++)
          {
            if (xhole[holesetorder[currentset]]<x)
            {
              if (holepositions[holeposition[holesetorder[j]]]=="Outer ")
              {
                maoloa=maoloa-holesetmassflow[holesetorder[currentset]];
                if (maoloa<0.0)  maoloa=0.0;
                mail=mail+holesetmassflow[holesetorder[currentset]];
              }
              else if (holepositions[holeposition[holesetorder[j]]]=="Inner ")
              {
                maolia=maolia-holesetmassflow[holesetorder[currentset]];
                if (maolia<0.0)  maolia=0.0;
                mail=mail+holesetmassflow[holesetorder[currentset]];
              }
              else /*on dome*/
              {
                mail=mail+holesetmassflow[holesetorder[currentset]];
              }
              dc=dcasing[holesetorder[currentset]];
              dci=dcasingi[holesetorder[currentset]];
              dl=dliner[holesetorder[currentset]];
              dli=dlineri[holesetorder[currentset]];
              currentset += 1;
            }
          }    /*for j*/
        }
        if (currentset<=2)
        {
          if (abs(x2-x1)<0.000001)
          {
            dc=(dc1+dc2)/2.0;
            dl=(dl1+dl2)/2.0;
          }
          else
          {
            dc=dc1+x*(dc2-dc1)/(x2-x1);
            dl=dl1+x*(dl2-dl1)/(x2-x1);
          }
        }
        localafr=mail/mf;
        evaluateelement2(p0,t0,tgas,dl,dli,dc,dci,lcomb,dx,mail,maoloa,maolia,
                         localafr,x,xprevoa,sprevoa,mprevoa,xprevia,sprevia,
                         mprevia,filmcooledelementoa,filmcooledelementia,
                         twli,twlo,twci,twco,etafc,twlii,twloi,twcii,twcoi,etafci);
        if ((i % 5)==0)
          cout << setw(2) << zone << rts(x*1000.0, 7, 1) << rts(tgas, 8, 1) << rts(mail, 6, 2) << rts(maoloa, 6, 2) << rts(maolia, 6, 2) <<
                       rts(dc, 6, 2) << rts(dci, 6, 2) << rts(dl, 6, 2) << rts(dli, 6, 2) << rts(localafr, 6, 1) << rts(twli, 7, 1) << rts(twlii, 7, 1) << endl;
        linert << setw(3) << zone << rts(x*1000.0, 7, 1) << rts(tgas, 8, 1) << rts(mail, 6, 2) << rts(maoloa, 6, 2) << rts(maolia, 6, 2) <<
                  rts(dc, 6, 2) << rts(dci, 6, 2) << rts(dl, 6, 2) << rts(dli, 6, 2) << rts(localafr, 6, 1) <<
                  rts(twli, 8, 1) << rts(twlo, 8, 1) << rts(twci, 8, 1) << rts(twco, 8, 1) << rts(etafc*100.0, 8, 2) <<
                  rts(twlii, 8, 1) << rts(twloi, 8, 1) << rts(twcii, 8, 1) << rts(twcoi, 8, 1) << rts(etafci*100.0, 8, 2) << endl;
/*       if (x>0.046) then
        writeln; */
      } while (!(i==nelements));
    }    /*for Zone*/
    /* Reset CAN-ANNULAR dimensions */
    if (cctype==3)
    {
      for( i=1; i <= nholesets; i ++)
        dcasing[i]=dcasingrem[i];
    }
    /* Close temperature profile data file */
    linert.close();
  }
  else /*CAN+CAN-ANNULAR*/
  {
    currentset=1;
    mail=0.0;           /*inside liner*/
    maol=mdot/ncans;    /*outside liner*/

    linert.open(rootdir+"LinerT.txt", ios::out);
    cout << "This is the one running\n\n";
    cout <<        " Zone   x     Tgas    maIL   maOL    dC     dL    AFR     TwLi    TwCi " << endl;
    cout <<        "  -     mm     K      kg/s   kg/s    m      m      -       K       K" << endl;
    linert << " Zone   x     Tgas    maIL   maOL    dC     dL    AFR     TwLi    TwLo    TwCi    TwCo    eta" << endl;
    linert << "        mm     K      kg/s   kg/s    m      m      -       K       K       K       K       %" << endl;
    /* Calculate effective casing sizes for CAN-ANNULAR case */
    if (cctype==3)
    {
      for( i=1; i <= nholesets; i ++)
      {
        aflow=0.25*pi*(sqr(dcasing[i])-sqr(dcasingi[i]))-ncans*0.25*pi*sqr(dliner[i]);
        dcasingrem[i]=dcasing[i];
        dcasing[i]=sqrt((4.0/pi)*aflow+dliner[i]*dliner[i]);
      }
    }
    /* Initial linear positions, casing diameters and lining diameters */
    dx1=0.0000001;
    x1=xhole[holesetorder[1]];
    x2=xhole[holesetorder[2]];
    dc1=dcasing[holesetorder[1]];
    dc2=dcasing[holesetorder[2]];
    dl1=dliner[holesetorder[1]];
    dl2=dliner[holesetorder[2]];
    for( zone=1; zone <= 4; zone ++)
    {
      switch (zone) {
        case 1 : {
              lzone=lrz;
              tin=trzi;
              tout=trzo;
              x=0.0;
            }
            break;
        case 2 : {
              lzone=lpz-lrz;
              tin=trpi;
              tout=trpo;
              x=lrz;
            }
            break;
        case 3 : {
              lzone=lsz-lpz;
              tin=tszi;
              tout=tszo;
              x=lpz;
            }
            break;
        case 4 : {
              lzone=lcomb-lsz;
              tin=tdzi;
              tout=tdzo;
              x=lsz;
            }
            break;
      }    /*case*/
      nelements=25;
      if (zone==1)
        dx=((lzone-x1-dx1)/(nelements-1));
      else
        dx=(lzone/nelements);
      x0=x;
      i=0;
      do {
        i += 1;
        if ((zone==1) && (i==1))
          x=x+x1+dx1;
        else
          x=x+dx;
        /* Determine the distance from previous slot */
        filmcooledelement=false;
        xprev=0.0;
        sprev=0.0;
        mprev=0.0;
        jrem=0;
        for( j=1; j <= nholesets; j ++)
        {
          test1=(holetypes[holetype[holesetorder[j]]-1]=="Splash hole ");
          test2=(holetypes[holetype[holesetorder[j]]-1]=="Wiggle strip");
          test3=(holetypes[holetype[holesetorder[j]]-1]=="Machnd. ring");
          test4=(xhole[holesetorder[j]]<x);
          if ((test1 || test2 || test3) && test4)  jrem=j;       /*Cooling device exists upstream */
        }
        if (jrem>0)
        {
          filmcooledelement=true;
          xprev=xhole[holesetorder[jrem]];
          sprev=slotdepth[holesetorder[jrem]];
          mprev=holesetmassflow[holesetorder[jrem]];
        }
        /* Calculate the mean gas temperature in current element */
        tgas=tin-(tin-tout)*((x-x0)/lzone);
        if (currentset<=nholesets)
        {
          for( j=currentset; j <= nholesets; j ++)
          {
            if (xhole[holesetorder[currentset]]<x)
            {
              maol=maol-(holesetmassflow[holesetorder[currentset]]/ncans);
              if (maol<0.0)  maol=0.0;
              mail=mail+(holesetmassflow[holesetorder[currentset]]/ncans);
              dc=dcasing[holesetorder[currentset]];
              dl=dliner[holesetorder[currentset]];
              currentset += 1;
            }
          }    /*for j*/
        }
        if (currentset<=2)
        {
          if (abs(x2-x1)<0.000001)
          {
            dc=(dc1+dc2)/2.0;
            dl=(dl1+dl2)/2.0;
          }
          else
          {
            dc=dc1+x*(dc2-dc1)/(x2-x1);
            dl=dl1+x*(dl2-dl1)/(x2-x1);
          }
        }
        localafr=mail/(mf/ncans);  /*maIL = air flow per can, CAN type ==> nCans=1*/
        evaluateelement13(p0,t0,tgas,dl,dc,lcomb,dx,mail,maol,localafr,
                          x,xprev,sprev,mprev,filmcooledelement,
                          twli,twlo,twci,twco,etafc);
        if ((i % 5)==0)
          cout <<      setw(3) << zone << rts(x*1000.0, 8, 2) << rts(tgas, 8, 2) << rts(mail, 7, 2) << rts(maol, 7, 2) << rts(dc, 7, 2) <<
                       rts(dl, 7, 2) << rts(localafr, 8, 2) << rts(twli, 8, 1) << rts(twci, 8, 1) << endl;
        linert << setw(3) << zone << rts(x*1000.0, 8, 2) << rts(tgas, 8, 1) << rts(mail, 7, 2) << rts(maol, 7, 2) << rts(dc, 7, 2) <<
                rts(dl, 7, 2) << rts(localafr, 8, 2) << rts(twli, 8, 1) << rts(twlo, 8, 1) << rts(twci, 8, 1) << rts(twco, 8, 2) << rts(etafc*100.0, 8, 3) << endl;
/*       if (x>0.046) then
        writeln; */
      } while (!(i==nelements));
    }    /*for Zone*/
    /* Reset CAN-ANNULAR dimensions */
    if (cctype==3)
    {
      for( i=1; i <= nholesets; i ++)
        dcasing[i]=dcasingrem[i];
    }
    /* Close temperature profile data file */
    linert.close();
  }    /*if CCtype*/
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
  cout << "Calculations complete - Press any key";
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
  char dummy=readkey();
  cout << endl;
}    /* of procedure CALCULATE */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

/* Procedure to display results of calculations */
void showresults()
{
  //text fn;
  fstream fn;
  double ag,sum;
  bool print,save;
  int i,currentset,error;
  char answer,dummy;
  string filename,message,resultfile;

  sum=0.0;
  for( i=1; i <= nholesets; i ++)
  {
    if (holetypes[holetype[i]-1]=="Wiggle strip")
      sum=sum+(dhole[i]*slotdepth[i])*nholes[i];
    else
      sum=sum+(pi/4.0)*sqr(dhole[i])*nholes[i];
  }
  /* Display results */
  do {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
    heading(maintitle+" - RESULTS 1 of 6",true);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << "\n  Case name / description ............... (CaseName) : " << casename << endl;
    cout << endl;
    switch (cctype) {
     case 1 : cout << "  Combustor type .......................... (CCtype) :  CAN" << endl; break;
     case 2 : cout << "  Combustor type .......................... (CCtype) :  ANNULAR" << endl; break;
     case 3 : cout << "  Combustor type .......................... (CCtype) :  CAN-ANNULAR (" << setw(1) << ncans << " cans)" << endl; break;
    }
    cout << "  Total number of hole sets ........................ = " << rts((double)nholesets, 8, 0) << " [-]" << endl;
    cout << "  Stagnation pressure upstream of combustion chamber = " << rts(p0/1000.0, 8, 1) << " [kPa]" << endl;
    cout << "  Pressure drop across combustion chamber .......... = " << rts((p0-pstatcan)/1000.0, 8, 1) << " [kPa]" << endl;
    cout << "  Percentage pressure drop ......................... = " << rts(((p0-pstatcan)*100.0/p0), 8, 1) << " [%]" << endl;
    cout << "  Total air mass flow rate .................. (mdot) = " << rts(mdot, 8, 3) << " [kg/s]" << endl;
    if (cctype==3)
      cout << "  Air mass flow rate per can .......... (mdot/nCans) = " << rts((mdot/ncans), 8, 2) << " [kg/s]" << endl;
    cout << endl;
    if (cctype==2)     /*ANNULAR*/
    {
      cout << "  Flow split (On Dome/Total) ....................... = " << rts((maod*100.0/mdot), 8, 2) << " [%]" << endl;
      cout << "  Flow split (Outside/Total) ....................... = " << rts(((1.0-split)*(mdot-maod)*100.0/mdot), 8, 2) << " [%]" << endl;
      cout << "  Flow split (Inside /Total) ....................... = " << rts((split*(mdot-maod)*100.0/mdot), 8, 2) << " [%]" << endl;
    }
    cout << string("  Reference area ............................ (Aref) = ")+rts(aref,8,2)+" [m^2]" << endl;
    cout << string("  Reference diameter ........................ (Dref) = ")+rts(dref,8,2)+" [m]" << endl;
    cout << string("  Theta parameter .......................... (theta) = ")+rts(theta,8,2)+" x 10^6 [-]" << endl;
    cout << string("  Combustion efficiency, NREC Fig 2.2 .... (CombEta) = ")+rts(combeta,8,2)+" [%]" << endl;
    cout << string("  Emmision index : CO ...................... (EI_CO) = ")+rts(ei_co,8,2)+" [g CO/kg fuel]" << endl;
    cout << string("  Emmision index : HC ...................... (EI_HC) = ")+rts(ei_hc,8,2)+" [g HC/kg fuel]" << endl;
    cout << string("  Emmision index : H2 ...................... (EI_H2) = ")+rts(ei_h2,8,2)+" [g H2/kg fuel]" << endl;
    cout << string("  Comb. efficiency ...................... (CombEta2) = ")+rts(combeta2,8,2)+" [%]" << endl;
    cout << string("  Pattern factor .............................. (PF) = ")+rts(pf,8,2)+" [-]" << endl;
    cout << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    cout << "[Q]uit (Main Menu) | [S]ave | [P]rintout  | show [M]ore results: ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cin >> answer; //=charprompt('Q',"QSPM",0,23,"[Q]uit (Main Menu) / [S]ave / [P]rintout  / show [M]ore results");
    answer = toupper(answer);
    switch (answer) {
         case 'M' : {
                 /* Display page 2 */
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
                 heading(maintitle+" - RESULTS 2 of 6",true);
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                 cout << endl;
                 cout << " # Set  Type      #holes Diameter   Cd      Ag     Ag%    va      ma      ma%   " << endl;
                 cout << "                           [mm]     [-]   [mm^2]   [%]   [m/s]  [kg/s]    [%]   " << endl;
                 for( i=1; i <= nholesets; i ++)
                 {
                   currentset=holesetorder[i];
                   if (holetypes[holetype[currentset]-1]=="Wiggle strip")
                     ag=dhole[currentset]*slotdepth[currentset]*nholes[currentset];
                   else
                     ag=(pi/4.0)*sqr(dhole[currentset])*nholes[currentset];
                   cout << setw(2) << i << setw(3) << currentset << "  " << holetypes[holetype[currentset]-1] << ' ' <<
                           setw(3) << nholes[currentset] << "  " << rts(dhole[currentset]*1000.0, 6, 2) << "  " <<
                           rts(holecd[currentset], 6, 2) << ' ' << rts(ag*1.0e+6, 7, 1) << ' ' << rts((ag*100.0/sum), 6, 2) <<
                           rts(holesetvelocity[currentset], 8, 2) << rts(holesetmassflow[currentset], 8, 4) <<
                           rts(((holesetmassflow[currentset]*100.0)/mdot), 8, 2) << endl;
                 }
                 cout << endl;
                 gotoxy(30,25);
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                 cout << "Press any key to continue";
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                 dummy=readkey();
                 if (dummy=='\0')  dummy=readkey();
                 /* Display page 3 */
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
                 heading(maintitle+" - RESULTS 3 of 6",true);
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                 cout << endl;
                 if (cctype==2)     /*ANNULAR*/
                 {
                   cout << " Set  Type          Cd  Pos.  #holes Diameter  Xpos   LOD    LID    COD    CID" << endl;
                   cout << "                   [-]                 [mm]    [mm]   [mm]   [mm]   [mm]   [mm] " << endl;
                   for( i=1; i <= nholesets; i ++)
                   {
                     cout << setw(2) << i << "   " << holetypes[holetype[i]-1];
                     switch (cdspecification[i]) {
                       case 1 : cout << rts(holecd[i], 5, 2); break;
                       case 2 : cout << " Calc"; break;
                       case 3 : cout << " Corr"; break;
                     }    /*case*/
                     cout << "  " << holepositions[holeposition[i]-1] << ' ' <<
                             setw(3) << nholes[i] << "   " << rts(dhole[i]*1000.0, 6, 2) << "  " <<
                             rts(xhole[i]*1000.0, 6, 1) << ' ' <<
                             rts(dliner[i]*1000.0, 6, 1) << ' ' << rts(dlineri[i]*1000.0, 6, 1) << ' ' <<
                             rts(dcasing[i]*1000.0, 6, 1) << ' ' << rts(dcasingi[i]*1000.0, 6, 1) << endl;
                   }
                 }
                 else  /* CAN + CAN-ANNULAR */
                 {
                   cout << " Set  Type           Cd   Number Diameter   Xpos    LOD      COD"  << endl;
                   cout << "                     [-]   [-]      [mm]    [mm]    [mm]     [mm]" << endl;
                   for( i=1; i <= nholesets; i ++)
                   {
                     cout << setw(3) << i << "   " << holetypes[holetype[i]-1];
                     switch (cdspecification[i]) {
                       case 1 : cout << rts(holecd[i], 6, 2); break;
                       case 2 : cout << "  Calc"; break;
                       case 3 : cout << "  Corr"; break;
                     }    /*case*/
                     cout << ' ' << setw(5) << nholes[i] << "   " << rts(dhole[i]*1000.0, 6, 2) << "  " << rts(xhole[i]*1000.0, 6, 1);
                     cout << ' ' << rts(dliner[i]*1000.0, 8, 1) << ' ' << rts(dcasing[i]*1000.0, 8, 1) << endl;
                   }
                 }
                 cout << endl;
                 gotoxy(30,25);
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                 cout << "Press any key to continue";
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                 dummy=readkey();
                 if (dummy=='\0')  dummy=readkey();
                 /* Display page 4 */
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
                 heading(maintitle+" - RESULTS 4 of 6",true);
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                 cout << endl;
                 cout << " Set  Type          angle     YRZ     YRP    Ysec    Ydil" << endl;
                 cout << "                     [ј]      [-]     [-]     [-]     [-] " << endl;
                 for( i=1; i <= nholesets; i ++)
                 {
                   cout << ' ' << setw(2) << i << "   " << holetypes[holetype[i]-1] << ' ';
                   if (set::of(range(3,7), eos).has(holetype[i]))
                     cout << rts(holeflowangle[i], 7, 2) << ' ';
                   else
                     cout << "    -   ";
                   cout << rts(primaryrzy[i], 7, 2) << ' ' <<
                           rts(primaryrpy[i], 7, 2) << ' ' <<
                           rts(secondaryy[i], 7, 2) << ' ' <<
                           rts(dilutiony[i], 7, 2) << endl;
                 }
                 cout << endl;
                 gotoxy(30,25);
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                 cout << "Press any key to continue";
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                 dummy=readkey();
                 if (dummy=='\0')  dummy=readkey();
                 /* Display page 5 */
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
                 heading(maintitle+" - RESULTS 5 of 6",true);
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                 cout << "\n  Mass flow rate in primary recirculation zone ..... = " << rts(mprz/ncans, 8, 4) << " [kg/s]" << endl;
                 cout << "  Mass flow rate in remainder of primary zone ...... = " << rts(mprp/ncans, 8, 4) << " [kg/s]" << endl;
                 cout << "  Mass flow rate in secondary zone ................. = " << rts(ms/ncans, 8, 4) << " [kg/s]" << endl;
                 cout << "  Mass flow rate in dilution zone .................. = " << rts(md/ncans, 8, 4) << " [kg/s]" << endl;
                 cout << endl;
                 cout << string("  Primary zone (recirc.) equivalence ratio . (phiRZ) = ")+rts(phirz,8,2)+" [-]" << endl;
                 cout << string("  Primary zone (remainder) equivalence ratio (phiRP) = ")+rts(phirp,8,2)+" [-]" << endl;
                 cout << string("  Secondary equivalence ratio .............. (phiSZ) = ")+rts(phisz,8,2)+" [-]" << endl;
                 cout << string("  Dilution zone equivalence ratio .......... (phiDZ) = ")+rts(phidz,8,2)+" [-]" << endl;
                 cout << endl;
                 cout << string("  Flametube flow area ........................ (Aft) = ")+rts(aft,8,2)+" [m^2]" << endl;
                 cout << string("  (Flametube flow area)/(Reference area)  (Aft/Aref) = ")+rts((aft/aref)*100.0,8,2)+" [%]" << endl;
                 cout << string("  Reference area ............................ (Aref) = ")+rts(aref,8,2)+" [m^2]" << endl;
                 cout << string("  Reference diameter ........................ (Dref) = ")+rts(dref,8,2)+" [m]" << endl;
                 cout << string("  Reference density ...................... (rhoaREF) = ")+rts(rhoaref,8,2)+" [kg/m3]" << endl;
                 cout << string("  Reference velocity ........................ (uref) = ")+rts(uref,8,2)+" [m/s]" << endl;
                 cout << string("  Reference velocity head ................... (qref) = ")+rts(qref,8,2)+" [Pa]" << endl;
                 cout << string("  Pressure difference 3 to 4 ................ (dp34) = ")+rts(srterm2*qref,8,2)+" [Pa]" << endl;
                 cout << endl;
                 cout <<         "                     dP3-4/p3 [%]     dP3-4/qref    mT3^0.5/(Aref*p3)" << endl;
                 switch (cctype) {
                   case 1 : {
                         cout << " CAN (typ)                5.3             40            3.0E-3  " << endl;
                         cout << " CAN (act)               " << rts(srterm1, 4, 1) << rts(srterm2, 15, 0) << rts(srterm3*1000.0, 15, 1)  << "E-3" << endl;
                       }
                       break;
                   case 2 : {
                         cout << " ANNULAR (typ)            6.0             20            4.5E-3  " << endl;
                         cout << " ANNULAR (act)           " << rts(srterm1, 4, 1) << rts(srterm2, 15, 0) << rts(srterm3*1000.0, 15, 1) << "E-3" << endl;
                       }
                       break;
                   case 3 : {
                         cout << " CAN-ANNULAR (typ)        5.4             30            3.5E-3  " << endl;
                         cout << " CAN-ANNULAR (act)       " << rts(srterm1, 4, 1) << rts(srterm2, 15, 0) << rts(srterm3*1000.0, 15, 1) << "E-3" << endl;
                       }
                       break;
                 }    /*case*/
                 gotoxy(30,25);
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                 cout << "Press any key to continue";
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                 dummy=readkey();
                 if (dummy=='\0')  dummy=readkey();
                 /* Display page 6 */
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
                 heading(maintitle+" - RESULTS 6 of 6",true);
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                 cout << string("\n  Total air mass flow rate ... (mdot) = ")+rts(mdot,8,2)+" [kg/s]" << endl;
                 cout << string("  Overall air/fuel ratio ...... (AFR) = ")+rts(afr,8,2)+" [-]" << endl;
                 cout << string("  Stagnation pressure  ......... (P0) = ")+rts(p0/1000.0,8,2)+" [kPa]" << endl;
                 cout << string("  Stagnation temperature ....... (T0) = ")+rts(t0,8,2)+" [K]" << endl;
                 cout << string("  Stagnation temperature ....... (T0) = ")+rts(t0-273.15,8,2)+" [јC]" << endl;
                 cout << string("  Specific heat of gas ......... (Cp) = ")+rts(cp,8,2)+" [J/kgK]" << endl;
                 cout << string("  Specific heat ratio ....... (gamma) = ")+rts(gamma,8,2)+" [-]" << endl;
                 cout << string("  Fuel atomic C/H ratio ........ (CH) = ")+rts(ch,8,2)+" [-]" << endl;
                 cout << string("  Fuel lower calorific value .. (LCV) = ")+rts(lcv/1000000.0,8,2)+" [MJ/kg]" << endl;
                 cout << endl;
                 if (filmcooling)
                   cout << "  Heat transfer/liner cooling model . =  FILM-COOLED LINER" << endl;
                 else
                   cout << "  Heat transfer/liner cooling model . =  UNCOOLED LINER" << endl;
                 if (simplified)
                   cout << "  Flame radiation model ............. =  SIMPLIFIED" << endl;
                 else
                   cout << "  Flame radiation model ............. =  ACCURATE" << endl;
                 cout << string("  Ambient air temperature  (Tambient) = ")+rts(tambient,8,2)+" [јC]" << endl;
                 cout << string("  Liner inner emissivity ...... (eLi) = ")+rts(eli,8,2)+" [-]" << endl;
                 cout << string("  Liner outer emissivity ...... (eLo) = ")+rts(elo,8,2)+" [-]" << endl;
                 cout << string("  Casing inner emissivity ..... (eCi) = ")+rts(eci,8,2)+" [-]" << endl;
                 cout << string("  Casing outer emissivity ..... (eCo) = ")+rts(eco,8,2)+" [-]" << endl;
                 cout << string("  Liner thermal conductivity ... (kL) = ")+rts(kl,8,2)+" [W/mK]" << endl;
                 cout << string("  Casing thermal conductivity .. (kC) = ")+rts(kc,8,2)+" [W/mK]" << endl;
                 cout << string("  Liner wall thickness ......... (tL) = ")+rts(tl*1000.0,8,2)+" [mm]" << endl;
                 cout << string("  Casing wall thickness ........ (tC) = ")+rts(tc*1000.0,8,2)+" [mm]" << endl;
                 gotoxy(30,25);
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                 cout << "Press any key to continue";
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                 dummy=readkey();
                 if (dummy=='\0')  dummy=readkey();
               }
               break;
     case 'P':case 'S' : {
                  char p_or_save = answer;

                 {
                   do {
                       answer='Y';
                       SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                       cout << "\n              Give the new filename for the result file (DEFAULT) ? ";
                       SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                       cin >> resultfile; //=filenameprompt("DEFAULT",0,23,"Give the new filename for the resultfile (DEFAULT) ? ");
                       SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                       cout << "\n              " <<string("Checking if result file:  ")+resultfile+" exists ...";
                       SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

                       fn.open(rootdir+resultfile+".txt", ios::in);

                       error=(int)strerror(errno);
                       if (fn.is_open())
                       {
                         fn.close();
                         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
                         cout << "\n              " <<string("Result file ")+resultfile+" already exists. Rewrite (Y/N) ? ";
                         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                         cin >> answer;
                         answer = toupper(answer);
                       }
                     } while (!(answer=='Y'));
                     save=true;
                     filename=rootdir+resultfile+".txt";
                     fn.open(filename, ios::out);

                     char filename1[ MAX_PATH ];

                      OPENFILENAME ofn;
                      ZeroMemory( &filename1, sizeof( filename1 ) );
                      ZeroMemory( &ofn,      sizeof( ofn ) );
                      ofn.lStructSize  = sizeof( ofn );
                      ofn.hwndOwner    = NULL;  // If you have a window to center over, put its HANDLE here
                      ofn.lpstrFilter  = "Text Files\0*.txt;*.HFI;*HFR;*CDF\0Any File\0*.*\0";
                      ofn.lpstrFile    = filename1;
                      ofn.nMaxFile     = MAX_PATH;
                      ofn.lpstrTitle   = "Select a File!";
                      ofn.Flags        = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

                   /* PRINT / SAVE RESULTS */
                   fn << maintitle << endl;
                   fn << "INPUT DATA SUMMARY" << endl;
                   fn << endl;
                   fn << "Case name / description ........... : " << casename << endl;
                   fn << endl;
                   switch (cctype) {
                    case 1 : fn << "Combustor type ........... (CCtype) :  CAN" << endl; break;
                    case 2 : fn << "Combustor type ........... (CCtype) :  ANNULAR" << endl; break;
                    case 3 : fn << "Combustor type ........... (CCtype) :  CAN-ANNULAR (" << setw(1) << ncans << " cans)" << endl; break;
                   }
                   fn << "Total number of hole sets ......... = " << setw(5) << nholesets << " [-]" << endl;
                   fn << endl;
                   switch (cctype) {
                    case 1 : { /*CAN*/
                          fn << "Set  Type          Cd   Number  Diameter   Xpos    LOD      COD"  << endl;
                          fn << "                   [-]   [-]       [mm]    [mm]    [mm]     [mm]" << endl;
                          for( i=1; i <= nholesets; i ++)
                          {
                            fn << setw(2) << i << "   " << holetypes[holetype[i]-1];
                            switch (cdspecification[i]) {
                              case 1 : fn << setw(6) << holecd[i]; break;
                              case 2 : fn << "  Calc"; break;
                              case 3 : fn << "  Corr"; break;
                            }    /*case*/
                            fn << ' ' << setw(5) << nholes[i] << "     " << setw(6) << dhole[i]*1000.0 << "  " << setw(6) << xhole[i]*1000.0;
                            fn << ' ' << setw(8) << dliner[i]*1000.0 << ' ' << setw(8) << dcasing[i]*1000.0 << endl;
                          }
                        }
                        break;
                    case 2 : { /*ANNULAR*/
                          fn << "Set  Type           Cd   Pos.   #holes  Diameter  Xpos   LOD    COD    CID" << endl;
                          fn << "                   [-]                    [mm]    [mm]   [mm]   [mm]   [mm] " << endl;
                          for( i=1; i <= nholesets; i ++)
                          {
                            fn << setw(2) << i << "  " << holetypes[holetype[i]-1];
                            switch (cdspecification[i]) {
                              case 1 : fn << setw(2) << i ; break;
                              case 2 : fn << "  Calc"; break;
                              case 3 : fn << "  Corr"; break;
                            }    /*case*/
                            fn << "  " << holepositions[holeposition[i]-1] << ' ' <<
                                    setw(5) << nholes[i] << "    " << setw(6) << dhole[i]*1000.0 << "  " <<
                                    setw(6) << xhole[i]*1000.0 << ' ' <<
                                    setw(6) << dliner[i]*1000.0 << ' ' <<
                                    setw(6) << dcasing[i]*1000.0 << ' ' << setw(6) << dcasingi[i]*1000.0 << endl;
                          }
                        }
                        break;
                    case 3 : { /*CAN-ANNULAR*/
                          fn << "Set  Type          Cd   #holes  Diameter  Xpos   LOD    LID    COD    CID" << endl;
                          fn << "                  [-]             [mm]    [mm]   [mm]   [mm]   [mm]   [mm] " << endl;
                          for( i=1; i <= nholesets; i ++)
                          {
                            fn << setw(2) << i << "  " << holetypes[holetype[i]-1];
                            switch (cdspecification[i]) {
                              case 1 : fn << setw(6) << holecd[i]; break;
                              case 2 : fn << "  Calc"; break;
                              case 3 : fn << "  Corr"; break;
                            }    /*case*/
                            fn << ' ' <<
                                    setw(5) << nholes[i] << "    " << setw(6) << dhole[i]*1000.0 << "  " <<
                                    setw(6) << xhole[i]*1000.0 << ' ' <<
                                    setw(6) << dliner[i]*1000.0 << ' ' <<
                                    setw(6) << dcasing[i]*1000.0 << ' ' << setw(6) << dcasingi[i]*1000.0 << endl;
                          }
                        }
                        break;
                   }    /*case*/
                   fn << endl;
                   fn << string("Total air mass flow rate ... (mdot) = ")+rts(mdot,8,3)+" [kg/s]" << endl;
                   if (cctype==3)
                     fn << string("Air mass flow per can  (mdot/nCans) = ")+rts(mdot/ncans,8,3)+" [kg/s]" << endl;
                   fn << string("Overall air/fuel ratio ...... (AFR) = ")+rts(afr,8,2)+" [-]" << endl;
                   fn << string("Stagnation pressure  ......... (P0) = ")+rts(p0/1000.0,8,2)+" [kPa]" << endl;
                   fn << string("Stagnation temperature ....... (T0) = ")+rts(t0,8,2)+" [K]" << endl;
                   fn << string("Stagnation temperature ....... (T0) = ")+rts(t0-273.15,8,2)+" [јC]" << endl;
                   fn << string("Specific heat of gas ......... (Cp) = ")+rts(cp,8,2)+" [J/kgK]" << endl;
                   fn << string("Specific heat ratio ....... (gamma) = ")+rts(gamma,8,2)+" [-]" << endl;
                   fn << string("Fuel atomic C/H ratio ........ (CH) = ")+rts(ch,8,2)+" [-]" << endl;
                   fn << string("Fuel lower calorific value .. (LCV) = ")+rts(lcv/1000000.0,8,2)+" [MJ/kg]" << endl;
                   if (filmcooling)
                     fn << "Heat transfer/liner cooling model . =  FILM-COOLED LINER" << endl;
                   else
                     fn << "Heat transfer/liner cooling model . =  UNCOOLED LINER" << endl;
                   if (simplified)
                     fn << "Flame radiation model ............. =  SIMPLIFIED" << endl;
                   else
                     fn << "Flame radiation model ............. =  ACCURATE" << endl;
                   fn << string("Ambient air temperature  (Tambient) = ")+rts(tambient,8,2)+" [јC]" << endl;
                   fn << string("Liner inner emissivity ...... (eLi) = ")+rts(eli,8,2)+" [-]" << endl;
                   fn << string("Liner outer emissivity ...... (eLo) = ")+rts(elo,8,2)+" [-]" << endl;
                   fn << string("Casing inner emissivity ..... (eCi) = ")+rts(eci,8,2)+" [-]" << endl;
                   fn << string("Casing outer emissivity ..... (eCo) = ")+rts(eco,8,2)+" [-]" << endl;
                   fn << string("Liner thermal conductivity ... (kL) = ")+rts(kl,8,2)+" [W/mK]" << endl;
                   fn << string("Casing thermal conductivity .. (kC) = ")+rts(kc,8,2)+" [W/mK]" << endl;
                   fn << string("Liner wall thickness ......... (tL) = ")+rts(tl*1000.0,8,2)+" [mm]" << endl;
                   fn << string("Casing wall thickness ........ (tC) = ")+rts(tc*1000.0,8,2)+" [mm]" << endl;
                   fn << endl;
                   fn << endl;
                   fn << endl;
                   fn << maintitle << endl;
                   fn << "CALCULATION RESULTS" << endl;
                   fn << endl;
                   fn << "Pressure drop across combustion chamber ........... = " << setw(8) << (p0-pstatcan)/1000.0 << " [kPa]" << endl;
                   fn << "Percentage pressure drop .......................... = " << setw(8) << ((p0-pstatcan)*100.0/p0) << " [%]" << endl;
                   fn << endl;
                   fn << " # Set#  Type    #holes Diameter   Cd      Ag     Ag%     va      ma      ma%   " << endl;
                   fn << "                         [mm]     [-]    [mm^2]   [%]    [m/s]   [kg/s]   [%]   " << endl;
                   for( i=1; i <= nholesets; i ++)
                   {
                     currentset=holesetorder[i];
                     if (holetypes[holetype[currentset]-1]=="Wiggle strip")
                       ag=dhole[currentset]*slotdepth[currentset]*nholes[currentset];
                     else
                       ag=(pi/4.0)*sqr(dhole[currentset])*nholes[currentset];
                     fn << setw(2) << i << setw(4) << currentset << "  " << holetypes[holetype[currentset]-1] <<
                             setw(3) << nholes[currentset] << "   " << setw(6) << dhole[currentset]*1000.0 << "  " <<
                             setw(6) << holecd[currentset] << ' ' << setw(8) << ag*1.0e+6 << ' ' << setw(6) << (ag*100.0/sum) <<
                             setw(8) << holesetvelocity[currentset] << setw(9) << holesetmassflow[currentset] <<
                             setw(7) << ((holesetmassflow[currentset]*100.0)/mdot) << endl;
                   }
                   fn << endl;
                   fn << "Set  Type         angle     YRZ     YRP    Ysec    Ydil" << endl;
                   fn << "                   [ј]      [-]     [-]     [-]     [-] " << endl;
                   for( i=1; i <= nholesets; i ++)
                   {
                     fn << setw(2) << 2 << "  " << holetypes[holetype[i]-1] << ' ';
                     if (set::of(range(3,7), eos).has(holetype[i]))
                       fn << setw(2) << holeflowangle[i] << ' ';
                     else
                       fn << "    -   ";
                     fn << setw(7) << primaryrzy[i] << ' ' <<
                                setw(7) << primaryrpy[i] << ' ' <<
                                setw(7) << secondaryy[i] << ' ' <<
                                setw(7) << dilutiony[i] << endl;
                   }
                   fn << endl;
                   fn << "Mass flow rate in primary recirculating zone ...... = " << setw(8) << mprz/ncans << " [kg/s]" << endl;
                   fn << "Mass flow rate in remainder of primary zone ....... = " << setw(8) << mprp/ncans << " [kg/s]" << endl;
                   fn << "Mass flow rate in secondary zone .................. = " << setw(8) << ms/ncans << " [kg/s]" << endl;
                   fn << "Mass flow rate in dilution zone ................... = " << setw(8) << md/ncans << " [kg/s]" << endl;
                   fn << endl;
                   fn << string("Primary zone (recirc.) equivalence ratio .. (phiRZ) = ")+rts(phirz,8,2)+" [-]" << endl;
                   fn << string("Primary zone (remainder) equivalence ratio  (phiRP) = ")+rts(phirp,8,2)+" [-]" << endl;
                   fn << string("Secondary equivalence ratio ............... (phiSZ) = ")+rts(phisz,8,2)+" [-]" << endl;
                   fn << string("Dilution zone equivalence ratio ........... (phiDZ) = ")+rts(phidz,8,2)+" [-]" << endl;
                   fn << endl;
                   fn << string("Theta parameter ........................... (theta) = ")+rts(theta,8,2)+" x 10^6 [-]" << endl;
                   fn << string("Combustion efficiency, NREC Figure 2.2 .. (CombEta) = ")+rts(combeta,8,2)+" [%]" << endl;
                   fn << string("Emmision index : CO ....................... (EI_CO) = ")+rts(ei_co,8,2)+" [g CO/kg fuel]" << endl;
                   fn << string("Emmision index : HC ....................... (EI_HC) = ")+rts(ei_hc,8,2)+" [g HC/kg fuel]" << endl;
                   fn << string("Emmision index : H2 ....................... (EI_H2) = ")+rts(ei_h2,8,2)+" [g H2/kg fuel]" << endl;
                   fn << string("Combustion efficiency .................. (CombEta2) = ")+rts(combeta2,8,2)+" [%]" << endl;
                   fn << endl;
                   fn << string("Pattern factor ............................... (PF) = ")+rts(pf,8,2)+" [-]" << endl;
                   fn << endl;
                   fn << string("Flametube flow area ......................... (Aft) = ")+rts(aft,8,2)+" [m^2]" << endl;
                   fn << string("(Flametube flow area)/(Reference area) . (Aft/Aref) = ")+rts((aft/aref)*100.0,8,2)+" [%]" << endl;
                   fn << string("Reference area ............................. (Aref) = ")+rts(aref,8,2)+" [m^2]" << endl;
                   fn << string("Reference diameter ......................... (Dref) = ")+rts(dref,8,2)+" [m]" << endl;
                   fn << string("Reference density ....................... (rhoaREF) = ")+rts(rhoaref,8,2)+" [kg/m3]" << endl;
                   fn << string("Reference velocity ......................... (uref) = ")+rts(uref,8,2)+" [m/s]" << endl;
                   fn << string("Reference velocity head .................... (qref) = ")+rts(qref,8,2)+" [Pa]" << endl;
                   fn << string("Pressure difference 3 to 4 ................. (dp34) = ")+rts(srterm2*qref,8,2)+" [Pa]" << endl;
                   fn << endl;
                   fn << "                     dP3-4/p3 [%]     dP3-4/qref    mT3^0.5/(Aref*p3)" << endl;
                   switch (cctype) {
                     case 1 : {
                           fn << " CAN (typical)            5.3             40            3.0E-3  " << endl;
                           fn << " CAN (actual)            " << setw(4) << srterm1 << setw(15) << srterm2 << setw(15) << srterm3*1000.0 << "E-3" << endl;
                         }
                         break;
                     case 2 : {
                           fn << " ANNULAR (typical)        6.0             20            4.5E-3  " << endl;
                           fn << " ANNULAR (actual)        " << setw(4) << srterm1 << setw(15) << srterm2 << setw(15) << srterm3*1000.0 << "E-3" << endl;
                         }
                         break;
                     case 3 : {
                           fn << " CAN-ANNULAR (typical)    5.4             30            3.5E-3  " << endl;
                           fn << " CAN-ANNULAR (actual)    " << setw(4) << srterm1 << setw(15) << srterm2 << setw(15) << srterm3*1000.0 << "E-3" << endl;
                         }
                         break;
                   }    /*case*/
                   if (answer=='P')
                   {
                     fn << endl;
                     fn << chr(12) << endl;   /* advance to top of next page */
                   }
                   fn.close();

                   if(p_or_save == 'P'){
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                    cout << "\n\n          NOW! SELECT THE FILE NAME YOU JUST ENTERED FOR [PDF] PRINTING";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                    cout << "\n\n                   ";
                    system("pause");
                    if (GetOpenFileNameA( &ofn ))
                      {
                        HINSTANCE result = ShellExecute(NULL,"print",filename1,NULL,NULL,SW_SHOWDEFAULT);
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                        cout << "\n                   ";
                        system("pause");
                      }
                   }
                 }
               }
               break;
    }    /*case*/
  } while (!(answer=='Q'));
}    /* of procedure SHOWRESULTS */

/* ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */
/* main program */
char dummy;
string paramstring;
int colour,code;
int main(int argc, const char* argv[])
{
  /* Initialize the colour set to be used */
  //pio_initialize(argc, argv);
  normalattr=11;
  brightattr=15;
  menuattr=27;
  titleattr=27;
  highlightattr=120;
  firstcharattr=31;
  firstcharhighlightattr=127;
  /* Display the title page */
  textattr=normalattr;
  clrscr();
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
  cout << "              ААААААААААААААААААААААААААААААААААААААААААААААААААААА" << endl;
  cout << "              А                                                   А" << endl;
  cout << "              А   лллллллл   лллллллллл   лллллллл   ллллллл      А" << endl;
  cout << "              А   лл         лл              лл      лл     лл    А" << endl;
  cout << "              А   лл         лл              лл      лл      лл   А" << endl;
  cout << "              А   лл         лллллллллл      лл      лллллллл     А" << endl;
  cout << "              А   лл                 лл      лл      лл    лл     А" << endl;
  cout << "              А   лл                 лл      лл      лл     лл    А" << endl;
  cout << "              А   лллллллл   лллллллллл   лллллллл   лл      лл   А" << endl;
  cout << "              А                                                   А" << endl;
  cout << "              ААААААААААААААААААААААААААААААААААААААААААААААААААААА" << endl;
  cout << endl;
  cout << "        Program          : HOLEFLOW " << version << " (GNU GCC Compiler v6.3.0)" << endl;
  cout << "                         " << endl;
  cout << "                         Combustion Chamber Flow Distribution Simulation" << endl;
  cout << "                         31 October 2017" << endl;
  cout << endl;
  cout << "        Created by       : Mr. S. Myeza" << endl;
  cout << "        Supervised by    : Mr. P. Tarwireyi" << endl << endl;
  cout << "                       Department of Computer Science" << endl;
  cout << "                       University of Zululand, (035) 902 6398" << endl;
  cout << endl;
  cout << "                       < Press any key to continue >";
  dummy=readkey();
  if (dummy=='\0')  dummy=readkey();
  /* Initialize all the variables - using last input file as default data */
  initialise();
  do {
    topwindow=nil;
    mainmenu();
    starttimer();
    calculate();
    if (! exitflag)
    {
      //double time=endtimer();
      showresults();
    }
  } while (!false);
  return EXIT_SUCCESS;
}
