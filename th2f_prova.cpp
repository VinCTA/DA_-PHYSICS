/*Si crei e riempia un TH2F che contenga una Gaussiana bidimensionale nel piano (x,y)
 centrata in (0., 0.) con σx = σy = 2., generata con la tecnica del teorema centrale
 del limite, ricordando che una distribuzione di probabilita' Gaussiana bi-dimnesionale 
 si ottiene facendo il prodotto di due distribuzioni di probabilita' Gaussiane
 mono-dimensionali. */
 
 
 

   #include "TH2F.h"

 #include "gen.h"

 #include <iostream>

 #include "TCanvas.h"

#include <cstdlib>

#include "TApplication.h"
using namespace std ; 

 #include <iostream>

#include <iomanip>

#include <cmath>



using namespace std ;



float f (float asc )

{

	return exp (-0.5 * asc * asc) ;

	

	

	

}





float rand_range (float min, float max)



  {



   return min + (max - min) * rand () / static_cast<float> (RAND_MAX) ;



   



   // oppure dichiarare una variabile interna alla funzione e returnare 



   // la variabile



  }  







float rand_TAC (float f (float), float xMin, float xMax, float yMax)

  {

    double x = 0. ;

    double y = 0. ; 

    do {

      x = rand_range (xMin, xMax) ;

      y = rand_range (0, yMax) ;

    } while (y > f (x)) ;

    return x ; 

  }

  //ritorna i numeri x associati alla y che deve essere maggiore 

  // di f(x) ,  srand time (NULL);

  

  

 

  

 int main ()

 {

	  
     TH2F h2 ("h2", "eventi pseudo-casuali Gaussiani", 200, -1.5, 1.5, 200, -1.5, 1.5) ;
	 float varab = 4.567 ; 

	 float uscita ;

	 for (int i = 0 ; i < 4000 ; i++)

	 {   varab = varab + i ; 

         uscita = rand_TAC (f , 2 ,65,70);
          h2.Fill(uscita,uscita);
		 cout << "l'" << i << "posto : " << uscita << endl ;

	 }
 TApplication theApp ("theApp", &argc, argv) ;
TCanvas c1 ;
h2.Draw ("ejammi") ;
theApp.Run () ;
	 

	 

	 
return 0 ;
	 

	 

	 

 }