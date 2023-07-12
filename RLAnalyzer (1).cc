#include "Analyzer.h"
#include "TFitResult.h"

Analyzer::Analyzer ()
{
  dataNumber_p = 0;
  minX_p       = 0;
  maxX_p       = 0;

  meanX_p      = 0;
  stdDevX_p    = 0;

  xMeas_p.clear();
  yMeas_p.clear();
  xErr_p .clear();
  yErr_p .clear();

  histo_p = NULL;
  graph_p = NULL;
}

Analyzer::~Analyzer ()
{
  if(histo_p) delete histo_p;
  if(graph_p) delete graph_p;
}

bool Analyzer::setData (const string fileName, string type)
{
  ifstream inF(fileName.c_str());
  if (inF.good() == false)
    {
      cout << "[Analyzer::setData]\tError opening the file: " << fileName.c_str() << endl;
      return false;
    }

  double x = 0, y = 0, xErr = 0, yErr = 0, weightX = 0, weightY = 0, varX = 0, varY = 0;
  int n = 0;

  while (true)
    {
      if      (type == "counts"  )     inF >> x;
      else if (type == "measurements") inF >> x >> y >> xErr >> yErr;
      else
        {
          cout << "[Analyzer::setData]\tError: wrong option " << type.c_str() << endl;
          return false;
        }

      if (inF.eof() == true) break;
      xMeas_p.push_back(x);

      if (type == "measurements")
        {
          yMeas_p.push_back(y);
          xErr_p .push_back(xErr);
          yErr_p .push_back(yErr);
        }

      n++;
      if (x < minX_p) minX_p = x;
      if (x > maxX_p) maxX_p = x;
    }
  inF.close();
  dataNumber_p = n;

  int nBins = 100;
  if (type == "counts")
    {
      histo_p = new TH1D("Counts_Distribution","Counts distribution",nBins,minX_p,maxX_p);
      graph_p = NULL;

      for (int i = 0; i <= xMeas_p.size()-1; i++)
        histo_p->Fill(xMeas_p.at(i));

      Analyzer::computeMomenta(xMeas_p,xErr_p,meanX_p,stdDevX_p,meanError_p);
    }
  else
    {
      graph_p = new TGraphErrors(fileName.c_str());
      histo_p = NULL;
      graph_p->SetTitle("Circuito RL");
      graph_p->GetXaxis()->SetTitle("Tempo (s)");
      graph_p->GetYaxis()->SetTitle("Tensione ai capi della resistenza (V)");
      graph_p->SetMarkerStyle(20);
      graph_p->SetMarkerSize(0.5);
    }

  return true;
}

TH1D* Analyzer::getHisto (void)
{
  if (histo_p != NULL) return histo_p;
  else
    {
      cout << "[Analyzer::getHisto]\tError: if data type was -measurements- use getGraph method to get a TGraphErrors" << endl;
      return 0;
    }
}

TGraphErrors* Analyzer::getGraph (void)
{
  if (graph_p != NULL) return graph_p;
  else
    {
      cout << "[Analyzer::getGraph]\tError: if data type was -counts- use getHisto method to get a TH1D" << endl;
      return 0;
    }
}

void Analyzer::computeMomenta (vector<double>& values, vector<double>& errors, double& mean, double& stdDev, double& meanError)
{
  double var = 0, weight = 0, weightedMean = 0;

  mean = 0, stdDev = 0, meanError = 0;

  for (int i = 0; i <= values.size()-1; i++)
    {
      if (errors.empty() == false)
        {
          weightedMean += values.at(i) / pow(errors.at(i),2);
          weight       += 1.           / pow(errors.at(i),2);
        }

      mean =  mean + values.at(i);
      var  += pow(values.at(i),2);
    }

  mean         =  mean / values.size();
  var          = (var / values.size() - pow(mean,2)) * values.size() / (values.size()-1); // Unbiased estimator
  weightedMean /= weight;

  stdDev = sqrt(var);

  if (errors.empty() == true) meanError = stdDev / sqrt(dataNumber_p);
  else
    {
      mean      = weightedMean;
      meanError = 1. / sqrt(weight);
    }
}

void Analyzer::fitData (TF1& fitFunc, double xMin, double xMax)
{
  gStyle->SetOptFit(1112);

  if      (histo_p != NULL) histo_p->Fit(&fitFunc,"","",xMin,xMax);
  else if (graph_p != NULL) {TFitResultPtr r = graph_p->Fit(&fitFunc,"S","",xMin,xMax+0.03); r->Print("V");}

  else
    {
      cout << "[Analyzer::fitData]\tError: Histogram and Graph are both empty" << endl;
      exit(1);
    }
}

void Analyzer::computeChi2 (TF1& fitFunc, double& Chi2, int& NDF, double& pvalue)
{
  Chi2 = 0, NDF = 0, pvalue = 0;
  if (histo_p != NULL)
    {
      for (int i = 1; i <= histo_p->GetNbinsX(); i++)
        if (histo_p->GetBinContent(i) != 0)
          {
            Chi2 = Chi2 + pow((histo_p->GetBinContent(i) - fitFunc.Eval(histo_p->GetBinCenter(i))) / (histo_p->GetBinError(i)),2);
            NDF++; // Tiene conto dei soli bin NON vuoti
          }

      NDF -= fitFunc.GetNpar();
    }
  else if (graph_p != NULL)
    {
      for (int i = 0; i < graph_p->GetN(); i++)
        Chi2 = Chi2 + pow((graph_p->GetY()[i] - fitFunc.Eval(graph_p->GetX()[i])),2) / (pow(graph_p->GetErrorY(i),2) + pow(fitFunc.Derivative(graph_p->GetX()[i]) * graph_p->GetErrorX(i),2));

      NDF = graph_p->GetN() - fitFunc.GetNpar();
    }
  else
    {
      cout << "[Analyzer::computeChi2]\tError: Histogram and Graph are both empty" << endl;
      exit(1);
    }

  pvalue = TMath::Prob(Chi2,NDF);
}

bool Analyzer::testCompatibility (double& pvalue, double meas1, double err1, double meas2, double err2, double significance, string testType, double n1, double n2)
{
  double stat;

  if (testType == "Normal")
    {
      stat   = fabs((meas1 - meas2) / sqrt(err1*err1 + err2*err2));
      pvalue = TMath::Erfc(stat/sqrt(2.));
    }
  else if (testType == "tStudent")
    {
      if ((err1 != 0) && (err2 != 0))
        stat = fabs((meas1 - meas2) / sqrt(((n1-1.)*err1*err1 + (n2-1.)*err2*err2) / (n1+n2-2.) * (1./n1 + 1./n2)));
      else if ((err1 != 0) && (err2 == 0))
        {
          stat = fabs((meas1 - meas2) / (err1/sqrt(n1)));
          n2   = 1;
        }
      else if ((err1 == 0) && (err2 != 0))
        {
          stat = fabs((meas1 - meas2) / (err2/sqrt(n2)));
          n1   = 1;
        }
      pvalue = 2. * (1. - TMath::StudentI(stat,n1+n2-2.));
    }
  else
    {
      cout << "[Analyzer::testCompatibility]\tError: unknown test type " << testType.c_str() << endl;
      return false;
    }

  if (pvalue < significance) return false;
  else                       return true;
}

TGraph* Analyzer::computeContour (TF1& myFun, double delta, unsigned int parA, unsigned int parB)
{
  int    nPoints = 600;
  int    nSigma  = 2;
  double xMin    = myFun.GetParameter(parA) - nSigma * myFun.GetParError(parA);
  double yMin    = myFun.GetParameter(parB) - nSigma * myFun.GetParError(parB);
  double xStep   = 2.*nSigma*myFun.GetParError(parA) / nPoints;
  double yStep   = 2.*nSigma*myFun.GetParError(parB) / nPoints;
  double minChi2 = 0;
  double pvalue  = 0;
  int    NDF     = 0;

  Analyzer::computeChi2(myFun,minChi2,NDF,pvalue);

  double ContourLevel = minChi2 + delta;

  TGraph* least2Contour2D = new TGraph;
  least2Contour2D->GetXaxis()->SetTitle("Parameter A");
  least2Contour2D->GetYaxis()->SetTitle("Parameter B");
  least2Contour2D->SetMarkerStyle(20);
  least2Contour2D->SetMarkerSize(0.3);

  double par0, par1;
  double newChi2  = 0;
  unsigned int it = 0;

  for (unsigned int i = 0; i < nPoints; i++)
    {
      bool belowLevel = false;

      myFun.SetParameter(parA,xMin + xStep*i);
      for (unsigned int j = 0; j < nPoints; j++)
        {
          myFun.SetParameter(parB,yMin + yStep*j);
          Analyzer::computeChi2(myFun,newChi2,NDF,pvalue);

          if (((newChi2 - ContourLevel) < 0) && (belowLevel == false))
            {
              belowLevel = true;
              par0 = myFun.GetParameter(parA);
              par1 = myFun.GetParameter(parB);
              least2Contour2D->SetPoint(it,par0,par1);
              it++;
            }

          else if (((newChi2 - ContourLevel) > 0) && (belowLevel == true))
            {
              belowLevel = false;
              par0 = myFun.GetParameter(parA);
              par1 = myFun.GetParameter(parB);
              least2Contour2D->SetPoint(it,par0,par1);
              it++;
            }

          else continue;
        }
    }

  return least2Contour2D;
}
