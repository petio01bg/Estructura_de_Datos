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

// añade las librerías de la STL que necesites

using namespace std;


// Elige el tipo representante adecuado para el TAD ElGrande e
// implementa sus operaciones. Puedes añadir métodos o funciones privadas
// si lo consideras conveniente. Justifica la elección de los tipos y el
// coste de las operaciones.
//@ <answer>

using jugador = string;
using region = string;

class ElGrande {

    // escribe aquí los atributos privados
private:
    set<region> regiones_disputadas;
    unordered_map<jugador, pair<int,unordered_map<region,int>>> jugadores;
    struct InfoRegion {
        bool disputada;
        map<int, unordered_set<jugador>> caballeros;
        string jug_max;
        unordered_set<jugador> jugadores_region;

        InfoRegion(bool disputada, string jug_max):disputada(disputada),jug_max(jug_max) {}
    };

    unordered_map < region, InfoRegion> regiones;

public:

    //O(1)
    ElGrande(){}

    //O(1)
    void anyadir_jugador(const jugador& j) {
        if (jugadores.count(j)) {
            throw domain_error("Jugador existente");
        }

        jugadores[j].first=0;
    }

    //O(log n)
    void colocar_caballero(const jugador& j, const region& r) {
        if (!jugadores.count(j)) {
            throw domain_error("Jugador no existente");
        }

        if (!regiones.count(r)) {
            regiones.insert({ r,InfoRegion(false,"") });
        }

        if (jugadores[j].second.count(r)) {
            int c = jugadores[j].second[r];
            regiones.at(r).caballeros.at(c).erase(j);
            jugadores[j].second[r]++;
            auto it = regiones.at(r).caballeros.end();
            it--;
            if (c + 1 == it->first && !regiones.at(r).disputada) {
                regiones.at(r).disputada = true;
                regiones_disputadas.insert(r);
                jugadores[regiones.at(r).jug_max].first--;
                regiones.at(r).jug_max = "";
            }
            else if (c + 1 > it->first) {
                if (regiones.at(r).disputada) {
                    regiones.at(r).disputada = false;
                    regiones_disputadas.erase(r);
                    jugadores[j].first++;
                    regiones.at(r).jug_max = j;
                }
            }
            regiones.at(r).caballeros[c + 1].insert(j);
        }
        else {
            regiones.at(r).jugadores_region.insert(j);
            jugadores[j].second[r]++;
            if (!regiones.at(r).caballeros.empty()) {
                auto it = regiones.at(r).caballeros.end();
                it--;
                if (jugadores[j].second[r] == it->first && !regiones.at(r).disputada) {
                    regiones.at(r).disputada = true;
                    regiones_disputadas.insert(r);
                    jugadores[regiones.at(r).jug_max].first--;
                    regiones.at(r).jug_max = "";
                }
                else if (jugadores[j].second[r] > it->first) {
                    if (regiones.at(r).disputada) {
                        regiones.at(r).disputada = false;
                        regiones_disputadas.erase(r);
                        jugadores.at(r).first++;
                        regiones.at(r).jug_max = j;
                    }
                }
            }
            else {
                regiones.at(r).caballeros[1].insert(j);
                regiones.at(r).jug_max = j;
                jugadores[j].first++;
            }
            
        }
    }

    //O(1)
    int puntuacion(const jugador& j) const {
        if (!jugadores.count(j)) {
            throw domain_error("Jugador no existente");
        }

        return jugadores.at(j).first;
    }

    //O(n) segun el numero de regiones en dispuita
    vector<region> regiones_en_disputa() const {
        vector<region> result = { regiones_disputadas.begin(),regiones_disputadas.end() };
        return result;
    }

    //O(n) segun el numero de jugadores en la region dada
    void expulsar_caballeros(const region& r) {
        if (!regiones.count(r) || regiones.at(r).caballeros.empty()) {
            throw domain_error("Region vacia");
        }

        if (regiones_disputadas.count(r)) {
            regiones_disputadas.erase(r);
        }

        for (auto it = regiones.at(r).jugadores_region.begin(); it != regiones.at(r).jugadores_region.end(); ++it) {
            if (regiones.at(r).jug_max == *it) {
                jugadores[*it].first--;
            }
            jugadores[*it].second.erase(r);
        }

        regiones.erase(r);
    }

};

//@ </answer>
// ¡No modificar nada debajo de esta línea!
// ----------------------------------------------------------------

bool resuelveCaso() {
    string orden, jug, reg;
    cin >> orden;
    if (!cin)
        return false;

    ElGrande juego;

    while (orden != "FIN") {
        try {
            if (orden == "anyadir_jugador") {
                cin >> jug;
                juego.anyadir_jugador(jug);
            }
            else if (orden == "colocar_caballero") {
                cin >> jug >> reg;
                juego.colocar_caballero(jug, reg);
            }
            else if (orden == "puntuacion") {
                cin >> jug;
                auto puntos = juego.puntuacion(jug);
                cout << "Puntuacion de " << jug << ": " << puntos << '\n';
            }
            else if (orden == "regiones_en_disputa") {
                auto regs = juego.regiones_en_disputa();
                cout << "Regiones en disputa:\n";
                for (auto const& r : regs) {
                    cout << r << '\n';
                }
            }
            else if (orden == "expulsar_caballeros") {
                cin >> reg;
                juego.expulsar_caballeros(reg);
            }
        }
        catch (domain_error& e) {
            cout << "ERROR: " << e.what() << '\n';
        }
        cin >> orden;
    }
    cout << "---\n";
    return true;
}

int main() {
    while (resuelveCaso());
    return 0;
}

/*int main() {
#ifndef DOMJUDGE
    std::ifstream in("casos3.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

    while (resuelveCaso());

#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    // Descomentar si se necesita
    // system("PAUSE");
#endif
    return 0;
}*/
