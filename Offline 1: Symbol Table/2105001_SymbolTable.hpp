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
        EnterScope(true);
    }

    ~SymbolTable()
    {
        while (this->current_scope != NULL)
        {
            ExitScope(true);
        }
    }

    void EnterScope(bool verbose = false)
    {
        this->total_scopes++;
        int id = this->total_scopes;
        this->current_scope = new ScopeTable(id, num_buckets, this->current_scope);
        if (verbose)
            cout << "\tScopeTable# " << id << " created" << endl;
    }

    void ExitScope(bool exitRoot = false)
    {
        ScopeTable *temp = current_scope;
        if (temp->parent_scope == NULL && !exitRoot)
        {
            return;
        }
        int id = temp->getId();
        current_scope = current_scope->parent_scope;
        delete temp;
        cout << "\tScopeTable# " << id << " removed" << endl;
    }

    bool Insert(string name, string type, bool verbose = false)
    {
        bool inserted = this->current_scope->Insert(name, type, verbose);
        if (!inserted)
        {
            if (verbose)
                cout << "\t'" << name << "' already exists in the current ScopeTable" << endl;
        }
        return inserted;
    }

    bool Remove(string name, bool verbose = false)
    {
        bool isRemoved = this->current_scope->Delete(name, verbose);
        if (!isRemoved)
        {
            if (verbose)
                cout << "\tNot found in the current ScopeTable" << endl;
        }
        return isRemoved;
    }

    SymbolInfo *LookUp(string name, bool verbose = false)
    {
        ScopeTable *curr = this->current_scope;
        SymbolInfo *found = NULL;
        while (curr != NULL)
        {
            found = curr->LookUp(name, verbose);
            if (found != NULL)
            {
                break;
            }
            curr = curr->parent_scope;
        }
        if (found == NULL)
        {
            if (verbose)
                cout << "\t'" << name << "'" << " not found in any of the ScopeTables" << endl;
        }
        return found;
    }

    void PrintCurrentScopeTable(int tabs = 1)
    {
        this->current_scope->Print(tabs);
    }

    void PrintAllScopeTables()
    {
        ScopeTable *curr = this->current_scope;
        int tabs = 1;
        while (curr != NULL)
        {
            curr->Print(tabs);
            curr = curr->parent_scope;
            tabs++;
        }
    }
};

#endif