#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <string>

#include "2105001_ScopeTable.hpp"

using namespace std;

class SymbolTable
{
    int num_buckets;
    int total_scopes;
    ScopeTable *current_scope;

public:
    SymbolTable(int n)
    {
        this->num_buckets = n;
        this->total_scopes = 0;
        current_scope = NULL;
    }

    ~SymbolTable()
    {
    }

    void EnterScope()
    {
        this->total_scopes++;
        int id = this->total_scopes;
        this->current_scope = new ScopeTable(id, num_buckets, this->current_scope);
        cout << "\tScopeTable# " << id << " created";
    }

    void ExitScope()
    {
        ScopeTable *temp = current_scope;
        if (temp->parent_scope == NULL)
        {
            return;
        }
        int id = temp->getId();
        current_scope = current_scope->parent_scope;
        delete temp;
        cout << "\tScopeTable# " << id << " removed";
    }

    bool Insert(string name, string type, bool verbose = false)
    {
        bool inserted = this->current_scope->Insert(name, type, verbose);
        if (!inserted)
        {
            if(verbose) cout << "'" << name << "' already exists in the current ScopeTable" << endl;
        }
        return inserted;
    }

    bool Remove(string name, bool verbose = false)
    {
        bool isRemoved = this->current_scope->Delete(name,verbose);
        if (!isRemoved){
            if(verbose) cout<<"Not found in the current ScopeTable"<<endl;
        }
        return isRemoved;
    }

    SymbolInfo* LookUp(string name,bool verbose = false){
        ScopeTable* curr = this->current_scope;
        SymbolInfo* found = NULL;
        while (curr!=NULL){
            found = curr->LookUp(name,verbose);
            if (found!=NULL){
                break;
            }
            curr = curr->parent_scope;
        }
        if (found==NULL){
            if(verbose) cout<<"'"<<name<<"'"<<" not found in any of the ScopeTables"<<endl;
        }
        return found;
    }

    void PrintCurrentScopeTable(){
        this->current_scope->Print();
    }

    void PrintAllScopeTables(){
        ScopeTable* curr = this->current_scope;
        while (curr!=NULL){
            curr->Print();
            curr=curr->parent_scope;
        }
    }
};

#endif