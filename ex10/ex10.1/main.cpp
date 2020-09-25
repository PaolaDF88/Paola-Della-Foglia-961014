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

int main(){ 
    
     Initialize();
     GeneraPath();
     cout << "START SIMULATED ANNEALING " << endl;
     for(int i=0; i<temp_vec.size(); i++){
	 temp = temp_vec[i];
	 beta = 1./temp;
	 accepted = 0;
	 attempted =0;
	 
         for(int j=0; j<n_step; j++){
	    Metropolis ();
	 }
	 cout << "Temp = " << setw(5) << temp << " -----> " << " Lunghezza miglior percorso = " <<Best.GetLen() << endl;	
	 cout << "Accettazione = " << double(accepted) / double(attempted) << endl << endl;
	 Measure(i);
    }
    Output();
    return 0;
}

void Initialize() {
	//Initializing 
	system("bash clean.sh");
	
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
			//double theta = rnd.Rannyu(0.,2*M_PI);
			//Cities lungo un cerchio
			//Cities_vec[i] = City(r*cos(theta), r*sin(theta), i+1);
			//Cities in un quadrato
			Cities_vec[i] = City(rnd.Rannyu(), rnd.Rannyu(), i+1);
                        WriteCities << Cities_vec[i].GetIndex() << " " << Cities_vec[i].GetX() << " " << Cities_vec[i].GetY() << endl;

		} 
		WriteCities.close();
		cout << "File 'cities.0' didn't exist, I created a new one" << endl;
	}
	ReadInput.close();
        double t=10.25;
	temp_vec.push_back( t );
	for(int i=0; i<1024; i++){
	    t -= 0.01;
	    temp_vec.push_back( t );
	}
	return;
}

void GeneraPath() {
        
        Path.Mescola(rnd, Cities_vec);
	Best = Path;
	cout << "Ho creato " << n_path <<" percorsi casuali" << endl;
	return;
}

void Metropolis(){
     
     Percorso OldPath = Path;
     Mutazione(Path);
     
     double p, l_old, l_new;
     l_old = OldPath.GetLen();
     l_new = Path.GetLen();
     
     if(l_new < Best.GetLen()){
	Best = Path;
     }
     p = exp(-beta * (l_new -l_old));
     if(p < rnd.Rannyu()) {
    	Path = OldPath;
     } else {
	      accepted++;
       }
       attempted++;    
       return;
}

void Measure(int index){
	
        ofstream SmallestPath;
	SmallestPath.open("SmallestPath.dat", ofstream::app);
	SmallestPath << index << " " << Best.GetLen() << endl;
	SmallestPath.close();	
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

void Output(){
     ofstream CitiesOutput;
     CitiesOutput.open("cities.final");
     for(int i=0; i<n_city-1; i++){
	 CitiesOutput << Cities_vec[Best.Get(i)].GetX() << " " << Cities_vec[Best.Get(i)].GetY() << endl;
     }
     CitiesOutput << Cities_vec[Best.Get(0) ].GetX() << " " << Cities_vec[Best.Get(0) ].GetY() << endl;
     cout << endl << "Ho salvato i dati in cities.final" << endl;
     CitiesOutput.close();
     return;
}


