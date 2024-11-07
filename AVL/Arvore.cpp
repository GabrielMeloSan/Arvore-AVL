#include <iostream>
#include <string>

using namespace std;

class Contato {
    public:
    string nome, email;
    int telefone;
    Contato* left;
    Contato* right;

    public:
    Contato(string name,string eMail, int tel){
        nome = name;
        email = eMail;
        telefone = tel;
        left = nullptr;
        right = nullptr;
    }
};

class BinaryTree {
    private:
    Contato* root;

    public:

    int CalcularLetra(string palavra){
        char letra = palavra[0];
        int numero = letra - 'A' + 1;
        return numero;
    }

    BinaryTree() {
    root = nullptr;
    }

    void insercao(string name, string eMail, int tel) {
    root = insert(root, name, eMail, tel);
    }

    void leituraEmOrdem() {
    inorder (root);
    cout << endl;
    }

    private:
    Contato* insert (Contato* contato, string name, string eMail, int tel) {
        if (contato == nullptr) {
            return new Contato(name, eMail, tel);
        }
        if (CalcularLetra(name) < CalcularLetra(contato->nome)) {
            contato->left = insert (contato->left, name, eMail, tel);
        } else if (CalcularLetra(name) > CalcularLetra(contato->nome)) {
            contato->right = insert (contato->right, name, eMail, tel);
        }
        return contato;
    }

    void inorder (Contato* contato) {
        if (contato != nullptr) {
            inorder (contato->left);
            cout << contato->nome << ", " << contato->email << ", " << contato->telefone << "\n";
            inorder (contato->right);
        }
    }
};

int main() {
    BinaryTree tree;

    tree.insercao("Joao", "abc@gmail.com", 4856985);
    tree.insercao("Andre", "abcd@gmail.com", 8848485);
    tree.insercao("Bruno", "abce@gmail.com", 98456846);
    tree.insercao("Flavio", "abcf@gmail.com", 484848);
    tree.insercao("Eduardo", "abcg@gmail.com", 123456);
    tree.insercao("Marcos","abch@gmail.com", 654321);
    tree.insercao("Mateus","abci@gmail.com", 250663);

    cout << "Leitura em Ordem da árvore:" << endl;
    tree.leituraEmOrdem();

 return 0;
 }

