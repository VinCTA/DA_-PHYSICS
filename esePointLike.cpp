/*
c++ -o esePointLike esePointLike.cpp `root-config --cflags --glibs`
*/

#include <iostream>
#include <fstream>
#include <cmath>

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TF1.h"

// #########################
// # Hard-coded parameters #
// #########################
#define MU      5.
#define SIGMA   0.5
#define TAU     5.
#define NPARAMS 3

using namespace std;


vector<double> data;


double pdf (double* x, double* par)
{
  double arg    = 0;
  double signal = 0;

  if (par[1] != 0)
    {
      arg    = (x[0] - par[0]) / par[1];
      signal = 1/sqrt(2*M_PI*par[1]*par[1]) * exp(-0.5*arg*arg);
    }

  double background = 1/par[2] * exp(-x[0]/par[2]);

  return signal + background;
}


double mLogLikelihood (const double* par)
{
  double logLike = 0;
  double pdfPar[NPARAMS];

  for (unsigned int i = 0; i < NPARAMS; i++) pdfPar[i] = par[i];

  for (unsigned int i = 0; i < data.size(); i++)
    {
      double x = data[i];
      logLike += log(pdf(&x,pdfPar));
    }

  return -logLike;
}


int main(int argc, char** argv)
{
  // ######################################################
  // # Code to generate data file for a single experiment #
  // ######################################################
  // int nSample = 10;
  // ofstream out;
  // out.open("data.txt",ios::out);
  // TF1* myFun = new TF1("myFun",pdf,0,50,3);
  // myFun->SetParameter(0,MU);
  // myFun->SetParameter(1,SIGMA);
  // myFun->SetParameter(2,TAU);
  // for (unsigned int i = 0; i < nSample; i++) out << myFun->GetRandom() << endl;
  // out.close();
  // return 0;


  if (argc < 2)
    {
      cout << "Digitare il nome dei file da riga di comando" << endl;
      cout << "\t./esePointLike data.txt" << endl;
      return 1;
    }


  // ##############################
  // # Code to read the data file #
  // ##############################
  double input;
  ifstream in;
  in.open(argv[1],ios::in);
  if (in.good() == false)
    {
      cout << "Problem opening the file: " << argv[1] << endl;
      return 1;
    }
  while (true)
    {
      in >> input;
      if (in.eof() == true) break;
      data.push_back(input);
    }
  in.close();


  // ####################################
  // # Prepare function to be minimized #
  // ####################################
  ROOT::Math::Minimizer* myLogLike = ROOT::Math::Factory::CreateMinimizer("Minuit2","Migrad");
  ROOT::Math::Functor _mLogLikelihood(&mLogLikelihood, NPARAMS);
  myLogLike->SetFunction(_mLogLikelihood);
  myLogLike->SetErrorDef(1.0); // Set scale for calculating the errors
  myLogLike->SetPrintLevel(3); // Set output printout details


  // ###################################################################
  // # Set parameter names, starting values and starting uncertainties #
  // ###################################################################
  myLogLike->SetVariable(0,"mu",    MU,    1);
  myLogLike->SetVariable(1,"sigma", SIGMA, 1);
  myLogLike->SetVariable(2,"tau",   TAU,   1);


  // #################
  // # Run minimizer #
  // #################
  myLogLike->Minimize();


  // ################################################################
  // # Retrieve the value of the parameters and their uncertainties #
  // ################################################################
  double fitStatus       = myLogLike->Status();
  double covMatrixStatus = myLogLike->CovMatrixStatus();
  // ####################################################################################################
  // # 0 = Not calculated at all                                                                        #
  // # 1 = Diagonal approximation only, not accurate                                                    #
  // # 2 = Full matrix, but forced positive-definite                                                    #
  // # 3 = Full accurate covariance matrix (after MIGRAD, this is the indication of normal convergence) #
  // ####################################################################################################

  double muhat     = myLogLike->X()[0];
  double muhat_err = myLogLike->Errors()[0];
  double muhat_errLo, muhat_errHi;
  myLogLike->GetMinosError(0, muhat_errLo, muhat_errHi);

  double sigmahat     = myLogLike->X()[1];
  double sigmahat_err = myLogLike->Errors()[1];
  double sigmahat_errLo, sigmahat_errHi;
  myLogLike->GetMinosError(1, sigmahat_errLo, sigmahat_errHi);

  double tauhat     = myLogLike->X()[2];
  double tauhat_err = myLogLike->Errors()[2];
  double tauhat_errLo, tauhat_errHi;
  myLogLike->GetMinosError(2, tauhat_errLo, tauhat_errHi);

  cout << "\nResults of the optimization" << endl;
  cout << "--> Fit status: " << fitStatus << " (0 = GOOD)" << endl;
  cout << "--> Covariance matrix status: " << covMatrixStatus << " (3 = FULL ACCURATE)" << endl;

  cout << "\n--> mu_hat = " << muhat << endl;
  cout << "\tUncertainty from parabolic, i.e. Hessian, appoximation: " << muhat_err << endl;
  cout << "\tUncertainty from Delta_log(Likelihood) = 0.5 projection: +" << muhat_errHi << " / " << muhat_errLo << endl;

  cout << "\n--> sigma_hat = " << sigmahat << endl;
  cout << "\tUncertainty from parabolic, i.e. Hessian, appoximation: " << sigmahat_err << endl;
  cout << "\tUncertainty from Delta_log(Likelihood) = 0.5 projection: +" << sigmahat_errHi << " / " << sigmahat_errLo << endl;

  cout << "\n--> tau_hat = " << tauhat << endl;
  cout << "\tUncertainty from parabolic, i.e. Hessian, appoximation: " << tauhat_err << endl;
  cout << "\tUncertainty from Delta_log(Likelihood) = 0.5 projection: +" << tauhat_errHi << " / " << tauhat_errLo << endl;


  return 0;
}
