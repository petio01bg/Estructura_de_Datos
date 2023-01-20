/*
 * ---------------------------------------------------
 *                ESTRUCTURAS DE DATOS
 * ---------------------------------------------------
 *              Manuel Montenegro Montes
 *              Facultad de Informática
 *         Universidad Complutense de Madrid
 * ---------------------------------------------------
 */

 /*
  * Esta implementación utiliza smart pointers para prevenir
  * fugas de memoria y problemas con la destrucción.
  */

#include <iostream>
#include <cassert>
#include <fstream>
#include <memory>
#include <queue>
#include <utility>
#include <limits>
#include <tuple>

using namespace std;

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

pair<int, int>excursionistas_aux(const BinTree<int>& arbol) {
    if (arbol.empty()) {
        return{ 0,0 };
    }
    else {
        auto [exc_izq, equipo_izq] = excursionistas_aux(arbol.left());
        auto [exc_der, equipo_der] = excursionistas_aux(arbol.right());

        int max_exc = max(exc_izq, exc_der);
        max_exc += arbol.root();

        int equipos = 1;
        if (arbol.root() > 0 && equipo_der == 0 && equipo_izq == 0) {
            return{ arbol.root(),equipos };
        }
        else {
            equipos = equipo_izq + equipo_der;
            return{ max_exc,equipos };
        }
    }
}

template <typename T>
tuple<bool, int, int>binario_completo(const BinTree<T>& arbol) {
    if (arbol.empty()) {
        return{ true,0,-1};
    }
    else {
        auto [izq, num_izq, niv_izq] = binario_completo(arbol.left());
        auto [der, num_der, niv_der] = binario_completo(arbol.right());

        bool completo = true;

        int hijos = num_izq + num_der;
        if (hijos != 0) {
            int div = hijos / 2;
            if (div % 2 != 0) {
                completo = false;
            }
        }
        
        bool total = completo & izq & der;

        return{ total,hijos,-1 };
    }
}

void excursionistas(const BinTree<int>& arbol) {
    cout << get<1>(excursionistas_aux(arbol)) << " ";
    cout << get<0>(excursionistas_aux(arbol)) << endl;
}



void tratar_caso() {
    BinTree<string> t = read_tree<string>(cin);
    bool ok = get<0>(binario_completo(t));

    if (ok) {
        cout << "SI" << endl;
    }
    else {
        cout << "NO" << endl;
    }
}

int main() {
    int num_casos;
    cin >> num_casos;

    for (int i = 0; i < num_casos; i++) {
        tratar_caso();
    }
    return 0;
}