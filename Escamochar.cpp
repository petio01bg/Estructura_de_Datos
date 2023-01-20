#include <iostream>
#include <cassert>
#include <fstream>

using namespace std;

class ListLinkedSingle {
private:
    struct Node {
        int value;
        Node* next;
    };

public:
    ListLinkedSingle() : head(nullptr) { }
    ~ListLinkedSingle() {
        delete_list(head);
    }

    ListLinkedSingle(const ListLinkedSingle& other)
        : head(copy_nodes(other.head)) { }

    void push_front(int elem) {
        Node* new_node = new Node{ elem, head };
        head = new_node;
    }

    void push_back(int elem);

    void pop_front() {
        assert(head != nullptr);
        Node* old_head = head;
        head = head->next;
        delete old_head;
    }

    void pop_back();

    int size() const;

    bool empty() const {
        return head == nullptr;
    };

    const int& front() const {
        assert(head != nullptr);
        return head->value;
    }

    int& front() {
        assert(head != nullptr);
        return head->value;
    }

    const int& back() const {
        return last_node()->value;
    }

    int& back() {
        return last_node()->value;
    }

    const int& at(int index) const {
        Node* result_node = nth_node(index);
        assert(result_node != nullptr);
        return result_node->value;
    }

    int& at(int index) {
        Node* result_node = nth_node(index);
        assert(result_node != nullptr);
        return result_node->value;
    }


    void display(std::ostream& out) const;
    void display() const {
        display(std::cout);
    }

    // Declaración del método. Impleméntalo antes de la
    // función tratar_caso()
    void escamochar(ListLinkedSingle& dest);

private:
    Node* head;

    void delete_list(Node* start_node);
    Node* last_node() const;
    Node* nth_node(int n) const;
    Node* copy_nodes(Node* start_node) const;

};

ListLinkedSingle::Node* ListLinkedSingle::copy_nodes(Node* start_node) const {
    if (start_node != nullptr) {
        Node* result = new Node{ start_node->value, copy_nodes(start_node->next) };
        return result;
    }
    else {
        return nullptr;
    }
}

void ListLinkedSingle::delete_list(Node* start_node) {
    if (start_node != nullptr) {
        delete_list(start_node->next);
        delete start_node;
    }
}

void ListLinkedSingle::push_back(int elem) {
    Node* new_node = new Node{ elem, nullptr };
    if (head == nullptr) {
        head = new_node;
    }
    else {
        last_node()->next = new_node;
    }
}

void ListLinkedSingle::pop_back() {
    assert(head != nullptr);
    if (head->next == nullptr) {
        delete head;
        head = nullptr;
    }
    else {
        Node* previous = head;
        Node* current = head->next;

        while (current->next != nullptr) {
            previous = current;
            current = current->next;
        }

        delete current;
        previous->next = nullptr;
    }
}

int ListLinkedSingle::size() const {
    int num_nodes = 0;

    Node* current = head;
    while (current != nullptr) {
        num_nodes++;
        current = current->next;
    }

    return num_nodes;
}


ListLinkedSingle::Node* ListLinkedSingle::last_node() const {
    assert(head != nullptr);
    Node* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }
    return current;
}

ListLinkedSingle::Node* ListLinkedSingle::nth_node(int n) const {
    assert(0 <= n);
    int current_index = 0;
    Node* current = head;

    while (current_index < n && current != nullptr) {
        current_index++;
        current = current->next;
    }

    return current;
}

void ListLinkedSingle::display(std::ostream& out) const {
    out << "[";
    if (head != nullptr) {
        out << head->value;
        Node* current = head->next;
        while (current != nullptr) {
            out << ", " << current->value;
            current = current->next;
        }
    }
    out << "]";
}

//@ <answer>
//---------------------------------------------------------------
// Modificar a partir de aquí
// --------------------------------------------------------------


// ¡No olvides indicar el coste del método!
//Coste del método es lineal O(n) respecto al numero de nodos
void ListLinkedSingle::escamochar(ListLinkedSingle& dest) {

    Node* curr = this->head;
    Node* ult = nullptr;
    Node* ultpos = nullptr;

    while (!empty() && front() < 0) {
        if (dest.head == nullptr) {       //Caso especial: lista dest vacia
            dest.head = curr;
            ult = curr;
        }
        else {
            ult->next = curr;
            ult = curr;
        }
        this->head = this->head->next;
        ult->next = nullptr;
        curr = this->head;
    }

    while (curr != nullptr) {
        if (curr->value >= 0) {
            ultpos = curr;
        }
        curr = curr->next;
    }

    if (ultpos != nullptr) {
        if (ult != nullptr) {
            ult->next = ultpos->next;
        }
        else {
            dest.head = ultpos->next;
        }

        ultpos->next = nullptr;
    }
}


void tratar_caso() {
    // Introduce aquí el código para tratar un caso de prueba.
    // Devuelve false si se ha leído la marca de fin de entrada;
    // true en caso contrario.
    int n, elem;
    ListLinkedSingle l1, l2;

    cin >> n;
    for (int j = 0; j < n; j++) {
        cin >> elem;
        l1.push_front(elem);
    }

    l1.escamochar(l2);

    l1.display();
    cout << endl;
    l2.display();
    cout << endl;
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

    int num_casos;
    cin >> num_casos;
    for (int i = 0; i < num_casos; i++) {
        tratar_caso();
    }

#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    // Descomentar si se trabaja en Windows
    // system("PAUSE");
#endif
    return 0;
}
