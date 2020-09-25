//parallel
int size_mpi, rank_mpi;
//data
int n_city=32;
int n_path=1024;
int n_generation =1024;
int ind_mut;
double r=1.;
Random rnd;
int seed[4];
int p1, p2;

//Settings
std::vector<City> Cities_vec (n_city);
std::vector<Percorso> Popolazione (n_path);

//probabilities of mutations
double prob_inversione = 0.1;
double prob_shift1 = 0.1;
double prob_shift2 = 0.1;
double prob_permutazione = 0.1;
double prob_reverse = 0.1;
double prob_crossover = 0.55;

//functions
void GeneraPopolazione();
void Initialize();
void Ordina(int, int);
int Selection();
void Measure(int);
void Mutazione(Percorso&);
void CompareAndOutput();
void Output();
void NextGeneration();
