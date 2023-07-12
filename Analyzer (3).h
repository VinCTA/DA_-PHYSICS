#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
#include <vector>
#include <fstream>

#include "TStyle.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TMath.h"

using namespace std;

class Analyzer
{
public:

  // Default constructor
  Analyzer ();

  // Destrusctor (!!! delete pointers if present !!!)
  ~Analyzer ();

  // Read data from file and fill plot and vectors (TH1D for type  "counts" , TGraphErrors for type "measurements")
  bool setData (const string fileName, string type);

  // Compute mean, standard deviation and mean error (use weighted mean and opportune error if errors are given)
  void computeMomenta (vector<double>& values, vector<double>&  errors, double& mean, double& stdDev, double& meanError);

  // Compute Chi-2, NDF and pValue when fitFunc is used to fit the TH1D or the TGrapgErrors
  void computeChi2 (TF1& fitFunc, double& chi2, int& NDF, double& pValue);

  // Fit the TH1D or TGraphErrors with a given function in a specific range
  void fitData (TF1& fitFunc, double xMin, double xMax);

  // Test whether two measurements are compatible using a statistical test set with testType: "Normal" or "tStudent"

  // Example of Normal test with significance = 5%
  //   two measurements M1+\-sigma1 and M2+\-sigma2
  //   testCompatibility (pvalue, M1, sigma1, M2, sigma2, 0.05, "Normal")

  // Example of t-Student test with significance = 5%
  //   two measurements estimated from two sets of data:
  //     - n1 measurements with average M1 and standard deviation of the n1 measurements sigma1
  //     - n2 measurements with average M2 and standard deviation of the n2 measurements sigma2
  //   testCompatibility (pvalue, M1, sigma1, M2, sigma2, 0.05, "tStudent", n1, n2)

  // Example of t-Student test with significance = 5%
  //   one measurement estimated from a set of data:
  //     - n1 measurements with average M1 and standard deviation of the n1 measurements sigma1
  //   a reference value M2
  //   testCompatibility (pvalue, M1, sigma1, M2, 0, 0.05, "tStudent", n1)
  bool testCompatibility (double& pvalue, double meas1, double err1, double meas2, double err2, double significance, string testType, double n1 = 1, double n2 = 1);

  // Produce contour plot for DMinChi2 = delta
  TGraph* computeContour (TF1& myFun, double delta, unsigned int parA = 0, unsigned int parB = 1);

  TH1D        *   getHisto      (void);
  TGraphErrors*   getGraph      (void);
  double          getMeanX      (void) { return meanX_p     ;}
  double          getMinX       (void) { return minX_p      ;}
  double          getMaxX       (void) { return maxX_p      ;}
  double          getStdDevX    (void) { return stdDevX_p   ;}
  double          getMeanError  (void) { return meanError_p ;}
  int             getDataNumber (void) { return dataNumber_p;}

  vector<double>* getxMeas      (void) { return &xMeas_p    ;}
  vector<double>* getyMeas      (void) { return &yMeas_p    ;}
  vector<double>* getxErr       (void) { return &xErr_p     ;}
  vector<double>* getyErr       (void) { return &yErr_p     ;}

  // Data members
private:

  int             dataNumber_p;
  double          minX_p;
  double          maxX_p;
  double          meanX_p;
  double          stdDevX_p;
  double          meanError_p;
  vector <double> xMeas_p;
  vector <double> yMeas_p;
  vector <double> xErr_p;
  vector <double> yErr_p;

  TH1D*         histo_p;
  TGraphErrors* graph_p;
};
#endif
