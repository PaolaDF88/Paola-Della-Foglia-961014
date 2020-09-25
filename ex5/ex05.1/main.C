#include <iostream>
#include <fstream>
#include <string>
#include "random.h"
#include "vettore.h"
#include "math.h"
#include "funzioni.h"
#include "random.h"
using namespace std;
   Random rnd;
   int n_equil=100;
   double x_new,y_new,z_new;
   double raggio;
   double delta = 1.2;	//set delta
   double x_old = 0., y_old = 0., z_old = 0.;
   double p_old, p_new;
   int acc = 0;
   //funzioni
   void Metropolis100(void);
   void Metropolis210(void);
   int main (int argc, char *argv[]){
//Leggi seed da numeri random
   int seed[4];
   int p1, p2;
   int M=100000; //totale di esperimenti
   int N=100;	 //n. di blocchi
   int L=M/N;    //n.di misure effettuate all'interno di un esperimento che è il blocco
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
//PART1 
  ofstream Psi100_file("Psi100.txt");						
    if (!Psi100_file.is_open()) cerr << "PROBLEM: Unable to open Psi100.txt" << endl;
    ofstream Raggio100_file("Raggio100.txt");						
      if (!Raggio100_file.is_open()) cerr << "PROBLEM: Unable to open Raggio100.txt" << endl;

   VettoreDati sum(N), ave(N), ave2(N), sum_prog (N), sum2_prog (N),err_prog (N); //dichiaro i vettori
   for(int j=0; j<n_equil; j++){		//EQUILIBRATURA
       Metropolis100();
   }
   for (int i=0; i<N ; i++){
       for( int j=0; j<L ; j++){
          Metropolis100();
          sum.SetComponent(i,sum.GetComponent(i)+raggio);
          Psi100_file << x_new << " " << y_new << " " << z_new << endl;
       }
       ave.SetComponent(i,sum.GetComponent(i)/L); //valor medio che rappresenta l'esito di un esperimento nel blocco i-esimo
       ave2.SetComponent(i,pow(ave.GetComponent(i),2)); //valore quadratico medio
   }
   //sum.Print();
   //cout << "raggio1 " << raggio <<endl;
   for (int i=0; i<N ; i++){
       for( int j=0; j<i+1 ; j++)
	   {
		sum_prog.SetComponent(i,sum_prog.GetComponent(i)+ave.GetComponent(j)); //valori medi cumulati o somma di valori medi
		sum2_prog.SetComponent(i,sum2_prog.GetComponent(i)+ave2.GetComponent(j));//somma di valori quadratici medi
	   }
	sum_prog.SetComponent(i,sum_prog.GetComponent(i)/(i+1));//media cumulata dei valori medi
	sum2_prog.SetComponent(i,sum2_prog.GetComponent(i)/(i+1));//media cumulata dei valori quadratici medi
	err_prog.SetComponent(i, error(sum_prog.GetComponent(i), sum2_prog.GetComponent(i), i));//dev_standard della media
	Raggio100_file << L*(i+1) << " " << sum_prog.GetComponent(i)<< " " <<err_prog.GetComponent(i)<< endl; 
        }
        Psi100_file.close();
	Raggio100_file.close();
	cout << "Dati salvati in Psi100.txt Psi100 in coordinate cartesiane in questa forma -> X Y Z " <<endl;
        cout << "Dati salvati in Raggio100.txt valor medio e incertezza su r in questa forma -> X Y Yerr " <<endl;
        cout << "Acceptation rate: " << double(acc)/M << endl;
//PART2 ->Psi n=2 l=1 m=0
        x_old = 0.;
	y_old = 0.;
	z_old = 2.5;
	delta = 2.8;
	acc = 0;
//file su cui savlo i risultati
	ofstream Psi210_file("Psi210.txt");						
          if (!Psi210_file.is_open()) cerr << "PROBLEM: Unable to open Psi210.txt" << endl;
	ofstream Raggio210_file("Raggio210.txt");				
           if (!Raggio210_file.is_open()) cerr << "PROBLEM: Unable to open Raggio210.txt" << endl;

        sum.SetN(N), ave.SetN(N), ave2.SetN(N), sum_prog.SetN(N), sum2_prog.SetN(N),err_prog.SetN(N); //reinizializzo i vettori
        for(int j=0; j<n_equil; j++){		//EQUILIBRAZIONE
           Metropolis210();
	}
	for(int i=0; i<N; i++){	
           for( int j=0; j<L ; j++){
           Metropolis210();
           sum.SetComponent(i,sum.GetComponent(i)+raggio);
           Psi210_file << x_new << " " << y_new << " " << z_new << endl;
           }
       ave.SetComponent(i,sum.GetComponent(i)/L); //valor medio che rappresenta l'esito di un esperimento nel blocco i-esimo
       ave2.SetComponent(i,pow(ave.GetComponent(i),2)); //valore quadratico medio
       }
   //sum.Print();
   //cout << "raggio1 " << raggio <<endl;
   for (int i=0; i<N ; i++){
       for( int j=0; j<i+1 ; j++){
		sum_prog.SetComponent(i,sum_prog.GetComponent(i)+ave.GetComponent(j)); //valori medi cumulati o somma di valori medi
		sum2_prog.SetComponent(i,sum2_prog.GetComponent(i)+ave2.GetComponent(j));//somma di valori quadratici medi
        }
	sum_prog.SetComponent(i,sum_prog.GetComponent(i)/(i+1));//media cumulata dei valori medi
	sum2_prog.SetComponent(i,sum2_prog.GetComponent(i)/(i+1));//media cumulata dei valori quadratici medi
	err_prog.SetComponent(i, error(sum_prog.GetComponent(i), sum2_prog.GetComponent(i), i));//dev_standard della media
	Raggio210_file << L*(i+1) << " " << sum_prog.GetComponent(i)<< " " <<err_prog.GetComponent(i)<< endl; 
   }
        Psi210_file.close();
	Raggio210_file.close();
	cout << "Dati salvati in Psi210.txt Psi210 in coordinate cartesiane in questa forma -> X Y Z " <<endl;
        cout << "Dati salvati in Raggio210.txt valor medio e incertezza su r in questa forma -> X Y Yerr " <<endl;
        cout << "Acceptation rate: " << double(acc)/M << endl;			
        rnd.SaveSeed();
return 0;
}
//funzioni
void Metropolis100(void){
	x_new = x_old + rnd.Rannyu(-delta, delta);	//nuove coordinate
	y_new = y_old + rnd.Rannyu(-delta, delta);
	z_new = z_old + rnd.Rannyu(-delta, delta);
	
	p_old = Psi100(x_old, y_old, z_old);		//calcolo probabilità
	p_new = Psi100(x_new, y_new, z_new);
	
	if(rnd.Rannyu() < min(p_new/p_old, 1.)) {	 //accept? altrimenti torna alla posizione precedente
		acc++;
	} else {
		x_new = x_old;
		y_new = y_old;
		z_new = z_old;
	}
	
	raggio = sqrt(x_new*x_new + y_new*y_new + z_new*z_new);	//calcolo r
	x_old = x_new;		//prepare for next step
	y_old = y_new;
	z_old = z_new;
	return;
}

void Metropolis210(void){
	x_new = x_old + rnd.Rannyu(-delta, delta);	
	y_new = y_old + rnd.Rannyu(-delta, delta);
	z_new = z_old + rnd.Rannyu(-delta, delta);
	
	p_old = Psi210(x_old, y_old, z_old);			
	p_new = Psi210(x_new, y_new, z_new);
	
	if(rnd.Rannyu() < min(p_new/p_old, 1.)) { //acceptance?
		acc++;
	} else {
		x_new = x_old;
		y_new = y_old;
		z_new = z_old;
	}
	raggio = sqrt(x_new*x_new + y_new*y_new + z_new*z_new);	
	x_old = x_new;											
	y_old = y_new;
	z_old = z_new;
	return;
}
