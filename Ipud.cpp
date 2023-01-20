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

using namespace std;

class IPud {
public:

	//O(1)
	IPud() { tiempoCanciones = 0; }

	void addSong(const string& S, const string& A, int D) {
		if (canciones.count(S)) {
			throw domain_error("Cancion ya existente en el IPud");
		}
		canciones[S] = {A,D,playlist.end(),reproducidas.end()};
	}

	void addToPlaylist(const string &S) {
		if (!canciones.count(S)) {
			throw domain_error("Cancion no esta en el IPud");
		}

		if (!playlist_busca.count(S)) {
			playlist_busca.insert(S);
			playlist.push_back(S);
			tiempoCanciones += canciones[S].duracion;
			auto it = --playlist.end();
			canciones[S].itplay = it;
		}
		
	}

	string current() const {
		if (playlist.empty()) {
			throw domain_error("PlayList vacia");
		}
		return playlist.front();
	}

	string play() {
		if (!playlist.empty()) {
			if (canciones[playlist.front()].itreproducida != reproducidas.end()) reproducidas.erase(canciones[playlist.front()].itreproducida);
			reproducidas.push_front(playlist.front());
			canciones[playlist.front()].itreproducida = reproducidas.begin();
			string can = playlist.front();
			canciones[playlist.front()].itplay = playlist.end();
			int d = canciones[playlist.front()].duracion;
			tiempoCanciones -= d;
			playlist.pop_front();
			return "Sonando " + can;
		}
		else {
			return "No hay cancuiones en la lista";
		}
	}

	int totalTime() const {
		return tiempoCanciones;
	}

	list<string> recent(int N) {
		if (N > reproducidas.size()) {
			N = reproducidas.size();
		}
		auto it = reproducidas.begin();
		list<string> result;
		for (int i = 0; i < N; i++) {
			result.push_back(*it);
			it++;
		}

		return result;
	}
	
	void deleteSong(const string& S) {
		auto it = canciones.find(S);

		if (it != canciones.end()) {
			auto& infoSong = it->second;

			if (infoSong.itplay != playlist.end()) {
				playlist.erase(infoSong.itplay);
			}

			if (infoSong.itreproducida != reproducidas.end()) reproducidas.erase(infoSong.itreproducida);

			canciones.erase(S);
		}
	}

private:

	using infoSong = struct {
		string artista;
		int duracion;
		list<string>::iterator itplay;
		list<string>::iterator itreproducida;
	};

	unordered_map<string, infoSong> canciones;
	int tiempoCanciones;
	list<string>playlist;
	unordered_set<string>playlist_busca;
	list<string>reproducidas;
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

	IPud iPud;

	string song, artista;
	int duracion;
	int n;

	while (fin != "FIN") {

		try {

			if (fin == "addSong") {
				cin >> song >> artista >> duracion;
				iPud.addSong(song, artista, duracion);
			}
			else if (fin == "addToPlaylist") {
				cin >> song;
				iPud.addToPlaylist(song);
			}
			else if (fin == "play") {
				song = iPud.play();
				cout << song << endl;
			}
			else if (fin == "deleteSong") {
				cin >> song;
				iPud.deleteSong(song);
			}
			else if (fin == "recent") {
				cin >> n;

				list<string> recentSong = iPud.recent(n);
				if (recentSong.empty()) cout << "No hay canciones recientes" << endl;
				else {
					cout << "Las " << recentSong.size() << " mas recientes \n";
					for (auto it : recentSong) cout << "    " << it << endl;
				}

			}
			else if (fin == "current") {
				iPud.current();
			}
			else if (fin == "totalTime") {
				n = iPud.totalTime();
				cout << "Tiempo total " << n << endl;
			}

		}
		catch (exception de) {
			cout << "ERROR " << fin << endl;
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