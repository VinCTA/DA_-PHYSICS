/*Si scriva un programma che, fissato un numero NMAX, generi NToys toy experiment, ciascuno contenente
 un campione di NMAX eventi che seguano una distribuzione a scelta e ne calcoli la media.*/


#include "TH1F.h"

#include "provstat.h"

 #include "TCanvas.h"

#include <cstdlib>

#include "TApplication.h"
using namespace std ; 

 #include <iostream>


float rand_range (float min, float max)
  {

   return min + (max - min) * rand () / static_cast<float> (RAND_MAX) ;
   
   // oppure dichiarare una variabile interna alla funzione e returnare 
   // la variabile 

   }


int main(int argc, char ** argv) {
	int NMAX = 10000 ;
	statistiche info(NMAX)  ;
	statistiche med ;
	
	int Ntoys ;
	cout << "digitare il numero di exp da compiere  " << endl ;
	cin >> Ntoys ; 
	TH1F h_medie ("h_medie", "distribuzione delle medie", 400, -30, 30);

	for (int j =0 ; j <Ntoys ; j++){
		
	
	
	for(int i = 0 ; i < NMAX ; i++ ) {
	info.aggiunginumero(rand_range(-30,30));
	}
	h_medie.Fill (info.media ()) ;
	med.aggiunginumero(info.media());
	info.reset();
	
	}
TApplication theApp ("theApp", &argc, argv) ;
TCanvas c1 ;
h_medie.Draw () ;
c1.Print ("esempio.png", "png") ;
theApp.Run () ;

	
	
	
	
}