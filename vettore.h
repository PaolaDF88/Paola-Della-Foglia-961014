#ifndef _VETTORE_H_
#define _VETTORE_H_


class Vettore {

	protected:
		unsigned int m_N;	//numero elementi
		double * m_v;		//vettore di m_N elementi
	public:
		Vettore();	//costruttore default
		Vettore(unsigned int N);	//costruttore con N elementi	
		Vettore(const Vettore&);	//copia un vettore
		~Vettore();	//ditruttore

		Vettore& operator=(const Vettore& v);	//nuova definizione di "="
		//altri metodi di modifica vettore
		void SetN(unsigned int N);
		unsigned int GetN() const { return m_N; };
		void SetComponent(unsigned int i, double a);
		void SetComponent(unsigned int i, int a);
		double GetComponent(unsigned int i) const;

};

class VettoreDati: public Vettore {

	private:
		void Scambia(unsigned int i, unsigned int j);	//scambia due componenti del vettore
		void Sort(unsigned int primo, unsigned int ultimo);	//riordina due elementi
	public:
		VettoreDati();
		VettoreDati(unsigned int N);
		VettoreDati(const char* nomefile);
		VettoreDati(unsigned int N, const char* nomefile);
		~VettoreDati();

		void Print() const;
		void Print(const char* nomefile) const;
		//double Errore(const VettoreDati& v1, const VettoreDati& v2, unsigned int n) const;


};
#endif
