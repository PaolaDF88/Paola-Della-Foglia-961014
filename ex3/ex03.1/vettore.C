#include "vettore.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

Vettore::Vettore() {
	m_N=0;
	m_v=NULL;
}

Vettore::Vettore(unsigned int N) {

	m_N=N;

	m_v=new double[N];
	for(unsigned int i=0; i<N; i++)
		m_v[i]=0.;

}

Vettore::Vettore(const Vettore& v) {

	m_N=v.GetN();

	m_v=new double[m_N];
	for(unsigned int i=0; i<m_N; i++)
		m_v[i]=v.GetComponent(i);

} 

Vettore::~Vettore() {
	delete []m_v;
	m_N=0;
	m_v=NULL;
}

Vettore& Vettore::operator=(const Vettore& v) {

	m_N=v.GetN();

	if(m_v)
		delete []m_v;

	m_v=new double[m_N];
	for(unsigned int i=0; i<m_N; i++)
		m_v[i]=v.GetComponent(i);

	return *this;

}

void Vettore::SetN(unsigned int N) {

	m_N=N;

	if(m_v)
		delete []m_v;

	m_v=new double[N];
	for(unsigned int i=0; i<N; i++)
		m_v[i]=0.;

}	

void Vettore::SetComponent(unsigned int i, double a) { m_v[i]=a; }

void Vettore::SetComponent(unsigned int i, int a) { m_v[i]=a; }

double Vettore::GetComponent(unsigned int i) const { return m_v[i]; }

VettoreDati::VettoreDati() : Vettore() { }

VettoreDati::VettoreDati(unsigned int N) : Vettore(N) { }

VettoreDati::VettoreDati(const char* nomefile) {

	unsigned int N=0;
	double appo;

	ifstream in(nomefile);
	if(!in){
		cout << endl << "Errore apertura file" << endl;
		exit(-1);
	}

	in >> appo;
	while(!in.eof()){
		N++;
		in >> appo;
	}		

	in.close();

	m_N=N;
	m_v=new double[N];

	in.open(nomefile);

	for(unsigned int i=0; i<N; i++)
		in >> m_v[i];

	in.close();
}

VettoreDati::VettoreDati(unsigned int N, const char* nomefile) : Vettore(N) {

	ifstream in(nomefile);
	if(!in){
		cout << endl << "Errore apertura file" << endl;
		exit(-1);
	}

	for(unsigned int i=0; i<N; i++)
		in >> m_v[i];	

	in.close();

}

VettoreDati::~VettoreDati() {
	delete []m_v;
	m_N=0;
	m_v=NULL;
}

void VettoreDati::Print() const {
	
	int width=int(log10(m_N)+1);
	for(unsigned int i=0; i<m_N; i++)
		cout << setw(width) << i << ") " << m_v[i] << endl;

}

void VettoreDati::Print(const char* nomefile) const {

	ofstream out(nomefile);
	int width=int(log10(m_N)+1);
	for(unsigned int i=0; i<m_N; i++){
		out << setw(width) << i << ") " << m_v[i] << endl;
           }
	out.close();
}
/*double VettoreDati::Errore(const VettoreDati& v1, const VettoreDati2& v2, unsigned int n) const {
	if (n==0)
        	return 0;
    	else
        	return sqrt((m_v2[n] - m_v[n]*m_v[n])/n);
		
}*/
