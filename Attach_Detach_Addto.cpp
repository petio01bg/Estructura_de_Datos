#include <iostream>
#include <cassert>
#include <fstream>

using namespace std;

class ListLinkedDouble {
private:
    struct Node {
        int value;
        Node* next;
        Node* prev;
    };

public:
    ListLinkedDouble() : num_elems(0) {
        head = new Node;
        head->next = head;
        head->prev = head;
    }

    ListLinkedDouble(const ListLinkedDouble& other) : ListLinkedDouble() {
        copy_nodes_from(other);
        num_elems = other.num_elems;
    }

    ~ListLinkedDouble() { delete_nodes(); }

    void push_front(const int& elem) {
        Node* new_node = new Node{ elem, head->next, head };
        head->next->prev = new_node;
        head->next = new_node;
        num_elems++;
    }

    void push_back(const int& elem) {
        Node* new_node = new Node{ elem, head, head->prev };
        head->prev->next = new_node;
        head->prev = new_node;
        num_elems++;
    }

    void pop_front() {
        assert(num_elems > 0);
        Node* target = head->next;
        head->next = target->next;
        target->next->prev = head;
        delete target;
        num_elems--;
    }

    void pop_back() {
        assert(num_elems > 0);
        Node* target = head->prev;
        target->prev->next = head;
        head->prev = target->prev;
        delete target;
        num_elems--;
    }

    int size() const { return num_elems; }

    bool empty() const { return num_elems == 0; };

    const int& front() const {
        assert(num_elems > 0);
        return head->next->value;
    }

    int& front() {
        assert(num_elems > 0);
        return head->next->value;
    }

    const int& back() const {
        assert(num_elems > 0);
        return head->prev->value;
    }

    int& back() {
        assert(num_elems > 0);
        return head->prev->value;
    }

    const int& operator[](int index) const {
        assert(0 <= index && index < num_elems);
        Node* result_node = nth_node(index);
        return result_node->value;
    }

    int& operator[](int index) {
        assert(0 <= index && index < num_elems);
        Node* result_node = nth_node(index);
        return result_node->value;
    }

    ListLinkedDouble& operator=(const ListLinkedDouble& other) {
        if (this != &other) {
            delete_nodes();
            head = new Node;
            head->next = head->prev = head;
            copy_nodes_from(other);
            num_elems = other.num_elems;
        }
        return *this;
    }

    void display(std::ostream& out) const;

    void display() const { display(std::cout); }


    // Nuevo m�todo
    // Se implementa m�s abajo
    void add_to(int index, int val);


private:
    Node* head;
    int num_elems;

    Node* nth_node(int n) const;
    void delete_nodes();
    void copy_nodes_from(const ListLinkedDouble& other);

    // Nuevos m�todos
    // Se implementan m�s abajo
    static void attach(Node* node, Node* before);
    static void detach(Node* node);
};

ListLinkedDouble::Node* ListLinkedDouble::nth_node(int n) const {
    int current_index = 0;
    Node* current = head->next;

    while (current_index < n && current != head) {
        current_index++;
        current = current->next;
    }

    return current;
}

void ListLinkedDouble::delete_nodes() {
    Node* current = head->next;
    while (current != head) {
        Node* target = current;
        current = current->next;
        delete target;
    }

    delete head;
}

void ListLinkedDouble::copy_nodes_from(const ListLinkedDouble& other) {
    Node* current_other = other.head->next;
    Node* last = head;

    while (current_other != other.head) {
        Node* new_node = new Node{ current_other->value, head, last };
        last->next = new_node;
        last = new_node;
        current_other = current_other->next;
    }
    head->prev = last;
}

void ListLinkedDouble::display(std::ostream& out) const {
    out << "[";
    if (head->next != head) {
        out << head->next->value;
        Node* current = head->next->next;
        while (current != head) {
            out << ", " << current->value;
            current = current->next;
        }
    }
    out << "]";
}

std::ostream& operator<<(std::ostream& out, const ListLinkedDouble& l) {
    l.display(out);
    return out;
}

//@ <answer>
//---------------------------------------------------------------
// Modificar a partir de aqu�
// --------------------------------------------------------------

// No olvides el coste!
//Coste O(1) ya que se anyade un nodo, reconectandolo a ambos.
void ListLinkedDouble::attach(Node* node, Node* before) {
    // Implementar
    node->prev = before->prev;
    before->prev->next = node;
    before->prev = node;
    node->next = before;
}
    
    

// No olvides el coste!
//Coste O(1) solo se reconectan nodos.
void ListLinkedDouble::detach(Node* node) {
    // Implementar
    node->next->prev = node->prev;
    node->prev->next = node->next;
}


// No olvides el coste!
//Coste O(n) respecto a index ya que puede ser la ultima posicion de la lista
void ListLinkedDouble::add_to(int index, int val) {
    // Implementar

    if (val != 0) {
        int num = this->operator[](index);
        num += val;
        Node* cur = head->next;

        for (int i = 0; i < index; i++) {
            cur = cur->next;
        }
        cur->value = num;
        detach(cur);

        Node* list = nullptr;
        list = head->next;

        while (num > list->value && list != head) {
            list = list->next;
        }

        attach(cur, list);
    }
}


//}}}  


bool tratar_caso() {
    // Introduce aqu� el c�digo para tratar un caso de prueba.
    // Devuelve false si se ha le�do la marca de fin de entrada;
    // true en caso contrario.

    ListLinkedDouble l;
    int n, i, m, dato;
    cin >> n >> i >> m;

    if (n == 0 && i == 0 && m == 0) {
        return false;
    }

    for (int i = 0; i < n; i++) {
        cin >> dato;
        l.push_back(dato);
    }

    l.add_to(i, m);
    cout << l << endl;

    return true;

}

//---------------------------------------------------------------
// No modificar por debajo de esta l�nea
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
