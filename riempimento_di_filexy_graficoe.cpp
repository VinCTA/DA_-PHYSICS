#include <iostream>

#include <fstream>

#include <vector>


#include "TF1.h"
#include "provstat.h"
#include "TCanvas.h"

#include "TApplication.h"



#include "TGraph.h"
using namespace std ;



int main (int argc, char ** argv)

  {

    

    if (argc < 2)

      {

        cerr << "uso: " << argv[0] << " nomeFile [numero_di_eventi]" << endl ;

        exit (1) ;

      }

 ifstream input_fil ; 
input_fil.open (argv[3], ios::in) ;
    int numdatiy = -1 ;

    if (argc > 2) numdatiy  = atoi (argv[2]) ;
	int numei = atoi(argv [2]) ;

    statistiche datay (numei) ; 

    float input_valo ;

 while (true) 

      {

        input_fil >> input_valo ;

        if (input_fil.eof () == true) break ;

        datay.aggiungiNumero (input_valo) ;

        if (numdatiy > 0 && datay.N() == numdatiy) break ;

      } 

    input_fil.close () ;
//------------------------------------------------------



    ifstream input_file ; 

    input_file.open (argv[1], ios::in) ;



    int numeroMax = -1 ;

    if (argc > 2) numeroMax = atoi (argv[2]) ;// ci devoo srive 10000,che sono il n° di dati nel file.txt

    int numeri = atoi(argv [2]) ;

    statistiche data (numeri) ; 

    float input_val ;

    while (true) 

      {

        input_file >> input_val ;

        if (input_file.eof () == true) break ;

        data.aggiungiNumero (input_val) ;

        if (numeroMax > 0 && data.N() == numeroMax) break ;

      } 

    input_file.close () ;



    cout << "letti " << data.N() << " eventi" << endl ;

	double somma = 0. ;

	double media ; 

    double max ;

	for (int i= 0 ; i < data.N () ;i++ )

	{

		somma=somma+data.at(i) ;

		/*if( data[i]>>data[i-1] ) max = data[i] ;

		else continue ; */

		

		

	}

cout << "la media di data vale : "  <<  data.media() << " / e la somma vale :  " << somma  << endl ;



cout << "il 3° posto di data vale : "  <<  data.at(4) << endl ;   

//----------------------------------------------------------ò

        cout << "letti " << datay.N() << " eventi" << endl ;
	double somm = 0. ;

	double medi ; 

    double maxs ;

	for (int i= 0 ; i < datay.N () ;i++ )

	{

		somm=somm+datay.at(i) ;

		/*if( data[i]>>data[i-1] ) max = data[i] ;

		else continue ; */

		

		

	}

cout << "la media di datay vale : "  <<  datay.media() << " / e la somma vale :  " << somm  << endl ;



cout << "il 3° posto di datay vale : "  <<  datay.at(4) << endl ; 

//----------------------------------------------------

//facciamo il grafico per il punto 2
  TGraph giullare;
  for (int k = 0 ; k < numdatiy ; k++ )
  {
  giullare.SetPoint(k ,data.at(k) , datay.at(k)) ;
  
  
  }
  // dobbiamo fare il fit con la legge di Schokley
  TF1 fitta ("fitta", " [0] *( exp (*x)  ", 0., 10.) ;

fitta.SetParameter (0,par  ) ;

TFitResultPtr fit_ml =  isto.Fit("fitta", "RLS"); //fit eseguito con la MaximumLIkeli

double parvero = fitta.GetParameter(0) ; //parametro ottenuto dal fit,sarebbe Io

cout << "  Io vale  : " << fitta.GetParameter(0) << "\t+/- " << fitta.GetParError(0) << endl;



  
  
  
  
  
  // ho reso il codice interattivo 
TApplication theApp ("theApp", &argc, argv) ;

fitta.SetLineColor (kGray + 2) ;

fitta.SetLineWidth (1) ;

fitta.Draw ("same") ;

TCanvas c1("c1", "c1", 100, 100, 1000, 1000)  ;

giullare.Draw () ;

c1.SetLogx () ;

c1.Print ("sigmaTrend.png", "png") ;




giullare.SetMarkerStyle (20) ;

giullare.SetMarkerColor (kAzure - 1) ;

giullare.SetLineColor (kGray + 1) ;

giullare.SetMarkerSize (2) ;

giullare.GetHistogram ()->GetXaxis ()->SetTitle ("numero di eventi nel campione") ;

giullare.GetHistogram ()->GetYaxis ()->SetTitle ("deviazione standard") ;

  
theApp.Run () ;

return 0 ;

  

  }

 


	
