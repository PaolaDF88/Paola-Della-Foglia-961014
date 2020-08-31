#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "function.h"

using namespace std;
//FUNZIONE-------------------------------------------------------------------------------------------------------------------------------------
Funzione::Funzione(){
	_a=0;
}

Funzione::Funzione(double a){
	_a=a;
}

Funzione::~Funzione(){};

double Funzione::Eval(double x) const {
	return (_a)*(M_PI/2)*cos(M_PI*x/2);
}
double Funzione::Eval_Importance(double x) const {
	return (_a)*(M_PI/2.) * cos( (M_PI * x) / 2.) / (2 * (1. - x));
}

void Funzione::SetA(double a){
	_a=a;
}

double Funzione::GetA() const{
	return _a;
}

void Funzione::Print() const{
	cout<<"f(x) = "<<"M_PI/2*cos(M_PI*x/2)= "<<endl;
}
double error (double AV, double AV2, int n) {
	if (n==0)
        return 0;
    else
        return sqrt((AV2 - AV*AV)/n);
}
