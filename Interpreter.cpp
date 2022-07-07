#include "Interpreter.h"
#include <iostream>
#include <sstream>
#include <fstream>

////////// Num_Variable Struct //////////
Num_Varible::Num_Varible()
{}

Num_Varible::Num_Varible(std::string intVarName)
{
    mVarName = intVarName;
}

Num_Varible::~Num_Varible()
{}

////////// String_Variable Struct //////////
String_Variable::String_Variable()
{}

String_Variable::String_Variable(std::string strVarName)
{
    mVarName = strVarName;
}

String_Variable::~String_Variable()
{}
////////// Interpreter Class //////////
Interpreter::Interpreter()
{}

bool Interpreter::IsVariable(std::string variable)
{
    //see if a string or number variable of that name exists first before executing the rest of the line.
    for (int i=0; i<stored_STRING_varibles.size(); i++){
        if (stored_STRING_varibles[i].mVarName == variable)
            return true;
    }
    for (int i=0; i<stored_NUM_varibles.size(); i++){
        if (stored_NUM_varibles[i].mVarName == variable)
            return true;
    }
    
    return false;
}

std::string Interpreter::VarType(std::string variable)
{
    for (int i=0; i<stored_STRING_varibles.size(); i++){
        if (stored_STRING_varibles[i].mVarName == variable)
            return "STRING";
    }
    for (int i=0; i<stored_NUM_varibles.size(); i++){
        if (stored_NUM_varibles[i].mVarName == variable)
            return "NUMBER";
    }
}

bool Interpreter::CreateStrVariable(std::string strVarName)
{
    //error check to see if variable name already exists, else create variable with said name.
    for (int i=0; i<stored_STRING_varibles.size(); i++){
        if (stored_STRING_varibles[i].mVarName == strVarName){
            std::cout << "Error: Attempted multiple declaration of same variable.";
            return false;
        }
    }
    for (int i=0; i<stored_NUM_varibles.size(); i++){
        if(stored_NUM_varibles[i].mVarName == strVarName){
            std::cout << "Error: Attempted multiple declaration of same variable.";
            return false;            
        }
    }

    String_Variable str_var;
    str_var.mVarName = strVarName;
    stored_STRING_varibles.push_back(str_var);

    return true;
}

void Interpreter::SetStrVariableValue(std::string variable, std::string string)
{
    for (int i=0; i<stored_STRING_varibles.size(); i++){
        if (stored_STRING_varibles[i].mVarName == variable){    //check to see if the variable exists.
            stored_STRING_varibles[i].mVarString = string;      //set it equal to what the code-line set it equal to, i.e. x = "hello", set x to hello.
        }
    }
}

bool Interpreter::CreateNumVariable(std::string intVarName)
{
    //error check to see if variable name already exists, else create variable with said name.
    for (int i=0; i<stored_NUM_varibles.size(); i++){
        if (stored_NUM_varibles[i].mVarName == intVarName){
            std::cout << "Error: Attempted multiple declaration of same variable.";
            return false;
        }
    }
    for (int i=0; i<stored_STRING_varibles.size(); i++){
        if (stored_STRING_varibles[i].mVarName == intVarName){
            std::cout << "Error: Attempted multiple declaration of same variable.";
            return false;
        }
    }

    Num_Varible num_var;
    num_var.mVarName = intVarName;
    stored_NUM_varibles.push_back(num_var);

    return true;
}

bool Interpreter::IsNumber(std::string token)
{
    for (int i=0; i<token.size(); i++){
        if(isdigit(token[i]) == true)
            continue;
        else
            return false;
    }
    return true;
}

void Interpreter::SetNumVariableValue(std::vector<std::string> tokens)
{
    std::vector<int> numbers;
    std::vector<std::string> mathOperators;

    for (int i=2; i<tokens.size(); i++){    //start after {=}
        if(IsNumber(tokens[i]) == true){
            numbers.push_back(std::stoll(tokens[i]));   //push back long long int values instead of strings of the numbers
        }
        else if(IsVariable(tokens[i]) == true){
            for (int k=0; k<stored_NUM_varibles.size(); k++){   //find the variable
                if(stored_NUM_varibles[k].mVarName == tokens[i])
                    numbers.push_back(stored_NUM_varibles[k].mVarNum);  //push in the value the variable holds into numbers vector
            }
        }
        else if(tokens[i] == "+" || tokens[i] == "-" || tokens[i] == "*"){
            mathOperators.push_back(tokens[i]);
        }
    }
    //should now have a vector of numbers and symbols
    
    for (int i=0; i<mathOperators.size(); i++){ 
        if(mathOperators[i] == "*"){
            numbers[i] = numbers[i]*numbers[i+1];
            numbers.erase(numbers.begin()+i+1);
            mathOperators.erase(mathOperators.begin()+i);
            i=-1;
        }

        else if(mathOperators[i] == "+"){
            if (mathOperators.size() > 1 && mathOperators[i+1] == "*"){ //if there's a '*' next, do not compute
                continue;
            }
            else{
                numbers[i] = numbers[i]+numbers[i+1];
                numbers.erase(numbers.begin()+i+1);
                mathOperators.erase(mathOperators.begin()+i);
                i=-1;
            }
        }
        else if(mathOperators[i] == "-"){
            if (mathOperators.size() > 1 && mathOperators[i+1] == "*"){ //if there's a '*' next, do not compute
                continue;
            }
            else{
                numbers[i] = numbers[i]-numbers[i+1];
                numbers.erase(numbers.begin()+i+1);
                mathOperators.erase(mathOperators.begin()+i);
                i=-1;
            }      
        }
    }


    for (int i=0; i<stored_NUM_varibles.size(); i++){
        if(stored_NUM_varibles[i].mVarName == tokens[0]){
            stored_NUM_varibles[i].mVarNum = numbers[0];
        }
    }
}

void Interpreter::PrintQuote(std::string string)
{
    for (int i=0; i<string.size(); i++){
        if(string[i] == '"'){
            continue;
        }
        else if(string[i] == '\\' && string[i+1] == 'n'){
            std::cout << std::endl;
            i++;
        }
        else if(string[i] == '\\' && string[i+1] == 't'){
            std::cout << '\t';
            i++;
        }
        else{
            std::cout << string[i];
        }
    }
}

void Interpreter::PrintVariable(std::string variable)
{
    for (int i=0; i<stored_NUM_varibles.size(); i++){
        if (stored_NUM_varibles[i].mVarName == variable)
            std::cout << stored_NUM_varibles[i].mVarNum;
    }
    for (int j=0; j<stored_STRING_varibles.size(); j++){
        if (stored_STRING_varibles[j].mVarName == variable)
            PrintQuote(stored_STRING_varibles[j].mVarString);
    }
}

void Interpreter::PrintNumber(std::string strNumber)
{
    std::cout << std::stoll(strNumber);
}

bool Interpreter::Process(std::vector<std::string> token_commands)
{
    //return TRUE to continue code execute, return FALSE to stop and show line error!

    if(token_commands[0] == "INVALID QUOTES"){  //"INVALID QUOTES" is returned by parser by checking if a code-line has 1 or >2 quotes in it
        std::cout << "Error: Quote count is either 1 or greater than 2. Only 2 quotes count as valid input.";
        return false;
    }

    else if(token_commands[0] == " "){  //blank line spaces return a vector of { }.
        return true;    //valid command, but do nothing
    }
    
    //============================================================ STRING ============================================================//
    else if (token_commands[0] == "STRING"){
        if(token_commands.size() != 2 || (isalpha(token_commands[1][0]) == false && token_commands[1][0] != '_') ){  //make sure there's a valid name token (doesnt start with a number) after STRING and nothing after variable name.
            std::cout << "Error: Used 'STRING' with no follow up variable name |OR| extra things after variable name was added |OR| used a number to start a variable name.";
            return false;
        }
        else{
            return CreateStrVariable(token_commands[1]);    //can return false if string variable already exists
        }
    }

    //============================================================ NUMBER ============================================================//
    else if(token_commands[0] == "NUMBER"){
        if(token_commands.size() != 2 || (isalpha(token_commands[1][0]) == false && token_commands[1][0] != '_') ){  //make sure there's a valid name token (doesnt start with a number) after NUMBER and nothing after variable name.
            std::cout << "Error: Used 'NUMBER' with no follow up variable name |OR| extra things after variable name was added |OR| used a number to start a variable name.";
            return false;
        }
        else{
            return CreateNumVariable(token_commands[1]);    //can return false if numeric variable already exists
        }
    }

    //============================================================ PRINT ============================================================//
    else if (token_commands[0] == "PRINT"){
        if(token_commands.size() != 2){
            std::cout << "Error: Used 'PRINT' alone |OR| tried to print more than one thing.";
            return false;
        }
        else if (token_commands[1][0] == '\"'){  //PRINT "Hello world"
            PrintQuote(token_commands[1]);
            return true;
        }
        else if (isdigit(token_commands[1][0]) == true){    //PRINT 100
            for (int i=0; i<token_commands[1].size(); i++){
                if(isdigit(token_commands[1][0]) == true)   //make sure if token[1][0] is digit, everything must be a digit! i.e. a number/integer
                    continue;
                else{
                    std::cout << "Error: Attempted to print an invalid token: '" << token_commands[1] << "'.";
                    return false;
                }
            }
            PrintNumber(token_commands[1]);
            return true;
        }
        else if (IsVariable(token_commands[1]) == true){  //PRINT var123
            PrintVariable(token_commands[1]);
            return true;
        }
        else{
            std::cout << "Error: Attempted to print undefined variable.";
            return false;
        }
    }

    //============================================================ ARITHMETIC/REDEFINING ============================================================//
    else if (IsVariable(token_commands[0]) == true){
        if(token_commands.size() < 3 || token_commands[1] != "="){
            std::cout << "Error: Token count too small to do any valid syntax |OR| Attempted a line with just a variable name |OR| '=' sign is not after variable name";
            return false;
        }

        else if (token_commands[1] == "="){
            if(VarType(token_commands[0]) == "STRING"){
                if (token_commands[2][0] == '\"'){
                    SetStrVariableValue(token_commands[0], token_commands[2]);
                    return true;
                }
                else if(IsVariable(token_commands[2]) == true && VarType(token_commands[2]) == "STRING"){
                    for (int i=0; i<stored_STRING_varibles.size(); i++){
                        if (stored_STRING_varibles[i].mVarName == token_commands[2]){
                            SetStrVariableValue(token_commands[0], stored_STRING_varibles[i].mVarString);
                            return true;
                        }
                    }
                }
                else{
                    std::cout << "Error: Attempted to assign a STRING type variable a non-string value.";
                    return false;
                }
            }
            else if(VarType(token_commands[0]) == "NUMBER"){
                if (IsVariable(token_commands[token_commands.size()-1]) == false && IsNumber(token_commands[token_commands.size()-1]) == false){
                    std::cout << "Error: Left an operator sign alone without a number on both sides.";
                    return false;
                }
                for (int i=2; i<token_commands.size(); i=i+2){    //start after '=', skip by 2
                    if (IsVariable(token_commands[i]) == true && VarType(token_commands[i]) == "NUMBER")    //see if its a NUMBER variable, continue
                        continue;
                    else if (IsNumber(token_commands[i]) == true)
                        continue;
                    else{
                        std::cout << "Error: Tried to set a NUMBER type variable to a new value of non-numbers. Note: No parenthesis allowed in syntax.";
                        return false;
                    }
                }
                for (int i=3; i<token_commands.size(); i = i+2){    //start after the first num or variable is declared after '=' sign, skip by 2
                    if (token_commands[i] == "+" || token_commands[i] == "-" || token_commands[i] == "*")
                        continue;
                    else{
                        std::cout << "Error: Did not contain strictly 1 '+', '-', or '*' operation sign between numbers/variables.";
                        return false;
                    }
                }
                SetNumVariableValue(token_commands);
                return true;
            }
        }
    }
    else{
        std::cout << "Error: Didn't use PRINT, STRING, NUMBER or a variable name to start line.";
        return false;
    }
}

Interpreter::~Interpreter()
{}

////////// Parser Class //////////
Parser::Parser()
{}

std::vector<std::string> Parser::operator()(std::string code_line)
{
    std::vector<std::string> tokenVec;
    std::vector<std::string> ErrorVec;
    std::vector<int> quoteVec;  // hold all the index positions that '"' occurs within code_line.
    std::string token = "";

    if (code_line == ""){    //check for blank lines.
        tokenVec.push_back(" ");
        return tokenVec;
    }

    if (code_line[0] == '#'){
        tokenVec.push_back(" ");
        return tokenVec;
    }

    int quoteCount = 0;
    for (int i=0; i<code_line.size(); i++){
        if(code_line[i] == '"'){
            quoteCount++;
        }
        else if (code_line[i] == '#'){
        		if(quoteCount == 1){    //to make sure to continue if we've run into a quote before a '#', this is in case we have a '#' in a string.
            	continue;
            }
        		else{
            	code_line = code_line.substr(0, i-1);   //only break out and redeclare the string when we run into our second quote aka quoteCount >1.
            	break;
            }
        }
    }


    for (int i = 0; i < code_line.size(); i++){  //find quote index positions.
        if(code_line[i] == '\"'){
            quoteVec.push_back(i);
        }
    }
    if(quoteVec.size() == 1 || quoteVec.size() > 2){
        ErrorVec.push_back("INVALID QUOTES");
        return ErrorVec;
    }

    for (int i = 0; i < code_line.size(); i++){ 
        if(code_line[i] == ' ' && code_line[i+1] == ' '){
            continue;
        }
        else if(code_line[i] == '\"'){ 
            token = code_line.substr(i, quoteVec[1] - quoteVec[0] + 1);    //start from first quote, write until after second quote.
            i = quoteVec[1];  
        }
        else if (code_line[i] == '+' || code_line[i] == '-' || code_line[i] == '*' || code_line[i] == '='){
            if(token != ""){
                tokenVec.push_back(token);
            }
            token = "";
            token = token + code_line[i];
            tokenVec.push_back(token);
            token = "";
        }
        else if(code_line[i] != ' ' && code_line[i] != '\t'){
            token = token + code_line[i];
        }
        else{
            if(token != ""){
                tokenVec.push_back(token);
            }
            token = "";
        }
    }
    if(token != ""){
        tokenVec.push_back(token);
    }

    return tokenVec;
}


Parser::~Parser()
{}

////////// ReadInFile Class //////////
ReadInFile::ReadInFile()
{}

void ReadInFile::read_f(std::string file_name)
{
    Interpreter engine;
    std::vector<std::string> temp;
    Parser parser;
    int counter = 1;

    std::fstream myFile;
    myFile.open(file_name, std::ios::in);  //read mode.
    if(myFile.is_open()){
        std::string line;
        while (getline(myFile, line)){
            temp = parser(line);
            if (engine.Process(temp) == true)
                counter++;
            else{
                std::cout << " See line " << counter << std::endl;
                break;
            }
        }
        myFile.close();
    }
}
    
ReadInFile::~ReadInFile()
{}


