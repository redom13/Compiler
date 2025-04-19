#include <bits/stdc++.h>

#include "2105001_SymbolTable.hpp"

using namespace std;

vector<string> split(const string &s)
{
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (ss >> token)
    {
        // cout<<token<<endl;
        tokens.push_back(token);
    }
    // cout<<"Split successfully"<<endl;
    return tokens;
}

int main(int argc, char *argv[]) // Modified main to accept command-line arguments
{
    string input_filename;
    string output_filename;

    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_filename> <output_filename>" << endl;
        return 1;
    } else {
        input_filename = argv[1];
        output_filename = argv[2];
    }

    int num_buckets;

    if (!freopen(input_filename.c_str(), "r", stdin))
    {
        cerr << "Error: Could not open input file: " << input_filename << endl;
        return 1;
    }
    if (!freopen(output_filename.c_str(), "w", stdout))
    {
        cerr << "Error: Could not open output file: " << output_filename << endl;
        return 1;
    }

    if (!(cin >> num_buckets))
    {
        cerr << "Error: Invalid input for number of buckets." << endl;
        return 1;
    }
    if (num_buckets <= 0)
    {
        cerr << "Error: Number of buckets must be greater than 0." << endl;
        return 1;
    }
    cin.ignore();

    SymbolTable *symbolTable = new SymbolTable(num_buckets);
    int cmd_count = 0;
    string command;

    while (getline(cin, command))
    {
        if (command == "")
        {
            continue;
        }
        cmd_count++;
        cout << "Cmd " << cmd_count << ": " << command << endl;

        vector<string> tokens = split(command);
        string op = tokens[0];
        int cmd_len = tokens.size();

        if (op == "I")
        {
            if (cmd_len < 3)
            {
                cout << "\tNumber of parameters mismatch for the command " << op << endl;
                continue;
            }
            string name = tokens[1];
            string type;
            // cout<<"Name: "<<name<<endl;
            if (tokens[2] == "FUNCTION")
            {
                if (cmd_len < 4)
                {
                    cout << "Invalid Insert Format for function" << endl;
                }
                else
                {
                    type = tokens[2] + "," + tokens[3] + "<==(";
                    int arg_len = cmd_len - 4;
                    for (int i = 0; i < arg_len; i++)
                    {
                        if (i != arg_len - 1)
                        {
                            type += (tokens[4 + i] + ",");
                        }
                        else
                            type += tokens[4 + i];
                    }
                    type += ")";
                }
            }
            else if (tokens[2] == "UNION" || tokens[2] == "STRUCT")
            {
                if ((cmd_len - 1) % 2 != 0 && cmd_len < 5)
                {
                    cout << "Invalid Insert Format for " << tokens[2] << endl;
                    continue;
                }
                else
                {
                    type = tokens[2] + ",{";
                    int arg_len = (cmd_len - 3) / 2;
                    for (int i = 0; i < arg_len; i++)
                    {
                        if (i != arg_len - 1)
                        {
                            type += "(" + tokens[3 + 2*i] + "," + tokens[4 + 2*i] + "),";
                        }
                        else
                        {
                            type += "(" + tokens[3 + 2*i] + "," + tokens[4 + 2*i] + ")}";
                        }
                    }
                }
            }
            else
            {
                type = tokens[2];
            }
            // cout<<"Type: "<<type<<endl;
            symbolTable->Insert(name, type, true);
        }
        else if (op == "L")
        {
            if (cmd_len != 2)
            {
                cout << "\tNumber of parameters mismatch for the command " << op << endl;
                continue;
            }
            string name = tokens[1];
            symbolTable->LookUp(name, true);
        }
        else if (op == "D")
        {
            if (cmd_len != 2)
            {
                cout << "\tNumber of parameters mismatch for the command " << op << endl;
                continue;
            }
            string name = tokens[1];
            symbolTable->Remove(name, true);
        }
        else if (op == "S")
        {
            if (cmd_len != 1)
            {
                cout << "\tNumber of parameters mismatch for the command " << op << endl;
                continue;
            }
            symbolTable->EnterScope(true);
        }
        else if (op == "E")
        {
            if (cmd_len != 1)
            {
                cout << "\tNumber of parameters mismatch for the command " << op << endl;
                continue;
            }
            symbolTable->ExitScope();
        }
        else if (op == "P")
        {
            if (cmd_len != 2)
            {
                cout << "\tNumber of parameters mismatch for the command " << op << endl;
                continue;
            }
            if (tokens[1] == "C")
            {
                symbolTable->PrintCurrentScopeTable();
            }
            else if (tokens[1] == "A")
            {
                symbolTable->PrintAllScopeTables();
            }
            else
            {
                cout << "Invalid Print Format" << endl;
                continue;
            }
        }
        else if (op == "Q")
        {
            if (cmd_len != 1)
            {
                cout << "\tNumber of parameters mismatch for the command " << op << endl;
                continue;
            }
            delete symbolTable;
            // cout << "Exiting..." << endl;
            break;
        }
        else
        {
            cout << "Invalid Command" << endl;
            continue;
        }
    }
}