/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
#include "random.h"
#include "vettore.h"
#include"function.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

   using namespace std;
   int main (int argc, char *argv[]){
//PART1 Random walk nel caso discreto
   ofstream Discreto("Discreto.txt");				//file con i risultati salvati
   	if (!Discreto.is_open()) cout << "PROBLEM: Unable to open Discreto.txt" << endl;
   ofstream Esempio("Esempio.txt");				//file con i risultati salvati
   	if (!Esempio.is_open()) cout << "PROBLEM: Unable to open Esempio.txt" << endl;
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
   rnd.SaveSeed();
   int M=1E4;	//n.totale di random walk
   int N=1E2;	//n. di step per ogni random walk M
   int dim=3;
   VettoreDati posizione_d(dim),ave_dist(N),ave2_dist(N),dist(N),dist2(N),err_dist(N),sum_prog(N),sum2_prog(N);
   int dir;  
   double sum=0.;
   for(int i=0; i<M; i++){			//ciclo su M Random Walk
       posizione_d.SetN(dim);			       //inizializzo la posizione dietro all'origine
       for(int j=0; j<N; j++){
          sum=0;			        //ciclo su N steps per ogni random walk
	  dir=rnd.Direzione(dim);			//genero una random direction 0,1,2 corrispondente a x,y,z
	  if(rnd.Rannyu()>0.5)			 	//mi muovo avanti/indietro in quella direzione con il 50% di probabilità
	    posizione_d.SetComponent(dir,posizione_d.GetComponent(dir)+1);	
	  else 
	      posizione_d.SetComponent(dir,posizione_d.GetComponent(dir)-1);
          for (int h=0;h<dim;h++){
              sum+=sqrt(pow(posizione_d.GetComponent(h),2));	//calcolo le distanze dall'origine
              //cout << "sum " <<sum <<endl; 
          }
          dist.SetComponent(j,dist.GetComponent(j)+sum);  //aggiungo al precedente random walk
          Esempio << posizione_d.GetComponent(0) << " " << posizione_d.GetComponent(1) << " " << posizione_d.GetComponent(2) << endl;
	}
   }
   //dist.Print();   
   for(int i=0; i<N ; i++){
       ave_dist.SetComponent(i,dist.GetComponent(i)/M);//valori medi distanze
       ave2_dist.SetComponent(i,pow(ave_dist.GetComponent(i),2));
       for( int j=0; j<i+1 ; j++)
	   {
		sum_prog.SetComponent(i,sum_prog.GetComponent(i)+ave_dist.GetComponent(j)); //valori medi cumulati o somma di valori medi
		sum2_prog.SetComponent(i,sum2_prog.GetComponent(i)+ave2_dist.GetComponent(j));//somma di valori quadratici medi
	}
	sum_prog.SetComponent(i,sum_prog.GetComponent(i)/(i+1));//media cumulata dei valori medi
	sum2_prog.SetComponent(i,sum2_prog.GetComponent(i)/(i+1));//media cumulata dei valori quadratici medi
	err_dist.SetComponent(i, error(sum_prog.GetComponent(i), sum2_prog.GetComponent(i), i));//calcolo errore
	cout<< (i+1) << " " << sqrt(ave_dist.GetComponent(i))<< " " <<err_dist.GetComponent(i)<< endl; 
	Discreto<< (i+1) << " " << sqrt(ave_dist.GetComponent(i))<< " " <<err_dist.GetComponent(i)<< endl; 
    }
    Discreto.close();
    //ave_dist.Print(); 
    //ave2_dist.Print();
    //err_dist.Print();
   cout << "I dati sono salvati nel file Discreto.txt" << endl;
   //PART2 Random walk nel caso continuo
   ofstream Continuo("Continuo.txt");				//file to write results
   	if (!Continuo.is_open()) cerr << "PROBLEM: Unable to open Continuo.txt" << endl;
   double theta,phi;
   VettoreDati posizione_c(dim);
   dist.SetN(N),ave_dist.SetN(N),ave2_dist.SetN(N),sum_prog.SetN(N),sum2_prog.SetN(N),err_dist.SetN(N);  //reinizializzo i vettori
   for(int i=0; i<M; i++){				
       posizione_c.SetN(dim);			
       for(int j=0; j<N; j++){
	   sum=0;					
	   theta=rnd.Theta();				//genera una direzione random(casuale)
	   phi=rnd.Rannyu(0., 2.*M_PI); 
	   posizione_c.SetComponent(0,posizione_c.GetComponent(0)+(sin(theta)*cos(phi)));//coordinate sferiche
	   posizione_c.SetComponent(1,posizione_c.GetComponent(1)+(sin(theta)*sin(phi)));
	   posizione_c.SetComponent(2,posizione_c.GetComponent(2)+(cos(theta)));
	   //posizione_c.Print(); 
	   for(int h=0;h<dim;h++){
	      	    sum+=sqrt(pow(posizione_c.GetComponent(h),2)); 
	   }
	   dist.SetComponent(j,dist.GetComponent(j)+sum);
       }
    }
    //dist.Print();
    for(int i=0; i<N ; i++){
       ave_dist.SetComponent(i,dist.GetComponent(i)/M);
       ave2_dist.SetComponent(i,pow(ave_dist.GetComponent(i),2));
       for( int j=0; j<i+1 ; j++){
		sum_prog.SetComponent(i,sum_prog.GetComponent(i)+ave_dist.GetComponent(j)); //valori medi cumulati o somma di valori medi
		sum2_prog.SetComponent(i,sum2_prog.GetComponent(i)+ave2_dist.GetComponent(j));//somma di valori quadratici medi
	}
	sum_prog.SetComponent(i,sum_prog.GetComponent(i)/(i+1));//media cumulata dei valori medi
	sum2_prog.SetComponent(i,sum2_prog.GetComponent(i)/(i+1));//media cumulata dei valori quadratici medi
	err_dist.SetComponent(i, error(sum_prog.GetComponent(i), sum2_prog.GetComponent(i), i));//calcolo errore
	cout<< (i+1) << " " << sqrt(ave_dist.GetComponent(i))<< " " <<err_dist.GetComponent(i)<< endl; 
	Continuo<< (i+1) << " " << sqrt(ave_dist.GetComponent(i))<< " " <<err_dist.GetComponent(i)<< endl; 
    }
    Continuo.close();
    //ave_dist.Print(); 
    //ave2_dist.Print();
   cout << "I dati sono salvati nel file Continuo.txt" << endl;	
return 0;		
}
