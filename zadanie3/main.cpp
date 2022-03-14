#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

class Vertice
{
    public:
	string name;			// nazwa wierzcholka
	int position;			// pozycja pierwszego nukleotydu w sekwencji
	string sequence;		// nazwa sekwencji, do ktorej nalezy podciag
	int index;				// ogolny, unikalny indeks wierzcholka


	Vertice(){				// konstruktor
		name = 'Vertice';
		position = 0;
		sequence = '0';
		index = 0;
	}

	~Vertice() {}			// destruktor

	Vertice(string verName, int posNum, string seqID, int inxNum) : name(verName), position(posNum), sequence(seqID), index(inxNum) {}
};

void seqVec(ifstream &file, vector<char> &sequences, string &name);
void qualVec(ifstream &file, vector<int> &qualities, vector<char> &sequences);
void delNucl(int minValue, vector<int> &qualities, vector<char> &sequences1, vector<char> &sequences2, vector<int> &positions);
void sequenceVertices(int subStrLen, string sequenceID, int indexNumber, vector<char> &sequences, vector<int> &positions, Vertice *pointer, vector<Vertice*>& verticeList);
void createVertice(string verName, int posNum, string seqID, int inxNum, Vertice *pointer, vector<Vertice*>& verticeList);
void showInfo(vector<Vertice*>& verList, int i);
void addEdges(vector<Vertice*>& verList1, vector<Vertice*>& verList2, vector<int> &buffer, int i);
void findClique(vector<vector<int> > &g, vector<int> &clique, int i, bool &cliqueExists, bool &verticeClique);

int main()
{
	// WCZYTYWANIE PLIKÓW
	string numberFile;
    string fastaFile;
    string qualFile;
	cout << "Wpisz numer instancji, ktora chcesz otworzyc: " << endl;
	cin >> numberFile;
	fastaFile = "seq" + numberFile + ".fasta";
	ifstream file;
	file.open(fastaFile.c_str(), ios::in);			// otwieranie pliku .fasta z sekwencja

	if(file.good()){
		vector<char> buff1;							// tworzenie wektorow dla kazdej sekwencji
		vector<char> buff2;
		vector<char> buff3;
		vector<char> buff4;
		vector<char> buff5;
		cout << fastaFile << endl;
		string seq1ID;
		string seq2ID;
		string seq3ID;
		string seq4ID;
		string seq5ID;

		seqVec(file, buff1, seq1ID);							// wypelnianie wektorow sekwencja nukleotydow
		seqVec(file, buff2, seq2ID);
		seqVec(file, buff3, seq3ID);
		seqVec(file, buff4, seq4ID);
		seqVec(file, buff5, seq5ID);

		file.close();

		qualFile = "qual" + numberFile + ".qual";
		file.open(qualFile.c_str(), ios::in);			// otwieranie pliku .qual z wartosciami wiarygodnosci
		if(file.good()){
			vector<int> Qual1;
			vector<int> Qual2;
			vector<int> Qual3;
			vector<int> Qual4;
			vector<int> Qual5;
			cout << qualFile << endl;

			qualVec(file, Qual1, buff1);					// wypelnianie wektorow wartosciami wiarygodnosci
			qualVec(file, Qual2, buff2);
			qualVec(file, Qual3, buff3);
			qualVec(file, Qual4, buff4);
			qualVec(file, Qual5, buff5);

			file.close();

			//USUWANIE NUKLEOTYDOW
			int minQual = 0;
			cout << "Wpisz minimalna wartosc dla wiarygodnosci nukleotydow: " << endl;
			cin >> minQual;
			vector<char> Seq1;							// tworzenie wektorow dla kazdej sekwencji po usunieciu nukleotydow
			vector<char> Seq2;
			vector<char> Seq3;
			vector<char> Seq4;
			vector<char> Seq5;
			vector<int> Pos1;							// tworzenie wektorow dla oryginalnej pozycji dla nieusunietych nukleotydow
			vector<int> Pos2;
			vector<int> Pos3;
			vector<int> Pos4;
			vector<int> Pos5;

			delNucl(minQual, Qual1, buff1, Seq1, Pos1);
			delNucl(minQual, Qual2, buff2, Seq2, Pos2);
			delNucl(minQual, Qual3, buff3, Seq3, Pos3);
			delNucl(minQual, Qual4, buff4, Seq4, Pos4);
			delNucl(minQual, Qual5, buff5, Seq5, Pos5);

			buff1.clear();								// zwalnianie pamieci poprzez usuniecie niepotrzebnych wektorow
			buff2.clear();
			buff3.clear();
			buff4.clear();
			buff5.clear();
			Qual1.clear();
			Qual2.clear();
			Qual3.clear();
			Qual4.clear();
			Qual5.clear();

			// TWORZENIE PODCIAGOW
			cout << "Wpisz dlugosc podciagow sekwencji, ktore utworza wierzcholki w grafie (wartosc od 4 do 9): " << endl;
			int strLen = 0;
			cin >> strLen;
			if(strLen < 4 || strLen > 9){
				cout << "Niepoprawna dlugosc podciagu" << endl;
				return 1;
			}

			int indNum = 0;
            Vertice *pointer;

            vector<Vertice*> verticeList1;
            vector<Vertice*> verticeList2;
            vector<Vertice*> verticeList3;
            vector<Vertice*> verticeList4;
            vector<Vertice*> verticeList5;

			sequenceVertices(strLen, seq1ID, indNum, Seq1, Pos1, pointer, verticeList1);
			indNum += verticeList1.size();

			sequenceVertices(strLen, seq2ID, indNum, Seq2, Pos2, pointer, verticeList2);
			indNum += verticeList2.size();

			sequenceVertices(strLen, seq3ID, indNum, Seq3, Pos3, pointer, verticeList3);
			indNum += verticeList3.size();

			sequenceVertices(strLen, seq4ID, indNum, Seq4, Pos4, pointer, verticeList4);
			indNum += verticeList4.size();

			sequenceVertices(strLen, seq5ID, indNum, Seq5, Pos5, pointer, verticeList5);

			// TWORZENIE KRAWEDZI
			vector<vector<int> > Graph;				// wektor na listy sasiedztwa poszczegolnych wierzcholkow, umieszczone sa w nim unikalne indeksy kazdego wierzcholka
			vector<int> inxBuf;						// wektor pomocniczy na indeksy
			inxBuf.clear();
			for(int i = 0; i < verticeList1.size(); i++){			// dla wierzcholkow z kazdej sekwencji tworzone sa listy sasiedztwa
				pointer = verticeList1.at(i);
				inxBuf.push_back(pointer->index);
				addEdges(verticeList1, verticeList2, inxBuf, i);
				addEdges(verticeList1, verticeList3, inxBuf, i);
				addEdges(verticeList1, verticeList4, inxBuf, i);
				addEdges(verticeList1, verticeList5, inxBuf, i);
				if(inxBuf.size()>1){
					Graph.push_back(inxBuf);
				}
				inxBuf.clear();
			}

			for(int i = 0; i < verticeList2.size(); i++){
				pointer = verticeList2.at(i);
				inxBuf.push_back(pointer->index);
				addEdges(verticeList2, verticeList1, inxBuf, i);
				addEdges(verticeList2, verticeList3, inxBuf, i);
				addEdges(verticeList2, verticeList4, inxBuf, i);
				addEdges(verticeList2, verticeList5, inxBuf, i);
				if(inxBuf.size()>1){
					Graph.push_back(inxBuf);
				}
				inxBuf.clear();
			}

			for(int i = 0; i < verticeList3.size(); i++){
				pointer = verticeList3.at(i);
				inxBuf.push_back(pointer->index);
				addEdges(verticeList3, verticeList1, inxBuf, i);
				addEdges(verticeList3, verticeList2, inxBuf, i);
				addEdges(verticeList3, verticeList4, inxBuf, i);
				addEdges(verticeList3, verticeList5, inxBuf, i);
				if(inxBuf.size()>1){
					Graph.push_back(inxBuf);
				}
				inxBuf.clear();
			}

			for(int i = 0; i < verticeList4.size(); i++){
				pointer = verticeList4.at(i);
				inxBuf.push_back(pointer->index);
				addEdges(verticeList4, verticeList1, inxBuf, i);
				addEdges(verticeList4, verticeList2, inxBuf, i);
				addEdges(verticeList4, verticeList3, inxBuf, i);
				addEdges(verticeList4, verticeList5, inxBuf, i);
				if(inxBuf.size()>1){
					Graph.push_back(inxBuf);
				}
				inxBuf.clear();
			}

			for(int i = 0; i < verticeList5.size(); i++){
				pointer = verticeList5.at(i);
				inxBuf.push_back(pointer->index);
				addEdges(verticeList5, verticeList1, inxBuf, i);
				addEdges(verticeList5, verticeList2, inxBuf, i);
				addEdges(verticeList5, verticeList3, inxBuf, i);
				addEdges(verticeList5, verticeList4, inxBuf, i);
				if(inxBuf.size()>1){
					Graph.push_back(inxBuf);
				}
				inxBuf.clear();
			}
            bool Edges = false;
			for(int i = 0; i < Graph.size(); i++){
				for(int j = 0; j < Graph[i].size(); j++){
					if(Graph[i].size() > 4){
						Edges = true;
					}
				}
			}
			cout << "\n" << endl;

            if(Edges == true){
				// SZUKANIE KLIKI
                vector<int> Clique;
                bool cliqueExists = false;
                bool verticeClique = true;
                int m = 0;
                while(cliqueExists == false && m < Graph.size()){
                    findClique(Graph, Clique, m, cliqueExists, verticeClique);
                    if(cliqueExists == true)
                        break;
                    if(verticeClique == false){
                        m++;
                        continue;
                    }
                    m++;
                }

                if(cliqueExists == true){
                    sort(Clique.begin(), Clique.end());
                    //cout << "Klika: ";
                    //for(int i = 0; i < Clique.size(); i++)
                       // cout << Clique[i] << " ";
                    cout << endl;
                    cout << "===================" << endl;
                    cout << endl;
                    for(int i = 0; i < verticeList1.size(); i++){
                        Vertice* p = verticeList1.at(i);
                        if(Clique[0] == p->index){
                            cout << "Podciag: " << p->name << "\n" << endl;
                            cout << "ID sekwencji: " << p->sequence;
                            cout << "\tPozycja: " << p->position << endl;
                        }
                    }
                    for(int i = 0; i < verticeList2.size(); i++){
                        Vertice* p = verticeList2.at(i);
                        if(Clique[1] == p->index){
                            cout << "ID sekwencji: " << p->sequence;
                            cout << "\tPozycja: " << p->position << endl;
                        }
                    }
                    for(int i = 0; i < verticeList3.size(); i++){
                        Vertice* p = verticeList3.at(i);
                        if(Clique[2] == p->index){
                            cout << "ID sekwencji: " << p->sequence;
                            cout << "\tPozycja: " << p->position << endl;
                        }
                    }
                    for(int i = 0; i < verticeList4.size(); i++){
                        Vertice* p = verticeList4.at(i);
                        if(Clique[3] == p->index){
                            cout << "ID sekwencji: " << p->sequence;
                            cout << "\tPozycja: " << p->position << endl;
                        }
                    }
                    for(int i = 0; i < verticeList5.size(); i++){
                        Vertice* p = verticeList5.at(i);
                        if(Clique[4] == p->index){
                            cout << "ID sekwencji: " << p->sequence;
                            cout << "\tPozycja: " << p->position << endl;
                        }
                    }
                }
                else
                    cout << "W podanej instancji nie udalo sie wyszukac zadnej kliki" << endl;
            }
            else
                cout << "Podana instancja nie zawiera zadnej kliki" << endl;
		}

		else
			cout << "Nie udalo sie poprawnie otworzyc pliku .qual z podanym numerem instacji." << endl;
	}
	else
		cout << "Nie udalo sie poprawnie otworzyc pliku .fasta z podanym numerem instacji." << endl;

    return 0;
}

void seqVec(ifstream &file, vector<char> &sequences, string &name){			// funkcja wypelniajajaca wektor sekwencja nukleotydowa
	char buff, newSeq = '>';
	file >> buff;
        do{
            if(!file.eof()){
                if(buff == newSeq){
                    getline(file, name);
                    name.resize(14);
                }
                else if(buff != '<'){
                    sequences.push_back(buff);
                }
                else break;
            }
            else if(file.eof() || buff != newSeq)
                break;
            file >> buff;
        }
        while(buff != newSeq);
}

void qualVec(ifstream &file, vector<int> &qualities, vector<char> &sequences){			// funkcja wypelniajajaca wektor wartosciami wiarygodnosci
	char buff;
	string buffer;
	int buf;
	file >> buff;
	if(buff == '>'){
        getline(file, buffer);
    }
	for(int i = 0; i < sequences.size(); i++){
		file >> buf;
		qualities.push_back(buf);
	}
}

void delNucl(int minValue, vector<int> &qualities, vector<char> &sequences1, vector<char> &sequences2, vector<int> &positions){			// funkcja usuwajaca nukleotydy z sekwencji
	for(int i = 0; i < qualities.size(); i++)
		if(qualities[i] < minValue){
			qualities[i] = 0;
			sequences1[i] = '0';
		}

	for(int i = 0; i < sequences1.size(); i++)
		if(sequences1[i] != '0'){
			int index = i+1;
			sequences2.push_back(sequences1[i]);
			positions.push_back(index);
		}
}

void sequenceVertices(int subStrLen, string sequenceID, int indexNumber, vector<char> &sequences, vector<int> &positions, Vertice *pointer, vector<Vertice*>& verticeList){
	int count = 0;
	string buffer;
	buffer.clear();
	int j;
	int lastPos = sequences.size()-subStrLen;
	for(int i = 0; i <= lastPos; i++){
		j = i;
		while(count < subStrLen){
			buffer += sequences[j];		// do bufora zostaje dodany nukleotyd z sekwencji
			count++;
			j++;
		}

		if(count == subStrLen){
			int pos = positions[i];
			createVertice(buffer, pos, sequenceID, indexNumber, pointer, verticeList);
			count = 0;
			buffer.clear();
			indexNumber++;
		}
	}
}

void createVertice(string verName, int posNum, string seqID, int inxNum, Vertice *pointer, vector<Vertice*>& verticeList){
	Vertice *ptrVertice = new Vertice(verName, posNum, seqID, inxNum);
	verticeList.push_back(ptrVertice);
}

void showInfo(vector<Vertice*>& verList, int i){
    cout << verList[i]->name << " ";
    cout << verList[i]->position << " ";
    cout << verList[i]->sequence << " ";
    cout << verList[i]->index << endl;
}

void addEdges(vector<Vertice*>& verList1, vector<Vertice*>& verList2, vector<int> &buffer, int i){
	Vertice* p = verList1.at(i);					// wskaźnik wskazujący na element o indeksie i z wektora verticeList
	string str1 = p->name;
	for(int j = 0; j < verList2.size(); j++){
		Vertice* p2 = verList2.at(j);
		string str2 = p2->name;
		if(str1.compare(str2) == 0){
			buffer.push_back(p2->index);
		}
	}
}

void findClique(vector<vector<int> > &g, vector<int> &clique, int i, bool &cliqueExists, bool &verticeClique){
	if(g[i].size() > 4){					// wierzcholek musi byc polaczony z co najmniej 4 krawedziami
		int ver = 0;
		int index = 0;
		int count = 0;
		vector<int> buffer;					// przechowuje wszystkie wierzcholki, z ktorych mozna utworzyc klike
		vector<int> bufferInx;				// przechowuje indeksy wierzcholkow z buffer, pod jakimi znajduja sie w wektorze wektorow g/Graph
		vector<int> buffer2;				// przechowuje wszystkie wierzcholki, ktorych sekwencje pojawiaja sie wiecej niz raz w buffer
		vector<int> buffer2Inx;				// przechowuje indeksy wierzcholkow z buffer2
		buffer.push_back(g[i][0]);
		bufferInx.push_back(i);

		for(int j = 1; j < g[i].size(); j++){
			ver = g[i][j];
			for(int k = 0; k < g.size(); k++){
				if(g[k][0] == ver){
					index = k;
				}
			}
			if(g[index].size() > 4){
				for(int k = 0; k < g[i].size(); k++){
					for(int l = 0; l < g[index].size(); l++){
						if(g[i][k] == g[index][l]){
							count++;
						}
					}
				}
				if(count >= 5){
					buffer.push_back(ver);		// zapisuje wierzcholek
					bufferInx.push_back(index);	// zapisuje jego indeks
				}
			}
			count = 0;
		}

		count = 0;
		if(buffer.size() >= 5){
			for(int j = 0; j < buffer.size(); j++){
				index = bufferInx[j];
				for(int k = 0; k < g[index].size(); k++){
					for(int l = 0; l < buffer.size(); l++){
						if(buffer[l] == g[index][k]){
							count++;
						}
					}
				}
				if(count == buffer.size()){
					clique.push_back(buffer[j]);	// jezeli wierzcholek wystepuje we wszystkich listach kazdego wierzcholka z buforu, to zostaje dodany do kliki
				}
				else if(count < buffer.size()){
						buffer2.push_back(buffer[j]);		// jezeli wiarygodnosc jest mniejsza od rozmiaru bufora, to dodaje te mniej wiarygodne wierzcholki do buforu pomocniczego
						buffer2Inx.push_back(bufferInx[j]);		// wraz z ich indeksami
                }
                count = 0;
            }

			if(clique.size() == 4){								// jezeli w strukturze znajduje sie wiecej niz jeden wierzcholek z jednej sekwencji
				clique.push_back(buffer2[0]);
				cliqueExists = true;
			}	// if (clique.size() == 4)

			else if(clique.size() == 3){
				clique.push_back(buffer2[0]);
				index = 0;
				count = 0;
				for(int i = 1; i < buffer2.size(); i++){
					if(cliqueExists == false){
						index = buffer2Inx[i];
						for(int j = 0; j < g[index].size(); j++){
							if(buffer2[0] != g[index][j]){			// sprawdzam, czy wierzcholki sa z tej samej sekwencji: sprawdzam, czy pierwszy wierzcholek z buforu wystepuje w listach
								count++;							// jakiegokolwiek innego wierzcholka z buforu; jezeli wierzcholki sie roznia, to licznik sie zwieksza
							}
						}
						if(count == g[index].size()){				// w przypadku, gdy dane wierzcholki beda z tej samej sekwencji, to count bedzie sie rownal calemu rozmiarowi listy sprawdzanego wczesniej wierzcholka
							;
						}
						else{
							clique.push_back(g[index][0]);
							cliqueExists = true;
							}
						count = 0;
					}
				}
			} // else if(clique.size() == 3)

			else if(clique.size() == 2){
				vector<int> bufferSeq1;
				vector<int> bufferSeq1Inx;
				index = 0;
				count = 0;
				clique.push_back(buffer2[0]);
				for(int i = 1; i < buffer2.size(); i++){
					index = buffer2Inx[i];
					for(int j = 0; j < g[index].size(); j++){
						if(buffer2[0] != g[index][j]){			// sprawdzam, czy wierzcholki sa z tej samej sekwencji: sprawdzam, czy pierwszy wierzcholek z buforu wystepuje w listach
							count++;							// jakiegokolwiek innego wierzcholka z buforu; jezeli wierzcholki sie roznia, to licznik sie zwieksza
						}
					}
					if(count == g[index].size()){				// w przypadku, gdy dane wierzcholki beda z tej samej sekwencji, to count bedzie sie rownal calemu rozmiarowi listy sprawdzanego wczesniej wierzcholka
						;
					}
					else{
						bufferSeq1.push_back(g[index][0]);
						bufferSeq1Inx.push_back(index);
					}
					count = 0;
				}

				clique.push_back(bufferSeq1[0]);
				for(int i = 1; i < bufferSeq1.size(); i++){
					if(cliqueExists == false){
						index = bufferSeq1Inx[i];
						for(int j = 0; j < g[index].size(); j++){
							if(bufferSeq1[0] != g[index][j]){			// sprawdzam, czy wierzcholki sa z tej samej sekwencji: sprawdzam, czy pierwszy wierzcholek z buforu wystepuje w listach
								count++;							// jakiegokolwiek innego wierzcholka z buforu; jezeli wierzcholki sie roznia, to licznik sie zwieksza
							}
						}
						if(count == g[index].size()){				// w przypadku, gdy dane wierzcholki beda z tej samej sekwencji, to count bedzie sie rownal calemu rozmiarowi listy sprawdzanego wczesniej wierzcholka
							;
						}
						else{
							clique.push_back(g[index][0]);
							cliqueExists = true;
							}
						count = 0;
					}
				}
			}// else if(clique.size() == 2)

			else if(clique.size() == 5){
				cliqueExists = true;
			}
			else if(clique.size() < 2){
                verticeClique = false;
			}
		} // zakonczenie if(buffer.size() >= 5)
		else{
            verticeClique = false;
		}
	}
}