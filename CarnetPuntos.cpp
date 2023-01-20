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

		carnet[dni] = 15;
		puntuaciones[15].insert({ dni });
	}

	//O(1)
	void quitar(const string& dni, int puntos) {
		if (!carnet.count(dni)) {
			throw domain_error("Conductor inexistente");
		}

		int p = carnet[dni];

		if (p - puntos <= 0) {
			puntuaciones[p].erase(dni);
			puntuaciones[0].insert({ dni });
			carnet[dni] = 0;
		}
		else {
			puntuaciones[p].erase(dni);
			puntuaciones[p-puntos].insert({ dni });
			carnet[dni] = p-puntos;
		}
	}

	//O(1)
	int consultar(const string& dni) const {
		if (!carnet.count(dni)) {
			throw domain_error("Conductor inexistente");
		}

		return carnet.at(dni);
	}

	//O(1)
	int cuantos_con_puntos(int puntos){
		if (puntos < 0 || puntos > 15) {
			throw domain_error("Puntos no validos");
		}

		return puntuaciones[puntos].size();
	}

private:
	unordered_map<string, int> carnet;
	unordered_map<int, unordered_set<string>> puntuaciones;
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

		if (fin == "consultar") {

			string conductor;
			cin >> conductor;

			try {
				int n = carnet.consultar(conductor);
				cout << "Puntos de "<< conductor << ": " << n << endl;
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

		cin >> fin;
	}

	cout << "---" << endl;

	return true;

}

int main() {
	while (tratar_caso()) {};
	return 0;
}