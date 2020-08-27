#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "random.h"
#include "vettore.h"
#include "math.h"
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
//PART1 Verifico che il valor medio della distrib. uniforme [0,1) sia r=1/2
   ofstream Risultati1("Risultati1.txt");				//file su cui scrivo i risultati
   	if (!Risultati1.is_open()) cerr << "PROBLEM: Unable to open Medie.txt" << endl;

   int M=100000; //totale di esperimenti
   int N=100;	 //n. di blocchi
   int L=M/N;    //n.di misure effettuate all'interno di un esperimento che è il blocco

   VettoreDati r(M), x(N), ave(N), ave2(N), sum_prog (N), sum2_prog (N),err_prog (N); //dichiaro i vettori
   for(int i=0; i<M; i++){
      r.SetComponent(i,rnd.Rannyu());
   }
   //r.Print();
   int k=0;
   double sum=0.;
   for (int i=0; i<N ; i++){
	sum=0.;
    for( int j=0; j<L ; j++)
	{
           k = j+i*L;
           sum+=r.GetComponent(k);
	}
   ave.SetComponent(i,sum/L); //valor medio che rappresenta l'esito di un esperimento nel blocco i-esimo
   ave2.SetComponent(i,pow(ave.GetComponent(i),2)); //valore quadratico medio
}
    for (int i=0; i<N ; i++){
    	for( int j=0; j<i+1 ; j++)
	   {
		sum_prog.SetComponent(i,sum_prog.GetComponent(i)+ave.GetComponent(j)); //valori medi cumulati o somma di valori medi
		sum2_prog.SetComponent(i,sum2_prog.GetComponent(i)+ave2.GetComponent(j));//somma di valori quadratici medi
	   }
	sum_prog.SetComponent(i,sum_prog.GetComponent(i)/(i+1));//media cumulata dei valori medi
	sum2_prog.SetComponent(i,sum2_prog.GetComponent(i)/(i+1));//media cumulata dei valori quadratici medi
	err_prog.SetComponent(i, error(sum_prog.GetComponent(i), sum2_prog.GetComponent(i), i));//dev_standard della media
	//err_prog.SetComponent(i, err_prog.Errore(sum_prog, sum2_prog, i));
	Risultati1 << L*(i+1) << " " << sum_prog.GetComponent(i)<< " " <<err_prog.GetComponent(i)<< endl; 
        }
	Risultati1.close();
	cout << "Dati salvati in Risultati1.txt: stima del valor medio e incertezza statistica per r nel formato -> x sum_prog err_prog" << endl;
//PART2 Calcolo della varianza del numero casuale distrib. uniformemente tra  [0,1)
	sum_prog.SetN(N), sum2_prog.SetN(N), ave.SetN(N),ave2.SetN(N),err_prog.SetN(N); //reinizializzo i vettori a zero
	ofstream Risultati2("Risultati2.txt");		
   	if (!Risultati2.is_open()) cerr << "PROBLEM: Unable to open Variance.txt" << endl;
   	for(int i=0; i<N ; i++){
		sum=0.;
    	for(int j=0; j<L ; j++)
	   {
           	k = j+i*L;
           	sum+=pow(r.GetComponent(k)-0.5,2.);// misure cumulate
	   }
   	ave.SetComponent(i,sum/L); //stima della varianza in ogni blocco
   	ave2.SetComponent(i,pow(ave.GetComponent(i),2.)); //valore quadratico della varianza
	}
    	for (int i=0; i<N ; i++){
    		for( int j=0; j<i+1 ; j++)
	   	{
			sum_prog.SetComponent(i,sum_prog.GetComponent(i)+ave.GetComponent(j)); //valori medi cumulati o somma di valori medi
			sum2_prog.SetComponent(i,sum2_prog.GetComponent(i)+ave2.GetComponent(j));//somma di valori quadratici medi
	   	}
	sum_prog.SetComponent(i,sum_prog.GetComponent(i)/(i+1));//media cumulata 
	sum2_prog.SetComponent(i,sum2_prog.GetComponent(i)/(i+1));//media cumulata dei valori quadratici medi
	err_prog.SetComponent(i, error(sum_prog.GetComponent(i), sum2_prog.GetComponent(i), i));//incertezza statistica sulla varianza
	Risultati2 << L*(i+1) << " " << sum_prog.GetComponent(i)<< " " <<err_prog.GetComponent(i)<< endl; 
        }
	Risultati2.close();
	cout << "Dati salvati in Risultati2.txt: stima della varianza e incertezza statistica su di essa per r nel formato -> x sum_prog err_prog" << endl;
//PART3 Test del Chi2	
	M = 100;			//Divido l'intervallo [0,1] in 100 intervalli identici
	double width = 1./double(M);	//ampiezza intervalli
	N = 1E4;			//n. di dati per ogni test del CHI2 
	int n_test = 100;	        //n. di test
	ofstream Risultati3("Risultati3.txt");		//file risultati test CHI2
	if (!Risultati3.is_open()) cerr << "PROBLEM: Unable to open CHI2" << endl;	
	cout << "Dati salvati in Risultati3.txt: per i 100 test del CHI2 nel format -> x y " << endl;	
	VettoreDati frequenza(M),CHI2(n_test);
	double tmp=0.;
	for(int i=0; i<n_test; i++){	
             frequenza.SetN(M); 
		for(int j=0; j<N; j++){		//ciclo per ogni test vengono generati N numeri pseudocasuali
			tmp = rnd.Rannyu();					
			for(int k=0; k<M; k++){
				if(is_in(double(k)*width, (double(k)+1)*width, tmp)) {frequenza.SetComponent(k,frequenza.GetComponent(k)+1); break;}//controllo in quale intervallo è il numero casuale e incremento la frequenza
		           }			
	           }	
	     double chi2=0.;
	     for(int j= 0; j<M; j++){		//compute CHI2
			chi2+=pow(frequenza.GetComponent(j)-double(N)/double(M), 2);
	   	}
	   CHI2.SetComponent(i,(chi2/(double(N)/double(M))));
	   Risultati3<<i<<" " << CHI2.GetComponent(i)<<endl;
	   }
	   Risultati3.close();
	   rnd.SaveSeed();		
return 0;
}

