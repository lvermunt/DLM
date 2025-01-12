
#include "TREPNI.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "DLM_CppTools.h"
#include "DLM_Histo.h"
#include "DLM_Random.h"
#include "CATStools.h"

TreParticle::TreParticle(TREPNI& database):Database(database){
  TreName = new char [Database.Len_PrtclName];
  Mass = new float [3];
  Width = new float [3];
  Abundance = new float [3];
  for(char i=0; i<3; i++){
    Mass[i]=0;Width[i]=0;Abundance[i]=0;
  }
  NumDecays = 0;
  Decay = NULL;
  MomPDF = NULL;
}

TreParticle::~TreParticle(){
  delete [] TreName; TreName=NULL;
  delete [] Mass; Mass=NULL;
  delete [] Width; Width=NULL;
  delete [] Abundance; Abundance=NULL;
  if(Decay){
    for(unsigned char uDec=0; uDec<NumDecays; uDec++){
      if(Decay[uDec]){delete Decay[uDec];Decay[uDec]=NULL;}
    }
    delete[]Decay;
    Decay=NULL;
  }
  if(MomPDF){
    delete MomPDF;
    MomPDF = NULL;
  }
}

void TreParticle::SetMomPDF(const DLM_Histo<float>& pdf){
  if(pdf.GetDim()>3){
    static bool ShowMessage=true;
    if(Database.PrintLevel>=1 && ShowMessage){
      printf("\033[1;31mERROR:\033[0m (TreParticle::SetMomPDF) The momentum distribution must have 1,2 or 3 dimensions\n");
    }
    return;
  }
  if(MomPDF){delete MomPDF; MomPDF = new DLM_Histo<float>(pdf);}
}

const DLM_Histo<float>* TreParticle::GetMomPDF() const{
  return MomPDF;
}

void TreParticle::FillMomXYZ(const float& xval, const float& yval, const float& zval){

}

void TreParticle::FillMomPtEtaPhi(const float& pt, const float& eta, const float& phi){

}

void TreParticle::FillMomPDF(CatsLorentzVector& cats_vector){

}


std::string TreParticle::GetName() const{
  return TreName;
}

float TreParticle::GetMass() const{
  return Mass[1];
}

float TreParticle::GetMassLow() const{
  return Mass[0];
}

float TreParticle::GetMassUp() const{
  return Mass[2];
}

float TreParticle::GetWidth() const{
  return Width[1];
}

float TreParticle::GetWidthLow() const{
  return Width[0];
}

float TreParticle::GetWidthUp() const{
  return Width[2];
}

float TreParticle::GetAbundance() const{
  return Abundance[1];
}

float TreParticle::GetAbundanceLow() const{
  return Abundance[0];
}

float TreParticle::GetAbundanceUp() const{
  return Abundance[2];
}

unsigned char TreParticle::GetNumDecays() const{
  return NumDecays;
}

const TREPNI* TreParticle::GetDatabase() const{
  return &Database;
}

void TreParticle::AppendInBinary(std::ofstream& file){

}

void TreParticle::LoadFromBinary(std::ifstream& file){

}

void TreParticle::SetName(const char* name){
  if(strlen(name)>=Database.Len_PrtclName){
    static bool ShowMessage=true;
    printf("\033[1;33mWARNING:\033[0m (TreParticle::SetName) The name of the particle is capped at %u characters\n",
    unsigned(Database.Len_PrtclName));
    if(Database.SingleError) ShowMessage=false;
    return;
  }
  strcpy(TreName,name);
}

void TreParticle::SetMass(const float& mass){
  Mass[1] = mass;
  if(Mass[0]==Mass[2] && Mass[0]==0){
    Mass[0] = Mass[1];
    Mass[2] = Mass[1];
  }
}

void TreParticle::SetMassLimit(const float& mass_low, const float& mass_up){
  Mass[0] = mass_low;
  Mass[2] = mass_up;
  if(Mass[0]==Mass[2] && Mass[0]==0){
    Mass[0] = Mass[1];
    Mass[2] = Mass[1];
  }
}

void TreParticle::SetWidth(const float& width){
  Width[1] = width;
  if(Width[0]==Width[2] && Width[0]==0){
    Width[0] = Width[1];
    Width[2] = Width[1];
  }
}

void TreParticle::SetWidthLimit(const float& width_low, const float& width_up){
  Width[0] = width_low;
  Width[2] = width_up;
  if(Width[0]==Width[2] && Width[0]==0){
    Width[0] = Width[1];
    Width[2] = Width[1];
  }
}

void TreParticle::SetAbundance(const float& abundance){
  Abundance[1] = abundance;
  if(Abundance[0]==Abundance[2] && Abundance[0]==0){
    Abundance[0] = Abundance[1];
    Abundance[2] = Abundance[1];
  }
}

void TreParticle::SetAbundanceLimit(const float& abundance_low, const float& abundance_up){
  Abundance[0] = abundance_low;
  Abundance[2] = abundance_up;
  if(Abundance[0]==Abundance[2] && Abundance[0]==0){
    Abundance[0] = Abundance[1];
    Abundance[2] = Abundance[1];
  }
}

TreChain* TreParticle::NewDecay(){
  ResizeArray(Decay,NumDecays,NumDecays+1);
  Decay[NumDecays] = new TreChain(*this);
  return Decay[NumDecays++];
}

TreChain* TreParticle::GetDecay(const unsigned char& whichone){
  return Decay[whichone];
}

void TreParticle::Print(){
  printf("--- Particle information ---\n");
  printf(" Name : %s\n",TreName);
  printf(" Mass : %e +(%e) -(%e)\n", Mass[1], Mass[2]-Mass[1], Mass[1]-Mass[0]);
  printf(" Width: %e +(%e) -(%e)\n", Mass[1], Width[2]-Width[1], Width[1]-Width[0]);
  printf(" Abund: %e +(%e) -(%e)\n", Abundance[1], Abundance[2]-Abundance[1], Abundance[1]-Abundance[0]);
  printf(" #decs: %u\n", NumDecays);
  for(unsigned char uDec=0; uDec<NumDecays; uDec++){
    printf("  -> ");
    for(unsigned char uDaugh=0; uDaugh<Decay[uDec]->NumDaughters; uDaugh++){
      if(uDaugh) printf(" ");
      printf("%s", Decay[uDec]->Daughter[uDaugh]->TreName);
    }
    printf("\n");
  }
}

TreChain::TreChain(TreParticle& mother):Mother(mother){
  NumDaughters = 0;
  Daughter = NULL;
  Branching = new float [3];
  Branching[0]=0;
  Branching[1]=0;
  Branching[2]=0;
}
TreChain::~TreChain(){
  if(Daughter){delete[]Daughter;Daughter=NULL;NumDaughters=0;}
  delete [] Branching;
}

void TreChain::AddDaughter(const TreParticle& daughter){
  ResizeArray(Daughter,NumDaughters,NumDaughters+1);
  Daughter[NumDaughters] = &daughter;
  NumDaughters++;
}

std::string TreChain::GetName(){
  std::string name;
  name = Mother.GetName();
  name += " -> ";
  for(unsigned char uDaugh=0; uDaugh<NumDaughters; uDaugh++){
    if(uDaugh) name += " ";
    name += Daughter[uDaugh]->GetName();
  }
  return name;
}

void TreChain::SetBranching(const float& br){
  Branching[1] = br;
  if(Branching[0]==Branching[2] && Branching[0]==0){
    Branching[0] = Branching[1];
    Branching[2] = Branching[1];
  }
}

void TreChain::SetBranchingLimit(const float& br_low, const float& br_up){
  Branching[0] = br_low;
  Branching[2] = br_up;
  if(Branching[0]==Branching[2] && Branching[0]==0){
    Branching[0] = Branching[1];
    Branching[2] = Branching[1];
  }
}

//void TreChain::SetDaughters(const unsigned char& numdaughters, const TreParticle* daughter){
//  if(Daughters){delete[]Daughters;Daughters=NULL;NumDaughters=0;}
//  Daughters = new TreParticle* [numdaughters];
//  NumDaughters = numdaughters;
//  for(unsigned char uch=0; uch<NumDaughters; uch++){
//    Daughters[uch] = daughter[uch];
//  }
//}

TREPNI::TREPNI(const unsigned short& version):Len_DtbsName(32),Len_PrtclName(24),
Version(version),MaxMemSteps(1024),NumFunctions(64){

  Particle = NULL;
  DatabaseName = new char [Len_DtbsName];
  NumParticles = 0;
  MaxParticles = 0;
  PrintLevel = 2;
  SingleError = true;
  ErrorOccured = new int[NumFunctions];
  for(short us=0; us<NumFunctions; us++) ErrorOccured[us] = 0;
  QA_passed = false;
  TotAbundance = 0;
  RanGen = new DLM_Random(1);
}

TREPNI::~TREPNI(){
  delete [] DatabaseName; DatabaseName=NULL;
  delete [] ErrorOccured; ErrorOccured=NULL;
  if(Particle){
    for(unsigned uPart=0; uPart<MaxParticles; uPart++){
      if(Particle[uPart]){
        delete Particle[uPart];
        Particle[uPart] = NULL;
      }
    }
    delete [] Particle;
    Particle = NULL;
  }
  delete RanGen; RanGen=NULL;
}

bool TREPNI::QA(const int& type){
  bool qa = true;
  if(type<Full || type>Abundance){return false;}
  if(type==Full || type==Name){
    qa *= QA_Name();
  }
  if(type==Full || type==Daughters){
    qa *= QA_Daughters();
  }
  if(type==Full || type==Mass){
    qa *= QA_Mass();
  }
  if(type==Full || type==Width){
    qa *= QA_Width();
  }
  if(type==Full || type==BR){
    qa *= QA_BR();
  }
  if(type==Full || type==Abundance){
    qa *= QA_Abundance();
  }
  QA_passed = qa;
  return qa;
}

bool TREPNI::QA_Name(){
  bool AllesGut = true;
  for(unsigned uPart=0; uPart<NumParticles; uPart++){
    for(unsigned uPart2=uPart+1; uPart2<NumParticles; uPart2++){
      if(strcmp(Particle[uPart]->TreName, Particle[uPart2]->TreName)==0){
        static bool ShowMessage=true;
        if(PrintLevel>=1 && ShowMessage){
          printf("\033[1;31mERROR:\033[0m (TREPNI::QA) Multiple instances of particle '%s'\n",Particle[uPart]->TreName);
          if(SingleError) ShowMessage=false;
        }
        AllesGut = false;
      }
    }
    if(strcmp(Particle[uPart]->TreName,"")==0){
      static bool ShowMessage=true;
      if(PrintLevel>=1 && ShowMessage){
        printf("\033[1;31mERROR:\033[0m (TREPNI::QA) The name of the particle cannot be blank\n");
        if(SingleError) ShowMessage=false;
      }
      AllesGut = false;
    }
    if(strncmp(Particle[uPart]->TreName,"anti_",5)==0){
      static bool ShowMessage=true;
      //printf("\033[1;35m%s %i\033[0m\n",name,int(ShowMessage));
      if(PrintLevel>=1 && ShowMessage){
        printf("\033[1;31mERROR:\033[0m (TREPNI::QA) Keyword 'anti_' is designated for anti-particles, "
        "which are auto-generated. Please only define particles!\n");
        if(SingleError) ShowMessage=false;
      }
      AllesGut = false;
    }
    for(unsigned uChar=0; uChar<strlen(Particle[uPart]->TreName); uChar++){
      if( strncmp(&Particle[uPart]->TreName[uChar]," ",1)==0 ||
          strncmp(&Particle[uPart]->TreName[uChar],",",1)==0 ||
          strncmp(&Particle[uPart]->TreName[uChar],".",1)==0 ||
          strncmp(&Particle[uPart]->TreName[uChar],";",1)==0 ||
          strncmp(&Particle[uPart]->TreName[uChar],"\"",1)==0 ||
          strncmp(&Particle[uPart]->TreName[uChar],"'",1)==0 ||
          strncmp(&Particle[uPart]->TreName[uChar],"\n",1)==0
        ){
        static bool ShowMessage=true;
        if(PrintLevel>=1 && ShowMessage){
          printf("\033[1;31mERROR:\033[0m (TREPNI::QA) The special characters , . ; empty spaces "
          "new lines or quation marks are not allowed within the naming convention.\n");
          if(SingleError) ShowMessage=false;
        }
        AllesGut = false;
      }
    }
  }
  return AllesGut;
}

bool TREPNI::QA_Daughters(){
  bool AllesGut = true;
  for(unsigned uPart=0; uPart<NumParticles; uPart++){
    for(unsigned char uDec=0; uDec<Particle[uPart]->NumDecays; uDec++){
      if(Particle[uPart]->Decay[uDec]->NumDaughters < 2){
        static bool ShowMessage=true;
        if(PrintLevel>=1 && ShowMessage){
          printf("\033[1;31mERROR:\033[0m (TREPNI::QA) The particle '%s' has a decay channel with less than 2 daughters\n",
          Particle[uPart]->TreName);
          if(SingleError) ShowMessage=false;
        }
        AllesGut = false;
      }
    }
  }
  return AllesGut;
}

bool TREPNI::QA_Mass(){
  float MotherMass;
  float MotherLow;
  float MotherUp;
  float DaughtersMass;
  bool AllesGut = true;
  for(unsigned uPart=0; uPart<NumParticles; uPart++){
    MotherMass = Particle[uPart]->Mass[1];
    MotherLow = Particle[uPart]->Mass[0];
    MotherUp = Particle[uPart]->Mass[2];
    for(unsigned char uDec=0; uDec<Particle[uPart]->NumDecays; uDec++){
      DaughtersMass = 0;
      for(unsigned char uDaugh=0; uDaugh<Particle[uPart]->Decay[uDec]->NumDaughters; uDaugh++){
        DaughtersMass += Particle[uPart]->Decay[uDec]->Daughter[uDaugh]->Mass[1];
      }
      if(MotherMass<DaughtersMass){
        static bool ShowMessage=true;
        if(PrintLevel>=1 && ShowMessage){
          printf("\033[1;31mERROR:\033[0m (TREPNI::QA) The decay %s is impossible (Mass checksum)\n",
          Particle[uPart]->Decay[uDec]->GetName().c_str());
          if(SingleError) ShowMessage=false;
        }
        AllesGut = false;
      }
    }
    if(MotherMass<0){
      static bool ShowMessage=true;
      if(PrintLevel>=1 && ShowMessage){
        printf("\033[1;31mERROR:\033[0m (TREPNI::QA) %s has negative mass\n",
        Particle[uPart]->TreName);
        if(SingleError) ShowMessage=false;
      }
      AllesGut = false;
    }
    if(MotherLow>MotherUp || MotherLow<0){
      static bool ShowMessage=true;
      if(PrintLevel>=1 && ShowMessage){
        printf("\033[1;31mERROR:\033[0m (TREPNI::QA) Invalid mass range for %s\n",
        Particle[uPart]->TreName);
        if(SingleError) ShowMessage=false;
      }
      AllesGut = false;
    }
    if(MotherMass<MotherLow || MotherMass>MotherUp){
      static bool ShowMessage=true;
      if(PrintLevel>=1 && ShowMessage){
        printf("\033[1;31mERROR:\033[0m (TREPNI::QA) The %s mass is outside of the allowed range\n",
        Particle[uPart]->TreName);
        if(SingleError) ShowMessage=false;
      }
      AllesGut = false;
    }
  }
  return AllesGut;
}

bool TREPNI::QA_Width(){
  float MotherWidth;
  float MotherLow;
  float MotherUp;
  bool AllesGut = true;
  for(unsigned uPart=0; uPart<NumParticles; uPart++){
    MotherWidth = Particle[uPart]->Width[1];
    MotherLow = Particle[uPart]->Width[0];
    MotherUp = Particle[uPart]->Width[2];
    if(MotherWidth<0){
      static bool ShowMessage=true;
      if(PrintLevel>=1 && ShowMessage){
        printf("\033[1;31mERROR:\033[0m (TREPNI::QA) %s has negative decay width\n",
        Particle[uPart]->TreName);
        if(SingleError) ShowMessage=false;
      }
      AllesGut = false;
    }
    if(MotherLow>MotherUp || MotherLow<0){
      static bool ShowMessage=true;
      if(PrintLevel>=1 && ShowMessage){
        printf("\033[1;31mERROR:\033[0m (TREPNI::QA) Invalid decay width range for %s\n",
        Particle[uPart]->TreName);
        if(SingleError) ShowMessage=false;
      }
      AllesGut = false;
    }
    if(MotherWidth<MotherLow || MotherWidth>MotherUp){
      static bool ShowMessage=true;
      if(PrintLevel>=1 && ShowMessage){
        printf("\033[1;31mERROR:\033[0m (TREPNI::QA) The %s decay width is outside of the allowed range\n",
        Particle[uPart]->TreName);
        if(SingleError) ShowMessage=false;
      }
      AllesGut = false;
    }
  }
  return AllesGut;
}

bool TREPNI::QA_BR(){
  bool AllesGut = true;
  const float TotalBR = 100;
  float MinBR = 0;
  float MaxBR = 0;
  float LowBR;
  float UpBR;
  float LowValue;
  float MeanValue;
  float UpValue;
  for(unsigned uPart=0; uPart<NumParticles; uPart++){
    if(Particle[uPart]->NumDecays==0) continue;
    MinBR = 0;
    MaxBR = 0;
    for(unsigned char uDec=0; uDec<Particle[uPart]->NumDecays; uDec++){
      LowValue = Particle[uPart]->Decay[uDec]->Branching[0];
      MeanValue = Particle[uPart]->Decay[uDec]->Branching[1];
      UpValue = Particle[uPart]->Decay[uDec]->Branching[2];
      if(MeanValue<0){
        static bool ShowMessage=true;
        if(PrintLevel>=1 && ShowMessage){
          printf("\033[1;31mERROR:\033[0m (TREPNI::QA) %s has negative BR\n",
          Particle[uPart]->Decay[uDec]->GetName().c_str());
          if(SingleError) ShowMessage=false;
        }
        AllesGut = false;
      }
      if(LowValue>UpValue || LowValue<0){
        static bool ShowMessage=true;
        if(PrintLevel>=1 && ShowMessage){
          printf("\033[1;31mERROR:\033[0m (TREPNI::QA) Invalid BR range for %s\n",
          Particle[uPart]->Decay[uDec]->GetName().c_str());
          if(SingleError) ShowMessage=false;
        }
        AllesGut = false;
      }
      if(MeanValue<LowValue || MeanValue>UpValue){
        static bool ShowMessage=true;
        if(PrintLevel>=1 && ShowMessage){
          printf("\033[1;31mERROR:\033[0m (TREPNI::QA) The %s BR is outside of the allowed range\n",
          Particle[uPart]->Decay[uDec]->GetName().c_str());
          if(SingleError) ShowMessage=false;
        }
        AllesGut = false;
      }
      MinBR += LowValue;
      MaxBR += UpValue;
    }
    LowBR = MinBR + 0.16*(MaxBR-MinBR);
    UpBR = MaxBR - 0.16*(MaxBR-MinBR);
    if(LowBR>TotalBR || UpBR<TotalBR){
      static bool ShowMessage=true;
      if(PrintLevel>=1 && ShowMessage){
        printf("\033[1;31mERROR:\033[0m (TREPNI::QA) The BRs of '%s' do not sum up to 100%%\n",Particle[uPart]->TreName);
        if(SingleError) ShowMessage=false;
      }
      AllesGut = false;
    }
  }
  return AllesGut;
}

bool TREPNI::QA_Abundance(){
  if(TotAbundance<=0){return true;}
  bool AllesGut = true;
  float MinAbund = 0;
  float MaxAbund = 0;
  float LowAbund;
  float UpAbund;
  for(unsigned uPart=0; uPart<NumParticles; uPart++){
    MinAbund += Particle[uPart]->Abundance[0];
    MaxAbund += Particle[uPart]->Abundance[2];
  }
  LowAbund = MinAbund + 0.16*(MaxAbund-MinAbund);
  UpAbund = MaxAbund - 0.16*(MaxAbund-MinAbund);
  if(LowAbund>TotAbundance || UpAbund<TotAbundance){
    static bool ShowMessage=true;
    if(PrintLevel>=1 && ShowMessage){
      printf("\033[1;31mERROR:\033[0m (TREPNI::QA) The total yield of all particles is outside the allowed limit\n");
      if(SingleError) ShowMessage=false;
    }
    AllesGut = false;
  }
  return AllesGut;
}

void TREPNI::SetTotalYield(const float& totyield){
  TotAbundance = totyield;
}

float TREPNI::GetYield() const{
  if(TotAbundance>0) return TotAbundance;
  float yield = 0;
  for(unsigned uPart=0; uPart<NumParticles; uPart++){
    yield += Particle[uPart]->GetAbundance();
  }
  return yield;
}

TreParticle* TREPNI::NewParticle(const char* name){
  ResizeArray(Particle,NumParticles,NumParticles+1);
  Particle[NumParticles] = new TreParticle(*this);
  if(name){Particle[NumParticles]->SetName(name);}
  return Particle[NumParticles++];
}

TreParticle* TREPNI::GetParticle(const unsigned& whichone) const{
  return Particle[whichone];
}

TreParticle* TREPNI::GetParticle(const char* name) const{
  for(unsigned uPart=0; uPart<NumParticles; uPart++){
    if(strcmp(Particle[uPart]->TreName,name)==0){
      return Particle[uPart];
    }
  }
  return NULL;
}

//for the sampling, some node structure for log performance would be nice
TreParticle* TREPNI::GetRandomParticle() const{
  const float Yield = GetYield();
  float RndYield = RanGen->Uniform(0,Yield);
  float Yield_Last = 0;
  float Yield_New = 0;
  for(unsigned uPart=0; uPart<NumParticles; uPart++){
    Yield_New += Particle[uPart]->GetAbundance();
    if(Yield_Last<=RndYield && RndYield<=Yield_New){
      return Particle[uPart];
    }
  }
  return NULL;
}

unsigned TREPNI::GetNumParticles(){
  return NumParticles;
}

void TREPNI::Randomize(){

}

void TREPNI::RandomizeMass(){

}

void TREPNI::RandomizeWidth(){

}

void TREPNI::RandomizeAbundance(){

}

void TREPNI::RandomizeBR(){

}

void TREPNI::SetSeed(const unsigned& seed){
  RanGen->SetSeed(seed);
}

void TREPNI::SetPrintLevel(const char& lvl, const bool& single){
  PrintLevel = lvl;
  if(PrintLevel<0) PrintLevel=0;
  if(PrintLevel>3) PrintLevel=3;
  SingleError = single;
}


/*
//FuntionID = 0
TREPNI::TREPNI(const unsigned short& version):Len_DtbsName(32),Len_PrtclName(24),
Version(version),MaxMemSteps(1024),NumFunctions(64),MaxDecayCh(16),MaxDaughters(8){
  DatabaseName = new char [Len_DtbsName];
  NumParticles = 0;
  MaxParticles = 0;
  PrintLevel = 2;
  SingleError = true;
  TrepName = NULL;
  Mass = NULL;
  Gamma = NULL;
  Nch = NULL;
  Ndaughter = NULL;
  Branching = NULL;
  DaughterID = NULL;
  ErrorOccured = new int[NumFunctions];
  for(short us=0; us<NumFunctions; us++) ErrorOccured[us] = 0;
}

//FuntionID = 1
TREPNI::~TREPNI(){
  delete [] DatabaseName;
  if(Mass){delete[]Mass;Mass=NULL;}
  if(Gamma){delete[]Gamma;Gamma=NULL;}
  if(Nch){delete[]Nch;Nch=NULL;}
  if(TrepName){
    for(unsigned uPart=0; uPart<MaxParticles; uPart++)
      if(TrepName[uPart]){delete[]TrepName[uPart];TrepName[uPart]=NULL;}
    delete[]TrepName;TrepName=NULL;
  }
  if(Ndaughter){
    for(unsigned uPart=0; uPart<MaxParticles; uPart++)
      if(Ndaughter[uPart]){delete[]Ndaughter[uPart];Ndaughter[uPart]=NULL;}
    delete[]Ndaughter;Ndaughter=NULL;
  }
  if(Branching){
    for(unsigned uPart=0; uPart<MaxParticles; uPart++)
      if(Branching[uPart]){delete[]Branching[uPart];Branching[uPart]=NULL;}
    delete[]Branching;Branching=NULL;
  }
  if(DaughterID){
    for(unsigned uPart=0; uPart<MaxParticles; uPart++){
      if(DaughterID[uPart]){
        for(char uDch=0; uDch<MaxDecayCh; uDch++){
          if(DaughterID[uPart][uDch])
          {delete[]DaughterID[uPart][uDch];DaughterID[uPart][uDch]=NULL;}
        }
        delete[]DaughterID[uPart];DaughterID[uPart]=NULL;
      }
    }
    delete[]DaughterID;DaughterID=NULL;
  }

  delete [] ErrorOccured;
}

void TREPNI::MemoryManager(const bool& destroy){
  if(NumParticles<MaxParticles){return;}

  unsigned NewSlots=0;
  unsigned TotSlots=0;
  if(!destroy){
    NewSlots = MaxParticles;
    if(NewSlots==0)NewSlots=1;
    if(NewSlots>MaxMemSteps) NewSlots=MaxMemSteps;
    TotSlots = MaxParticles+NewSlots;
  }

  ResizeArray(TrepName,MaxParticles,TotSlots);
  ResizeArray(Mass,3*MaxParticles,3*TotSlots);
  ResizeArray(Gamma,3*MaxParticles,3*TotSlots);
  ResizeArray(Nch,MaxParticles,TotSlots);
  ResizeArray(Ndaughter,MaxParticles,TotSlots);
  ResizeArray(Branching,MaxParticles,TotSlots);
  ResizeArray(DaughterID,MaxParticles,TotSlots);
  for(unsigned uPart=MaxParticles; uPart<TotSlots; uPart++){
    TrepName[uPart] = new char [24];
    Ndaughter[uPart] = new int [MaxDecayCh];
    Branching[uPart] = new float [3*MaxDecayCh];
    DaughterID[uPart] = new int* [MaxDecayCh];
    for(unsigned uDch=0; uDch<MaxDecayCh; uDch++){
      DaughterID[uPart][uDch] = new int [MaxDaughters];
    }
  }
  MaxParticles = TotSlots;
}

std::string TREPNI::GetParticleName(const int& id) const{
  int ErrorID=0;
  int LineID = fabs(id)-1;
  if(id==0){
    if(PrintLevel>=1){
      ErrorID=0;
      if( (ErrorOccured[getparticlename]&(1 << ErrorID))==0 ){
        printf("\033[1;31mERROR:\033[0m (TREPNI::GetParticleName) The particle numbering (ID) starts from 1\n");
        //occupy the bit in case we dont want to repeat this error
        if(SingleError){
          ErrorOccured[getparticlename] ^= (1 << ErrorID);
        }
      }
    }
    return "";
  }

  if(LineID>=NumParticles&&NumParticles){
    if(PrintLevel>=2){
      //if the corresponding bit (0 in this example)
      //is zero, than we go ahead and print out the error
      ErrorID=1;
      if( (ErrorOccured[getparticlename]&(1 << ErrorID))==0 ){
        printf("\033[1;33mWARNING:\033[0m (TREPNI::GetParticleName) There are only %u number of particles defined\n",NumParticles);
        //occupy the bit in case we dont want to repeat this error
        if(SingleError){
          ErrorOccured[getparticlename] ^= (1 << ErrorID);
        }
      }
    }
  }
  else if(LineID>=MaxParticles){
    if(PrintLevel>=1){
      ErrorID=2;
      if( (ErrorOccured[getparticlename]&(1 << ErrorID))==0 ){
        printf("\033[1;31mERROR:\033[0m (TREPNI::GetParticleName) There are only %u number of particles defined and %u of maximum slots\n",NumParticles,MaxParticles);
        //occupy the bit in case we dont want to repeat this error
        if(SingleError){
          ErrorOccured[getparticlename] ^= (1 << ErrorID);
        }
      }
    }
    return "";
  }
  std::string str;
  str = TrepName[LineID];
  if(id<0) str.insert(0,"anti_");
  return str;
}

int TREPNI::GetParticleId(const char* name) const{
  char* search_name = new char [29];
  int Particle = 1;
  if(strncmp(name,"anti_",5)==0){
    strcpy(search_name,&name[5]);
    Particle = -1;
  }
  else{
    strcpy(search_name,name);
  }

  for(unsigned uPart=0; uPart<NumParticles; uPart++){
    if(strcmp(search_name,TrepName[uPart])==0){
      delete [] search_name;
      return Particle*int(uPart+1);
    }
  }

  if(PrintLevel>=2){
    int ErrorID=0;
    if( (ErrorOccured[getparticleid]&(1 << ErrorID))==0 ){
      printf("\033[1;33mWARNING:\033[0m (TREPNI::GetParticleId) The particle %s does not exist\n",name);
      //occupy the bit in case we dont want to repeat this error
      if(SingleError){
        ErrorOccured[getparticleid] ^= (1 << ErrorID);
      }
    }
  }
  delete [] search_name;
  return 0;
}


void TREPNI::SetPrintLevel(const char& lvl, const bool& single){
  PrintLevel = lvl;
  if(PrintLevel<0) PrintLevel=0;
  if(PrintLevel>3) PrintLevel=3;
  SingleError = single;
}

void TREPNI::SetParticle(const char* name, const double& mass_min, const double& mass_max,
                          const double& gamma_min, const double& gamma_max){

  int ErrorID;

  if(strcmp(name,"")==0){
    if(PrintLevel>=1){
      ErrorID=0;
      if( (ErrorOccured[setparticle]&(1 << ErrorID))==0 ){
        printf("\033[1;31mERROR:\033[0m (TREPNI::SetParticle) The name of the particle cannot be black\n");
        if(SingleError) ErrorOccured[setparticle] ^= (1 << ErrorID);
      }
    }
    return;
  }
  if(strncmp(name,"anti_",5)==0){
    if(PrintLevel>=1){
      ErrorID=1;
      if( (ErrorOccured[setparticle]&(1 << ErrorID))==0 ){
        printf("\033[1;31mERROR:\033[0m (TREPNI::SetParticle) Keyword 'anti_' is designated for anti-particles, "
        "which are auto-generated. Please only define particles!\n");
        if(SingleError) ErrorOccured[setparticle] ^= (1 << ErrorID);
      }
    }
    return;
  }
  for(unsigned uChar=0; uChar<strlen(name); uChar++){
    if( strncmp(&name[uChar]," ",1)==0 ||
        strncmp(&name[uChar],",",1)==0 ||
        strncmp(&name[uChar],".",1)==0 ||
        strncmp(&name[uChar],";",1)==0 ||
        strncmp(&name[uChar],"\"",1)==0 ||
        strncmp(&name[uChar],"'",1)==0 ||
        strncmp(&name[uChar],"\n",1)==0
      ){
      if(PrintLevel>=1){
        ErrorID=2;
        if( (ErrorOccured[setparticle]&(1 << ErrorID))==0 ){
          printf("\033[1;31mERROR:\033[0m (TREPNI::SetParticle) The special characters , . ; empty spaces "
          "new lines or quation marks are not allowed within the naming convention.\n");
          if(SingleError) ErrorOccured[setparticle] ^= (1 << ErrorID);
        }
      }
      return;
    }
  }
  if(mass_min<0||mass_min!=mass_min){
    if(PrintLevel>=1){
      ErrorID=3;
      if( (ErrorOccured[setparticle]&(1 << ErrorID))==0 ){
        printf("\033[1;31mERROR:\033[0m (TREPNI::SetParticle) The mass of the particle is either negative or n/a\n");
        if(SingleError) ErrorOccured[setparticle] ^= (1 << ErrorID);
      }
    }
    return;
  }
  if(mass_max<0||mass_max!=mass_max){
    if(PrintLevel>=1){
      ErrorID=3;
      if( (ErrorOccured[setparticle]&(1 << ErrorID))==0 ){
        printf("\033[1;31mERROR:\033[0m (TREPNI::SetParticle) The mass of the particle is either negative or n/a\n");
        if(SingleError) ErrorOccured[setparticle] ^= (1 << ErrorID);
      }
    }
    return;
  }
  if(mass_max<mass_min){
    if(PrintLevel>=1){
      ErrorID=4;
      if( (ErrorOccured[setparticle]&(1 << ErrorID))==0 ){
        printf("\033[1;31mERROR:\033[0m (TREPNI::SetParticle) The lower limit of the mass is larger than the upper limit\n");
        if(SingleError) ErrorOccured[setparticle] ^= (1 << ErrorID);
      }
    }
    return;
  }

  if(gamma_min<0||gamma_min!=gamma_min){
    if(PrintLevel>=1){
      ErrorID=5;
      if( (ErrorOccured[setparticle]&(1 << ErrorID))==0 ){
        printf("\033[1;31mERROR:\033[0m (TREPNI::SetParticle) The width of the particle is either negative or n/a\n");
        if(SingleError) ErrorOccured[setparticle] ^= (1 << ErrorID);
      }
    }
    return;
  }
  if(gamma_max<0||gamma_max!=gamma_max){
    if(PrintLevel>=1){
      ErrorID=5;
      if( (ErrorOccured[setparticle]&(1 << ErrorID))==0 ){
        printf("\033[1;31mERROR:\033[0m (TREPNI::SetParticle) The width of the particle is either negative or n/a\n");
        if(SingleError) ErrorOccured[setparticle] ^= (1 << ErrorID);
      }
    }
    return;
  }
  if(gamma_max<gamma_min){
    if(PrintLevel>=1){
      ErrorID=6;
      if( (ErrorOccured[setparticle]&(1 << ErrorID))==0 ){
        printf("\033[1;31mERROR:\033[0m (TREPNI::SetParticle) The lower limit of the width is larger than the upper limit\n");
        if(SingleError) ErrorOccured[setparticle] ^= (1 << ErrorID);
      }
    }
    return;
  }

  //stop here if you find a particle with that name (rewrite it, no memory update)
  //btw, if we have already defined decay channels, we might get into a conflict by
  //changing the mass to an unrealistic value. Best implement a QA function
  //to be able to run whenever you save to a file
  for(unsigned uPart=0; uPart<NumParticles; uPart++){
    if(strcmp(name,TrepName[uPart])==0){
      Mass[3*uPart] = mass_min;
      Mass[3*uPart+1] = (mass_min+mass_max)*0.5;
      Mass[3*uPart+2] = mass_max;

      Gamma[3*uPart] = gamma_min;
      Gamma[3*uPart+1] = (gamma_min+gamma_max)*0.5;
      Gamma[3*uPart+2] = gamma_max;
      return;
    }
  }

  int id = NumParticles++;
  MemoryManager();

  strcpy(TrepName[id],name);

  Mass[3*id] = mass_min;
  Mass[3*id+1] = (mass_min+mass_max)*0.5;
  Mass[3*id+2] = mass_max;

  Gamma[3*id] = gamma_min;
  Gamma[3*id+1] = (gamma_min+gamma_max)*0.5;
  Gamma[3*id+2] = gamma_max;
}

*/
