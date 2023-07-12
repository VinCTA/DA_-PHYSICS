#include <iostream>

#include <fstream>

#include <vector>

#include "provstat.h"
#include "TCanvas.h"
#include "TApplication.h"

#include "TH1F.h"

using namespace std ;



int main (int argc, char ** argv)

  {

    

    if (argc < 2)

      {

        cerr << "uso: " << argv[0] << " nomeFile [numero_di_eventi]" << endl ;

        exit (1) ;

      }



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
	double media = data.media()
cout << "la media di data vale : "  <<  data.media() << " / e la somma vale :  " << media << endl ;
 TH1F istogramma ("istogramma", "dati_filetxt ",10000,0. ,5 * media ) ;
 	//il numero di bin (,10000 in questo caso) in cui dividere l'intervallo di definizione della variabile
    //l'intervallo di definizione della variabile (-5., 5. in questo caso)

cout << "il 4° posto di data vale : "  <<  data.at(4) << endl ;
for(int j = 0 ; j < numeri  ; j++) istgrama.Fill(data.at(j)) ;

TApplication theApp ("theApp", NULL, NULL) ;
TCanvas c1 ;
istogramma.Draw () ;
istogramma.SetFillColor (kOrange + 1) ;
istogramma.GetXaxis ()->SetTitle ("asse x") ;
istogramma.GetYaxis ()->SetTitle ("conteggi per bin") ;
theApp.Run () ;
/*c1.Print ("esempio.png", "png") ;
c1.Update();
c1.Draw();*/

return 0 ;

}