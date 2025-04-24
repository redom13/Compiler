#include <bits/stdc++.h>

#include "2105001_SymbolTable.hpp"
#include "2105001_Hash.hpp"

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

int main() // Modified main to accept command-line arguments
{
    string input_filename = "input2.txt";
    string output_filename = "report.txt";
    uint64_t (*hashFunctions[])(string, uint64_t) = {
        Hash::sdbm_hash,
        Hash::fnv1a_hash,
        Hash::jenkins_hash};

    string hashFunctionNames[] = {
        "sdbm_hash",
        "fnv1a_hash",
        "jenkins_hash"};


    // if (!freopen(output_filename.c_str(), "w", stdout))
    // {
    //     cerr << "Error: Could not open output file: " << output_filename << endl;
    //     return 1;
    // }
    ofstream reportFile(output_filename);
    if (!reportFile.is_open()) {
        cerr << "Error: Could not open output file: " << output_filename << endl;
        return 1;
    }

    reportFile << "Hash Function\t\t\t\tMean Collisions" << endl;
    reportFile << "-------------------------------------------------" << endl;
    for (int i = 0; i < 3; i++)
    {
        ifstream inputFile(input_filename);
        if (!inputFile.is_open()) {
            // Print error to the redirected stdout (report file)
            cout << "Error: Could not open input file: " << input_filename << " for " << hashFunctionNames[i] << endl;
            cerr << "Error: Could not open input file: " << input_filename << " for " << hashFunctionNames[i] << endl; // Also to stderr
            continue; // Skip to the next hash function
        }

        int num_buckets;
        // Read number of buckets from the input file stream
        if (!(inputFile >> num_buckets))
        {
            cout << "Error: Invalid or missing input for number of buckets in " << input_filename << endl;
            cerr << "Error: Invalid or missing input for number of buckets in " << input_filename << endl; // Also to stderr
            inputFile.close(); // Close the file before continuing/exiting
            continue; // Skip to the next hash function
        }
        if (num_buckets <= 0)
        {
            cout << "Error: Number of buckets must be greater than 0. Found: " << num_buckets << endl;
            cerr << "Error: Number of buckets must be greater than 0. Found: " << num_buckets << endl; // Also to stderr
            inputFile.close();
            continue; // Skip to the next hash function
        }
        // Consume the rest of the line after reading the number
        inputFile.ignore();
        SymbolTable *symbolTable = new SymbolTable(num_buckets, hashFunctions[i]);
        int cmd_count = 0;
        string command;
        while (getline(inputFile, command))
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
                                type += "(" + tokens[3 + 2 * i] + "," + tokens[4 + 2 * i] + "),";
                            }
                            else
                            {
                                type += "(" + tokens[3 + 2 * i] + "," + tokens[4 + 2 * i] + ")}";
                            }
                        }
                    }
                }
                else
                {
                    type = tokens[2];
                }
                // cout<<"Type: "<<type<<endl;
                symbolTable->Insert(name, type,true);
            }
            else if (op == "L")
            {
                if (cmd_len != 2)
                {
                    cout << "\tNumber of parameters mismatch for the command " << op << endl;
                    continue;
                }
                string name = tokens[1];
                symbolTable->LookUp(name);
            }
            else if (op == "D")
            {
                if (cmd_len != 2)
                {
                    cout << "\tNumber of parameters mismatch for the command " << op << endl;
                    continue;
                }
                string name = tokens[1];
                symbolTable->Remove(name);
            }
            else if (op == "S")
            {
                if (cmd_len != 1)
                {
                    cout << "\tNumber of parameters mismatch for the command " << op << endl;
                    continue;
                }
                symbolTable->EnterScope(hashFunctions[i]);
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
                reportFile << hashFunctionNames[i] <<"\t\t\t\t" << symbolTable->getMeanCollisions() << endl;
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
        inputFile.close();
    }
    reportFile << "-------------------------------------------------" << endl;
    reportFile.close();
}