// TAD IPud
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
#include <map>

using namespace std;

class Autoescuela {
public:
	Autoescuela() {}

	void alta(const string& A, const string& P) {
		if (alumnos.count(A)) {
			auto it = alumnos.find(A);
			profesores[P].insert({ it->first,it->second.second });
			string prof = it->second.first;
			profesores[prof].erase(A);
			alumnos[A].first = P;
		}
		else {
			alumnos[A] = { P,0 };
			auto it = alumnos.find(A);
			profesores[P].insert({ it->first,it->second.second });
		}
	}
	

	bool es_alumno(const string& A, const string& P) {
		if (profesores[P].count(A)) {
			return true;
		}
		else {
			return false;
		}
	}

	int puntuacion(const string& A) {
		if (!alumnos.count(A)) {
			throw domain_error("El alumno " + A + " no esta matriculado");
		}

		return alumnos[A].second;
	}

	void actualizar(const string& A, int N) {
		if (!alumnos.count(A)) {
			throw domain_error("El alumno " + A + " no esta matriculado");
		}

		alumnos[A].second += N;
		profesores[alumnos[A].first][A] = alumnos[A].second;
	}

	list<string> examen(const string& P, int N) {
		list<string> result;

		for (auto it = profesores[P].begin(); it != profesores[P].end(); ++it) {
			if (it->second >= N) {
				result.push_back(it->first);
			}
		}

		return result;
	}

	void aprobar(const string& A) {
		if (!alumnos.count(A)) {
			throw domain_error("El alumno " + A + " no esta matriculado");
		}

		profesores[alumnos[A].first].erase(A);
		alumnos.erase(A);
	}

private:
	unordered_map<string, pair<string,int>> alumnos;
	unordered_map<string, map<string,int>> profesores;
	
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

	Autoescuela autoescuela;

	string alumno, profesor;
	int puntuacion;

	while (fin != "FIN") {

		try {

			if (fin == "alta") {
				cin >> alumno >> profesor;
				autoescuela.alta(alumno, profesor);
			}
			else if (fin == "es_alumno") {
				cin >> alumno >> profesor;
				if (autoescuela.es_alumno(alumno, profesor)) {
					cout << alumno + " es alumno de " + profesor << endl;
				}
				else {
					cout << alumno + " no es alumno de " + profesor << endl;
				}
			}
			else if (fin == "puntuacion") {
				cin >> alumno;
				cout << "Puntuacion de " + alumno + ": " << autoescuela.puntuacion(alumno) << endl;
			}
			else if (fin == "actualizar") {
				cin >> alumno>>puntuacion;
				autoescuela.actualizar(alumno, puntuacion);
			}
			else if (fin == "examen") {
				cin >> profesor>>puntuacion;

				list<string> alumnosexam = autoescuela.examen(profesor, puntuacion);
				
				cout << "Alumnos de " + profesor + " a examen:" << endl;
				for (auto it : alumnosexam) cout << it << endl;
				

			}
			else if (fin == "aprobar") {
				cin >> alumno;
				autoescuela.aprobar(alumno);
			}
		}
		catch (domain_error &de) {
			cout << "ERROR " << de.what() << endl;
		}

		cin >> fin;
	}

	cout << "---\n";
	return true;

}

int main() {
	while (tratar_caso()) {};
	return 0;
}