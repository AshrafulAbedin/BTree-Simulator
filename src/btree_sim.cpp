#include <bits/stdc++.h>
using namespace std;

// ============ ANSI Color Codes ============
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

void enableANSI() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

// Banner
void printBanner() {
    cout << BOLD << CYAN;
    cout << "\n╔════════════════════════════════════╗\n";
    cout << YELLOW << "║        B-Tree Simulator            ║\n";
    cout << GREEN  << "║     CSE 4307 - Database Systems    ║\n";
    cout << CYAN << "╚════════════════════════════════════╝\n";
    cout << RESET << endl;
}

// ============ B-Tree Structures ============
struct Node {
    vector<int> keys;
    vector<Node*> children;
    bool leaf;

    Node(bool _leaf) : leaf(_leaf) {}
};

class BTree {
    Node* root;
    int m; 

public:
    BTree(int order) : m(order), root(nullptr) {}

    void printNode(Node* node) {
        cout << "[";
        for (size_t i = 0; i < node->keys.size(); ++i) {
            cout << node->keys[i];
            if (i < node->keys.size() - 1) cout << "|";
        }
        cout << "]";
    }

    void printPaths(Node* node, vector<Node*>& path) {
        if (!node) return;
        path.push_back(node);

        if (node->leaf) {
            for (size_t i = 0; i < path.size(); ++i) {
                printNode(path[i]);
                if (i < path.size() - 1) cout << "->";
            }
            cout << " ";
        } else {
            for (Node* child : node->children) {
                printPaths(child, path);
            }
        }

        path.pop_back();
    }

    void printTree() {
        if (!root) {
            cout << "[]" << endl;
            return;
        }
        vector<Node*> path;
        printPaths(root, path);
        cout << endl;
    }

    void insert(int key) {
        if (!root) {
            root = new Node(true);
            root->keys.push_back(key);
            return;
        }

        Node* leaf = root;
        vector<Node*> path;

        while (!leaf->leaf) {
            path.push_back(leaf);
            int i = 0;
            while (i < leaf->keys.size() && key > leaf->keys[i]) i++;
            leaf = leaf->children[i];
        }

        leaf->keys.push_back(key);
        sort(leaf->keys.begin(), leaf->keys.end());

        Node* current = leaf;
        bool splitNeeded = (current->keys.size() >= m);

        while (splitNeeded) {
            Node* newNode = new Node(current->leaf);

            int midIndex = (current->keys.size() - 1) / 2;
            int splitKey = current->keys[midIndex];

            for (int i = midIndex + 1; i < current->keys.size(); ++i)
                newNode->keys.push_back(current->keys[i]);

            current->keys.resize(midIndex);

            if (!current->leaf) {
                for (int i = midIndex + 1; i < current->children.size(); ++i)
                    newNode->children.push_back(current->children[i]);

                current->children.resize(midIndex + 1);
            }

            if (current == root) {
                Node* newRoot = new Node(false);
                newRoot->keys.push_back(splitKey);
                newRoot->children.push_back(current);
                newRoot->children.push_back(newNode);
                root = newRoot;
                break;
            } else {
                Node* parent = path.back();
                path.pop_back();

                int idx = 0;
                while (idx < parent->keys.size() && splitKey > parent->keys[idx]) idx++;

                parent->keys.insert(parent->keys.begin() + idx, splitKey);
                parent->children.insert(parent->children.begin() + idx + 1, newNode);

                if (parent->keys.size() >= m) {
                    current = parent;
                } else {
                    break;
                }
            }
        }
    }
};

// ============ Main ============
int main() {
    system("chcp 65001 > nul"); //If use powershell, run this: chcp 65001
    enableANSI();   
    printBanner();

    int n;
    cin >> n;

    vector<int> values(n);
    for (int i = 0; i < n; ++i)
        cin >> values[i];

    int m;
    cin >> m;

    BTree tree(m);

    for (int i = 0; i < n; ++i) {
        cout << GREEN << "Step " << (i + 1)
             << ": Insert " << YELLOW << values[i]
             << RESET << endl;

        tree.insert(values[i]);
        tree.printTree();
    }

    cout << BOLD << BLUE << "\nSimulation Complete!\n" << RESET;
}