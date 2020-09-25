#ifndef __Percorso_h__
#define __Percorso_h__

#include <vector>
#include "city.h"
#include "random.h"
//classe Percorso
class Percorso {
public:
	//costruttori e distruttori
	Percorso ();
	~Percorso ();
	
	//metodi
	void Mescola(Random& , std::vector<City>);
	void Sort(int&, int&);
	bool Check();
	void Print(void);
	double GetLen(void);
	std::vector<int> Getp(void);
	Percorso operator=(Percorso);
	void Set(int, int);
	int Get(int);
	void Norma(std::vector<City>);
	void Inversione(Random&);
	void Shift1(Random&);
	void Shift2(Random&);
	void Permutazione(Random&);
	void Reverse(Random&);
	void Crossover(Random&, Percorso&);
	
protected:
	double m_lenght=0.;
	int m_lpath=32;
	std::vector<int> m_p;
};

#endif /* __Percorso_h__ */
