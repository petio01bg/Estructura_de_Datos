#include <iostream>
#include <cassert>
#include <fstream>
#include <memory>
#include <queue>
#include <utility>
#include <limits>
#include <tuple>


template <class T> class BinTree {

private:
    struct TreeNode;
    using NodePointer = std::shared_ptr<TreeNode>;

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

    template <typename U> void preorder(U func) const {
        preorder(root_node, func);
    }

    template <typename U> void inorder(U func) const { inorder(root_node, func); }

    template <typename U> void postorder(U func) const {
        postorder(root_node, func);
    }

    template <typename U> void levelorder(U func) const;

private:
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

    template <typename U> static void preorder(const NodePointer& node, U func);

    template <typename U> static void inorder(const NodePointer& node, U func);

    template <typename U> static void postorder(const NodePointer& node, U func);
};

template <typename T>
template <typename U>
void BinTree<T>::preorder(const NodePointer& node, U func) {
    if (node != nullptr) {
        func(node->elem);
        preorder(node->left, func);
        preorder(node->right, func);
    }
}

template <typename T>
template <typename U>
void BinTree<T>::inorder(const NodePointer& node, U func) {
    if (node != nullptr) {
        inorder(node->left, func);
        func(node->elem);
        inorder(node->right, func);
    }
}

template <typename T>
template <typename U>
void BinTree<T>::postorder(const NodePointer& node, U func) {
    if (node != nullptr) {
        postorder(node->left, func);
        postorder(node->right, func);
        func(node->elem);
    }
}

template <typename T>
template <typename U>
void BinTree<T>::levelorder(U func) const {
    std::queue<NodePointer> pending;
    if (root_node != nullptr) {
        pending.push(root_node);
    }
    while (!pending.empty()) {
        NodePointer current = pending.front();
        pending.pop();
        func(current->elem);
        if (current->left != nullptr) {
            pending.push(current->left);
        }
        if (current->right != nullptr) {
            pending.push(current->right);
        }
    }
}

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

//Coste 
// T(n) = k1 si n = 0
// T(n) = T(ni) + T(nd) + k2 si n > 0
//O(n)

//Componentes:
//1º Valor maximo de la diferencia entre dos elementos del arbol
//2º Valor de la hoja del arbol que va a continuar la secuencia del preorder
tuple<int, int,int> cuadrado_perfecto(const const BinTree<int>& t) {

    if (t.left().empty() && t.right().empty()) {
        return{ t.root(), 0 ,-1};
    }
    else if (t.empty()) {
        return{ 0,0 ,-1};
    }
    else {
        auto [ni, ai,i] = cuadrado_perfecto(t.left());
        auto [nr, ar,r] = cuadrado_perfecto(t.right());

        if (ni + nr == t.root()) {
            if (ai >= ar ) {
                if (i != -1) {
                    return { t.root() + ni + nr,ai + 1,i };
                }
                else {
                    return { t.root() + ni + nr,ai + 1,t.root()};
                }
            }
            else {
                if (i != -1) {
                    return { t.root() + ni + nr,ar + 1,r };
                }
                else {
                    return { t.root() + ni + nr,ar + 1,t.root() }; 
                }
            }
        }
        else {
            return{t.root(),1, -1}
        }
    }
}

pair<bool, int> diestro(const const BinTree<int>& t) {
    if (t.empty()) {
        return{ true,0 };
    }
    else if (t.left().empty() && t.right().empty()) {
        return{ true, t.root() };
    }
    else {
        auto [bl, ni] = diestro(t.left());
        auto [br, nr] = diestro(t.right());

        bool ok;
        if (nr > ni) {
            ok = true;
            return{ ok && bl && br, ni + nr + t.root() };
        }
        else {
            ok = false;
            return{ ok && bl && br, ni + nr + t.root() };
        }
    }
}

pair<int, int>maximo_aux(const BinTree<int>& t) {
    if (t.empty()) {
        return{ -1,0 };
    }
    else {

        int op1 = 0, op2 = 0;
        if (!t.left().empty())
            op1 = abs(t.root() - t.left().root());
        else if (!t.right().empty())
            op1 = abs(t.root() - t.right().root());

        auto [max_izq, valor_izq] = maximo_aux(t.left());

        if (max_izq != -1 && !t.right().empty()) {
            op2 = abs(valor_izq - t.right().root());
        }

        auto [max_der, valor_der] = maximo_aux(t.right());


        int maxf1 = max(op1, op2);
        int maxf2 = max(max_izq, max_der);
        int maxf = max(maxf1, maxf2);

        if (max_der == -1 && max_izq == -1) {
            return{ maxf,t.root() };
        }
        else if (max_der != -1) {
            return{ maxf,valor_der };
        }
        else {
            return{ maxf,valor_izq };
        }
    }
}

int maximo(const BinTree<int>& tree) {
    return get<0>(maximo_aux(tree));
}

// No olvides el coste!
//Coste O(n) al llamar la funcion maximo(t) que llama a una funcion maximo_aux() que es de coste lineal O(n)
int maxima_diferencia(const BinTree<int>& t) {
    // Implementa aquí la función. No puedes modificar el tipo
    // de entrada ni de salida. No obstante, puedes apoyarte en
    // funciones auxiliares recursivas, siempre que tengan
    // un único parámetro de entrada (de tipo const BinTree<int> &)
    return maximo(t);
}

// Función para tratar un caso de prueba
void tratar_caso() {
    BinTree<int> t = read_tree<int>(cin);
    cout << maxima_diferencia(t) << "\n";
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
