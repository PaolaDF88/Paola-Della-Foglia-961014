#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "function.h"

using namespace std;

double error (double AV, double AV2, int n) {
	if (n==0)
        return 0;
    else
        return sqrt((AV2 - AV*AV)/n);
}
double n(double x){
	return 0.5 * (1. + erf(x/sqrt(2.)));
}

