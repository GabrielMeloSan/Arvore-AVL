#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Contato {
public:
    string nome, email;
    int telefone;
    bool favorito;
    Contato* left;
    Contato* right;
    int altura;

    Contato(string name, string eMail, int tel) {
        nome = name;
        email = eMail;
        telefone = tel;
        favorito = false;
        left = nullptr;
        right = nullptr;
        altura = 1;
    }
};

class AVLTree {
private:
    Contato* root;

public:
    AVLTree() {
        root = nullptr;
    }

    void adicionarContato(string name, string eMail, int tel) {
        root = insert(root, name, eMail, tel);
    }

    void removerContato(string nome) {
        root = remove(root, nome);
    }

    Contato* buscarContato(string nome) {
        return buscarContato(root, nome);
    }

    void listarContatosEmOrdem() {
        inorder(root);
        cout << endl;
    }

    void adicionarFavorito(string nome) {
        Contato* contato = buscarContato(root, nome);
        if (contato && !contato->favorito) {
            contato->favorito = true;
            cout << "Contato " << nome << " adicionado aos favoritos!" << endl;
        } else if (contato && contato->favorito) {
            cout << "Contato " << nome << " já é favorito!" << endl;
        } else {
            cout << "Contato " << nome << " não encontrado!" << endl;
        }
    }

    void listarFavoritos() {
        cout << "Contatos Favoritos:" << endl;
        listarFavoritosRec(root);
        cout << endl;
    }

    // Função para exportar contatos para um arquivo CSV
    void exportarCSV(string nomeArquivo) {
        ofstream arquivo(nomeArquivo);
        if (!arquivo) {
            cout << "Erro ao abrir o arquivo para exportação!" << endl;
            return;
        }

        arquivo << "Nome,Email,Telefone,Favorito\n";
        exportarCSVRec(root, arquivo);

        arquivo.close();
        cout << "Contatos exportados para " << nomeArquivo << " com sucesso!" << endl;
    }

    // Função para importar contatos de um arquivo CSV
    void importarCSV(string nomeArquivo) {
        ifstream arquivo(nomeArquivo);
        if (!arquivo) {
            cout << "Erro ao abrir o arquivo para importação!" << endl;
            return;
        }

        string linha;
        getline(arquivo, linha); // Pular a linha de cabeçalho
        while (getline(arquivo, linha)) {
            if (linha.empty()) continue;

            stringstream ss(linha);
            string nome, email, telefoneStr, favoritoStr;
            int telefone;
            bool favorito;

            getline(ss, nome, ',');
            getline(ss, email, ',');
            getline(ss, telefoneStr, ',');
            telefone = stoi(telefoneStr);
            getline(ss, favoritoStr, ',');
            favorito = (favoritoStr == "1");

            adicionarContato(nome, email, telefone);
            if (favorito) {
                adicionarFavorito(nome);
            }
        }

        arquivo.close();
        cout << "Contatos importados de " << nomeArquivo << " com sucesso!" << endl;
    }
    


private:
    int altura(Contato* contato) {
        return contato ? contato->altura : 0;
    }

    int getBalance(Contato* contato) {
        return contato ? altura(contato->left) - altura(contato->right) : 0;
    }

    Contato* rotateRight(Contato* y) {
        Contato* x = y->left;
        Contato* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->altura = max(altura(y->left), altura(y->right)) + 1;
        x->altura = max(altura(x->left), altura(x->right)) + 1;

        return x;
    }

    Contato* rotateLeft(Contato* x) {
        Contato* y = x->right;
        Contato* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->altura = max(altura(x->left), altura(x->right)) + 1;
        y->altura = max(altura(y->left), altura(y->right)) + 1;

        return y;
    }

    Contato* insert(Contato* contato, string name, string eMail, int tel) {
        if (contato == nullptr)
            return new Contato(name, eMail, tel);

        if (name < contato->nome)
            contato->left = insert(contato->left, name, eMail, tel);
        else if (name > contato->nome)
            contato->right = insert(contato->right, name, eMail, tel);
        else
            return contato;

        contato->altura = 1 + max(altura(contato->left), altura(contato->right));

        int balance = getBalance(contato);

        if (balance > 1 && name < contato->left->nome)
            return rotateRight(contato);
        if (balance < -1 && name > contato->right->nome)
            return rotateLeft(contato);
        if (balance > 1 && name > contato->left->nome) {
            contato->left = rotateLeft(contato->left);
            return rotateRight(contato);
        }
        if (balance < -1 && name < contato->right->nome) {
            contato->right = rotateRight(contato->right);
            return rotateLeft(contato);
        }

        return contato;
    }

    Contato* buscarContato(Contato* contato, string nome) {
        if (contato == nullptr || contato->nome == nome)
            return contato;
        if (nome < contato->nome)
            return buscarContato(contato->left, nome);
        return buscarContato(contato->right, nome);
    }

    Contato* remove(Contato* root, string nome) {
        if (root == nullptr) return root;

        if (nome < root->nome)
            root->left = remove(root->left, nome);
        else if (nome > root->nome)
            root->right = remove(root->right, nome);
        else {
            if ((root->left == nullptr) || (root->right == nullptr)) {
                Contato* temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else
                    *root = *temp;
                delete temp;
            } else {
                Contato* temp = minValueNode(root->right);
                root->nome = temp->nome;
                root->right = remove(root->right, temp->nome);
            }
        }
        if (root == nullptr) return root;

        root->altura = 1 + max(altura(root->left), altura(root->right));

        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0)
            return rotateRight(root);
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0)
            return rotateLeft(root);
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rotateRight(root);
            return rotateLeft(root);
        }

        return root;
    }

    Contato* minValueNode(Contato* contato) {
        Contato* atual = contato;
        while (atual->left != nullptr)
            atual = atual->left;
        return atual;
    }

    void inorder(Contato* contato) {
        if (contato != nullptr) {
            inorder(contato->left);
            cout << contato->nome << ", " << contato->email << ", " << contato->telefone << "\n";
            inorder(contato->right);
        }
    }

    void listarFavoritosRec(Contato* contato) {
        if (contato != nullptr) {
            listarFavoritosRec(contato->left);
            if (contato->favorito) {
                cout << contato->nome << ", " << contato->email << ", " << contato->telefone << "\n";
            }
            listarFavoritosRec(contato->right);
        }
    }

    void exportarCSVRec(Contato* contato, ofstream& arquivo) {
        if (contato != nullptr) {
            exportarCSVRec(contato->left, arquivo);
            arquivo << contato->nome << "," << contato->email << "," << contato->telefone << "," << (contato->favorito ? "1" : "0") << "\n";
            exportarCSVRec(contato->right, arquivo);
        }
    }
    
    
};

void exibirMenu() {
    cout << "\nMenu de Opções:\n";
    cout << "1. Adicionar Contato\n";
    cout << "2. Remover Contato\n";
    cout << "3. Buscar Contato\n";
    cout << "4. Listar Contatos\n";
    cout << "5. Listar Contatos Favoritos\n";
    cout << "6. Exportar para CSV\n";
    cout << "7. Importar de CSV\n";
    cout << "8. testeListagemOrdenada\n";
    cout << "0. Sair\n";
}

int main() {
    AVLTree tree;
    int escolha;
    string nome, email, nomeArquivo;
    int telefone;

    while (true) {
        exibirMenu();
        cout << "Escolha uma opção: ";
        cin >> escolha;
        cin.ignore();  // Para limpar o buffer do teclado

        switch (escolha) {
            case 1:
                cout << "Nome: ";
                getline(cin, nome);
                cout << "Email: ";
                getline(cin, email);
                cout << "Telefone: ";
                cin >> telefone;
                tree.adicionarContato(nome, email, telefone);
                break;

            case 2:
                cout << "Nome do contato a remover: ";
                getline(cin, nome);
                tree.removerContato(nome);
                break;

            case 3:
                cout << "Nome do contato a buscar: ";
                getline(cin, nome);
                {
                    Contato* contato = tree.buscarContato(nome);
                    if (contato) {
                        cout << "Nome: " << contato->nome << "\n";
                        cout << "Email: " << contato->email << "\n";
                        cout << "Telefone: " << contato->telefone << "\n";
                    } else {
                        cout << "Contato não encontrado!" << endl;
                    }
                }
                break;

            case 4:
                tree.listarContatosEmOrdem();
                break;

            case 5:
                tree.listarFavoritos();
                break;

            case 6:
                cout << "Nome do arquivo CSV para exportação: ";
                getline(cin, nomeArquivo);
                tree.exportarCSV(nomeArquivo);
                break;

            case 7:
                cout << "Nome do arquivo CSV para importação: ";
                getline(cin, nomeArquivo);
                tree.importarCSV(nomeArquivo);
                break;
            case 8:
                
                break;
                
            case 0:
                cout << "Fechando...\n";
                return 0;

            default:
                cout << "Opção inválida! Tente novamente.\n";
        }
    }
    return 0;
}
