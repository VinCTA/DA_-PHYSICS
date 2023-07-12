/*Si scriva un programma che legge i numeri salvati nel file dati_esponenziali.txt e 
li scriva in un vector<double>.
Si faccia in modo che il nome del file sia passato a linea di comando.
Si faccia in modo che il secondo argomneto a linea di comando, opzionale, 
sia il numero massimo di numeri da leggere dal file.
Si aggiunga al programma precedente il codice sorgente che calcoli la media,
 il valor minimo ed il valor massimo dei numeri inseriti.
Si aggiunga al programma precedente il codice sorgente che riempia un istogramma con
 i numeri presenti nel vector dove sono stati trasferiti e che visualizzi l'istogramma a schermo.*/


#include <iostream>
#include <fstream>
#include <vector>
#include "provstat.h"


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
	for (int i= 0 ; i < data.size () ;i++ )
	{
		somma=somma+data.at(i) ;
		/*if( data[i]>>data[i-1] ) max = data[i] ;
		else continue ; */
		
		
	}
	
	cout << "la media di data vale : "  <<  data.media << " / e la somma vale :  " << somma  << endl ;
    return 0 ;
}










