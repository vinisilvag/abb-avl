#include <iostream>

using namespace std;

struct info {
  unsigned key;
  string name;
  unsigned long long int cpf;
};

ostream &operator<<(ostream &output, const info &i) {
  output << "Nome: " << i.name << " | CPF: " << i.cpf << " | Cod: " << i.key;
  return output;
}

istream &operator>>(istream &input, info &i) {
  input >> i.key >> i.name >> i.cpf;
  return input;
}

class Node {
  friend class ABB;

private:
  info data;
  Node *left;
  Node *right;

public:
  Node(info data);
};

Node::Node(info data) {
  this->data = data;
  left = NULL;
  right = NULL;
}

class ABB {
  friend ostream &operator<<(ostream &output, ABB *arvore);

private:
  Node *root;
  void destruct(Node *node);
  Node *insertAux(Node *node, info data, int *accessedNodes);
  Node *searchAux(unsigned key, int *accessedNodes);
  void printRight(const string &prefix, const Node *node);
  void printLeft(const string &prefix, const Node *node, bool hasBrother);

public:
  ABB();
  ~ABB();
  void insert(info data, int *accessedNodes);
  info search(unsigned key, int *accessedNodes);
  void print();
};

ABB::ABB() { root = NULL; }

void ABB::destruct(Node *node) {
  if (node) {
    destruct(node->left);
    destruct(node->right);
    delete node;
  }
}

ABB::~ABB() { destruct(root); }

Node *ABB::insertAux(Node *node, info data, int *accessedNodes) {
  if (!node) {
    Node *newNode = new Node(data);
    return newNode;
  } else {
    if (data.key < node->data.key) {
      *accessedNodes = *accessedNodes + 1;
      node->left = insertAux(node->left, data, accessedNodes);
    } else {
      *accessedNodes = *accessedNodes + 1;
      node->right = insertAux(node->right, data, accessedNodes);
    }
  }

  return node;
}

void ABB::insert(info data, int *accessedNodes) {
  root = insertAux(root, data, accessedNodes);
}

Node *ABB::searchAux(unsigned key, int *accessedNodes) {
  Node *current = root;

  while (current != NULL) {
    if (current->data.key == key)
      return current;
    else if (current->data.key > key) {
      *accessedNodes = *accessedNodes + 1;
      current = current->left;
    } else {
      *accessedNodes = *accessedNodes + 1;
      current = current->right;
    }
  }

  return current;
}

info ABB::search(unsigned key, int *accessedNodes) {
  Node *searchedNode = searchAux(key, accessedNodes);

  if (searchedNode) {
    return searchedNode->data;
  } else {
    cout << "Nós acessados ao total: " << *accessedNodes << endl;
    throw runtime_error("Erro na busca: elemento não encontrado!");
  }
}

ostream &operator<<(ostream &output, ABB *tree) {
  tree->print();
  return output;
}

void ABB::printRight(const string &prefix, const Node *node) {
  if (node != nullptr) {
    cout << prefix << "└d─"
         << "(" << node->data.key << "," << node->data.name << ")" << endl;

    printLeft(prefix + "    ", node->left, node->right == nullptr);
    printRight(prefix + "    ", node->right);
  }
}

void ABB::printLeft(const string &prefix, const Node *node, bool hasBrother) {
  if (node != nullptr) {
    cout << prefix;

    if (hasBrother)
      cout << "└e─";
    else
      cout << "├e─";

    cout << "(" << node->data.key << "," << node->data.name << ")" << endl;

    printLeft(prefix + "│   ", node->left, node->right == nullptr);
    printRight(prefix + "│   ", node->right);
  }
}

void ABB::print() {
  if (this->root != nullptr) {
    cout << "(" << this->root->data.key << "," << this->root->data.name << ")"
         << endl;

    printLeft(" ", this->root->left, this->root->right == nullptr);
    printRight(" ", this->root->right);
  } else
    cout << "Arvore vazia" << endl;
}

int main() {
  ABB *tree = new ABB();

  char option;
  info data;
  unsigned key;
  int accessedNodes = 0;

  do {
    try {
      cin >> option;
      switch (option) {
      case 'i':
        accessedNodes = 0;
        cin >> data;
        tree->insert(data, &accessedNodes);
        cout << "Nós acessados ao total: " << accessedNodes << endl;
        break;
      case 'b':
        accessedNodes = 0;
        cin >> key;
        data = tree->search(key, &accessedNodes);
        cout << data << endl;
        cout << "Nós acessados ao total: " << accessedNodes << endl;
        break;
      case 'e':
        cout << tree;
        break;
      case 'f':
        break;
      default:
        cout << "Comando invalido!" << endl;
      }
    } catch (runtime_error &e) {
      cout << e.what() << endl;
    }
  } while (option != 'f');
}