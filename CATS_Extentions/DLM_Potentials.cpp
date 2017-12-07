
#include "DLM_Potentials.h"
#include "DLM_StefanoPotentials.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

//int DlmPot=0;
//int DlmPotFlag=0;
DLM_StefanoPotentials** fV18pot=NULL;

void CleanUpV18Pot(){
    if(fV18pot){
        for(unsigned iPot=0; iPot<30; iPot++){
            delete fV18pot[iPot];
        }
        delete [] fV18pot;
        fV18pot = NULL;
    }
}

double ZeroPotential(double* Radius){
    return 0;
}

//I took it from Oli, he found it in some paper somewhere...
//the difference to the CRAB potential is only in the 3P1 (triplet) state
double ReidSoftCore(const int& polar, double* Radius){
    double r = Radius[0];
    double pmux,f1,f2,f3,f4,f7,vr;
    /* See the appendix of B.D. Day, PRC24, p. 1203 (1981).
     with Tensor forces neglected */
    if(polar==0){
        /* S=0 */
        pmux=r*0.7;
        f1=exp(-pmux);
        f4=(f1*f1*f1*f1);
        f7=f4*(f1*f1*f1);
        vr=-10.463*f1/pmux-1650.6*f4/pmux+6484.2*f7/pmux;
    }
    else if(polar>0){
        /* S=1 */
        pmux=r*0.7;
        f1=exp(-pmux);
        f2=f1*f1;
        f3=f2*f1;
        f4=f2*f2;
        //f6=f4*f2;

        //ANNALS OF PHYSICS: 50, 411448 (1968)
        //This potential is originally included in CRAB (check), but describes the 3P2 - 3F2 mixing according to Reid paper
        //vr=((-10.463/3.0)*f1-933.48*f4+4152.1*f6)/pmux;

        //potential according to Reid:
        vr = 10.463 * ( (1. + 2./pmux + 2./(pmux*pmux))*f1 - (8./pmux + 2./(pmux*pmux))*f4 )/pmux - 135.25 * f2/pmux + 472.81 * f3/pmux;
    }

    return vr;//MeV
}

double ReidSoftCore1S0(double* Radius){
    return ReidSoftCore(0, Radius);
}

double ReidSoftCore3P(double* Radius){
    return ReidSoftCore(1, Radius);
}

//input in fm
double fReidMeVfm(const double& rad,const unsigned& polar){
    double r = rad;//convert in fm
    double pmux,f1,f2,f4,f6,f7,vr;
    /* See the appendix of B.D. Day, PRC24, p. 1203 (1981).
     with Tensor forces neglected */
    if(polar==0){
        /* S=0 */
        pmux=r*0.7;
        f1=exp(-pmux);
        f4=(f1*f1*f1*f1);
        f7=f4*(f1*f1*f1);
        vr=-10.463*f1/pmux-1650.6*f4/pmux+6484.2*f7/pmux;
        //vr=0;
    }
    else if(polar>0){
        /* S=1 */
        pmux=r*0.7;
        f1=exp(-pmux);
        f2=f1*f1;
        f4=f2*f2;
        f6=f4*f2;
        vr=((-10.463/3.0)*f1-933.48*f4+4152.1*f6)/pmux;
    }

    return vr;//MeV
}

double fReidMeVfm1S0(double* rad){
    return fReidMeVfm(*rad, 0);
}
double fReidMeVfm3P(double* rad){
    return fReidMeVfm(*rad, 1);
}

double fReidDlm(const double& rad,const unsigned short& s,const unsigned short& l,const unsigned short& j){
    const double mu_const = 0.7;
    const double h_const = 10.463;
    double f1,f2,f3,f4,f6,f7,vr;
    const double pmux=rad*mu_const;
    f1=exp(-pmux);
    f2=f1*f1;
    f3=f2*f1;
    f4=f2*f2;
    f6=f4*f2;
    f7=f4*f3;
    vr=0;
    if(s==0 && l==0 && j==0){
        /* 1S0 */
        f1=exp(-pmux);
        f4=(f1*f1*f1*f1);
        f7=f4*(f1*f1*f1);
        vr=-h_const*f1/pmux-1650.6*f4/pmux+6484.2*f7/pmux;
        //vr=0;
    }
    else if(s==1 && l==1 && j==0){
        /* 3P0 */
        vr =    -h_const*((1.+4./pmux+4./pmux/pmux)*f1 - (16./pmux+4./pmux/pmux)*f4)/pmux
                + 27.133*f2/pmux - 790.74*f4/pmux + 20662.*f7/pmux;
    }
    else if(s==1 && l==1 && j==1){
        /* 3P1 */
        vr =    h_const*((1.+2./pmux+2./pmux/pmux)*f1 - (8./pmux+2./pmux/pmux)*f4)/pmux
                - 135.25*f2/pmux + 472.81*f3/pmux;
    }
    else if(s==1 && l==1 && j==2){
        /* 3P2 - 3F2 */
        vr =    (h_const*f1/3. - 933.48*f4 + 4152.1*f6)/pmux;
    }
    return vr;//MeV
}

double fReidDlm1S0(double* rad){
    return fReidDlm(*rad,0,0,0);
}
double fReidDlm3P0(double* rad){
    return fReidDlm(*rad,1,1,0);
}
double fReidDlm3P1(double* rad){
    return fReidDlm(*rad,1,1,1);
}
double fReidDlm3P2(double* rad){
    return fReidDlm(*rad,1,1,2);
}
double fReidDlm3P(double* rad){
    return (fReidDlm(*rad,1,1,0)+fReidDlm(*rad,1,1,1)+fReidDlm(*rad,1,1,2))/3.;
}

//N.B. the potentials are never deleted, i.e. they stay there until termination
double fV18potential(const int& V18Pot, const int& DlmPotFlag,
                     const int& IsoSpin, const int& t2p1, const int& t2p2,
                     const int& Spin, const int& AngMom, const int& TotMom, double* Radius){
    if( (V18Pot<1||V18Pot>24) && (V18Pot<112||V18Pot>114) && (V18Pot<122||V18Pot>124) ){
        return 0;
    }
    if(!fV18pot){
        fV18pot = new DLM_StefanoPotentials* [30];
        for(unsigned uPot=0; uPot<30; uPot++){
            unsigned WhichPot = uPot+1;
            if(uPot==24) WhichPot=112;
            else if(uPot==24+1) WhichPot=113;
            else if(uPot==24+2) WhichPot=114;
            else if(uPot==24+3) WhichPot=122;
            else if(uPot==24+4) WhichPot=123;
            else if(uPot==24+5) WhichPot=124;
            fV18pot[uPot] = new DLM_StefanoPotentials(WhichPot);
        }
    }
    unsigned StefPotId = V18Pot-1;
    if(V18Pot==112) StefPotId=24;
    else if(V18Pot==113) StefPotId=25;
    else if(V18Pot==114) StefPotId=26;
    else if(V18Pot==122) StefPotId=27;
    else if(V18Pot==123) StefPotId=28;
    else if(V18Pot==124) StefPotId=29;
//printf("StefPotId=%i --> V=%f\n",StefPotId,fV18pot[StefPotId]->EvalCATS_v1_0(Radius[0],0));
    //return fV18pot[StefPotId]->Eval_PWprojector_pp(Radius[0],Spin,AngMom,TotMom,DlmPotFlag);
    return fV18pot[StefPotId]->Eval_PWprojector(Radius[0],IsoSpin,t2p1,t2p2,Spin,AngMom,TotMom,DlmPotFlag);
}



////////////////////////////////
//! pLambda

//(this is Oliver's version, not CRAB!)
double UsmaniPotentialOli(const int& Spin, double* Radius)
{

  double r = Radius[0];
  //Values for the potential
  const double vbar = 6.2;

  const double vsigma = 0.25;

  const double wc = 2137;

  double x=r*0.7;
  double vc = wc/(1+exp((r-0.5)/0.2));
  double tpi = (1.0+3.0/x+3.0/(x*x)) * (exp(-x)/x) * pow(1.-exp(-2.*r*r),2.);

  double v = 0.;

  if (Spin == 0) v = vc - (vbar + 0.75*vsigma)*tpi*tpi;//Usmani singlet
  else if (Spin == 1)  v = vc - (vbar - 0.25*vsigma)*tpi*tpi;//Usmani triplet
  else printf ("wrong polarization\n");

  return v;

}


////////////////////////////////////////////


double ppDlmPot(const int& DlmPot, const int& DlmFlag, const int& Spin, const int& AngMom, const int& TotMom, double* Radius){
    return fDlmPot(DlmPot,DlmFlag,1,1,1,Spin,AngMom,TotMom,Radius);
}

//[2] = DlmPot, [3] = DlmFlag
double ppDlmPot1S0(double* Pars){
    return ppDlmPot(Pars[2],Pars[3],0,0,0,Pars);
}
double ppDlmPot3S1(double* Pars){
    return ppDlmPot(Pars[2],Pars[3],1,0,1,Pars);
}
double ppDlmPot3P0(double* Pars){
    return ppDlmPot(Pars[2],Pars[3],1,1,0,Pars);
}
double ppDlmPot3P1(double* Pars){
    return ppDlmPot(Pars[2],Pars[3],1,1,1,Pars);
}
double ppDlmPot3P2(double* Pars){
    return ppDlmPot(Pars[2],Pars[3],1,1,2,Pars);
}
double ppDlmPot3P(double* Pars){
    return (ppDlmPot3P0(Pars)+ppDlmPot3P1(Pars)+ppDlmPot3P2(Pars))/3.;
}

double pLambdaDlmPot(const int& DlmPot, const int& DlmFlag, const int& Spin, const int& AngMom, const int& TotMom, double* Radius){
    return fDlmPot(DlmPot,DlmFlag,0,0,0,Spin,AngMom,TotMom,Radius);
}
//[2] is Pot Type, 3 is [3] is PotFlag
double pLambdaDlmPot1S0(double* Pars){
    return ppDlmPot(Pars[2],Pars[3],0,0,0,Pars);
}
double pLambdaDlmPot3S1(double* Pars){
    return ppDlmPot(Pars[2],Pars[3],1,0,1,Pars);
}

//t2p1 - 2xIsospin of particle 1, t2p2 same for particle 2
double fDlmPot(const int& DlmPot, const int& DlmPotFlag,
               const int& IsoSpin, const int& t2p1, const int& t2p2, const int& Spin, const int& AngMom, const int& TotMom, double* Radius){
    //printf("V=%f\n",fV18potential(9,Spin,AngMom,TotMom,Radius)) ;
    switch(DlmPot){
        case NN_AV18 : return fV18potential(9,DlmPotFlag,IsoSpin,t2p1,t2p2,Spin,AngMom,TotMom,Radius);
        case NN_ReidV8 : return fV18potential(2,DlmPotFlag,IsoSpin,t2p1,t2p2,Spin,AngMom,TotMom,Radius);
        case pp_ReidSC : return fReidDlm(Radius[0],Spin,AngMom,TotMom);
        case pp_ReidOli : return ReidSoftCore(Spin,Radius);
        case pp_ReidCrab : return fReidMeVfm(Radius[0],Spin);
        case pL_UsmaniOli : return UsmaniPotentialOli(Spin,Radius);
        default : return 0;
    }
}

//[0] radius, [1] momentum
//[2] PotentialType, [3] PotentialFlag,
//[4] IsoSpin, [5] ParticleType1 (1 proton, -1 neutron), [6] ParticleType2
//[7] Spin (s), [8] AngMom (l), [9] TotMom (j)
double fDlmPot(double* Parameters){
    return fDlmPot(round(Parameters[2]),round(Parameters[3]),round(Parameters[4]),round(Parameters[5]),
                   round(Parameters[6]),round(Parameters[7]),round(Parameters[8]),round(Parameters[9]),Parameters);
}


void GetDlmPotName(const int& potid, const int& potflag, char* name){
    double Radius=1;
    //fV18potential(1,0,0,0,&Radius);
    fV18potential(1,0,1,1,1,0,0,0,&Radius);
    switch(potid){
        case NN_AV18 :
            fV18pot[8]->PotentialName(9, name);
            break;
        case NN_ReidV8 :
            fV18pot[1]->PotentialName(2, name);
            break;
        case pp_ReidSC :
            strcpy(name,"Reid Soft-Core");
            break;
        case pp_ReidOli :
            strcpy(name,"Reid 3P2-3F2");
            break;
        case pp_ReidCrab :
            strcpy(name,"Reid 3P1");
            break;
        default :
            strcpy(name,"Unknown potential");
            break;
    }
    char Buffer[16];
    sprintf(Buffer, "%i", potflag);
    if(potflag!=0 && strcmp(name,"Unknown potential")){strcat(name, "^{(");strcat(name,Buffer);strcat(name,")}");}
}