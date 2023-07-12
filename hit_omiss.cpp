/*Si implementi il metodo di integrazione hit-or-miss con la funzione di esempio f(x) = sin (x).
Si scriva l'algoritmo che calcola l'integrale come una funzione esterna al programma main, 
facendo in modo che prenda come parametri di ingresso, oltre agli estremi lungo l'asse x e l'asse y,
 anche il numero di punti pseudo-casuali da generare.
Si faccia in modo che l'algoritmo ritorni un contenitore contenente due elementi:
 il primo elemento sia il valore dell'integrale, il secondo sia la sua incertezza.*/
 #include <cmath>
 #include <iostream>
 #include <cstdlib>
 
 double calcolo_int ( int hittati ,int num_cas,double xinf ,double xsup , double yinf , double ysup)
 {
	double base = xsup-xinf ;
	 double h = ysup-yinf ;
	 double A = base * h ;
	 return A*(hittati/num_cas ) ;
	 
	 
	 
 }
 
 
 
 double dev_std (double & calcolo_int,double xinf ,double xsup , double yinf , double ysup )
 {
	 double base = xsup-xinf ;
	 double h = ysup-yinf ;
	 double A = base * h 
	 return sqrt(calcolo_int/A) ;
	 
	 
 }
 
 double fsin(double asc)
 {
	 return sin(x);
 }
 
  double rand_range (double min, double max)
  {

   return min + (max - min) * rand () / static_cast<double> (RAND_MAX) ;
   
   // oppure dichiarare una variabile interna alla funzione e returnare 
   // la variabile 

   }
 
 
   
    int main(int argc, char ** argv) // argv è l'array
	{
 
 
 
 int N = 10000 ;
int nHit = 0 ;
double xMin = 0. ;
double xMax = 2*M_PI ; 
double yMin = 0. ; 
double yMax = 2. ;

 bool isBelow (double g (double), double xMin, double xMax,
              double yMin, double yMax)
 // come se fosse  bool isBelow = (double g (double), double xMin, double xMax,double yMin, double yMax)
 {
    double x = rand_range (xMin, xMax) ;
    double y = rand_range (yMin, yMax) ; 
    if (y < g (x)) return true ; 
    return false ;
  }

for (int i = 0 ; i < N ; ++i) 
  {
    if (isBelow (fsin, xMin, xMax, yMin, yMax) == true) ++nHit ; // dovrebbe essere ridontante ovvero:
//if (isBelow (fsin, xMin, xMax, yMin, yMax) == true)=if (isBelow (fsin, xMin, xMax, yMin, yMax))
  }
  
 /*in pratica se mi ritorna che il valore di y è caduto sotto g (che poi sarebbe senx)
 allora aggiungo un unità ad nhit (++nhit)*/
 double book = calcolo_int ( nHit ,N,xMin ,xMax ,yMin ,yMax );
 cout << "l intg vale : " << book << "e  la sigma vale : " << dev_std(book, xMin ,xMax ,yMin ,yMax) << endl ;
 
  
  
	
	
	}