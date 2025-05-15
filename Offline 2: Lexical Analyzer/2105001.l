%option noyywrap
%x CONST_CHAR_STATE
%x STRING_STATE
%x BACKSLASH_CMNT_STATE
%x STAR_CMNT_STATE
%{
    #include <bits/stdc++.h>
    #include "headers/2105001_SymbolTable.hpp"
    using namespace std;

    int line_count = 1;
    int warning_count = 0;
    int error_count = 0;

    string current_const_char = "";
    string current_string = "";
    string current_cmnt_text = "";

    int current_lexeme_line_num = 0;
    int current_string_line_cnt = 0;

    string current_converted_txt = "";

    ofstream logout("2105001_log.txt");
    ofstream tok("2105001_tokens.txt");

    int nest_lvl = 0;

    int num_buckets = 7;
    SymbolTable symbolTable(num_buckets,logout);

    class TokenLexeme {
        public:
            string token;
            string lexeme;

            TokenLexeme(string token, string lexeme) {
                this->token = token;
                this->lexeme = lexeme;
            }
            string to_string(){
                return "<"+this->token+", "+this->lexeme+">";
            }
    };

    void capitalize(string &str) { // Needed for keywords
        for (char &c : str) {
            c = toupper(c);
        }
    }

    // void upperToLower(string &str) { // Needed for keywords
    //     for (char &c : str) {
    //         c = tolower(c);
    //     }
    // }

    char getSpecialCharASCII(string txt){
        char converted;
        char ch = txt[1];
        cout << "Special char: " << ch << endl;
        switch (ch) {
            case 'n': converted = '\n'; break;
            case 't': converted = '\t'; break;
            case 'f': converted = '\f'; break;
            case 'v': converted = '\v'; break;
            case '0': converted = '\0'; break;
            case 'r': converted = '\r'; break;
            case 'a': converted = '\a'; break;
            case 'b': converted = '\b'; break;
            case '\\': converted = '\\'; break;
            case '\'': converted = '\''; break;
            case '\"': converted = '\"'; break;
            default: converted = '#'; break; // Invalid special char
        }
        cout << "Converted special char: " << converted << endl;
        return converted;
    }

    void writeToToken(TokenLexeme t) {
        tok << t.to_string()<<" ";
    }

    void writeToLog(TokenLexeme t,TokenLexeme extra = TokenLexeme("", "")) {
        int line_num = line_count;
        // if (t.token == "MULTI_LINE_STRING" || t.token == "COMMENT"){
        //     line_num = current_lexeme_line_num;
        // }
        if (t.token == "CONST_CHAR" || t.token == "STRING"){
            logout << "Line no "<<line_num<<": Token <" <<t.token<<"> Lexeme "<<t.lexeme<<" found --> "<<extra.to_string()  << "\n\n";
        }
        else {
            logout << "Line no "<<line_num<<": Token <" <<t.token<<"> Lexeme "<<t.lexeme<<" found"  << "\n\n";
        }
    }

    void handleKeyword(string txt){
        string token = txt;
        capitalize(token);
        token = "<"+token+">";
        tok << token<<" ";
        logout << "Line no "<<line_count<<": Token " <<token<<" Lexeme "<<txt<<" found"  << "\n\n";
    }

    void handleToken(TokenLexeme t){
        writeToToken(t);
        writeToLog(t);

        if (t.token == "LCURL"){
            nest_lvl++;
            symbolTable.EnterScope();
        }
        else if (t.token == "RCURL"){
            if(nest_lvl) nest_lvl--;
            symbolTable.ExitScope();
        }
        else if (t.token == "ID" || t.token == "CONST_INT" || t.token == "CONST_FLOAT"){
            bool inserted = symbolTable.Insert(t.lexeme,t.token);
            if (inserted){
                symbolTable.PrintAllScopeTables();
            }
        }
    }

    void handleError(string error,string txt,int line_num = -1){
        if (line_num == -1){
            line_num = line_count;
        }
        error_count++;
        logout<<"Error at line no "<<line_num<<": "<<error<<" "<<txt<<"\n\n";
    }

    void handleCurrentConstChar(){
        int len = current_converted_txt.length();
        if (len == 2){
            handleError("Empty character constant error",current_const_char);
        }
        else if (len > 3){
            handleError("Multi character constant error",current_const_char);
        }
        else if (len == 3){
            char ch = current_converted_txt[1];
            if (ch == '#'){
                handleError("Invalid special character",current_const_char);
            }
            else{
                // handleToken(TokenLexeme("CONST_CHAR",string(1,current_converted_txt[1])));
                TokenLexeme t1("CONST_CHAR",string(1,current_converted_txt[1]));
                writeToToken(t1);
                TokenLexeme t2("CONST_CHAR",current_const_char);
                writeToLog(t2,t1);
                bool inserted = symbolTable.Insert(t2.lexeme,t2.token);
                if (inserted){
                    symbolTable.PrintAllScopeTables();
                }
            }
        }
    }

    void handleCurrentString(){
        string token;
        if (current_string_line_cnt == 1){
            cout << "Single line string: " << current_string << endl;
            token = "STRING";
        }
        else if (current_string_line_cnt > 1){
            cout << "Multi line string: " << current_string << endl;
            token = "STRING";
        }
        TokenLexeme t1(token,current_string);
        TokenLexeme t2(token,current_converted_txt);
        writeToLog(t1,t2);
        writeToToken(t2);
    }

    void handleCmnt(){
        writeToLog(TokenLexeme("COMMENT",current_cmnt_text));
    }
%}

KEYWORD "if"|"for"|"do"|"int"|"float"|"void"|"switch"|"default"|"else"|"while"|"break"|"char"|"double"|"return"|"case"|"continue"|"goto"|"long"|"short"|"static"|"unsigned"
ADDOP "+"|"-"
MULOP "*"|"/"|"%"
INCOP "++"|"--"
RELOP "<"|"<="|">"|">="|"=="|"!="
ASSIGNOP "="
LOGICOP "&&"|"||"
NOT "!"
LPAREN "("
RPAREN ")"
LCURL "{"
RCURL "}"
LTHIRD "["
RTHIRD "]"
COMMA ","
SEMICOLON ";"
WHITESPACE [ \t\v\r\f]+
DIGIT [0-9]
ALPHABET [a-zA-Z]
ALPHANUMERIC {ALPHABET}|{DIGIT}
CONST_INT {DIGIT}+
FRACTION \.{DIGIT}+
SIMPLE_FLOAT {DIGIT}*{FRACTION}
EXPONENT [Ee][+-]?{DIGIT}+
CONST_FLOAT ({SIMPLE_FLOAT}|{CONST_INT}){EXPONENT}?
IDENTIFIER ({ALPHABET}|"_")({ALPHANUMERIC}|"_")*
SINGLE_QUOTE "\'"
DOUBLE_QUOTE "\""
NEWLINE "\n"|"\r\n"
BACKSLASH "\\"
SPECIAL_CHAR "\\n"|"\\t"|"\\f"|"\\v"|"\\0"|"\\r"|"\\a"|"\\b"|"\\\\"|"\\\'"|"\\\""
REDUNDANT_DECIMAL_POINTS {DIGIT}*(\.{DIGIT}*){2,}{EXPONENT}?
ILLFORMED_FLOAT ({SIMPLE_FLOAT}|{CONST_INT})[Ee][+-]?{SIMPLE_FLOAT}
INVALID_IDENTIFIER {CONST_FLOAT}{IDENTIFIER}

%%
{WHITESPACE} {}

{NEWLINE} {
    line_count++;
}

{KEYWORD} {
    handleKeyword(yytext);
}

{ADDOP} {
    handleToken(TokenLexeme("ADDOP",yytext));
}

{MULOP} {
    handleToken(TokenLexeme("MULOP",yytext));
}

{INCOP} {
    handleToken(TokenLexeme("INCOP",yytext));
}

{RELOP} {
    handleToken(TokenLexeme("RELOP",yytext));
}

{ASSIGNOP} {
    handleToken(TokenLexeme("ASSIGNOP",yytext));
}

{LOGICOP} {
    handleToken(TokenLexeme("LOGICOP",yytext));
}

{NOT} {
    handleToken(TokenLexeme("NOT",yytext));
}

{LPAREN} {
    handleToken(TokenLexeme("LPAREN",yytext));
}

{RPAREN} {
    handleToken(TokenLexeme("RPAREN",yytext));
}

{LCURL} {
    handleToken(TokenLexeme("LCURL",yytext));
}

{RCURL} {
    handleToken(TokenLexeme("RCURL",yytext));
}

{LTHIRD} {
    handleToken(TokenLexeme("LTHIRD",yytext));
}

{RTHIRD} {
    handleToken(TokenLexeme("RTHIRD",yytext));
}

{COMMA} {
    handleToken(TokenLexeme("COMMA",yytext));
}

{SEMICOLON} {
    handleToken(TokenLexeme("SEMICOLON",yytext));
}

{CONST_INT} {
    handleToken(TokenLexeme("CONST_INT",yytext));
}

{CONST_FLOAT} {
    handleToken(TokenLexeme("CONST_FLOAT",yytext));
}

{REDUNDANT_DECIMAL_POINTS} {
    handleError("Too many decimal points",yytext);
}

{ILLFORMED_FLOAT} {
    handleError("Ill formed number",yytext);
}

{INVALID_IDENTIFIER} {
    handleError("Invalid prefix on ID or invalid suffix on Number",yytext);
}

{IDENTIFIER} {
    handleToken(TokenLexeme("ID",yytext));
}

{SINGLE_QUOTE} {
    current_const_char = yytext;
    current_converted_txt = yytext;
    BEGIN(CONST_CHAR_STATE);
}

<CONST_CHAR_STATE>{
    {SINGLE_QUOTE} {
        current_const_char += yytext; // For logging errors
        current_converted_txt += yytext; // For token generation
        cout << "Const char: " << current_const_char << endl;
        handleCurrentConstChar();
        BEGIN(INITIAL);
    }
    {SPECIAL_CHAR} {
        cout << "Special char: " << yytext << endl;
        current_converted_txt += getSpecialCharASCII(yytext);
        current_const_char += yytext;
    }
    {NEWLINE} {
        handleError("Unterminated character",current_const_char);
        line_count++;
        BEGIN(INITIAL);
    }
    <<EOF>> {
        handleError("Unterminated character",current_const_char);
        BEGIN(INITIAL);
    }
    (.) {
        current_const_char += yytext;
        current_converted_txt += yytext;
    }
}

{DOUBLE_QUOTE} {
    current_string = yytext;
    current_converted_txt = "";
    current_lexeme_line_num = line_count;
    current_string_line_cnt = 1;
    BEGIN(STRING_STATE);
}

<STRING_STATE>{
    {DOUBLE_QUOTE} {
        current_string += yytext;
        cout << "String at the end: " << current_string << endl;
        cout << "String line count at the end: " << current_string_line_cnt << endl;
        handleCurrentString();
        BEGIN(INITIAL);
    }
    {BACKSLASH}{NEWLINE} {
        cout << "Going for multi line string" << endl;
        current_string += yytext;
        current_string_line_cnt++;
        line_count++;
        cout << "String line count: " << current_string_line_cnt << endl;
    }
    {NEWLINE} {
        handleError("Unterminated string",current_string,current_lexeme_line_num);
        line_count++;
        BEGIN(INITIAL);
    }
    <<EOF>> {
        handleError("Unterminated string",current_string,current_lexeme_line_num);
        BEGIN(INITIAL);
    }
    {SPECIAL_CHAR} {
        cout << "Special char raw: " << yytext << endl;
        current_converted_txt += getSpecialCharASCII(yytext);
        current_string += yytext;
    }
    (.) {
        current_string += yytext;
        current_converted_txt += yytext;
    }
}

"\/\/" {
    current_cmnt_text = yytext;
    current_lexeme_line_num = line_count;
    BEGIN(BACKSLASH_CMNT_STATE);
}

<BACKSLASH_CMNT_STATE>{
    {NEWLINE} {
        cout << "Comment at the end: " << current_cmnt_text << endl;
        handleCmnt();
        line_count++;
        BEGIN(INITIAL);
    }
    <<EOF>> {
        cout << "Comment at the end: " << current_cmnt_text << endl;
        handleCmnt();
        BEGIN(INITIAL);
    }
    {BACKSLASH}{NEWLINE} {
        current_cmnt_text += yytext;
        line_count++;
        /* cout << "Comment line count: " << current_string_line_cnt << endl; */
    }
    (.) {
        current_cmnt_text += yytext;
    }
}

"\/\*" {
    current_cmnt_text = yytext;
    current_lexeme_line_num = line_count;
    BEGIN(STAR_CMNT_STATE);
}

<STAR_CMNT_STATE>{
    {NEWLINE} {
        current_cmnt_text += yytext;
        line_count++;
    }
    <<EOF>> {
        handleError("Unterminated comment",current_cmnt_text,current_lexeme_line_num);
        BEGIN(INITIAL);
    }
    "\*\/" {
        current_cmnt_text += yytext;
        cout << "Comment at the end: " << current_cmnt_text << endl;
        handleCmnt();
        BEGIN(INITIAL);
    }
    (.) {
        current_cmnt_text += yytext;
    }
}

(.) {
    handleError("Unrecognized character",yytext);
}

%%

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    string filename = argv[1];
    FILE *fin = fopen(filename.c_str(), "r");
    if (!fin) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }
    yyin = fin; // Set the input file for the lexer
    yylex(); // Start the lexer
    symbolTable.PrintAllScopeTables();
    logout << "Total lines: "<<line_count<<endl;
    logout << "Total errors: "<<error_count<<endl;
    fclose(yyin);
    logout.close();
    tok.close();
    return 0;
}