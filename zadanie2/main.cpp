#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>

using namespace std;


int main()
{
	int V;	// zmienna na wierzcholki

	// wczytywanie z pliku
	string fname;
	cout << "Wpisz nazwe pliku, z ktorego chcesz wczytac graf: ";
	cin >> fname;
	string fname2;
	fname2 = fname + ".txt";
	ifstream file;
	file.open(fname2.c_str(), ios::in);
	if (file.good()){
		cout << "Poprawnie otworzono plik." << endl;
		file >> V;
		cout << "Liczba wierzcholkow: " << V << endl;
		vector<vector<int> > G;		// tworzenie wektorow
		vector<int> N;
		int v;
		for(int i = 0; i < V ; i++){
			N.clear();
            do{
               file >> v;
               if(v != 0)
                    N.push_back(v);		// zapisywanie listy nastepnikow w grafie
               }
            while(v != 0);
			G.push_back(N);
		}

		file.close();

		for(int i = 0; i < V; i++) {					// wypisywanie
        	for(int j = 0; j < G[i].size(); j++){
            	cout << G[i][j] << " ";
        	}
        	cout << endl;
		}

		// sprawdzanie czy graf jest 1-grafem
		bool is1graf = true;
		for(int i = 0; i < V; i++) {
        	for(int j = 0; j < G[i].size(); j++){
                while(G[i][j+1]){						// sprawdzanie czy istnieje nastepnik
                	//cout << "nastepnik istnieje" << endl;
                	if(j != 0){							// wartosci G[i][j] dla j=0 oznaczaja numery wierzcholkow
                		//cout << "j != 0" << endl;
            			if(G[i][j] == G[i][j+1]){
            				is1graf = false;
            				//cout << "Wystepuja dwa takie same nastepniki: " << G[i][j] << " oraz " << G[i][j+1] << ", co sprawia, ze ten graf nie jest 1-grafem." << endl;
            			}
                	}
            		if (is1graf == false){
            			//cout << "przypisuje wartosc false" << endl;
            			break;
            		}
                    else{
                        is1graf = true;
                        //cout << "przypisuje wartosc true" << endl;
                        break;
                    }
            	}
        	}
		}

		if(is1graf == false)
			cout << endl << "Wczytany graf nie jest 1-grafem." << endl;

		if(is1graf == true){
			cout << endl << "Wczytany graf jest 1-grafem." << endl;

			//sprawdzanie czy jest grafem sprzezonym
			bool isAdjoint = true;
			for (int i = 0; i < V; i++){
				for (int j = 1; j < G[i].size(); j++){
					if (G[i][j]){		// jezeli wierzcholek ma nastepniki
						for (int k = i + 1; k < V; k++){
							for (int l = 1; l < G[k].size(); l++){
								if (G[k][l]){			// jezeli nastepny wierzcholek ma nastepniki
									if (G[i][j] == G[k][l]){		// jezeli nastepniki sie powtarzaja w dwoch wierzcholkach
                                        if(equal (G[i].begin()+1, G[i].end(), G[k].begin()+1) ) // sprawdzanie, czy wszystkie nastepniki sie pokrywaja
                                        	;
										else
											isAdjoint = false;
									}
								}
							}
						}
					}
				}
			}
			if (isAdjoint == false)
				cout << "Graf nie jest sprzezony" << endl;

			if (isAdjoint == true){
				cout << "Graf jest sprzezony" << endl;

				// sprawdzanie, czy graf jest liniowy
				bool islinegraph = true;
				int m1, m2;								// zmienne na indeksy wierzcholkow, ktore wystepuja jako nastepniki w sprawdzanym wierzcholku
				int p;

				for(int i = 0; i < V; i++){
					for(int j = 1; j < G[i].size(); j++){
						if ((G[i].size() >= 3) && ((j+1) < G[i].size())){	 // jezeli wierzcholek ma przynajmniej dwa nastepniki
							//if(G[i][j+1]){				// jezeli nastepny od j nastepnik istnieje; prawdopodobnie to bedzie niepotrzebne
								m1 = G[i][j];
								m1 -= 1;			// do jednej zmiennej przypisuje indeks jednego nastepnika obecnego wierzcholka
								m2 = G[i][j+1];
								m2 -= 1;				// do drugiej zmiennej przypisuje indeks kolejnego nastepnika obecnego wierzcholka
								cout << i << ": " << m1 << ", " << m2 << endl;
								for (int k = 1; k < G[m1].size(); k++){
									for (int l = 1; l < G[m2].size(); l++){
										if((m1 != i) && (m2 != i))			// warunek ¿eby nie wykrywalo petli wlasnych
										if(G[m1][k] == G[m2][l]){			// sprawdzam, czy w tych dwoch wierzcholkach znajduje sie taki sam nastepnik
											islinegraph = false;
											cout << "Graf nie jest grafem liniowym, warunek 1 lub 2" << endl;
										}
									}
								}
								p = i + 1;				// numer obecnie sprawdzanego wierzcholka
								if(i == m1 || i == m2){		// jezeli wierzcholek ma petle wlasna, czyli ze ma siebie samego w nastepnikach (porownujemy indeksy); warunek 3.2
									for(int l = 1; l < G[m2].size(); l++){
										if((G[m2].size() >= 3) && ((l+1) < G[m2].size())){		// sprawdzam, czy wierzcholek o indeksie m2 ma wiecej niz jeden nastepnik
											//if(G[m2][l+1]){	// sprawdzam, czy nastepny od j nastepnik istnieje; prawdopodobnie to bedzie niepotrzebne
															// sprawdzam, czy ktorykolwiek z nastepnikow zawiera w swoich nastepnikach numer obecnego wierzcholka; warunek 3.3
													int m = m1 + 1;						// numer wierzcholka o indeksie m2
													if((G[m1][l] == m) || (G[m1][l+1] == m)){
														if((G[m1][l] == p) || (G[m1][l+1] == p)){		// sprawdzam, czy ktorykolwiek z nastepnikow to ten tworzacy petle wlasna; warunek 3.4, czyli ostatni dla warunku 3
														islinegraph = false;
														cout << "Graf nie jest grafem liniowym, warunek 3" << endl;
													}
												}
											//}
										}
									}
								}
							//}
						}
					}
				}

				if (islinegraph == true)//{
					cout << "Graf jest grafem liniowym." << endl;

					//transformacja grafu
					N.clear();
					vector<vector<int> > H;
					for(int i = 0; i < V; i++){			// tworze wektor, ktory poczatkowo wypelniam tylko dwiema wartosciami odpowiadajacymi parom lukow rozlacznych
						for(int j = 0; j <2; j++){
							int p = i+1;					// p odpowiada numerowi wierzcholka w grafie G
							cout << p << endl;
							if(j==0){
								int q = (2*p - 1);
								N.push_back(q);
							}
							if(j==1){
								int h = (2*p);
								N.push_back(h);
							}

							//cout << "Graf H: " << H[i][0] << endl;
						}
						H.push_back(N);
						N.clear();
					}
					for(int i = 0; i < V; i++) {					// wypisywanie
                        for(int j = 0; j < 2; j++){
                            cout << H[i][j] << " ";
                        }
                    cout << endl;
					}

					for(int i = 0; i < V; i++){
						for(int j = 1; j < G[i].size(); j++){		// istnieje luk miedzy wierzcholkiem o numerze i+1 oraz G[i][j]
							int m = G[i][j];						//zapisuje numer wierzcholka ktory w G jest nastepnikiem
							int h = H[m-1][0];						//zapisuje numer wierzcholka ktory bede zmieniac w grafie H na wartosc m
							H[m-1][0] = H[i][1];					//kompresuje wierzcholek oznaczajacy koniec jednego luku i poczatek drugiego do jednego wierzcholka
							cout << "Kompresja z " << h << " na " << H[m-1][0] << endl;
							int c = H[m-1][0] + 1;
							for(int k = 0; k < V; k++){
								for(int l = 0; l < H[k].size(); l++){
									if(H[k][l] == h){
										cout << "Nastepuje przeindeksowanie wierzcholka " << H[k][l] << " na " << H[m-1][0] << endl;
										H[k][l] = H[m-1][0];
										cout <<"Nastapilo przeindeksowanie wierzcholka " << H[k][l] << endl;
									}
								}
							}
						}
					}
					for(int i = 0; i < V; i++) {					// wypisywanie
                        for(int j = 0; j < 2; j++){
                            cout << H[i][j] << " ";
                        }
                    cout << endl;
					}

					cout << endl << endl;

					size_t index = 1;						// zmienna dla indeksów po przeindeksowaniu 
					map<size_t, size_t> translationMap;			// klucz: indeks wejsciowy, wartosc: indeks wyjsciowy
					for(size_t k = 0; k < H.size(); k++){
    					for(size_t l = 0; l < H[k].size(); l++){	
      						if(translationMap.count(H[k][l]) == 0){	// gdy klucz nie istnieje, to aktualna wartosc dodawna jest do mapy jako klucz
      							translationMap[H[k][l]] = index++; // indeks zwieksza sie o 1
      						}
      					H[k][l] = translationMap[H[k][l]]; // gdy klucz istnieje, to wartosc w wektorze jest zmieniana na to co jest w mapie pod aktualna wartoscia tablicy
    					}
    				}

    				for(size_t k = 0; k < H.size(); k++) {					// wypisywanie
                        for(size_t l = 0; l < H[k].size(); l++){
                            cout << H[k][l] << " ";
                        }
                    cout << endl;
					}

					// przepisywanie grafu w postaci listy nastepnikow
					int maxValue = 0;
					for(int i = 0; i < H.size(); i++){
						for(int j = 0; j < H[i].size(); j++){
							if(H[i][j] > maxValue){
								maxValue = H[i][j];			}
						}
					}
					cout << "Numer ostatniego wierzcholka: " << maxValue << endl;

					int vertex = 1;
					bool indexExists = false;
					N.clear();
					vector<vector<int> > originalGraph;
					do{
						for(int i = 0; i < H.size(); i++){						// stworzenie nowego grafu, ktory bedzie w formacie listy nastepnikow
							if(H[i][0] == vertex){
								if(indexExists == false){
									N.push_back(H[i][0]);
									indexExists = true;
								}
								if(indexExists == true){
									N.push_back(H[i][1]);
								}
							}
						}
						if(N.empty())
							N.push_back(vertex);
                        else
                            partial_sort(N.begin()+1, N.end(), N.end());
						originalGraph.push_back(N);
						N.clear();
						vertex++;
						indexExists = false;
					}
					while(originalGraph.size() < maxValue);


					for(int i = 0; i < originalGraph.size(); i++) {					// wypisywanie
                        for(int j = 0; j < originalGraph[i].size(); j++){
                            cout << originalGraph[i][j] << " ";
                        }
                    cout << endl;
					}


					// zapis do pliku
					string fname3 = fname + "Oryginalny.txt";
					ofstream outfile;
					outfile.open(fname3.c_str(), ios::out);
					if(outfile.good()){
						outfile << originalGraph.size() << endl;
						for(int i = 0; i < originalGraph.size(); i++){
							for(int j = 0; j < originalGraph[i].size(); j++){
								outfile << originalGraph[i][j] << " ";
							}
							outfile << "0" << endl;
						}
						outfile.close();
					}

					else
						cout << "Nie udalo sie zapisac pliku." << endl;
				//}
			}
		}
	}

	else
		cout << "Nie udalo sie otworzyc pliku o podanej nazwie." << endl;

    return 0;
}


/*int main()
{
    vector<int> v;
    for(int i = 10; i > 0; i--){
        v.push_back(i);
    }

    cout << "Before partial sort: ";
    for(int i = 0; i < v.size(); i++){
        cout << v[i] << " ";
    }
    cout << endl;
    cout << "=========" << endl;
    cout << endl;

    partial_sort(v.begin()+1, v.end(), v.end());

    cout << "After partial sort: ";
    for(int i = 0; i < v.size(); i++){
        cout << v[i] << " ";
    }
    cout << endl;
    return 0;
}*/
