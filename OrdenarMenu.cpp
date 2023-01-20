#include <iostream>
#include <cassert>
#include <fstream>
#include <cassert>
#include <list>
#include <string>

using namespace std;


enum class Categoria { Primero, Segundo, Postre };

struct Plato {
    Categoria categoria;
    string nombre;
};

// Sobrecarga del operador << para imprimir platos
ostream& operator<<(ostream& out, const Plato& plato) {
    switch (plato.categoria) {
    case Categoria::Primero:
        out << "1";
        break;
    case Categoria::Segundo:
        out << "2";
        break;
    case Categoria::Postre:
        out << "P";
        break;
    }
    out << " " << plato.nombre;
    return out;
}

//@ <answer>
//---------------------------------------------------------------
// Modificar a partir de aquí
// --------------------------------------------------------------


// Indica el coste en función del tamaño del parámetro de entrada
//Coste O(n) respecto al numero de platos que tenemos que reordenar
void ordenar_menu(list<Plato>& platos) {

    auto ordenado = platos.begin();
    auto it = platos.begin();

    while (it != platos.end()) {
        if (it->categoria == Categoria::Primero) {
            platos.insert(ordenado, *it);
            if (ordenado == it) {
                ordenado++;
            }
            it = platos.erase(it);
        }
        else {
            ++it;
        }
    }

    it = platos.begin();

    while (it != platos.end()) {
        if (it->categoria == Categoria::Segundo) {
            platos.insert(ordenado, *it);

            if (ordenado == it) {
                ordenado++;
            }

            it = platos.erase(it);
        }
        else {
            ++it;
        }
    }
}

bool tratar_caso() {
    // Introduce aquí el código para tratar un caso de prueba.
    // Devuelve false si ya no hay más casos de prueba que leer,
    // true en caso contrario.

    int n;
    cin >> n;

    string cate;
    char nombre[100];

    if (n == 0) {
        return false;
    }

    list<Plato> lista;
    Plato plato;

    for (int i = 0; i < n; i++) {
        cin >> cate;
        cin.getline(nombre, 100);
        if (cate == "P") {
            plato.categoria = Categoria::Postre;
        }
        else if (cate == "1") {
            plato.categoria = Categoria::Primero;
        }
        else if (cate == "2") {
            plato.categoria = Categoria::Segundo;
        }

        plato.nombre = nombre;

        lista.push_back(plato);
    }


    ordenar_menu(lista);

    for (auto it = lista.begin(); it != lista.end(); ++it) {
        cout << *it << endl;
    }

    cout << "---" << endl;

    return true;
}


//---------------------------------------------------------------
// No modificar por debajo de esta línea
// --------------------------------------------------------------
//@ </answer>

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

