#ifndef __TREE_H__
#define __TREE_H__

#include <vector>
#include <string>
#include <iostream>

namespace Parser {

    class Tree {
    public:
        class Node {
        public:
            Node();
            Node(Node* parent);

            Node* getParent();
            const Node* getParent() const;
            const std::vector<Node*>& getChildren();

            void addChild(Node* child);

            virtual std::string parse() const = 0;
            virtual std::string repr() const;

        protected:
            Node* _parent;
            std::vector<Node*> _children;
        };

        Tree();
        explicit Tree(Node* root);
        std::ostream& operator<<(std::ostream& out);

        Node* getCurrent();
        const Node* getCurrent() const;

        //Tree navigation
        void goToRoot();

        void goToParent();
        void goToChild(std::size_t n);
    
        bool isLeaf();
        bool isRoot();
    
        //TODO make it an iterator
        Node* traverse();

    protected:
        Node* _root;
        Node* _current;
    };

}


#endif