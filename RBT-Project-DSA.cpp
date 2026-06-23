#include <iostream>
#include <string>

using namespace std;

const int RED = 0;
const int BLACK = 1;

struct Node {
    int id;
    string name;
    string course;
    double cgpa;
    int color;
    Node* left;
    Node* right;
    Node* parent;
};

class RBTree {
private:
    Node* root;
    Node* nullNode;

    void leftRotate(Node* x)
    {
        Node* y = x->right;
        x->right = y->left;

        if (y->left != nullNode)
        {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == nullNode)
        {
            root = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* x)
    {
        Node* y = x->left;
        x->left = y->right;

        if (y->right != nullNode)
        {
            y->right->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == nullNode)
        {
            root = y;
        }
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }
        else
        {
            x->parent->left = y;
        }

        y->right = x;
        x->parent = y;
    }

    void insertFixup(Node* z)
    {
        while (z->parent->color == RED)
        {
            if (z->parent == z->parent->parent->left)
            {
                Node* uncle = z->parent->parent->right;

                if (uncle->color == RED)
                {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->right)
                    {
                        z = z->parent;
                        leftRotate(z);
                    }

                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            }
            else
            {
                Node* uncle = z->parent->parent->left;

                if (uncle->color == RED)
                {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->left)
                    {
                        z = z->parent;
                        rightRotate(z);
                    }

                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }

        root->color = BLACK;
    }

    void replaceNode(Node* u, Node* v)
    {
        if (u->parent == nullNode)
        {
            root = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void deleteFixup(Node* x)
    {
        Node* w;
        while (x != root && x->color == BLACK)
        {
            if (x == x->parent->left)
            {
                w = x->parent->right;

                if (w->color == RED)
                {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }

                if (w->left->color == BLACK && w->right->color == BLACK)
                {
                    w->color = RED;
                    x = x->parent;
                }
                else
                {
                    if (w->right->color == BLACK)
                    {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }

                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else
            {
                w = x->parent->left;

                if (w->color == RED)
                {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }

                if (w->left->color == BLACK && w->right->color == BLACK)
                {
                    w->color = RED;
                    x = x->parent;
                }
                else
                {
                    if (w->left->color == BLACK)
                    {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }

                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void inorder(Node* node)
    {
        if (node == nullNode)
            return;

        inorder(node->left);

        cout << "ID: " << node->id << endl;
        cout << "Name: " << node->name << endl;
        cout << "Course: " << node->course << endl;
        cout << "CGPA: " << node->cgpa << endl;

        if (node->color == RED)
            cout << "Color: RED" << endl;
        else
            cout << "Color: BLACK" << endl;

        cout << endl;

        inorder(node->right);
    }

    void destroyTree(Node* node)
    {
        if (node == nullNode)
            return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    RBTree() {
        nullNode = new Node();
        nullNode->color = BLACK;
        nullNode->left = nullNode;
        nullNode->right = nullNode;
        nullNode->parent = nullNode;
        root = nullNode;
    }

    void insert(int id, string name, string course, double cgpa) {
        Node* z = new Node();
        z->id = id;
        z->name = name;
        z->course = course;
        z->cgpa = cgpa;
        z->color = RED;
        z->left = nullNode;
        z->right = nullNode;

        Node* y = nullNode;
        Node* x = root;

        while (x != nullNode) {
            y = x;
            if (z->id < x->id)
                x = x->left;
            else
                x = x->right;
        }

        z->parent = y;
        if (y == nullNode)
            root = z;
        else if (z->id < y->id)
            y->left = z;
        else
            y->right = z;

        insertFixup(z);
    }

    bool search(int id, Node*& result) {
        Node* x = root;
        while (x != nullNode) {
            if (id == x->id) {
                result = x;
                return true;
            }
            else if (id < x->id)
                x = x->left;
            else
                x = x->right;
        }
        result = nullNode;
        return false;
    }

    void remove(int id) {
        Node* z = root;
        while (z != nullNode) {
            if (id == z->id) break;
            else if (id < z->id) z = z->left;
            else z = z->right;
        }

        if (z == nullNode) {
            cout << "Student with ID " << id << " not found!" << endl;
            return;
        }

        Node* y = z;
        Node* x;
        int yOriginalColor = y->color;

        if (z->left == nullNode) {
            x = z->right;
            replaceNode(z, z->right);
        }
        else if (z->right == nullNode) {
            x = z->left;
            replaceNode(z, z->left);
        }
        else {
            y = z->right;
            while (y->left != nullNode) {
                y = y->left;
            }
            yOriginalColor = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            }
            else {
                replaceNode(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            replaceNode(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (yOriginalColor == BLACK) {
            deleteFixup(x);
        }
    }

    void display() {
        if (root == nullNode) {
            cout << "================Tree is empty===============" << endl;
            return;
        }
        cout << "===============All Students (In-order)============" << endl;
        inorder(root);
        cout << endl;
    }

    ~RBTree() {
        destroyTree(root);
        delete nullNode;
    }
};

int main() {
    RBTree tree;
    int choice, id;
    string name, course;
    double cgpa;

    while (true) {
        cout << "================ Student Record Management System (Red-Black Tree) ====================" << endl;
        cout << "1. Insert Student" << endl;
        cout << "2. Search Student" << endl;
        cout << "3. Delete Student" << endl;
        cout << "4. Display All Students" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter Student ID: ";
            cin >> id;
            cout << "Enter Student Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Course: ";
            getline(cin, course);
            cout << "Enter CGPA: ";
            cin >> cgpa;

            tree.insert(id, name, course, cgpa);
            cout << "Student inserted successfully!" << endl;
            break;

        case 2: {
            cout << "Enter Student ID to search: ";
            cin >> id;
            Node* result;
            if (tree.search(id, result)) {
                cout << "===============Student Found================";
                cout << "ID     : " << result->id << endl;
                cout << "Name   : " << result->name << endl;
                cout << "Course : " << result->course << endl;
                cout << "CGPA   : " << result->cgpa << endl;
            }
            else {
                cout << "Student not found!" << endl;
            }
            break;
        }

        case 3:
            cout << "Enter Student ID to delete: ";
            cin >> id;
            tree.remove(id);
            break;

        case 4:
            tree.display();
            break;

        case 5:
            cout << "Thank you for using the system!" << endl;
            return 0;

        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}