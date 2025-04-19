#ifndef SCOPE_TABLE_H
#define SCOPE_TABLE_H

#include <iostream>
#include <string>

#include "2105001_Hash.hpp"
#include "2105001_SymbolInfo.hpp"

using namespace std;

class ScopeTable
{
    int id;
    int num_buckets;
    SymbolInfo **hashTable;

    void printTabs(int n)
    {
        for (int i = 0; i < n; i++)
        {
            cout << "\t";
        }
    }

public:
    ScopeTable *parent_scope;

    ScopeTable(int id, int n, ScopeTable *parent_scope = NULL)
    {
        this->id = id;
        this->num_buckets = n;
        this->hashTable = new SymbolInfo *[num_buckets];
        this->parent_scope = parent_scope;

        for (int i = 0; i < num_buckets; i++)
        {
            this->hashTable[i] = NULL;
        }
    }

    ~ScopeTable()
    {
        for (int i = 0; i < num_buckets; i++)
        {
            SymbolInfo *curr = this->hashTable[i];
            SymbolInfo *temp;
            while (curr != NULL)
            {
                temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
        delete[] this->hashTable;
    }

    int getIndex(string key)
    {
        return Hash::sdbm_hash(key) % num_buckets;
    }

    int getId()
    {
        return this->id;
    }

    int getPositionInChain(string name)
    {
        int pos = 0;
        int index = this->getIndex(name);
        SymbolInfo *curr = this->hashTable[index];
        while (curr != NULL)
        {
            pos++;
            if (curr->getName() == name)
            {
                return pos;
            }
            curr = curr->next;
        }
        return -1;
    }

    SymbolInfo *LookUp(string name, bool verbose = false)
    {
        int index = getIndex(name);
        SymbolInfo *curr = this->hashTable[index];
        int pos = 0;
        while (curr != NULL)
        {
            if (curr->getName() == name)
            {
                if (verbose)
                    cout << "\t'" << name << "'" << " found in ScopeTable# " << this->id << " at position " << index + 1 << ", " << pos + 1 << endl;
                return curr;
            }
            curr = curr->next;
            pos++;
        }
        return NULL;
    }

    bool Insert(string name, string type, bool verbose = false)
    {
        // Checking for duplicates
        SymbolInfo *prev = LookUp(name);
        if (prev == NULL)
        { // No duplicates
            // Appending at the beginning
            int index = getIndex(name);
            SymbolInfo *curr = this->hashTable[index];
            SymbolInfo *symbol = new SymbolInfo(name, type, NULL);
            int pos = 0;
            if (curr == NULL)
            {
                this->hashTable[index] = symbol;
            }
            else
            {
                pos = 1;
                while (curr->next != NULL)
                {
                    curr = curr->next;
                    pos++;
                }
                curr->next = symbol;
            }
            if (verbose)
                cout << "\tInserted in ScopeTable# " << this->id << " at position " << index + 1 << ", " << pos + 1 << endl;
            return true;
        }
        else
        { // Duplicate
            return false;
        }
    }

    bool Delete(string name, bool verbose = false)
    {

        SymbolInfo *symbol = LookUp(name);

        if (symbol != NULL)
        { // symhol exists
            int index = getIndex(name);
            SymbolInfo *curr = this->hashTable[index];
            int pos = 0;

            if (curr == symbol)
            {
                this->hashTable[index] = curr->next;
                delete symbol;
            }
            else
            {
                pos = 1;
                while (curr != NULL)
                {
                    if (curr->next == symbol)
                    {
                        curr->next = symbol->next;
                        delete symbol;
                        break;
                    }
                    curr = curr->next;
                    pos++;
                }
            }

            if (verbose)
                cout << "\tDeleted '" << name << "'" << " from ScopeTable# " << this->id << " at position " << index + 1 << ", " << pos + 1 << endl;

            return true;
        }
        else
        { // symbol not found
            return false;
        }
    }

    void Print(int tabs=1)
    {
        printTabs(tabs);
        cout << "ScopeTable# " << this->id << endl;
        for (int i = 0; i < num_buckets; i++)
        {
            printTabs(tabs);
            cout << i + 1 << "-->";
            SymbolInfo *curr = this->hashTable[i];
            while (curr != NULL)
            {
                cout << " <" << curr->getName() << "," << curr->getType() << ">";
                curr = curr->next;
            }
            cout << endl;
        }
    }
};

#endif