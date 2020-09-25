#include "mpi.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <cmath>
#include <iomanip>
#include <string>
#include <vector>
#include "city.h"
#include "random.h"
#include "percorso.h"
#include "main.h"
using namespace std;

int main(int argc, char* argv[]){ 
	
	MPI::Init(argc,argv);		//start the parallelized work
	
	size_mpi = MPI::COMM_WORLD.Get_size();	//numero di processi
	rank_mpi = MPI::COMM_WORLD.Get_rank();	//assegna il rank
	double tstart=MPI::Wtime();
	Initialize();
	GeneraPopolazione();
	Ordina(0,n_path-1);
	Measure(0);
	for(int gen=1; gen<=n_generation; gen++){
		NextGeneration();
		if(gen%100 == 0) cout << "Generation number: " << gen << endl;
		Ordina(0, n_path-1);
		Measure(gen);
     	}
     	//Output();
	CompareAndOutput();
     	double tend=MPI::Wtime();
     	double dt=tend-tstart; //tempo di esecuzione codice
     	cout << " tempo di esecuzione= " << dt << " s " << endl;
     	MPI::Finalize();
     	return 0;
}

void Initialize() {
	//Initializing 
	system("bash clean.sh");
	
   	ifstream Primes("Primes");
   	if (Primes.is_open()){
   		for(int i=0; i<rank_mpi; i++) Primes >> p1 >> p2;
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
	
	//read cities
	ifstream ReadInput;
	ReadInput.open("cities.0");
	if (ReadInput.is_open()){
		double x, y;
		int index;
		
		for(int i=0; i<n_city; i++){
			ReadInput >> index >> x >> y;
			Cities_vec[i].SetX(x);
			Cities_vec[i].SetY(y);
			Cities_vec[i].SetIndex(index);
		}
		cout << "Leggo le posizioni delle cities.0" << endl;

	} else {
		
		ofstream WriteCities;
		WriteCities.open("cities.0");

		for(int i=0; i<n_city; i++) {
			if (rank_mpi==0){
			//double theta = rnd.Rannyu(0.,2*M_PI);
			//Cities lungo un cerchio
			//Cities_vec[i] = City(r*cos(theta), r*sin(theta), i+1);
			//Cities in un quadrato
			Cities_vec[i] = City(rnd.Rannyu(), rnd.Rannyu(), i+1);
                        WriteCities << Cities_vec[i].GetIndex() << " " << Cities_vec[i].GetX() << " " << Cities_vec[i].GetY() << endl;
			}
		} 
		WriteCities.close();
		cout << "File 'cities.0' didn't exist, I created a new one" << endl;
	}
	ReadInput.close();
	return;
}

void GeneraPopolazione() {
	for(int i=0; i<n_path; i++){
		Popolazione[i].Mescola(rnd, Cities_vec);
	}
	if(rank_mpi==0) cout << " Ho creato " << n_path <<" percorsi casuali" << endl;
	return;
}

void Ordina(int left, int right){
	int i = left, j = right;
	Percorso tmp;
	
	double pivot = Popolazione[(left + right) / 2].GetLen();
	while (i <= j) {	//bisezione
	      while (Popolazione[i].GetLen() < pivot)
		     i++;
	      while (Popolazione[j].GetLen() > pivot)
		    j--;
	      if (i <= j) {
	         tmp = Popolazione[i];
		 Popolazione[i] = Popolazione[j];
		 Popolazione[j] = tmp;
		 i++;
	     	 j--;
	      }
	}
	if (left < j)	//ricorsività
	   Ordina(left, j);
	if (i < right)
	   Ordina(i, right);
	return;
}

int Selection(){
    	double r=rnd.Rannyu(0.,1.);
        while (r==1){
		r=rnd.Rannyu(0.,1.);
	}
	double p=1.3;			//p>1 
	int number = int( double(n_path) * pow(r, p));
	while( number == 0){
	      r=rnd.Rannyu(0.,1.);
              while (r==1){
              	    r=rnd.Rannyu(0.,1.);
             }
	     number = int( double(n_path) * pow(r, p));
	}
        return number;
}

void Measure(int generation){
	ofstream SmallestPath, MeanPath;
	
	SmallestPath.open("SmallestPath.thread"+to_string(rank_mpi)+".dat", ofstream::app);
	//SmallestPath.open("SmallestPath.dat", ofstream::app);
        //MeanPath.open("MeanPath.dat", ofstream::app);

	int cont=0;
	double avg=0.;
	
	for(int i=0; i<(n_path/2); i++){
	    avg += Popolazione[i].GetLen();
	    cont++;
	}
	
	SmallestPath << generation << " " << Popolazione[0].GetLen() << endl;
	MeanPath << generation << " " << avg/(double)cont << endl;
	
	SmallestPath.close();
	MeanPath.close();	
	return;
}

void Mutazione(Percorso& p){
	int which_mutation = (int)rnd.Rannyu(0., 5.);
	switch(which_mutation){
		//inversione di due città (permutazione di coppia)
		case 0:
			if(rnd.Rannyu() < prob_inversione)
			   p.Inversione(rnd);
			break;
		//shift1 delle città
		case 1:
			if(rnd.Rannyu() < prob_shift1)
			   p.Shift1(rnd);
			break;
		//shift2 città contigue
		case 2:
			if(rnd.Rannyu() < prob_shift2)
			   p.Shift2(rnd);
			break;
		//permuta m città con altre m città diverse
		case 3:
			if(rnd.Rannyu() < prob_permutazione)
			   p.Permutazione(rnd);
			break;	
		//Reverse l'ordine delle m città
		case 4:
			if(rnd.Rannyu() < prob_reverse)
			   p.Reverse(rnd);
			break;
			
		default:
			cout << "ERROR: Mutazione aspettata ----> which_mutation = " << which_mutation << endl;
			break;
	}

	p.Norma(Cities_vec);
	return;
}

void NextGeneration(){
	vector<Percorso> NuovaPopolazione (n_path);
	//si conservano le prime due popolazioni
	NuovaPopolazione[0] = Popolazione[0];
	NuovaPopolazione[1] = Popolazione[1];

	for(int i=2; i<n_path; i+=2){
	//sceglie 2 percorsi dalla generazione precedente
	    int father = Selection();
	    int mother = Selection();
	    while (father == mother){
		   mother = Selection();
	    }
	    NuovaPopolazione[i]   = Popolazione[father];
	    NuovaPopolazione[i+1] = Popolazione[mother];
	    if(rnd.Rannyu() < prob_crossover){
		NuovaPopolazione[i].Crossover( rnd, NuovaPopolazione[i+1] );
	    }
	    Mutazione(NuovaPopolazione[i]);
	    Mutazione(NuovaPopolazione[i+1]);

	    NuovaPopolazione[i].Norma(Cities_vec);
            NuovaPopolazione[i+1].Norma(Cities_vec);	
	}
	Popolazione = NuovaPopolazione;
	return;
}

void Output(){
	ofstream CitiesOutput;
     	CitiesOutput.open("cities.final");
     	for(int i=0; i<n_city; i++){
		if (rank_mpi==0){
	 	CitiesOutput<< Cities_vec[Popolazione[0].Get(i)].GetX() << " " << Cities_vec[Popolazione[0].Get(i)].GetY() << endl;
		}
     		
	}
     	CitiesOutput<< Cities_vec[Popolazione[0].Get(0) ].GetX() << " " << Cities_vec[Popolazione[0].Get(0) ].GetY() << endl;
        cout << endl << "Ho salvato i dati in cities.final" << endl;
     	CitiesOutput.close();
     	return;
}
void CompareAndOutput(){
	double best = Popolazione[0].GetLen();
	double results_from_threads[size_mpi];
	int best_thread=0;
	
	MPI_Gather(&best,1,MPI_DOUBLE,&(results_from_threads[rank_mpi]),1,MPI_DOUBLE,0, MPI::COMM_WORLD);
	
	//choose the best one from all threads
	if(rank_mpi==0){
		for(int i=0; i<size_mpi; i++){
			cout << "thread " << i << " len " << results_from_threads[i] << endl;
			if(results_from_threads[best_thread] > results_from_threads[i]){
				best_thread = i;
			}
			
		}
		cout << "The best thread was the number " << best_thread << " which found a path with lenght = " << results_from_threads[best_thread] << endl;
		
	}
	//send the best thread to everyone
	MPI_Bcast(&best_thread,1,MPI_INTEGER,0,MPI::COMM_WORLD);
	
	if(best_thread == rank_mpi) Output();	
	return;
}

