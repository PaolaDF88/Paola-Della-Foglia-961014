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
#include "vettore.h"
#include "statistiche.h"
using namespace std;
 
int main (int argc, char *argv[]){
//Read seed for random numbers
   Random rnd;
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
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cout << "PROBLEM: Unable to open seed.in" << endl;
//PART1 Stima del valore di PIGreco per l'esperimento di Buffon  PIGreco=(2*L*num_lanci/)(num_intersezioni*d); 
   ofstream PIGreco("PIGreco.txt");				//file su cui scrivo i risultati
   	if (!PIGreco.is_open()) cout << "PROBLEM: Unable to open PIGreco.txt" << endl;

   int M=100000; //totale di esperimenti
   int N=100;	 //n. di blocchi
   int num_lanci=M/N;    //n.di misure effettuate (lanci) all'interno di un esperimento che è il blocco
   double MIN=0., MAX=20.;//Area in cui misuro la probabilità per l'esp.di Buffon 
   double d=1., L=0.7;	// d>L (d=distanza linee L=lunghezza aghi) 
   int success = 0; //contatore n.successi (volte in cui gli aghi intersecano le linee della porzione di reticolo)
   double y=0, theta; 
   VettoreDati PI(N), PI2(N), PI_prog (N), PI2_prog (N),PI_err_prog (N); //dichiaro i vettori
   for (int i=0; i<N ; i++){
	success = 0;
	//sum=0.;
    for(int j=0; j<num_lanci ; j++){
		 y = rnd.Rannyu(MIN, MAX);
        	//k = j+i*num_lanci;
	   	double x_box=rnd.Rannyu(-1.,1.), y_box=rnd.Rannyu(-1.,1.);
           	//sum+=r.GetComponent(k);
	   	while(sqrt(x_box*x_box + y_box*y_box)>1){
		      	x_box=rnd.Rannyu(-1.,1.);
			y_box=rnd.Rannyu(-1.,1.);
		      }
		      theta = atan(y_box/x_box);
		      if(intersezione(y, theta, L, d, MIN, MAX)){		
				success++;//contatore aghi che intersecano le linee
	                }
        }
   PI.SetComponent(i,2*L*num_lanci/success*d); //stima PIGreco
   PI2.SetComponent(i,pow(PI.GetComponent(i),2)); //valore quadratico medio PIGreco
   }
   //PI.Print(); 
   //PI2.Print();
   for (int i=0; i<N ; i++){
        for( int j=0; j<i+1 ; j++)
	   {
		PI_prog.SetComponent(i,PI_prog.GetComponent(i)+PI.GetComponent(j)); //valori medi cumulati o somma di valori medi
		PI2_prog.SetComponent(i,PI2_prog.GetComponent(i)+PI2.GetComponent(j));//somma di valori quadratici medi
	   }
	PI_prog.SetComponent(i,PI_prog.GetComponent(i)/(i+1));//media cumulata delle stime di PiGreco
	PI2_prog.SetComponent(i,PI2_prog.GetComponent(i)/(i+1));//media cumulata dei valori quadratici medi
	PI_err_prog.SetComponent(i, error(PI_prog.GetComponent(i), PI2_prog.GetComponent(i), i));//dev_standard della media
	//err_prog.SetComponent(i, err_prog.Errore(sum_prog, sum2_prog, i));
	cout<< num_lanci*(i+1) << " " << PI_prog.GetComponent(i)<< " " <<PI_err_prog.GetComponent(i)<< endl; 
	PIGreco << num_lanci*(i+1) << " " << PI_prog.GetComponent(i)<< " " <<PI_err_prog.GetComponent(i)<< endl; 
        }
	PIGreco.close();
	cout << "Dati salvati in PIGreco.txt: stima del valore di PiGreco e della sua incertezza nel formato -> x PI_prog PI_err_prog" << endl;
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
