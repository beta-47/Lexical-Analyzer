// NOTE: The following program does not incorporate the use of post- and pre-increment operators. It assumes that the 
// input program is properly formatted.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <regex>
using namespace std;

vector<string> keywords = {

"short", "int", "char", "long", "float", "double", "void", "#include", "<iostream>",
"if", "else", "for", "while", "switch", "case",
"return", "continue", "break", "endl", "using", "namespace", "std",
"struct", "char", "cout"

};

multimap<string, string> symbol_table;

void display_symbol_table()
{
	cout << "SYMBOL TABLE\n\n";
	for(auto it = symbol_table.begin(); it != symbol_table.end(); ++it)
		cout << it->first << " : " << it->second << endl;
}

bool is_digit(char a) { return a >= '0' && a <= '9'; }
bool is_separator(char a) { return a == ';' || a == ',' || a == '(' || a == ')'; }
bool is_whitespace(char a) { return a == ' ' || a == '\n' || a == '\t'; }
bool is_keyword(string lexeme) { return find(keywords.begin(), keywords.end(), lexeme) != keywords.end(); }
bool is_string_literal(string lexeme) { return lexeme[0] == '\"' && lexeme[lexeme.length() - 1] == '\"'; }

bool is_identifier(string lexeme)
{
	regex id("[_a-zA-Z][_a-zA-Z0-9]*");
	return regex_match(lexeme, id);
}

bool is_operator(string lexeme)
{
	return lexeme == "<" || lexeme == ">" || lexeme == "<=" || lexeme == ">=" || 
		   lexeme == "=" || lexeme == "==" || lexeme == "!=" || lexeme == "++" || 
		   lexeme == "--" || lexeme == "+" || lexeme == "-" || lexeme == "*" ||
		   lexeme == "/" || lexeme == "%" || lexeme == "<<" || lexeme == ">>";
}

bool is_punctuator(string lexeme)
{
	return lexeme == "(" || lexeme == ")" || lexeme == "{" || lexeme == "," || lexeme == "}" || lexeme == ";";
}

bool is_numeric_constant(string lexeme)
{
	for(auto character: lexeme)
		if(!is_digit(character)) return false;
	return true;
}


string match(string lexeme)
{
	string token;
	if(is_keyword(lexeme)) token = "keyword"; 
	else if(is_identifier(lexeme)) token = "identifier";
	else if(is_operator(lexeme)) token = "operator";
	else if(is_numeric_constant(lexeme)) token = "numeric constant";
	else if(is_string_literal(lexeme)) token = "string literal";
	else if(is_punctuator(lexeme)) token = "punctuator";
	return token;
}

vector<string> scan(string &input)
{
	vector<string> tokens;
	int n = input.length();
	string buffer = "";
	for(int i = 0; i < n; ++i)
	{
		if((is_separator(input[i]) || is_whitespace(input[i])) && buffer != "") 
		{
			tokens.push_back(buffer);
			if(is_separator(input[i])) tokens.push_back(string(1, input[i]));
			buffer = "";
		}
		else if(input[i] == '\"')
		{
			buffer = "\"";
			i++;
			while(input[i] != '\"') buffer += input[i++];
			buffer += "\"";
		}
		else if(!is_whitespace(input[i])) buffer += input[i];
	}
	return tokens;
}

void lexical_analyzer(vector<string> lexemes)
{
	for(auto lexeme: lexemes)
	{
		string token = match(lexeme);
		symbol_table.insert(pair<string, string>(token, lexeme));
	}
}

void generate_tokens(string input)
{
	vector<string> tokens = scan(input);
	lexical_analyzer(tokens);
	display_symbol_table();
}

int main()
{
	freopen("input.cpp", "r", stdin);
	string input = "", line;
	while(getline(cin, line)) 
	{
		input += line;
		input += '\n';
	}
	generate_tokens(input);
	
	return 0;
}