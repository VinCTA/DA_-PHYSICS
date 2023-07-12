//esperienza due: analisi dati per circuito RL in corrente impulsata

#include "TF1.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TFormula.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"


#include <cstring>
#include <iostream>
#include <fstream> 
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include "RL.h"

using namespace std;

int main (int argc, char ** argv)
{
   if (argc < 2)
   {
      cerr << "mancano parametri al programma: inserire il nome del file in fase di compilazione" << endl;
      return 1;
   }

   TApplication App ("App", NULL, NULL);
   TCanvas* CanvasR= new TCanvas ("CanvasR", "CanvasR", 0, 0, 700, 500);
   TCanvas* CanvasL= new TCanvas ("CanvasL", "CanvasL", 0, 0, 700, 500);//definisco due Canvas in cui fare il grafico dei punti

   //lettura file di dati e salvataggio su tre vettori, tempo, potenziale resistenza e potenziale induttanza
   vector <double> time, V_R, V_L; 
   
   ifstream infile (argv[1]);
   double t, resist, impede;
   while (true)
   {
      infile >> t >> impede >> resist;
      if (infile.eof() == true) break;//esci dal ciclo se non ho piu elementi
        
      time.push_back (t);
      V_R.push_back (resist);
      V_L.push_back (impede);
   }
   infile.close();//chiudo il file
	 
   gStyle->SetOptFit(1112);
   TGraphErrors* GraphR = new TGraphErrors ();//default constructor per i due grafici
   TGraphErrors* GraphL = new TGraphErrors (); 
	
   //creazione dei due grafici con ascissa tempo, ordinata potenziale e errore associato solo al potenziale di 100 mV
   double periodo = 0.0058; //definisco il periodo. rappresento i dati solo fino a questo valore. 
   double err_t = 0., err_v = 0.1; 
   for (int i = 0; time[i] < periodo; i++)
   {
      GraphR -> SetPoint (i, time [i], V_R [i]);
      GraphR -> SetPointError (i, err_t, err_v );

      GraphL -> SetPoint (i, time [i], V_L[i]);
      GraphL -> SetPointError (i, err_t, err_v);		
   }

   //fit dei dati

   int N_par = 2;
   TF1 R_carica ("R_carica", Resistenza_carica, 0, periodo, N_par);
   R_carica.SetParameter (0,9);
   R_carica.SetParName (0, "Vg");
   R_carica.SetParameter (1, 0.0058);
   R_carica.SetParName (1, "tau");

   /*TF1 R_scarica ("R_scarica", Resistenza_scarica, periodo/2, periodo, N_par);
   R_scarica.SetParameter (0, 5);
   R_scarica.SetParName (0, "Vg");
   R_scarica.SetParameter (1, 0.002);
   R_scarica.SetParName (1, "tau");*/

   
   TF1 L_carica ("L_carica", Induttanza_carica, 0, periodo, N_par);
   L_carica.SetParameter (0, 9);
   L_carica.SetParName (0, "Vg");
   L_carica.SetParameter (1, 0.0058);
   L_carica.SetParName (1, "tau");

  /* TF1 L_scarica ("L_scarica", Induttanza_scarica, periodo/2,periodo, N_par);
   L_scarica.SetParameter (0, 5);
   L_scarica.SetParName (0, "Vg");
   L_scarica.SetParameter (1, 0.002);
   L_scarica.SetParName (1, "tau");*/

   
	
   CanvasR -> cd();
   TFitResultPtr Results_Rc = GraphR -> Fit ("R_carica","S", "R", 0, periodo);//risultati fit carica R, "R" permette di fare il fit solo nella regione difinita dal range della funzione
   //TFitResultPtr Results_Rs = GraphR -> Fit ("R_scarica","S", "R", periodo/2, periodo); 

   GraphR -> SetMarkerStyle(7);    
   GraphR -> SetMarkerSize (0.7);
   GraphR -> SetMarkerColor (kBlue);
   GraphR -> SetTitle ("Potenziale Resistenza");
   GraphR -> GetXaxis() -> SetTitle ("Time s");
   GraphR -> GetYaxis() -> SetTitle ("Potenziale V");
   GraphR -> Draw ("AP");
   R_carica.Draw ("same");
   cout << "Resistenza carica"<< endl;
   Results_Rc -> Print ("V");
   TMatrixDSym cov = Results_Rc -> GetCovarianceMatrix(); 
   
   CanvasR -> Modified ();
   CanvasR -> Update ();
   
   CanvasL -> cd();

   TFitResultPtr Results_Lc = GraphL -> Fit ("L_carica", "S", "R", 0, periodo);//risultati fit carica L
   //TFitResultPtr Results_Ls = GraphL -> Fit ("L_scarica", "S", "R", 0.0101, periodo);//risultati fit carica L
   

   GraphL -> SetMarkerStyle(7);    
   GraphL -> SetMarkerSize (0.7);
   GraphL -> SetMarkerColor (kGreen);
   GraphL -> SetTitle ("Potenziale Induttanza");
   GraphL -> GetXaxis() -> SetTitle ("Time s");
   GraphL -> GetYaxis() -> SetTitle ("Potenziale V");
   GraphL -> Draw ("AP");
   //L_carica.Draw ("same");
   cout << "Induttanza carica"<< endl;
   Results_Lc -> Print ("V");
   //cout << "Induttanza scarica"<< endl;
   //Results_Ls -> Print ("V");
  

   //CanvasR -> Modified ();
   //CanvasR -> Update ();
   CanvasL -> Modified ();
   CanvasL -> Update ();
	
  //Induttanza noto che il valore della resistenza è di 10 ohm piu o meno 0.1 
  double R = 10., err_R = 0.1;
  double tau = R_carica.GetParameter (1);
  double L = R_carica.GetParameter(1) * R;
  double err_L = sqrt((R*R) * (cov(1,1) * cov(1,1))  +  tau*tau  *(err_R*err_R));
  
  cout << "Induttanza associata a resistenza: " << L << " +/- " << err_L << endl;


  App.Run();
		
	
  return 0; 
}
