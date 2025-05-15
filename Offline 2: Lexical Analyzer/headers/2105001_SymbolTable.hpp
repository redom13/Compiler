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
    int total_collisions;
    ostream &out;
    ScopeTable *current_scope;
    // uint64_t (*hashFunction)(string, uint64_t);

public:
    SymbolTable(int n, ostream& out = cout, bool verbose = true)
        : out(out)
    {
        // this->hashFunction = hashFunction;
        this->num_buckets = n;
        this->total_scopes = 0;
        this->total_collisions = 0;
        current_scope = new ScopeTable("1", num_buckets, out);
        // EnterScope();
    }

    ~SymbolTable()
    {
        while (this->current_scope != NULL)
        {
            ExitScope(true);
        }
    }

    // void EnterScope(bool verbose = false)
    // {
    //     this->total_scopes++;
    //     int id = this->total_scopes;
    //     this->current_scope = new ScopeTable(id, num_buckets,out, this->current_scope, this->hashFunction);
    //     if (verbose)
    //         out << "\tScopeTable# " << id << " created" << endl;
    // }

    void EnterScope(bool verbose = false) {
        string parent_id = current_scope->getId();
        int child_count = 1;

        // Count existing child scopes to determine the new scope's ID
        ScopeTable *temp = current_scope;
        while (temp != NULL) {
            string temp_id = temp->getId();
            if (temp_id.find(parent_id + ".") == 0) { // Check if it's a child of the current scope
                int last_dot = temp_id.find_last_of('.');
                int child_num = stoi(temp_id.substr(last_dot + 1));
                child_count = max(child_count, child_num + 1);
            }
            temp = temp->parent_scope;
        }

        // Create the new scope with the hierarchical ID
        string new_id = parent_id + "." + to_string(child_count);
        ScopeTable *new_scope = new ScopeTable(new_id, num_buckets, out, current_scope);
        current_scope = new_scope;

        // out << "Entered new scope with ID: " << new_id << endl;
    }

    void ExitScope(bool exitRoot = false)
    {
        ScopeTable *temp = current_scope;
        if (temp->parent_scope == NULL && !exitRoot)
        {
            return;
        }
        // int id = temp->getId();
        current_scope = current_scope->parent_scope;
        // total_collisions += temp->getCollisions();
        delete temp;
        //out << "\tScopeTable# " << id << " removed" << endl;
    }

    bool Insert(string name, string type, bool verbose = false)
    {
        int prev_collisions = this->current_scope->getCollisions();
        bool inserted = this->current_scope->Insert(name, type, verbose);
        if (!inserted)
        {
            if (verbose)
                out << "\t'" << name << "' already exists in the current ScopeTable" << endl;
        }
        if (prev_collisions != this->current_scope->getCollisions())
        {
            this->total_collisions++;
        }
        return inserted;
    }

    bool Remove(string name, bool verbose = false)
    {
        bool isRemoved = this->current_scope->Delete(name, verbose);
        if (!isRemoved)
        {
            if (verbose)
                out << "\tNot found in the current ScopeTable" << endl;
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
                out << "\t'" << name << "'" << " not found in any of the ScopeTables" << endl;
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
        out << endl;
    }

    float getMeanCollisions()
    {
        return (float)this->total_collisions / (float)this->total_scopes;
    }
};

#endif