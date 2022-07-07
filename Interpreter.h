#pragma once
#include <string>
#include <vector>

struct Num_Varible
{
    Num_Varible();
    Num_Varible(std::string intVarName);
    ~Num_Varible();
    std::string mVarName;
    long long mVarNum = 0;
};

struct String_Variable
{
    String_Variable();
    String_Variable(std::string strVarName);
    ~String_Variable();
    std::string mVarName;
    std::string mVarString = "";
};

class Interpreter
{
public:
    Interpreter();

    bool IsVariable(std::string variable);
    std::string VarType(std::string variable);

    bool CreateStrVariable(std::string strVarName);
    void SetStrVariableValue(std::string variable, std::string string);

    bool CreateNumVariable(std::string intVarName);
    bool IsNumber(std::string token);
    void SetNumVariableValue(std::vector<std::string> tokens);

    void PrintQuote(std::string string);
    void PrintVariable(std::string variable);
    void PrintNumber(std::string strNumber);

    bool Process(std::vector<std::string> parsed_command);

    ~Interpreter();
private:
    std::vector<Num_Varible> stored_NUM_varibles;
    std::vector<String_Variable> stored_STRING_varibles;
};

class Parser
{
public:
    Parser();
    std::vector<std::string> operator()(std::string code_line);
    ~Parser();
private:
};

class ReadInFile
{
public:
    ReadInFile();
    void read_f(std::string file_name);
    ~ReadInFile();
private:
};
