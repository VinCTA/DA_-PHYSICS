#include <stdio.h>
#include <stdlib.>
#include <iostream>



using namespace std ;

int main(int argc, char** argv)
{
	int nIter ;
	double x ;
	double a ;
	
	nIter = atoi(argv[1]);
	x  =atoi(argv[2]);
	a = atoi(argv[3]);
	double prova = recurrentLogistic (int nIter, double x, double a);
	
	

}