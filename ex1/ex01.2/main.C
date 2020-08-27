/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include "random.h"
#include "math.h"
using namespace std;
 
int main (int argc, char *argv[]){
//Read seed for random numbers
   Random rnd;
   int seed[4];
   int p1, p2;
   
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;
//PART1 10^4 realizzazioni del valor medio del campionamento delle variabili casuali (per N=1,2,10,100) per un dado standard
   ofstream DadoStandard("DadoStandard.txt");//file su cui scrivo i risultati
   	if (!DadoStandard.is_open()) cerr << "PROBLEM: Unable to open DadoStandard.txt" << endl;

   int M=10000; //n.realizzazioni
   double N[4]={1,2,10,100};
   double sum=0;
   for(int i=0; i<4; i++){
   	for(int j=0; j<M; j++){
		sum=0;
		for(int k=0; k<N[i]; k++){
	    	sum += rnd.Dado(6);
	    	}
	    DadoStandard<<sum/double(N[i]) << endl;
	    //cout<<" media " << " = " << sum/double(N[i]) << endl;
	    }
      }
      cout << "Dati salvati in DadoStandard.txt" << endl;
//PART2 10^4 realizzazioni del valore medio del campionamento delle variabili casuali (per N=1,2,10,100) per un dado esponenziale
   ofstream DadoEsponenziale("DadoEsponenziale.txt");//file su cui scrivo i risultati
   	if (!DadoEsponenziale.is_open()) cerr << "PROBLEM: Unable to open DadoEsponenziale.txt"<<endl;
   double lambda=1;
   for (int i=0; i<4;i++){
	for (int j=0; j<M ;j++){
	    sum=0;
	    for(int k=0; k<N[i]; k++){
		sum+=rnd.Exp(lambda);
		}
	    DadoEsponenziale<<sum/double(N[i])<<endl;
	    //cout << " media " << " = " << sum/double(N[i])<<endl;
	    }
	}
	cout << "Dati salvati in DadoEsponenziale.txt" << endl;	
//PART3 10^4 realizzazioni del valore medio del campionamento delle variabili casuali (per N=1,2,10,100) per un dado lorentziano
ofstream DadoLorentziano("DadoLorentziano.txt");//file su cui scrivo i risultati
   	if (!DadoLorentziano.is_open()) cerr << "PROBLEM: Unable to open DadoLorentziano.txt"<<endl;
   double mean=0., gamma=1.;
   for (int i=0; i<4;i++){
	for (int j=0; j<M ;j++){
	    sum=0;
	    for(int k=0; k<N[i]; k++){
		sum+=rnd.Lorentz(mean,gamma);
		}
	    DadoLorentziano<<sum/double(N[i])<<endl;
	    //cout << " media " << " = " << sum/double(N[i])<<endl;
	    }
	}
	cout << "Dati salvati in DadoLorentziano.txt" << endl;		
return 0;
}
/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
