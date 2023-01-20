#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <utility>
#include <stdexcept>
#include <unordered_map>
#include <map>

using namespace std;

// Tipo de datos para representar direcciones
// Para hacer referencia a los elementos de este tipo enumerado se utiliza:
//   Direccion::Norte, Direccion::Sur, etc.
enum class Direccion { Norte, Sur, Este, Oeste };

// Operador para leer una dirección desde un flujo de entrada.
// Ejemplo de uso:
//
// Direccion dir;
// cin >> dir;
//
istream& operator>>(istream& is, Direccion& dir) {
	string palabra; is >> palabra;
	if (palabra == "Norte") dir = Direccion::Norte;
	else if (palabra == "Sur") dir = Direccion::Sur;
	else if (palabra == "Este") dir = Direccion::Este;
	else if (palabra == "Oeste") dir = Direccion::Oeste;
	else throw std::domain_error("Lectura de direccion incorrecta");
	return is;
}

using Torre = string;

class Desierto {
public:
	//O(1)
	Desierto() { }

	//O(log n)
	void anyadir_torre(const string& nombre, int x, int y) {
		if (torres.count(nombre) > 0) {//O(1)
			throw domain_error("Torre ya existente");
		}

		if (coordenadasx[x].count(y) > 0) {//O(log n)
			throw domain_error("Posicion ocupada");
		}

		torres.insert({ nombre,Coordenadas(x,y) });//O(1)
		coordenadasx[x].insert({ y,nombre });//O(log n)
		coordenadasy[y].insert({ x,nombre });//O(log n)

	}
	//O(log n)
	void eliminar_torre(const string& nombre) {
		auto it = torres.find(nombre);//O(1)
		if (it != torres.end()) {
			coordenadasx[it->second.x].erase(it->second.y);//O(log n)
			if (coordenadasx[it->second.x].empty()) {//O(log n)
				coordenadasx.erase(it->second.x);//O(log n)
			}

			coordenadasy[it->second.y].erase(it->second.x);//O(log n)
			if (coordenadasy[it->second.y].empty()) {//O(log n)
				coordenadasy.erase(it->second.y);//O(log n)
			}

			torres.erase(it);//O(1)
		}
		else {
			throw domain_error("Torre no existente");
		}
	}

	//O(log n)
	pair<bool, string> torre_en_posicion(int x, int y) const {
		if (coordenadasx.count(x) > 0) {//O(log n)
			if (coordenadasx.at(x).count(y) > 0) {//O(log n)
				string nombre = coordenadasx.at(x).at(y);//O(log n)
				return{ true,nombre };
			}
		}

		return{ false,"" };
	}

	//O(1)
	pair<int, int> posicion(const string& nombre) const {
		auto it = torres.find(nombre);//O(1)
		if (it != torres.end()) {
			return { it->second.x,it->second.y };
		}
		else {
			throw domain_error("Torre no existente");
		}
	}

	//O(log n)
	string torre_mas_cercana(const string& nombre, const Direccion& dir) const {
		auto it_torre = torres.find(nombre);//O(1)
		if (it_torre != torres.end()) {
			if (Direccion::Norte == dir || Direccion::Sur == dir) {
				auto ity = coordenadasx.find(it_torre->second.x);//O(log n)
				auto torre = ity->second.find(it_torre->second.y);//O(log n)
				if (Direccion::Norte == dir) {
					torre++;

					if (torre == ity->second.end()) {
						throw domain_error("No hay torres en esa direccion");
					}

					return torre->second;
				}

				if (Direccion::Sur == dir) {

					if (torre == ity->second.begin()) {
						throw domain_error("No hay torres en esa direccion");
					}

					torre--;

					return torre->second;
				}
			}

			if (Direccion::Este == dir || Direccion::Oeste == dir) {
				auto itx = coordenadasy.find(it_torre->second.y);//O(log n)
				auto torre = itx->second.find(it_torre->second.x);//O(log n)
				if (Direccion::Este == dir) {
					torre++;

					if (torre == itx->second.end()) {
						throw domain_error("No hay torres en esa direccion");
					}

					return torre->second;

				}

				if (Direccion::Oeste == dir) {

					if (torre == itx->second.begin()) {
						throw domain_error("No hay torres en esa direccion");
					}

					torre--;

					return torre->second;

				}
			}
		}
		else {
			throw domain_error("Torre no existente");
		}
	}


private:
	// Añade aquí los atributos que conforman la representación
	// interna del TAD
	//
	// También puedes añadir métodos auxiliares privados
	struct Coordenadas {
		int x;
		int y;

		Coordenadas(const int& x, const int& y) :x(x), y(y) {}
	};

	unordered_map<Torre, Coordenadas> torres;
	map<int, map<int, Torre>> coordenadasx;
	map<int, map<int, Torre>> coordenadasy;

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

	Desierto desierto;

	while (fin != "FIN") {
		if (fin == "anyadir_torre") {
			string torre;
			cin >> torre;
			int x, y;
			cin >> x >> y;
			try {
				desierto.anyadir_torre(torre, x, y);
			}
			catch (domain_error& e) {
				cout << e.what() << endl;
			}

		}

		if (fin == "torre_en_posicion") {
			int x, y;
			cin >> x >> y;
			if (desierto.torre_en_posicion(x, y).first) {
				cout << "SI" << " " << desierto.torre_en_posicion(x, y).second << endl;
			}
			else {
				cout << "NO" << endl;
			}
		}

		if (fin == "posicion") {
			string nombre;
			cin >> nombre;
			try {
				cout << desierto.posicion(nombre).first << " " << desierto.posicion(nombre).second << endl;
			}
			catch (domain_error& e) {
				cout << e.what() << endl;
			}
		}

		if (fin == "eliminar_torre") {
			string nombre;
			cin >> nombre;
			try {
				desierto.eliminar_torre(nombre);
			}
			catch (domain_error& e) {
				cout << e.what() << endl;
			}
		}

		if (fin == "torre_mas_cercana") {
			string nombre;
			cin >> nombre;
			Direccion dir;
			cin >> dir;
			try {
				cout << desierto.torre_mas_cercana(nombre, dir) << endl;
			}
			catch (domain_error& e) {
				cout << e.what() << endl;
			}
		}

		cin >> fin;
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