/*Si implementi un generatore di numeri pseudo-casuali che utilizzi il metodo 

try-and-catch per generare numeri casuali secondo un distribuzione di probabilita' 

arbitraria.



Si prenda come parametro di ingresso la pdf da utilizzare 

per la generazione di numeri casuali.*/

 

 

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

    } while (y < f (x)) ;

    return x ; 

  }

  //ritorna i numeri x associati alla y che deve essere maggiore 

  // di f(x) ,  srand time (NULL);

  

  

 

  

 int main ()

 {

	  

	 float varab = 4.567 ; 

	 float uscita ;

	 for (int i = 0 ; i < 4000 ; i++)

	 {   varab = varab + i ; 

		 uscita = rand_TAC (f , 2 ,65,70);

		 cout << "l'" << i << "posto : " << uscita << endl ;

		 

		 

		 

		 

	 }

	 

	 

	 

	 

	 

	 

 }