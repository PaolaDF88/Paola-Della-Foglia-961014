#include <iostream>
#include <cmath>
#include "percorso.h"
#include "random.h"

Percorso::Percorso(){
	for(int i=0; i<m_lpath; i++){
	    m_p.push_back(i);
	}
}
Percorso::~Percorso(){
}

void Percorso::Sort (int &a, int &b){
	int c;
	c=a;
	a=b;
	b=c;
	return;
}

bool Percorso::Check(){
	for(int i=0; i<m_lpath-1; i++){
	    for(int j=i+1; j<m_lpath; j++){
		if(m_p[i]==m_p[j]){
		   std::cout << "Il percorso contiene una ripetizione" << std::endl;
		   return false;
		}
	    }
	}
        return true;
}

void Percorso::Print(void){
	for(int i=0; i<m_lpath; i++){
	    std::cout << m_p[i] << " ";
	}
	std::cout << " ---> lunghezza percorso: " << m_lenght << std::endl;
	return;
}

double Percorso::GetLen() {
       return m_lenght;
}

std::vector<int> Percorso::Getp(void){
        return m_p;
}

void Percorso::Set(int index, int value){
	m_p[index] = value;
	return;
}

int Percorso::Get(int index){
	return m_p[index];
}

Percorso Percorso::operator=(Percorso p2){
	m_lenght = p2.GetLen();
	for(int i=0; i<m_lpath; i++){
	   m_p[i] = p2.Get(i);
	}
        return *this;
}
void Percorso::Norma(std::vector<City> vec_city){
	int city1, city2;
	m_lenght = 0.;
	for(int i=0; i<vec_city.size(); i++){
	    city1 = m_p[i];
	    if(i==vec_city.size()-1){
		city2 = m_p[0];
	    } else{
		     city2 = m_p[i+1];
	      }
	//std::cout<< i+1 <<std::endl;
	//std::cout<<city1 << " " << city2 <<std::endl;
     m_lenght += sqrt(pow(vec_city[city1].GetX() - vec_city[city2].GetX(), 2) + pow(vec_city[city1].GetY() - vec_city[city2].GetY(), 2) );

	}
     return;
}

void Percorso::Inversione(Random& rnd){
	int pos1, pos2;
	pos1 = (int)rnd.Rannyu(0., 32.);
	pos2 = (int)rnd.Rannyu(0., 32.);
	while(pos1==pos2){
	     pos2 = (int)rnd.Rannyu(0., 32.);
	}
	Sort(m_p[pos1], m_p[pos2]);
	if(!Check())
	  std::cout << "ERROR : INVERSIONE" << std::endl;
        return;
}

void Percorso::Mescola(Random& rnd, std::vector<City> vec_city){
	for(int i=0; i<100; i++){
		Inversione(rnd);
	}
	Check();
	Norma(vec_city);
        return;
}

void Percorso::Shift1(Random& rnd){
        std::vector<int> tmp (m_p);
	int index = (int)rnd.Rannyu(1., m_lpath);
	for(int i=0; i<m_lpath; i++){
	   m_p[index] = tmp[i];
	   index++;
	   if(index==m_lpath) index=0;
	}
	if(!Check())
	   std::cout << "ERROR : SHIFT" << std::endl;
        return;
}

void Percorso::Shift2(Random& rnd){
	std::vector<int> tmp (m_p);
	int start = (int)rnd.Rannyu(0., m_lpath/2);
	int index = (int)rnd.Rannyu(0., m_lpath/2);
	for(int i=0; i<start; i++){
		m_p[i] = tmp[i];
	}
	for(int i=start; i<m_lpath; i++){
	    m_p[index+start] = tmp[i];
	    index++;
	    if(index+start==m_lpath) index=0;
	}
	if(!Check())
	   std::cout << "ERROR : SHIFT città contigue" << std::endl;
	return;
}

void Percorso::Permutazione(Random& rnd){
	std::vector<int> tmp (m_p);
	int lenght = (int)rnd.Rannyu(0., m_lpath/2);
	int first = (int)rnd.Rannyu(0., m_lpath/2 - lenght);
	int second = (int)rnd.Rannyu(m_lpath/2, m_lpath - lenght);
	
	for(int i=0; i<first; i++)
		m_p[i] = tmp[i];
	for(int i=0; i<lenght; i++)
		m_p[first+i] = tmp[second+i];
	for(int i=0; i<second-first; i++)
		m_p[first+lenght+i] = tmp[first+lenght+i];
	for(int i=0; i<lenght; i++)
		m_p[second+i] = tmp[first+i];
	for(int i=0; i<m_lpath-second-lenght; i++)
		m_p[second+lenght+i] = tmp[second+lenght+i];
	if(!Check())
	   std::cout << "ERROR : PERMUTAZIONE" << std::endl;
	return;
}

void Percorso::Reverse(Random& rnd){
	std::vector<int> tmp (m_p);
	int begin = (int)rnd.Rannyu(0., m_lpath-2);
	int lenght = (int)rnd.Rannyu(0., m_lpath-begin+1);
	
	for(int i=0; i<begin; i++)
		m_p[i] = tmp[i];
	for(int i=0; i<lenght; i++)
		m_p[begin+i] = tmp[begin+lenght-i-1];
	for(int i=0; i<m_lpath-begin-lenght; i++)
		m_p[begin+lenght+i] = tmp[begin+lenght+i];
	if(!Check())
	  std::cout << "ERROR : REVERSE" << std::endl;
	return;
}

void Percorso::Crossover(Random& rnd, Percorso& p2){
	std::vector<int> tmp1 (m_p);
	std::vector<int> tmp2 (p2.Getp());
	
	std::vector<int> new1 (m_p);
	std::vector<int> new2 (p2.Getp());
	
	std::vector<int> missingfrom1;
	std::vector<int> missingfrom2;
	
	int divide = (int)rnd.Rannyu(0., m_lpath);
	
	for(int i=divide; i<m_lpath; i++){
	    missingfrom1.push_back( m_p[i] );
            missingfrom2.push_back( p2.Get(i) );
	    new1.pop_back();
	    new2.pop_back();
	}
	
	for(int i=0; i<m_lpath; i++){
	    for(int j=0; j<missingfrom1.size(); j++){
		if(tmp2[i] == missingfrom1[j]){
		   new1.push_back(tmp2[i]);
		   break;
		}
	    }
	}
	
	for(int i=0; i<m_lpath; i++){
	    for(int j=0; j<missingfrom2.size(); j++){
		if(tmp1[i] == missingfrom2[j]){
		   new2.push_back(tmp1[i]);
		   break;
		}
	    }
	}
	
	for(int i=0; i<m_lpath; i++){
		m_p[i] = new1[i];
		p2.Set(i, new2[i]);
	}
	
	if(!Check() || !p2.Check()){
	   std::cout << "ERROR: CROSSOVER" << std::endl;
	}
	return;
}

