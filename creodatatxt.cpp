/*
c++ -o main_03 `root-config --glibs --cflags` main_03.cpp
In pratica riempio un file che è vuoto e che passso io da linea di comando,
il nome del file lo scelgo io 

*/

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>

#include "TApplication.h"
#include "TF1.h"
#include "TH1F.h"
#include "TCanvas.h"

using namespace std ;


int main (int argc, char ** argv)
  {

    if (argc < 3)
      {
        cout << "usage: " << argv[0] << " outputfile.txt N_events" << endl ;
        exit (1) ;
      }

    double p0 = 30. ; // integrale
    double p1 = 10. ; // media
    double p2 = 2. ;  // sigma

    TF1 * model = new TF1 ("model", "gaus(0)", 0., 20.) ;
    model->SetParameter (0, p0) ;
    model->SetParameter (1, p1) ;
    model->SetParameter (2, p2) ;

    TH1F h_campione ("h_campione", "", 100, 0., 20.) ;

    ofstream f_campione ;
    f_campione.open (argv[1]) ; // sto aprendo il file passatogli da linea di comando 
    int N_eventi = atoi (argv[2]) ; // numero eventi che riempiranno il file 

    for (int i = 0 ; i < N_eventi ; ++i)
      {
        double event = model->GetRandom () ; 
        cout << "event vale : " << event << endl ;
		f_campione << event << "\n" ;
        /*Qui su ho dato il nome alla variabile ausiliaria event 
		che avrà valore di uno dei dati dell'oggetto model,non so quale 
		forse uno casuale */
		h_campione.Fill (event) ;
      }
    f_campione.close () ;
TApplication theApp ("theApp", &argc, argv) ;
    TCanvas c1 ("c1", "", 800, 800) ;
    c1.SetLeftMargin (0.15) ;
    h_campione.GetXaxis ()->SetTitle ("x") ;
    h_campione.GetYaxis ()->SetTitle ("eventi nel bin") ;
    h_campione.SetFillColor (kOrange + 1) ;
    h_campione.SetLineColor (kGray + 1) ;
    h_campione.SetStats (0) ;
    h_campione.Draw () ;
    c1.Print ("gauss.png", "png") ;
  theApp.Run () ;
    delete model ;
    return 0 ;
  }





















