#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <list>
#include <unordered_map>
#include <map>



using namespace std;

// Función para tratar un caso de prueba
// Devuelve true si se ha procesado el caso de prueba
// o false si no se ha procesado porque se ha encontrado la
// marca de fin de entrada


//Coste de la funcion
//n: numero de jugadores
//c: numero de buques por jugador
//t: numero de jugadas
//sm: longitud de la lista de cada casilla del diccionario

//Coste: O(((n * log n) * c) + (t*(sm * log sm)))
bool tratar_caso() {
	// Implementar.
	int n, c, t;
	cin >> n >> c;

	if (n == 0 && c == 0) {
		return false;
	}

	unordered_map <string, list<string>> jugadores;
	map<string, int> list_jug;
	string jug, cas;

	//O((n * log n)*c)
	for (int i = 0; i < n; i++) {
		cin >> jug;
		list_jug[jug] = c;//log n
		for (int j = 0; j < c; j++) {
			cin >> cas;
			list<string> &aux = jugadores[cas];//O(1)
			aux.push_back(jug);//O(1)
		}
	}

	cin >> t;
	
	string jm, cm;
	list<string> resultados;
	int cont = 0, auxc = 0;
	bool hund = false;

	//O(t)
	for (int i = 0; i < t; i++) {
		cin >> jm >> cm;
		if(jugadores.count(cm) > 0){
			list<string>& sm = jugadores[cm];//O(1)
			cont = sm.size();
			auto it = sm.begin();
			//O(sm)
			while(it != sm.end()) {
				if (*it != jm) {
					list_jug[*it]--;
					if (list_jug[*it] == 0) {
						hund = true;
						list_jug.erase(*it);//Considerando que son arboles reequilibrados el coste es O(log sm)
					}
					it = sm.erase(it);//O(1)
				}
				else {
					++it;
				}
			}
			auxc = sm.size();//O(sm)
		
			if(auxc == cont) {
				resultados.push_back("AGUA");
			}
			else if (hund) {
				resultados.push_back("VENCIDO");
				hund = false;
			}
			else {
				resultados.push_back("HUNDIDO");
			}

			if (jugadores.count(cm) == 0)
				jugadores.erase(cm);//O(1)
		}
		else {
			resultados.push_back("AGUA");
		}
	}

	for (auto elem : resultados) {
		cout << elem << endl;
	}

	cout << "---" << endl;

	return true;
}

int main() {
#ifndef DOMJUDGE
	std::ifstream in("sample.in");
	auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

	while (tratar_caso()) {}

#ifndef DOMJUDGE
	std::cin.rdbuf(cinbuf);
	// Descomentar si se trabaja en Windows
	// system("PAUSE");
#endif
	return 0;
}

