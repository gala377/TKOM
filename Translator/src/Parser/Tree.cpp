//
// Created by igor on 10.04.18.
//

#include <stdexcept>
#include <algorithm>
#include "Parser/Tree.h"

using namespace Parser;

//
// Node implementation
//

Tree::Node::Node(): _parent(nullptr) {}

Tree::Node::Node(Node* parent): _parent(parent) {}


const std::shared_ptr<Tree::Node> Tree::Node::getParent() const {
    return std::shared_ptr<Node>(_parent.get());
}

const std::vector<std::shared_ptr<Tree::Node>> & Tree::Node::getChildren() {
    return _children;
}


void Tree::Node::addChild(Node* child) {
    _children.emplace_back(std::shared_ptr<Node>(child));
}

void Tree::Node::addChild(std::shared_ptr<Tree::Node> child) {
    _children.push_back(child);
}


std::string Tree::Node::repr() const {
    return "Empty Node";
}

std::string Tree::Node::strWithIntend(const std::size_t intend) const {
    std::string res;
    for(std::size_t i = 0; i < intend; ++i){
        res += "----";
    }
    res += repr();
    std::for_each(_children.begin(), _children.end(),
        [&res, intend](auto &arg) {
            res += arg->strWithIntend(intend+1);
    });
    return res;
}

//
// Tree implementation
//

Tree::Tree() : _root(nullptr), _current(nullptr) {
}

Tree::Tree(Parser::Tree::Node *root): _root(root), _current(root) {}


const std::shared_ptr<Tree::Node> Tree::getCurrent() const {
    return _current;
}


void Tree::goToRoot() {
    _current = _root;
}

void Tree::goToParent() {
    if(_current->getParent() == nullptr) {
        throw std::runtime_error("Cannot traverse to the top");
    }
    _current = _current->getParent();
}

void Tree::goToChild(std::size_t n) {
    _current = _current->getChildren()[n];
}

bool Tree::isRoot() {
    return _current == _root;
}

bool Tree::isLeaf() {
    return _current->getChildren().empty();
}