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

///Definicion de cada parametro:
//1º Indica si el arbol esta equilibrado
//2º Altura del arbol
//3º Numero de hijos del arbol
template <typename T>
tuple<bool, int, int> balanced_height(const BinTree<T>& tree) {
    if (tree.empty()) {
        return { true, 0, 0 };
    }
    else {
        auto [bal_left, height_left, num_i] = balanced_height(tree.left());
        auto [bal_right, height_right, num_d] = balanced_height(tree.right());

        bool ok = true;

        if (height_left > height_right && num_d == 1) {
            ok = false;
        }

        if (height_right > height_left && num_i == 1) {
            ok = false;
        }

        int hijos = 0;

        if (height_left == 0 && height_right == 0) {
            hijos += 1;
        }

        hijos += num_i + num_d;
        bool balanced = bal_left & bal_right & ok;
        int height = 1 + max(height_left, height_right);
        return { balanced, height, hijos };
    }
}

template <typename T>
bool balanced(const BinTree<T>& tree) {
    return get<0>(balanced_height(tree));
}


// ¡No olvides el coste!
//Es lineal O(n) respecto a las llamadas de la funcion balanced() que tiene coste O(n)
template <typename T>
bool estable(const BinTree<T>& arbol) {
    // Implementa aquí la función pedida. No puedes
    // modificar la cabecera, pero puedes apoyarte en funciones
    // auxiliares, si necesitas devolver más de un resultado.

    return balanced(arbol);
}


// Función que trata un caso de prueba
void tratar_caso() {
    BinTree<int> t = read_tree<int>(cin);
    cout << (estable(t) ? "SI" : "NO") << "\n";
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