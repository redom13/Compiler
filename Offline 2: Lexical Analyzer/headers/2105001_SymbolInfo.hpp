#ifndef SYMBOLINFO_H
#define SYMBOLINFO_H

#include <iostream>
#include <string>

using namespace std;

class SymbolInfo {
    string name, type;
    
public:
    SymbolInfo* next;
    SymbolInfo(string name,string type,SymbolInfo* next=NULL)  {
        this->name = name;
        this->type = type;
        this->next = next;
    }

    string getName() {
        return name;
    }

    string getType() {
        return type;
    }

    void setName(string name) {
        this->name = name;
    }

    void setType(string type) {
        this->type = type;
    }

    void printSymbolInfo(){
        cout << "Symbol Info:" << endl;
        cout << "Name: " << name << endl;
        cout << "Type: " << type << endl;
        cout << endl;
    }

};

#endif
