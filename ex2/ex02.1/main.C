/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
#include "function.h"
#include "integral.h"
#include "random.h"
#include "vettore.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace std;
int main (int argc, char *argv[]){

   ofstream MC("MC.txt");
	     if (!MC.is_open()) cerr << "PROBLEM: Unable to open Importance.txt" << endl;
   Random *rnd= new Random ();
   FunzioneBase *f = new Funzione(1);
//PART1 -> estimating integral and error using a uniform distribution in [0,1)
   double M=10000;
   int N=100;
   int L=M/N;
   double tmp=0;
   Integral *integrale= new Integral (0.,1.,f,rnd);	

   VettoreDati I(N),I2(N),I_prog(N), I2_prog(N),err_I_prog(N);
   for (int i=0; i<N ;i++){
	tmp=integrale->Media(N);
	I.SetComponent(i,tmp); //stima valore integrale MC
	I2.SetComponent(i,pow(tmp,2));//stima valor quadratico medio integrale MC
       }
   //I.Print();	
   //I2.Print();
    for (int i=0; i<N; i++){
    	for( int j=0; j<i+1 ; j++)
	   {
		I_prog.SetComponent(i,I_prog.GetComponent(i)+I.GetComponent(j)); //valori medi cumulati o somma di valori medi
		I2_prog.SetComponent(i,I2_prog.GetComponent(i)+I2.GetComponent(j));//somma di valori quadratici medi
	   }
	I_prog.SetComponent(i,I_prog.GetComponent(i)/(i+1));//media cumulata dei valori medi
	I2_prog.SetComponent(i,I2_prog.GetComponent(i)/(i+1));//media cumulata dei valori quadratici medi
        err_I_prog.SetComponent(i, error(I_prog.GetComponent(i), I2_prog.GetComponent(i), i));//dev_standard della media
	MC << L*(i+1) << " " << I_prog.GetComponent(i)<< " " <<err_I_prog.GetComponent(i)<< endl; 
      }
   cout<<"media cumulata stima integrale MC " << endl;
   I_prog.Print();
   cout<<"media cumulata valori quadratici medi integrale MC " << endl;
   I2_prog.Print();
   cout<<"incertezza stima integrale MC " <<endl;  
   err_I_prog.Print();
   MC.close();
   cout << "Dati salvati in soluzione.txt: stima del valor medio e incertezza statistica per I con il metodo MC nel formato -> x I_prog err_I_prog" << endl;	
   delete integrale;
//PART2 -> estimating integral and error using a linear distribution in [0,1) p(x)=2(1-x) Importance Sampling
   I.SetN(N),I2.SetN(N),I_prog.SetN(N), I2_prog.SetN(N),err_I_prog.SetN(N); //reinizializzazione
   ofstream Importance_Sampling("Importance_Sampling.txt");				//file to write results
   	    if (!Importance_Sampling.is_open()) cerr << "PROBLEM: Unable to open Importance.txt" << endl;
   Integral *integrale_IS= new Integral (0.,1.,f,rnd);
    for (int i=0; i<N ;i++){
	tmp=integrale->Media_Importance(N);
	I.SetComponent(i,tmp); //stima valore integrale MC
	I2.SetComponent(i,pow(tmp,2));//stima valor quadratico medio integrale MC
       }
   //I.Print();	
   //I2.Print();
    for (int i=0; i<N; i++){
    	for( int j=0; j<i+1 ; j++)
	   {
		I_prog.SetComponent(i,I_prog.GetComponent(i)+I.GetComponent(j)); //valori medi cumulati o somma di valori medi
		I2_prog.SetComponent(i,I2_prog.GetComponent(i)+I2.GetComponent(j));//somma di valori quadratici medi
	   }
	I_prog.SetComponent(i,I_prog.GetComponent(i)/(i+1));//media cumulata dei valori medi
	I2_prog.SetComponent(i,I2_prog.GetComponent(i)/(i+1));//media cumulata dei valori quadratici medi
        err_I_prog.SetComponent(i, error(I_prog.GetComponent(i), I2_prog.GetComponent(i), i));//dev_standard della media
	Importance_Sampling << L*(i+1) << " " << I_prog.GetComponent(i)<< " " <<err_I_prog.GetComponent(i)<< endl; 
      }
   cout<<"media cumulata stima integrale IS " << endl;
   I_prog.Print();
   cout<<"media cumulata valori quadratici medi integrale IS " << endl;
   I2_prog.Print();
   cout<<"incertezza stima integrale IS " <<endl;  
   err_I_prog.Print();
   Importance_Sampling.close();
   cout << "Dati salvati in soluzione.txt: stima del valor medio e incertezza statistica per I con il metodo Importance_Sampling nel formato -> x I_prog err_I_prog" << endl;	
   delete integrale_IS;
return 0;		
}
