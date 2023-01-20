#include <iostream>
#include <cassert>
#include <fstream>
#include <cassert>

using namespace std;

#include <cassert>
#include <iostream>

template <typename T> class QueueLinkedList {
public:
    QueueLinkedList() : front_node(nullptr), back_node(nullptr) {}
    QueueLinkedList(const QueueLinkedList& other) {
        copy_nodes_from(other.front_node);
    }

    ~QueueLinkedList() { free_nodes_from(front_node); }

    QueueLinkedList& operator=(const QueueLinkedList& other) {
        if (this != &other) {
            free_nodes_from(front_node);
            copy_nodes_from(other.front_node);
        }
        return *this;
    }

    void push(const T& elem) {
        Node* new_node = new Node{ elem, nullptr };
        if (back_node == nullptr) {
            back_node = new_node;
            front_node = new_node;
        }
        else {
            back_node->next = new_node;
            back_node = new_node;
        }
    }

    void pop() {
        assert(front_node != nullptr);
        Node* target = front_node;
        front_node = front_node->next;
        if (back_node == target) {
            back_node = nullptr;
        }
        delete target;
    }

    T& front() {
        assert(front_node != nullptr);
        return front_node->value;
    }

    const T& front() const {
        assert(front_node != nullptr);
        return front_node->elem;
    }

    bool empty() const { return (front_node == nullptr); }

private:
    struct Node {
        T value;
        Node* next;
    };

    void copy_nodes_from(Node* other);
    void free_nodes_from(Node* other);

    Node* front_node;
    Node* back_node;
};

template <typename T> void QueueLinkedList<T>::copy_nodes_from(Node* other) {
    if (other == nullptr) {
        front_node = nullptr;
        back_node = nullptr;
    }
    else {
        front_node = new Node{ other->value, nullptr };
        back_node = front_node;
        Node* current = other->next;
        while (current != nullptr) {
            Node* new_node = new Node{ current->value, nullptr };
            back_node->next = new_node;
            current = current->next;
            back_node = new_node;
        }
    }
}

template <typename T> void QueueLinkedList<T>::free_nodes_from(Node* other) {
    Node* current = other;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}




//@ <answer>
//---------------------------------------------------------------
// Modificar a partir de aquí
// --------------------------------------------------------------

//Coste O(n) resoecto al tiempo dedicado al experimento
int resolve(int N, int M,int T, QueueLinkedList<int> cola) {
    int ret = 0;
    int i = 0;

    QueueLinkedList<int> colaYemas;
    for (int j = N; j < M; j++)
        colaYemas.push(0);
    
    while (i < T) {
        cola.push(cola.front() + colaYemas.front());
        colaYemas.push(cola.front());
        colaYemas.pop();
        cola.pop();
        i++;
    }

    for (int i = 0; i < N; i++) {
        ret += cola.front();
        cola.pop();
    }
    
    for (int i = N; i < M; i++) {
        ret += colaYemas.front();
        colaYemas.pop();
    }

    return ret;
}

// Indica el coste en función de los parámetros del problema (N, M, T)
bool tratar_caso() {
	// Introduce aquí el código para tratar un caso de prueba.
	// Devuelve false si ya no hay más casos de prueba que leer,
	// true en caso contrario.
    int N, M, T;
    cin >> N >> M >> T;

    QueueLinkedList<int> cola;

    if (!std::cin) {
        return false;
    }

    for (int i = 0; i < N-1; i++) {
        cola.push(0);
    }

    cola.push(1);

    int s = resolve(N, M, T, cola);

    cout << s << endl;

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

