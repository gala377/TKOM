//
// Created by igor on 21.04.18.
//

#include "Parser/Scope.h"

using namespace Parser;


Scope::Scope(Scope* parent): _parent(parent) {}


Scope& Scope::newSubScope() {
    _children.emplace_back(Scope(this));
    return _children.back();
}

void Scope::addIdentifier(Identifier id) {
    _defined_ids.push_back(id);
}


const Identifier& Scope::find(std::string symbol) const {
    for(const auto& id: _defined_ids) {
        if(id.symbol == symbol) {
            return id;
        }
    }
    if(_parent) {
        return _parent->find(symbol);
    }
    throw std::runtime_error("Symbol " + symbol + " is not define in scope");
}

bool Scope::isDefined(std::string symbol) const {
    for(auto id: _defined_ids) {
        if(id.symbol == symbol) {
            return true;
        }
    }
    if(_parent) {
        return _parent->isDefined(symbol);
    }
    return false;
}
