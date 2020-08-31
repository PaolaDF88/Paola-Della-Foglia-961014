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
#include <cmath>
#include <vector>
#include "random.h"
#include "function.h"
#include "vettore.h"

using namespace std;
 
int main (int argc, char *argv[]){

	//Random generator
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
	
//PART1 -> opzione_put e opzione_call nel caso a 1 step t=0 -->t=1 (caso diretto)

	int M=1E4;            //n.totale esperimenti
	int N=100;            //n.blocchi
	int L=M/N;    	      
        VettoreDati ave_put(N),sum_put(N),ave2_put(N),sum_prog_put(N),sum2_prog_put(N),err_prog_put(N);
        VettoreDati ave_call(N), ave2_call(N), sum_call(N),sum_prog_call(N), sum2_prog_call(N),err_prog_call(N); 
	double St,put, call;								
	//Dati iniziali
	double S0=100.;		//prezzo titolo
	double T=1.;		//data chiusura contratto
	double t=0.;		//tempo 
	double K=100.;		//prezzo pattuito nel contratto
	double r=0.1;		//tasso d'interesse
	double sigma=0.25; 	//volatilità
	ofstream Opzione_call("Call_direct.txt");		        //file su cui salvo i risultati
   	if (!Opzione_call.is_open()) cerr << "PROBLEM: Unable to open Call_direct.txt" << endl;
	ofstream Opzione_put("Put_direct.txt");				//file su cui salvo i risultati
   	if (!Opzione_put.is_open()) cerr << "PROBLEM: Unable to open Put_direct.txt" << endl;

	for(int i=0; i<N; i++){				//ciclo sui blocchi		
	   for(int j=0; j<L; j++){
              St = S0*exp((r-sigma*sigma*0.5)*(T-t)+sigma*rnd.Gauss(0.,(T-t)));	//prezzo opzione al tempo t=0
              put = exp(-r*(T-t))*max(0.,K-St); 			
	      call = exp(-r*(T-t))*max(0.,St-K);
              sum_put.SetComponent(i,sum_put.GetComponent(i)+put);	
              sum_call.SetComponent(i,sum_call.GetComponent(i)+call);			
	   }
           ave_put.SetComponent(i,sum_put.GetComponent(i)/L); //valori medi opzione_put
           ave2_put.SetComponent(i,pow(ave_put.GetComponent(i),2)); //valori quadratici medi opzione_put 
           ave_call.SetComponent(i,sum_call.GetComponent(i)/L);//valori medi opzione_call
           ave2_call.SetComponent(i,pow(ave_call.GetComponent(i),2)); //valori quadrat medi opzione_call         
        }
        //ave_put.Print();
	//ave_call.Print();
        for (int i=0; i<N ; i++){
    	    for( int j=0; j<i+1 ; j++){
		sum_prog_put.SetComponent(i,sum_prog_put.GetComponent(i)+ave_put.GetComponent(j)); //valori medi cumulati opzione_put
		sum2_prog_put.SetComponent(i,sum2_prog_put.GetComponent(i)+ave2_put.GetComponent(j));//somma val quad medi opzione_put
                sum_prog_call.SetComponent(i,sum_prog_call.GetComponent(i)+ave_call.GetComponent(j)); //valori medi cumulati opzione_call
		sum2_prog_call.SetComponent(i,sum2_prog_call.GetComponent(i)+ave2_call.GetComponent(j));//somma val quad medi opzione_call
	    }
	sum_prog_put.SetComponent(i,sum_prog_put.GetComponent(i)/(i+1));//media cumulata dei valori medi opz_put
	sum2_prog_put.SetComponent(i,sum2_prog_put.GetComponent(i)/(i+1));//media cumulata dei valori quadratici medi opzione_put
	err_prog_put.SetComponent(i, error(sum_prog_put.GetComponent(i), sum2_prog_put.GetComponent(i), i));//incertezza opzione_put
	Opzione_put << L*(i+1) << " " << sum_prog_put.GetComponent(i)<< " " <<err_prog_put.GetComponent(i)<< endl;
        sum_prog_call.SetComponent(i,sum_prog_call.GetComponent(i)/(i+1));//media cumulata dei valori medi opz_call
	sum2_prog_call.SetComponent(i,sum2_prog_call.GetComponent(i)/(i+1));//media cumulata dei valori quadratici medi opzione_call
	err_prog_call.SetComponent(i, error(sum_prog_call.GetComponent(i), sum2_prog_call.GetComponent(i), i));//incertezza opzione_call 
        Opzione_call << L*(i+1) << " " << sum_prog_call.GetComponent(i)<< " " <<err_prog_call.GetComponent(i)<< endl;
        }
	Opzione_put.close();
        Opzione_call.close();
	cout << "Dati salvati in Put_direct.txt: stima del valor medio opzione_put e incertezza su di essa-> x sum_prog_put err_prog_put" << endl;
        cout << "Dati salvati in Call_direct.txt: stima del valor medio opzione_call e incertezza su di essa-> x sum_prog_call err_prog_call" << endl;
//PART2 Opzione_Put e opzione_Call in 100 step caso t=0 --> t=0.01 --> .. --> t=1 (caso discreto)
	int n_step = 100;
	ofstream Put_discreto("Put_discreto.txt");				
   	if (!Put_discreto.is_open()) cerr << "PROBLEM: Unable to open Put_discreto.txt" << endl;
	ofstream Call_discreto("Call_discreto.txt");				
   	if (!Call_discreto.is_open()) cerr << "PROBLEM: Unable to open Call_discreto.txt" << endl;
	ave_put.SetN(N),sum_put.SetN(N),ave2_put.SetN(N),sum_prog_put.SetN(N),sum2_prog_put.SetN(N),err_prog_put.SetN(N);//reinizializzo vet
        ave_call.SetN(N),sum_call.SetN(N),ave2_call.SetN(N),sum_prog_call.SetN(N),sum2_prog_call.SetN(N),err_prog_call.SetN(N);	

        for(int i=0; i<N; i++){				//ciclo sui blocchi		
	   for(int j=0; j<L; j++){
              St = S0;
              for(int h=0;h<n_step;h++){
                 St = St*exp((r-sigma*sigma*0.5)*(T-t)/double(n_step)+sigma*rnd.Gauss(0.,1.)*sqrt(T/double(n_step)));
              }	
              put = exp(-r*(T-t))*max(0.,K-St); 			
	      call = exp(-r*(T-t))*max(0.,St-K);
              sum_put.SetComponent(i,sum_put.GetComponent(i)+put);	
              sum_call.SetComponent(i,sum_call.GetComponent(i)+call);			
	   }
           ave_put.SetComponent(i,sum_put.GetComponent(i)/L); //valori medi opzione_put
           ave2_put.SetComponent(i,pow(ave_put.GetComponent(i),2)); //valori quadratici medi opzione_put 
           ave_call.SetComponent(i,sum_call.GetComponent(i)/L);//valori medi opzione_call
           ave2_call.SetComponent(i,pow(ave_call.GetComponent(i),2)); //valori quadrat medi opzione_call       
        }
        ave_put.Print();
        cout <<"----" <<endl;
	ave_call.Print();
        for (int i=0; i<N ; i++){
    	    for( int j=0; j<i+1 ; j++){
		sum_prog_put.SetComponent(i,sum_prog_put.GetComponent(i)+ave_put.GetComponent(j)); //valori medi cumulati opzione_put
		sum2_prog_put.SetComponent(i,sum2_prog_put.GetComponent(i)+ave2_put.GetComponent(j));//somma val quad medi opzione_put
                sum_prog_call.SetComponent(i,sum_prog_call.GetComponent(i)+ave_call.GetComponent(j)); //valori medi cumulati opzione_call
		sum2_prog_call.SetComponent(i,sum2_prog_call.GetComponent(i)+ave2_call.GetComponent(j));//somma val quad medi opzione_call
	    }
	sum_prog_put.SetComponent(i,sum_prog_put.GetComponent(i)/(i+1));//media cumulata dei valori medi opz_put
	sum2_prog_put.SetComponent(i,sum2_prog_put.GetComponent(i)/(i+1));//media cumulata dei valori quadratici medi opzione_put
	err_prog_put.SetComponent(i, error(sum_prog_put.GetComponent(i), sum2_prog_put.GetComponent(i), i));
	Put_discreto << L*(i+1) << " " << sum_prog_put.GetComponent(i)<< " " <<err_prog_put.GetComponent(i)<< endl;
        sum_prog_call.SetComponent(i,sum_prog_call.GetComponent(i)/(i+1));//media cumulata dei valori medi opz_call
	sum2_prog_call.SetComponent(i,sum2_prog_call.GetComponent(i)/(i+1));//media cumulata dei valori quadratici medi opzione_call
	err_prog_call.SetComponent(i, error(sum_prog_call.GetComponent(i), sum2_prog_call.GetComponent(i), i)); 
        Call_discreto<< L*(i+1) << " " << sum_prog_call.GetComponent(i)<< " " <<err_prog_call.GetComponent(i)<< endl;
        }
	Put_discreto.close();
        Call_discreto.close();
	cout << "Dati salvati in Put_discreto.txt: stima del valor medio opzione_put e incertezza su di essa-> x sum_prog_put err_prog_put" << endl;
        cout << "Dati salvati in Call_discreto.txt: stima del valor medio opzione_call e incertezza su di essa-> x sum_prog_call err_prog_call" << endl;
  	rnd.SaveSeed();
   	return 0;
}

