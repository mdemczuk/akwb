#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <set>
#include <ctime>

using namespace std;

void readFile(ifstream &file);
void checkNewton(int multi_set_size, int k, bool &qualified_for_mapping);
void search(int ind, bool &exists);

int maxEl = 0, maxInd = 0;
clock_t start, finish;
vector<int> multi_set;
vector<int> used;
vector<int> map;

int main()
{
	string fileNumber;
	cout << "Wpisz numer instancji, ktora chcesz otworzyc: " << endl;
	cin >> fileNumber;
	string fileName = "instance" + fileNumber + ".txt";
	ifstream file;
	file.open(fileName.c_str(), ios::in);

	if(file.good()){
		readFile(file);
		file.close();
    	bool qualifiedForMapping = false;
    	int multi_setSize = multi_set.size();
    	int k = 0;
    	while(qualifiedForMapping == false && k < multi_setSize){
    		k++;
    		checkNewton(multi_setSize, k, qualifiedForMapping);
    	}

    	if(qualifiedForMapping){
    		int maxEl2 = 0;
    		for(int i = 0; i < multi_setSize; i++){
    			if(multi_set[i] > maxEl)
    				maxEl = multi_set[i];
    		}

    		for(int i = 0; i < multi_setSize; i++){
    			if(multi_set[i] > maxEl2 && multi_set[i] < maxEl)
    				maxEl2 = multi_set[i];
    		}

    		int firstEl = maxEl - maxEl2;
    		map.push_back(firstEl);
    		used.push_back(firstEl);
    		maxInd = k+1;
    		bool mapExists = false;
    		start = clock();
    		search(1, mapExists);
    		finish = clock();
    		double time = (double)(finish - start) / (double)(CLOCKS_PER_SEC);
    		if(!mapExists)
    			cout << "Dla podanego multizbioru nie istnieje mapa." << endl;
    		else
    			cout << "Rozwiazanie znaleziono w czasie rownym " << time << "s" << endl;
    	} // if qualifiedForMapping
    	else
    		cout << "Nie istnieje liczba ciec przekladajaca sie na licznosc multizbioru" << endl;
	}
    else
    	cout << "Nie udalo sie poprawnie otworzyc pliku z instancja" << endl;

    return 0;
}

void readFile(ifstream &file){
	int buff;
	do{
		file >> buff;
		if(file.eof())
			break;
		multi_set.push_back(buff);
	}
	while(!file.eof());
}

void checkNewton(int multi_set_size, int k, bool &qualified_for_mapping){
	double factorial1 = 1;			// liczona silnia dla k+2
	for(int i = 1; i <= (k+2); i++)
		factorial1 = factorial1 * i;

	double factorial2 = 1;			// liczona silnia dla k
	for(int i = 1; i <= k; i++)
		factorial2 = factorial2 *i;

	int newton_symbol;
	newton_symbol = factorial1 / (2 * factorial2);

	if(newton_symbol == multi_set_size){
		qualified_for_mapping = true;
	}
}

void search(int ind, bool &exists){
    bool found = false;
	if(ind == maxInd){
		set<int> s1;
    	s1.insert(multi_set.begin(), multi_set.end());
    	set<int> s2;
    	s2.insert(used.begin(), used.end());
		if(s1 == s2){
			cout << "Znaleziono rozwiazanie: " << endl;
			for(int i = 0; i < map.size(); i++)
				cout << map[i] << " ";
			cout << endl;
			exists = true;
			found = true;
		}
	}

	if(!found){
		int el_checked = 0;
		for(int i = 0; i < multi_set.size(); i++){
			int multi_set_count = 0;
			int used_count = 0;
			for(int j = 0; j < multi_set.size(); j++)
				if(multi_set[i] == multi_set[j])
					multi_set_count++;

			for(int j = 0; j < used.size(); j++)
				if(multi_set[i] == used[j])
					used_count++;

			if(multi_set_count <= used_count || multi_set[i] == el_checked)
				continue;

			int current_used_size = used.size();
			map.push_back(multi_set[i]);	// dodaje do mapy potencjalny element
			used.push_back(multi_set[i]);	// do wektora z uzytymi elementami dodaje potencjalny element
			int sum = multi_set[i];
			bool sum_exists = false;
			bool next = false;

			for(int j = map.size()-2; j >= 0; j--){
				int count_multi_set = 0;
				int count_used = 0;
				sum += map[j];
				for(int k = 0; k < multi_set.size(); k++)		// sprawdzam, czy suma istnieje w multizbiorze
					if(multi_set[k] == sum){
						count_multi_set++;
						sum_exists = true;
					}

				for(int l = 0; l < used.size(); l++)	// sprawdzam, czy suma zostala juz uzyta
					if(used[l] == sum)
						count_used++;

				if(count_multi_set > count_used)			// jezeli suma istnieje i nie zostala uzyta
					used.push_back(sum);				// to moze zostac dodana do wektora z uzytymi elementami

				if(!sum_exists || count_multi_set <= count_used){	// jezeli sprawdzana suma w ogole nie istnieje w multizbiorze lub zostala juz uzyta
					next = true;		// to nie warto kontynuowac tego rozwiazania
					for(int j = used.size()-1; j >= current_used_size; j--)
						used.pop_back();
					break;
				}
			}	// for liczacy sumy

			if(!next){
				int diff_multi_set = 0;
				int diff_used = 0;
				int end_distance = maxEl - sum;
				for(int j = 0; j < multi_set.size(); j++)
					if(end_distance == multi_set[j])
						diff_multi_set++;

				for(int j = 0; j < used.size(); j++)
					if(end_distance == used[j])
						diff_used++;

				if(diff_multi_set > diff_used && ind < maxInd)
					search(ind+1, exists);

				else if(ind == maxInd - 1 && diff_multi_set == 0 && sum == maxEl)
					search(ind+1, exists);
			}
			el_checked = map.back();
			map.pop_back();
			for(int j = used.size()-1; j >= current_used_size; j--)
				used.pop_back();

			if(exists)
				break;
		} // for dodawanie kazdego elementu z mapy
	}
}