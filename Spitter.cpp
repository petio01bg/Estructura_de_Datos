#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <utility>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include <list>


using namespace std;

//P: numero de publicaciones en la aplicacion
//H: numero de hashtags por publicacion

class Spitter {
public:

	Spitter() {}

	//O(H)
	void nueva_publicacion(int id, const string& texto, const vector<string>& hashtags) {

		if (publicaciones.count(id)) {
			throw domain_error("Identificador duplicado");
		}

		lista.push_front(texto);
		publicaciones.insert({ id,info_pub(texto,hashtags,lista.begin())});

		//O(H)
		for (auto i = hashtags.begin(); i != hashtags.end(); ++i) {

			if (hashtag_ord.count(*i)) {
				hashtag_ord[*i]++;
			}
			else {
				hashtag_ord[*i] = 1;
			}
		}
	}

	//O(H)
	void borrar_publicacion(int id) {
		auto pub = publicaciones.find(id);
		if (pub != publicaciones.end()) {
			

			//O(H)
			for (auto it = pub->second.hashtags.begin(); it != pub->second.hashtags.end(); ++it) {
				auto hash = hashtag_ord.find(*it);
				hash->second--;
				if (hash->second == 0) {
					hashtag_ord.erase(hash);
				}
			}

			lista.erase(pub->second.it);
			publicaciones.erase(pub);
		}
	}

	//O(n) si n < list.size() y O(list.size()) si n > list.size()
	vector<string> mas_recientes(int n) const {
		vector<string> pub;
		
		if (n > lista.size()) {
			auto el = lista.begin();
			for (int i = 0; i < lista.size(); i++) {

				pub.push_back(*el);
				el++;
			}
		}
		else {
			auto el = lista.begin();
			for (int i = 0; i < n; i++) {
				
				pub.push_back(*el);
				el++;
			}
		}


		return pub;
	}

	//O(1)
	vector<string> hashtags(int id) const {
		auto it = publicaciones.find(id);
		if (it == publicaciones.end()) {
			throw domain_error("Publicacion no existente");
		}

		set<string>ordenado = { it->second.hashtags.begin(),it->second.hashtags.end() };
		vector<string> result = { ordenado.begin(),ordenado.end() };
		return result;
	}

	//O(1)
	void anyadir_hashtag(int id, const string& hashtag) {
		auto it = publicaciones.find(id);
		if (it == publicaciones.end()) {
			throw domain_error("Publicacion no existente");
		}

		unordered_set<string> aux = { it->second.hashtags.begin(),it->second.hashtags.end() };
		if (!aux.count(hashtag)) {
			it->second.hashtags.push_back(hashtag);

			auto it_h = hashtag_ord.find(hashtag);
			if (it_h != hashtag_ord.end()) {
				it_h->second++;
			}
			else {
				hashtag_ord.insert({ hashtag,1 });
			}
		}
	}

	//O(1)
	int popularidad(const string& hashtag) const {
		auto it_h = hashtag_ord.find(hashtag);
		if (it_h != hashtag_ord.end()) {
			return it_h->second;
		}
		else {
			return 0;
		}
	}


private:
	// Añade aquí los atributos que conforman la representación
	// interna del TAD
	//
	// También puedes añadir métodos auxiliares privados
	struct info_pub {
		string texto;
		vector<string> hashtags;
		list<string>::iterator it;

		info_pub(string texto, vector<string> aux, list<string>::iterator it) :
			texto(texto), hashtags(aux), it(it) { }
	};

	unordered_map<int, info_pub> publicaciones; //id y texto
	unordered_map<string, int> hashtag_ord; //hashtag y numero de veces que aparece
	list<string> lista;//textos mas recientes

	

	
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

	Spitter spitter;

	while (fin != "FIN") {
		if (fin == "nueva_publicacion") {
			int n;
			string texto, hashtag, space_delimiter = " ";
			vector<string> aux;
			cin >> n;
			getline(cin, texto);
			getline(cin, hashtag);

			istringstream isstream(hashtag);

			while (!isstream.eof()) {
				string tempStr;
				isstream >> tempStr;
				if (tempStr != "") {
					aux.push_back(tempStr);
				}
			}

			try {
				spitter.nueva_publicacion(n, texto, aux);
			}
			catch (domain_error& e) {
				cout << "ERROR: " << e.what() << endl;
			}
		}

		if (fin == "mas_recientes") {
			int n;
			cin >> n;
			vector<string> aux = spitter.mas_recientes(n);

			cout << "Las " << aux.size() << " publicaciones mas recientes:" << endl;

			for (int i = 0; i < aux.size(); i++) {
				cout << "   " << aux[i] << endl;
			}
		}

		if (fin == "hashtags") {
			int id;
			cin >> id;

			try {
				vector<string> ordenado = spitter.hashtags(id);

				for (auto it = ordenado.begin(); it != ordenado.end(); it++) {
					cout << *it << " ";
				}

				cout << endl;
			}
			catch (domain_error& e) {
				cout << "ERROR: " << e.what() << endl;
			}
		}

		if (fin == "anyadir_hashtag") {
			int id;
			cin >> id;
			string hash;
			cin >> hash;

			try {
				spitter.anyadir_hashtag(id, hash);
			}
			catch (domain_error& e) {
				cout << "ERROR: " << e.what() << endl;
			}
		}

		if (fin == "popularidad") {
			string hash;
			cin >> hash;

			cout << spitter.popularidad(hash) << endl;
		}

		if (fin == "borrar_publicacion") {
			int id;
			cin >> id;
			spitter.borrar_publicacion(id);
		}


		cin >> fin;
	}

	cout << "---" << endl;

	return true;

}

int main() {
	while (tratar_caso()) {}
	return 0;
}

/*int main() {
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
}*/