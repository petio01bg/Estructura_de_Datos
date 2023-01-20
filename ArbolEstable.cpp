#include <iostream>
#include <cassert>
#include <fstream>
#include <memory>
#include <utility>
#include <tuple>

template <class T> class BinTree {
public:
    BinTree() : root_node(nullptr) {}

    BinTree(const T& elem)
        : root_node(std::make_shared<TreeNode>(nullptr, elem, nullptr)) {}

    BinTree(const BinTree& left, const T& elem, const BinTree& right)
        : root_node(std::make_shared<TreeNode>(left.root_node, elem,
            right.root_node)) {}

    bool empty() const { return root_node == nullptr; }

    const T& root() const {
        assert(root_node != nullptr);
        return root_node->elem;
    }

    BinTree left() const {
        assert(root_node != nullptr);
        BinTree result;
        result.root_node = root_node->left;
        return result;
    }

    BinTree right() const {
        assert(root_node != nullptr);
        BinTree result;
        result.root_node = root_node->right;
        return result;
    }

    void display(std::ostream& out) const { display_node(root_node, out); }

private:
    // Las definiciones de TreeNode y NodePointer dependen recursivamente
    // la una de la otra. Por eso declaro 'struct TreeNode;' antes de NodePointer
    // para que el compilador sepa, cuando analice la definición de NodePointer,
    // que TreeNode va a ser definida más adelante.

    struct TreeNode;
    using NodePointer = std::shared_ptr<TreeNode>;

    struct TreeNode {
        TreeNode(const NodePointer& left, const T& elem, const NodePointer& right)
            : elem(elem), left(left), right(right) {}

        T elem;
        NodePointer left, right;
    };

    NodePointer root_node;

    static void display_node(const NodePointer& root, std::ostream& out) {
        if (root == nullptr) {
            out << ".";
        }
        else {
            out << "(";
            display_node(root->left, out);
            out << " " << root->elem << " ";
            display_node(root->right, out);
            out << ")";
        }
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const BinTree<T>& tree) {
    tree.display(out);
    return out;
}

template <typename T> BinTree<T> read_tree(std::istream& in) {
    char c;
    in >> c;
    if (c == '.') {
        return BinTree<T>();
    }
    else {
        assert(c == '(');
        BinTree<T> left = read_tree<T>(in);
        T elem;
        in >> elem;
        BinTree<T> right = read_tree<T>(in);
        in >> c;
        assert(c == ')');
        BinTree<T> result(left, elem, right);
        return result;
    }
}

using namespace std;

//@ <answer>
//---------------------------------------------------------------
// Modificar a partir de aquí
// --------------------------------------------------------------


//T(n) = k1 si n = 0
//     = T(ni) + T(nd) + k2 si n > 0      T(n) => O(n)
template <typename T>
int estable(const BinTree<T>& arbol, const int& max, int cont) {
    int ret = 0;

    if (arbol.empty()) {
        ret = 0;
    }
    else {
        
        ret += estable(arbol.left(), max, cont+1);
        ret += estable(arbol.right(), max, cont+1);

        if (cont + 1 == max) {
            ret++;
        }
    }

    return ret;
}

//T(n) = k1 si n = 0
//     = T(ni) + T(nd) + k2 si n > 0      T(n) => O(n)
template <typename T>
int altura(const BinTree<T>& tree) {
    if (tree.empty()) {
        return 0;
    }
    else {
        return 1 + max(altura(tree.left()), altura(tree.right()));
    }
}

// No olvides el coste!
//Es lineal O(n) respecto a las llamadas de las funciones estable y altura que tienen coste O(n)
template <typename T>
bool estable_altura(const BinTree<T>& arbol) {
    // Implementa aquí la función. No puedes modificar el tipo
    // de entrada ni de salida. No obstante, puedes apoyarte en
    // funciones auxiliares con el tipo que quieras.

    bool ok;
    int s = altura(arbol);

    int num = estable(arbol, s, 0);

    if (num == 1) {
        ok = false;
    }
    else {
        ok = true;
    }

    return ok;
}

// Función para tratar un caso de prueba
void tratar_caso() {
    BinTree<int> t = read_tree<int>(cin);
    cout << (estable_altura(t) ? "SI" : "NO") << "\n";
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

