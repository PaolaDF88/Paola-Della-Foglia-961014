//data
int n_city=32;
int ind_mut;
int n_path=1024;
//int n_generation =1024;
double r=1.;
std::vector<double> temp_vec;
int Temp_num_step;
double beta;
double temp;
int n_step = 15000;
int attempted, accepted;
Random rnd;
int seed[4];
int p1, p2;

//Settings
std::vector<City> Cities_vec (n_city);
Percorso Path;
Percorso Best;

//probabilities of mutations
double prob_inversione = 0.1;
double prob_shift1 = 0.1;
double prob_shift2 = 0.1;
double prob_permutazione = 0.1;
double prob_reverse = 0.1;

//functions
void GeneraPath();
void Initialize();
void Metropolis();
void Mutazione(Percorso&);
void Output();
void Measure(int);
