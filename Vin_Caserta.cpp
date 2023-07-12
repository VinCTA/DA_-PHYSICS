#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// Vincenzo Caserta -> (Matr. 841895)

#include <TF1.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TFitResultPtr.h>
#include <TApplication.h>

//comando compilazione : c++ -o Vin_Caserta 'root-config --glibs --cflags ' fanzioni.cc Vin_Caserta.cpp

// inserire dopo il comando per l'output (/.Vin_Caserta) il nome del file di testo con elongazione -> .txt 
//Successivamente  il numero di dati contenuti, ovvero : 18 

 #include "fanzioni.h"

using namespace std ;



int main (int argc, char ** argv)

  {
     if (argc < 2)

      {

        cout << "uso: " << argv[0] << " nomeFile [numero_di_eventi]" << endl ;

        exit (1) ;

      }
    ifstream input_file ; 

    input_file.open (argv[1], ios::in) ;



    int numdaties= -1 ;

    if (argc > 2) numdaties = atoi (argv[2]) ;

   vector<double> daties ; // vector contenente i dati 

   double val ;

    while (true) 

      {

        input_file >> val ;

        if (input_file.eof () == true) break ;

        daties.push_back (val) ;

        if (numdaties > 0 && daties.size () == numdaties) break ;

      } 

    input_file.close () ;



    cout << "letti " << daties.size () << " eventi" << endl ;
 /*fino ad ora ho riempito un tipo vector,acquisendo i dati attraverso la scritta a linea di 
 comando del nome del file contenente i dati stessi e acquisendo come 2° carattere  il nuemro di dati conteneti,
 anche se quest'ultimo è un passaggio che poteva esser omesso grazie alla funzione size() di vector */
      

    double ausilio ;

	 for ( int f = 0 ; f < numdaties ; f++ )

	 {    

           ausilio = ausilio + daties.at(f);	 

	 }

	 

	 double media = ausilio/daties.size();

	 double par = 1/ media ;// k per inizializzare il fit di TF1


/*nella parte di programma sovrastante ho ricavato il valore della media dei dati e successivamente 
 un valore di inizializzazione per il k della distribuzione esponenziale,per fare il fit,facendo il reciproco
della media,poichè il tipo di distribuzione mi dice che il valore atteso è il reciproco di k  */


   TH1F isto ("isto", " dati.txt ", 18, -1., 11.) ;

   for (int i = 0 ; i <  numdaties ;i++) 

  {

    isto.Fill(daties.at(i)) ;

  } 
// qui su ho dichiarato un istogramma di tipo float e lo ho riempito
// con i dati che ci soo stati arrecati


TApplication theApp ("theApp", &argc, argv) ;

TF1 fitta ("fitta", " [0] * exp (-[0]*x)  ", 0., 10.) ;

fitta.SetParameter (0,par  ) ;

TFitResultPtr fit_ml =  isto.Fit("fitta", "RLS");

double parvero = fitta.GetParameter(0) ; //parametro ottenuto dal fit

cout << "  k vale  : " << fitta.GetParameter(0) << "\t+/- " << fitta.GetParError(0) << endl;

/*	qui su, oltre a rendere il codice interaativo tramite la funzione TApplication,ho
dichiarato la funzione esponenziale dataci nel testo esattamente nel punto 2,ho quindi fatto
il fit con il comando TFitResultPtr e ho ricavato il valore di k ,scritto a schermo,  con l'errore */

TCanvas can ("istogramma", "isto", 100, 100, 500, 500) ; // ho inizializzato il canvas per l'istogramma di tipo TH1F (isto)



fitta.SetLineColor (kGray + 2) ;

fitta.SetLineWidth (1) ;

fitta.Draw ("same") ;
//le tre righe qui sopra servono per disegnare la distribuzione nell istogramma chiamato isto 


isto.Draw () ;

can.Print ("esercizio8_integrale.png", "png") ; 
 //ho quindi disegnato tutto

//---------------------------------------------

// disegno il TGraph della loglikelihood in funzione del campionameto di k

TGraph logk ;


double varpar = parvero -0.107722; // minimo del k
// parvero è il k ottenuto dal precedente Fit
double maxpar = parvero +0.107722; // massimo del k

for (int p = 0 ; p < 100 ; p++)

{

logk.SetPoint (logk.GetN (), varpar,loglike (daties, varpar));

varpar =varpar +0.00215444;

}
//nel ciclo ho riempito il TGraph chiamato logk

//varpar = parvero -0.135204;

TCanvas can2 ("Loglike", "kappa", 100, 100, 500, 500) ; // serve per il TGraph dell log (likelihood) in funzione del k che varia 

//can2.SetLogx () ;





logk.SetMarkerStyle (21) ;

 

  logk.SetMarkerColor (kBlue + 1) ;

  logk.SetLineColor (kBlue - 9) ;

  logk.SetMarkerSize (2) ;

  logk.GetHistogram ()->GetXaxis ()->SetTitle ("k") ;

  logk.GetHistogram ()->GetYaxis ()->SetTitle ("log(likelihood)") ;

logk.Draw () ;

can2.Print ("andamento likelihood.png" "png") ;

// queste sopra sono tutte funzioni decorative per il canvas di logk, e poi il comando per disegnare

//----------------------------------------


// qui sotto ho infine calcolato il k con il matodo della sezione aurea
//in confronto al k precedente non c'è  un netto distacco 
 //double parametro ;

 

double max_loglik =  sezione_aurea_max( loglike,

   parvero -0.107722,

   parvero +0.107722,

  daties) ;

  

  cout << "il k ,calcolato attraverso la sezione aurea ,vale :" <<  max_loglik << endl ;



theApp.Run () ;







 

    return 0 ;

}