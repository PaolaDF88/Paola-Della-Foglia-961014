#include "function.h"
#include "integral.h"
#include "random.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
using namespace std;
//INTEGRALE--------------------------------------------------------------------------------------------------------------------------------
Integral::Integral(double a, double b, FunzioneBase *function) {
	_integrand = function;
	_a = min(a,b);
	_b = max(a,b);
	
	if (a>b) {
		_sign = -1;
	}	
	else {
		_sign = 1;
	}
}
Integral::Integral(double a, double b, FunzioneBase *function, Random *rand) {	
	_integrand = function;
	_rand= new Random();
	int M=100000;
	int seed[4];
   	int p1, p2;
   	ifstream Primes("Primes");
   	if (Primes.is_open()){
      		Primes >> p1 >> p2 ;
   	   } else cout << "PROBLEM: Unable to open Primes" << endl;
   	Primes.close();

   	ifstream input("seed.in");
   	string property;
   	if (input.is_open()){
      	   while ( !input.eof() ){
         	input >> property;
           	if( property == "RANDOMSEED" ){
            	input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            	_rand->SetRandom(seed,p1,p2);
                }
               }
      input.close();
   } else cout<< "PROBLEM: Unable to open seed.in" << endl;

   for(int i=0; i<M; i++){
      //cout << _rand->Rannyu() << endl; stampo a schermo i n.casuali generati con il generatore lineare congruenziale
   }
   _rand->SaveSeed();	
	if (a>b) {
		_sign = -1;
	}	
	else {
		_sign = 1;
	}
}

Integral::~Integral(){};
double Integral::Media(int nstep){
	double f=0;
	double a=0,b=1;
	double x;
	for(int k=0;k<nstep;k++){
		x = _rand->Rannyu(a,b);
		f = f + _integrand->Eval(x);
	}
	return ((b-a)/nstep)*f;
}
double Integral::Media_Importance(int nstep){
	double f=0;
	double a=0,b=1;
	double x;
	for(int k=0;k<nstep;k++){
		//x = _rand->Rannyu(a,b);
                x = _rand->Line();
		f = f + _integrand->Eval_Importance(x);
	}
	return ((b-a)/nstep)*f;
}
