#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "function.h"

using namespace std;

double error (double AV, double AV2, int n){
	if (n==0)
           return 0;
        else
            return sqrt((AV2 - AV*AV)/n);
}
