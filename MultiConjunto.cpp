#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_ELEMS = 2000;


class Multiconjunto {
public:

    Multiconjunto();
    void anyadir(int elem);
    void eliminar(int elem);
    bool pertenece(int elem) const;

private:
    struct Elem {
        int valor;
        int multiplicidad;
    };
    Elem elems[MAX_ELEMS];
    int num_elems;

    int busquedaBinaria(int inicio, int fin, int elem) const;
};

//Coste constante ya que es una constructora por defecto O(1)
Multiconjunto::Multiconjunto() {
    num_elems = 0;
}

//La busqueda binaria tiene siempre coste O(log n) porque siempre se divide la busqueda en mitades
int Multiconjunto::busquedaBinaria(int inicio, int fin, int elem) const {
    int ret;
    if (inicio >= fin) {
        ret = inicio;
    }
    else {
        int mitad = (inicio + fin) / 2;
        if (elems[mitad].valor > elem) {
            ret = busquedaBinaria(0, mitad, elem);
        }
        else if (elems[mitad].valor == elem) {
            ret = busquedaBinaria(mitad, mitad, elem);
        }
        else {
            ret = busquedaBinaria(mitad + 1, fin, elem);
        }
    }

    return ret;
}

//Pertenece tiene coste O(log n) porque dentro del metodo se llama busquedaBinaria() que tiene complejidad O(log n)
bool Multiconjunto::pertenece(int elem) const {
    int pos = busquedaBinaria(0, num_elems, elem);
    return pos < num_elems&& elems[pos].valor == elem;
}

//Anyadir tiene coste O(n) porque dentro de la funcion se llama a busquedaBinaria() 
//y aparte de se le pasa un elemento para añadirlo o aumentar su multiplicidad
void Multiconjunto::anyadir(int elem) {
    int pos = busquedaBinaria(0, num_elems, elem);
    if (pos == num_elems || elems[pos].valor != elem) {
        for (int i = num_elems; i > pos; i--)
            elems[i] = elems[i - 1];

        num_elems++;
        elems[pos].valor = elem;
        elems[pos].multiplicidad = 1;
    }
    else {
        elems[pos].multiplicidad++;
    }
}

//Eliminar tiene coste O(n) porque dentro de la funcion se llama a busquedaBinaria() 
//y aparte de se le pasa un elemento para eliminarlo o disminuir su multiplicida
void Multiconjunto::eliminar(int elem) {
    int pos = busquedaBinaria(0, num_elems, elem);
    if (pos < num_elems && elems[pos].valor == elem) {
        if (elems[pos].multiplicidad > 1)
            elems[pos].multiplicidad--;
        else {
            for (int i = pos; i < num_elems - 1; i++)
                elems[i] = elems[i + 1];
            num_elems--;
        }
    }

}

void resuelve(int n, Multiconjunto mul, vector<int> clave, vector<int> introducir) {

    vector <char>sol;

    //Establezco todo la solucion con puntos que representa el color gris
    for (int i = 0; i < n; i++) {
        sol.push_back('.');
    }

    //Si hay casos verdes se sustituyen por # en el array sol
    for (int j = 0; j < n; j++) {
        if (mul.pertenece(introducir[j]) && clave[j] == introducir[j]) {
            sol[j] = '#';
            mul.eliminar(introducir[j]);
        }
    }

    //Si hay caso amarillo se sutituye por O en el array sol
    for (int h = 0; h < n; h++) {
        if (mul.pertenece(introducir[h]) && clave[h] != introducir[h]) {
            sol[h] = 'O';
            mul.eliminar(introducir[h]);
        }
    }

    for (int y = 0; y < n; y++) {
        cout << sol[y];
    }

    cout << endl;
}

// Función que trata un caso de prueba.
// Devuelve false si se ha leído el 0 que marca el fin de la entrada,
// o true en caso contrario.
bool tratar_caso() {
    // Implementar
    int n, dato;//n es el tamaño de la clave
    vector<int> clave, introducir;//el array clave reresenta la clave secreta y el array introducir es la clave del jugador
    Multiconjunto mul;

    cin >> n;
    if (n == 0)
        return false;

    for (int i = 0; i < n; i++) {
        cin >> dato;
        mul.anyadir(dato);
        clave.push_back(dato);
    }
    for (int i = 0; i < n; i++) {
        cin >> dato;
        introducir.push_back(dato);
    }

    resuelve(n, mul, clave, introducir);

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
    // Descomentar en Windows si la consola se cierra inmediatamente
    // system("PAUSE");
#endif
    return 0;
}