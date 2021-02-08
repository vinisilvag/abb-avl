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
  friend class AVL;
  friend unsigned currentHeight(Node *node);
  friend void updateHeight(Node *node);
  friend int balanceFactor(Node *node);

private:
  info data;
  unsigned height;
  Node *left;
  Node *right;

public:
  Node(info data);
};

Node::Node(info data) {
  this->data = data;
  height = 1;
  left = NULL;
  right = NULL;
}

unsigned currentHeight(Node *node) {
  if (node == NULL)
    return 0;
  else
    return node->height;
}

void updateHeight(Node *node) {
  unsigned leftTreeHeight = currentHeight(node->left);
  unsigned rightTreeHeight = currentHeight(node->right);
  node->height = 1 + max(leftTreeHeight, rightTreeHeight);
}

int balanceFactor(Node *node) {
  unsigned leftTreeHeight = currentHeight(node->left);
  unsigned rightTreeHeight = currentHeight(node->right);

  return leftTreeHeight - rightTreeHeight;
}

class AVL {
  friend ostream &operator<<(ostream &output, AVL *arvore);

private:
  Node *root;
  void destruct(Node *node);
  Node *insertAux(Node *node, info data, int *accessedNodes);
  Node *leftRotate(Node *node, int *accessedNodes);
  Node *rightRotate(Node *node, int *accessedNodes);
  Node *fixBalancing(Node *node, int *accessedNodes);
  Node *searchAux(unsigned key, int *accessedNodes);
  void printRight(const string &prefix, const Node *node);
  void printLeft(const string &prefix, const Node *node, bool hasBrother);

public:
  AVL();
  ~AVL();
  void insert(info data, int *accessedNodes);
  info search(unsigned key, int *accessedNodes);
  void print();
};

AVL::AVL() { root = NULL; }

void AVL::destruct(Node *node) {
  if (node) {
    destruct(node->left);
    destruct(node->right);
    delete node;
  }
}

AVL::~AVL() { destruct(root); }

Node *AVL::insertAux(Node *node, info data, int *accessedNodes) {
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

  return fixBalancing(node, accessedNodes);
}

void AVL::insert(info data, int *accessedNodes) {
  root = insertAux(root, data, accessedNodes);
}

Node *AVL::fixBalancing(Node *node, int *accessedNodes) {
  if (!node)
    return node;

  *accessedNodes = *accessedNodes + 1;

  updateHeight(node);

  int balance = balanceFactor(node);

  if ((balance >= -1) and (balance <= 1)) {
    return node;
  } else if ((balance > 1) and (balanceFactor(node->left) >= 0)) {
    return rightRotate(node, accessedNodes);
  } else if ((balance > 1) and (balanceFactor(node->left) < 0)) {
    node->left = leftRotate(node->left, accessedNodes);
    return rightRotate(node, accessedNodes);
  } else if ((balance < -1) and (balanceFactor(node->right) <= 0)) {
    return leftRotate(node, accessedNodes);
  } else if ((balance < -1) and (balanceFactor(node->right) > 0)) {
    node->right = rightRotate(node->right, accessedNodes);
    return leftRotate(node, accessedNodes);
  }

  return node;
}

Node *AVL::leftRotate(Node *node, int *accessedNodes) {
  Node *auxNode = node->right;

  node->right = auxNode->left;

  auxNode->left = node;

  updateHeight(node);
  updateHeight(auxNode);

  *accessedNodes = *accessedNodes + 2;

  return auxNode;
}

Node *AVL::rightRotate(Node *node, int *accessedNodes) {
  Node *auxNode = node->left;

  node->left = auxNode->right;

  auxNode->right = node;

  updateHeight(node);
  updateHeight(auxNode);

  *accessedNodes = *accessedNodes + 2;

  return auxNode;
}

Node *AVL::searchAux(unsigned key, int *accessedNodes) {
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

info AVL::search(unsigned key, int *accessedNodes) {
  Node *searchedNode = searchAux(key, accessedNodes);

  if (searchedNode) {
    return searchedNode->data;
  } else {
    cout << "Nós acessados ao total: " << *accessedNodes << endl;
    throw runtime_error("Erro na busca: elemento não encontrado!");
  }
}

ostream &operator<<(ostream &output, AVL *tree) {
  tree->print();
  return output;
}

void AVL::printRight(const string &prefix, const Node *node) {
  if (node != nullptr) {
    cout << prefix << "└d─"
         << "(" << node->data.key << "," << node->data.name << ")" << endl;

    printLeft(prefix + "    ", node->left, node->right == nullptr);
    printRight(prefix + "    ", node->right);
  }
}

void AVL::printLeft(const string &prefix, const Node *node, bool hasBrother) {
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

void AVL::print() {
  if (this->root != nullptr) {
    cout << "(" << this->root->data.key << "," << this->root->data.name << ")"
         << endl;

    printLeft(" ", this->root->left, this->root->right == nullptr);
    printRight(" ", this->root->right);
  } else
    cout << "Arvore vazia" << endl;
}

int main() {
  AVL *tree = new AVL();

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