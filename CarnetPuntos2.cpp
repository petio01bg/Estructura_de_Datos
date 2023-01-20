// TAD CarnetPuntos
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <utility>
#include <stdexcept>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <list>

using namespace std;

class CarnetPuntos {
public:

	//O(1)
	CarnetPuntos() {}

	//O(1)
	void nuevo(const string& dni) {
		if (carnet.count(dni)) {
			throw domain_error("Conductor duplicado");
		}

		puntuaciones[15].push_front(dni);
		auto it = puntuaciones[15].begin();
		carnet[dni] = { 15,it };
	}

	//O(1)
	void quitar(const string& dni, int puntos) {
		if (!carnet.count(dni)) {
			throw domain_error("Conductor inexistente");
		}

		int p = carnet[dni].first;
		auto it = carnet[dni].second;

		if (p - puntos <= 0) {
			puntuaciones[p].erase(it);
			puntuaciones[0].push_front(dni);
			auto it2 = puntuaciones[0].begin();
			carnet[dni] = { 0,it2 };
		}
		else {
			puntuaciones[p].erase(it);
			puntuaciones[p-puntos].push_front(dni);
			auto it2 = puntuaciones[p-puntos].begin();
			carnet[dni] = { p-puntos,it2 };
		}
	}

	//O(1)
	void recuperar(const string& dni, int puntos) {
		if (!carnet.count(dni)) {
			throw domain_error("Conductor inexistente");
		}

		int p = carnet[dni].first;
		auto it = carnet[dni].second;

		if (p + puntos >= 15 && p!= 15) {
			puntuaciones[p].erase(it);
			puntuaciones[15].push_front(dni);
			auto it2 = puntuaciones[15].begin();
			carnet[dni] = { 15,it2 };
		}
		else if(p != 15) {
			puntuaciones[p].erase(it);
			puntuaciones[p + puntos].push_front(dni);
			auto it2 = puntuaciones[p + puntos].begin();
			carnet[dni] = { p + puntos,it2 };
		}
	}

	//O(1)
	int consultar(const string& dni) const {
		if (!carnet.count(dni)) {
			throw domain_error("Conductor inexistente");
		}

		return carnet.at(dni).first;
	}

	//O(1)
	int cuantos_con_puntos(int puntos) {
		if (puntos < 0 || puntos > 15) {
			throw domain_error("Puntos no validos");
		}

		return puntuaciones[puntos].size();
	}

	list<string> lista_por_puntos(int puntos) {
		if (puntos < 0 || puntos > 15) {
			throw domain_error("Puntos no validos");
		}
		return puntuaciones[puntos];
	}

private:
	unordered_map<string, pair<int,list<string>::iterator>> carnet;
	unordered_map<int, list<string>> puntuaciones;
};


// Función para tratar un caso de prueba
// Devuelve true si se ha procesado el caso de prueba
// o false si no se ha procesado porque se ha encontrado el
// fin de fichero
bool tratar_caso() {
	// Implementar.
	string fin;
	cin >> fin;

	if (cin.eof()) {
		return false;
	}

	CarnetPuntos carnet;

	while (fin != "FIN") {
		if (fin == "nuevo") {
			string conductor;
			cin >> conductor;

			try {
				carnet.nuevo(conductor);
			}
			catch (domain_error& e) {
				cout << "ERROR: " << e.what() << endl;
			}
		}

		if (fin == "quitar") {
			string conductor;
			int puntos;
			cin >> conductor >> puntos;

			try {
				carnet.quitar(conductor, puntos);
			}
			catch (domain_error& e) {
				cout << "ERROR: " << e.what() << endl;
			}
		}

		if (fin == "recuperar") {
			string conductor;
			int puntos;
			cin >> conductor >> puntos;

			try {
				carnet.recuperar(conductor, puntos);
			}
			catch (domain_error& e) {
				cout << "ERROR: " << e.what() << endl;
			}
		}

		if (fin == "consultar") {

			string conductor;
			cin >> conductor;

			try {
				int n = carnet.consultar(conductor);
				cout << "Puntos de " << conductor << ": " << n << endl;
			}
			catch (domain_error& e) {
				cout << "ERROR: " << e.what() << endl;
			}
		}

		if (fin == "cuantos_con_puntos") {
			int puntos;
			cin >> puntos;

			try {
				int n = carnet.cuantos_con_puntos(puntos);
				cout << "Con " << puntos << " puntos hay " << n << endl;
			}
			catch (domain_error& e) {
				cout << "ERROR: " << e.what() << endl;
			}
		}

		if (fin == "lista_por_puntos") {
			int puntos;
			cin >> puntos;

			try {
				list<string>aux = carnet.lista_por_puntos(puntos);
				cout << "Tienen " << puntos << " puntos:";
				int n = aux.size();
				for (int i = 0; i < n; i++) {
					cout << " " << aux.front();
					aux.pop_front();
				}
				cout << endl;
			}
			catch (domain_error& e) {
				cout << "ERROR: " << e.what() << endl;
			}
		}

		cin >> fin;
	}

	cout << "---" << endl;

	return true;

}

int main() {
	while (tratar_caso()) {};
	return 0;
}