#include "pch.h"
#include "openShell.h"

//Shell function that halts the process
static void exit_shell(SHELL_ARGS args, Shell &shell) {

	shell.set_halt(0x1);

}

//SHELL CLASS

Shell::Shell() {

	this->map_function("exit", exit_shell);

}

Shell::Shell(unsigned int flags) {

	this->map_function("exit", exit_shell);

	this->on_error_halt = SHELL_FLAG_ON_ERROR_HALT & flags;
	this->on_error_print = SHELL_FLAG_ON_ERROR_PRINT & flags;

	this->on_error_handle = (this->on_error_halt || this->on_error_print);
}

/*
Turns a string and divides it by the provided separator character to create a list of tokens

input:
	str : string to tokenize
	separator : char to delimit tokens

output:
	list of tokens
*/

SHELL_ARGS Shell::tokenize(const std::string str, char separator) {

	std::vector<std::string> tokens;

	std::string token;

	std::stringstream stream(str);

	while (std::getline(stream, token, separator)) tokens.push_back(token);

	return tokens;
}

/*
Calls the funciton asociated with the first token and passes the rest as arguments

input:
	tokens: A list of tokens

output:
	nothing*/

void Shell::execute_args(SHELL_ARGS tokens) {

	if (functions.find(tokens[0]) == functions.end()) raise_error(SHELL_ERROR_UNK_FUNC);

	else functions.at(tokens[0])(tokens, *this);
}

/*
Executes according to each of the error handling flags
*/

void Shell::handle_error(void) {

	if (on_error_print) std::cout << "ERROR " << error_code << " : " << error_msg << "\n";

	halt = on_error_halt;
}

/*
Sets the values of the raised error according to the code and calls the handler if needed
input:
	error_code : int corresponding to an error that "should" exist in the error_to_string map
*/

void Shell::raise_error(int error_code) {

	if (error_code_to_string.find(error_code) == error_code_to_string.end()) {
		raise_error(SHELL_ERROR_UNK_ERR);
		return;
	}

	else {
		this->error_code = error_code;

		this->error_msg = this->error_code_to_string.at(error_code);
	}

	if (on_error_handle) handle_error();
}

/*
Runs the command string throug the tokenizer and executer functions to finally call the command
Also pushes the command to the history.
input:
	command : string that "should" correspond to a command the Shell can find in the 'functions' map
*/

void Shell::run_command(std::string command) {

	history.push(command);

	std::vector<std::string> tokens = tokenize(command, token_separator_char);

	if (tokens.size()) execute_args(tokens);
}

/*
Runs every command found in the given file separated by newline
input:
	filename : name or location of the file to execute
*/

void Shell::run_file(std::string filename) {

	halt = false;

	std::ifstream file(filename);

	if (!file.good()) {

		raise_error(SHELL_ERROR_FILE);
		return;

	}

	std::string command;

	while (getline(file, command) and !halt) {

		run_command(command);

	}
}

/*
Adds a function to the 'functions' map so it can be executed later
input:
	key : name of the function
	func : function that takes a vector of strings and a reference to the shell as parameters and returns nothing
*/

void Shell::map_function(std::string key, SHELL_FUNC func) {

	functions.insert(std::pair<std::string, SHELL_FUNC>(key, func));
}

/*
Adds an error code and msg to the Shell map, so the user can raise custom errors
input:
	key : error code
	msg : Message loaded to error_msg when it's raised by its code throug 'raise_error'
*/

void Shell::map_error(int key, std::string msg) {

	error_code_to_string[key] = msg;

}

/*
Runs in a loop where it gets user input (a string) and runs it throug 'run_command' until 'halt' is set to true
*/

void Shell::run() {

	halt = false;

	while (!halt) {

		std::string input;

		std::cout << cursor << ' ';

		getline(std::cin, input);

		run_command(input);
	}

}

//SETTERS

void Shell::set_halt(char val) {

	halt = val;

}

void Shell::set_cursor(std::string cursor) {
	this->cursor = cursor;
}

void Shell::set_token_separator_char(char separator) {
	this->token_separator_char = separator;
}

void Shell::set_flags(unsigned int flags) {

	this->on_error_halt = SHELL_FLAG_ON_ERROR_HALT & flags;
	this->on_error_print = SHELL_FLAG_ON_ERROR_PRINT & flags;

	this->on_error_handle = (this->on_error_halt || this->on_error_print);
}

//Returns only last error raised
std::string Shell::get_error(void) {

	return this->error_msg;

}