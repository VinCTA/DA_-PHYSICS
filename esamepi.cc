#include "esamepi.h"

#include<vector>
#include <cmath>
double genera_uniforme (const double & min, const double & max)
{
  return min + (max - min) * rand () / static_cast <double> (RAND_MAX) ; 
}
double calc_pi (const int & Npoints)
{
  int Ninside = 0 ;
  for (int i = 0 ; i < Npoints ; ++i)
    {
       double x = genera_uniforme (0., 1.) ;
       double y = genera_uniforme (0., 1.) ;
       if ( (x*x + y*y) < 1) ++Ninside ;
    }
  return 4. * static_cast<double> (Ninside) / static_cast<double> (Npoints) ;
   
    }
 
 //exp=numero di volte che si ripete l esperimento
 // ar_pi sta per array contenete i vari valori di pi_greco
 double calc_piexv (int & Nexp ,int & Npoints ) 
   { 
      vector<double> muhat
	  vector <double> ar_pi ;
	  vector <double> ngen ;
      vector <double> variance ; 
      for (int t = 0 ; t < 5 ; t++ )
{    if (t<1) Npoints =Npoints;
     else Npoints *=10;
       

       for (j=0 ; j < Nexp ; j++ ) 
      {   
       double muhat_pi ;
	   double variance_pi ;
       int Ninside = 0 ;
         for (int i = 0 ; i < Npoints ; ++i)
          {
            double x = genera_uniforme (0., 1.) ;
            double y = genera_uniforme (0., 1.) ;
            if ( (x*x + y*y) < 1) ++Ninside ;
          }
      ar_pi[j] = 4. * static_cast<double> (Ninside) / static_cast<double> (Npoints) ;
      muhat_pi=muhat_pi+ar_pi[j]; 
	  muhat_pi=muhat_pi/Nexp ;
	  variance_pi = muhat_pi/sqrt(Nexp) ;
     
       } 
     variance[t] = variance_pi ;
	 ngen[t] = Npoints ;
	 muhat[t]= muhat_pi;
	 cout << "varianza vale : " << variance[t] << "pi_greco vale : " << muhat[t] << endl ;
	 
    	
     	 
   }
    return variance;
   
   
   
   }