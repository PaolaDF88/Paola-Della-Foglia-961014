#ifndef __function_h__
#define __function_h__
#include <iostream>
#include <fstream>
#include <cstdlib>

class FunzioneBase{
	
public:
	virtual double Eval(double x) const =0;
        virtual double Eval_Importance (double x) const=0;
	virtual void Print() const =0;
};

class Funzione: public FunzioneBase {

public:
	Funzione();
	Funzione(double a);
	~Funzione();
	double Eval(double x) const;
	double Eval_Importance(double x) const;
	void SetA(double a);
	double GetA() const;
	void Print() const;
private:
	double _a;
	
};
double error (double, double, int);//Funzione per la stima dell'incertezza statistica
#endif 

