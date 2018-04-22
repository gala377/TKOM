#ifndef __TREE_H__
#define __TREE_H__

#include <vector>
#include <string>
#include <iostream>
#include <memory>

namespace Parser {

    class Tree {
    public:
        class Node {
        public:
            Node();
            Node(Node* parent);

            Node* getParent() const;
            const std::vector<std::shared_ptr<Node>>& getChildren();

            void addChild(Node* child);
            void addChild(std::shared_ptr<Node> child);

            virtual std::string parse() const = 0;
            virtual std::string repr() const;

            std::string strWithIntend(std::size_t intend) const;
        protected:
            Node* _parent;
            std::vector<std::shared_ptr<Node>> _children;
        };

        Tree();
        explicit Tree(Node* root);

        friend std::ostream& operator<<(std::ostream& out, Tree& tree) {
            std::cout << "Writing tree\n\n\n";
            out << tree._root->strWithIntend(0);
            std::cout << "\n\n\n";
            return out;
        }

        Node* getCurrent() const;

        //Tree navigation
        void goToRoot();

        void goToParent();
        void goToChild(std::size_t n);
    
        bool isLeaf();
        bool isRoot();
    
        //TODO make it an iterator
        std::shared_ptr<Node> traverse();

    protected:
        std::unique_ptr<Node> _root;
        Node* _current;

    };
}


#endif