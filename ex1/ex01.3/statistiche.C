#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "statistiche.h"

using namespace std;

double error (double AV, double AV2, int n) {
	if (n==0)
        return 0;
    else
        return sqrt((AV2 - AV*AV)/n);
}
bool is_in (double a, double b, double x){
	if(a>b) cout<<"Inserisci a<b!"<<endl;
	if(x>a && x<b) return true;
	return false;
}
bool intersezione (double y,double theta,double L, double d,double MIN, double MAX){
	double tmp=0,ymax=0,ymin=0,yline=MIN;
	ymax=y+L/2*sin(theta);
	ymin=y-L/2*sin(theta);
	if (ymin>ymax){
	   tmp=ymin;
	   ymin=ymax;
	   ymax=tmp;
	   }
	while(yline<MAX+L/2){
	     if(ymin<yline && ymax>yline)
			return true;
		yline+=d;
	     } 
	return false;
}
