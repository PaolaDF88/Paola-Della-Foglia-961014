#ifndef __integral_h__
#define __integral_h__
#include "function.h"
#include "random.h"
class Integral {
	
public:
	Integral(double a, double b, FunzioneBase *function);
	Integral(double a, double b, FunzioneBase *function, Random *rand);
	~Integral();
	double Media(int nstep);
        double Media_Importance (int nstep);
	double ErrMedia(int nstep);
	double HitMiss(int nstep, double _h);
	double GetA();
	double GetB();
	double Geth() {return _h;};
	
private:
	double _a,_b;
	double _h;
	int _sign;
	double _integral;
	FunzioneBase *_integrand;
	Random *_rand;	
};

#endif
