#ifndef __TREE_H__
#define __TREE_H__

#include <vector>

namespace Parser {

    class Tree {
    public:
        class Node {
            public:

            Node* getParent();    
            std::vector<Node *> getChildren();
            void addChild(Node* child);

            private:
            Node* _parent;
            std::vector<Node*> _children;
        };

    Tree();
    Tree(Node* root);
    
    Node* getCurrent();
    
    //Tree navigation
    void goToParent();
    void goToChild(int n);
    
    bool isLeaf();
    bool isRoot();
    
    //TODO make it an iterator
    Node* traverse();

    private:
    Node* _root;
    Node* _current;
    };

}


#endif