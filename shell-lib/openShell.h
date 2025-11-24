#ifndef _SHELL_H
#define _SHELL_H

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <stack>
#include <set>
#include <functional>
#include <fstream>
//#include <thread>

#define SHELL_ARGS std::vector<std::string>
#define SHELL_FUNC std::function<void(SHELL_ARGS, Shell&)>

#define SHELL_NO_ERROR 0x0
#define SHELL_ERROR_UNK_FUNC 0x1
#define SHELL_ERROR_UNK_ERR 0x2
#define SHELL_ERROR_FILE 0x3
//#define SHELL_ERROR_THREAD 0x4
// if(!code_works){ kill_myself()}


#define SHELL_FLAG_NONE 0x0
#define SHELL_FLAG_ON_ERROR_HALT 0x1
#define SHELL_FLAG_ON_ERROR_PRINT 0x2
#define SHELL_FLAG_THREADED 0x4

class Shell {

private:

	//Defaults
	std::string cursor = "@";
	char token_separator_char = ' ';
	std::string error_msg = "No error";
	std::map<unsigned int, std::string > error_code_to_string = {
		{SHELL_NO_ERROR, "No error"},
		{SHELL_ERROR_UNK_FUNC, "Unknown function"},
		{SHELL_ERROR_UNK_ERR, "Unknown error code"},
		{SHELL_ERROR_FILE, "Failed opening file"}
		//{SHELL_ERROR_FILE, "Failed creating thread"}
	};

	//Flags
	bool halt = false;
	//bool threaded;

	//Flags II: Errors
	bool on_error_halt = false;
	bool on_error_print = false;
	bool on_error_handle = false; //Set to true if any of the previous two are

	unsigned int error_code = SHELL_NO_ERROR;


	std::map<std::string, SHELL_FUNC > functions;
	std::stack<std::string> history;

	//Methods

	SHELL_ARGS tokenize(const std::string str, char separator);

	void execute_args(SHELL_ARGS tokens);

	void handle_error(void);

public:
	//Setters
	void set_halt(char val);
	void set_cursor(std::string cursor);
	void set_token_separator_char(char separator);
	void set_flags(unsigned int flags);

	//Getters
	std::string get_error(void);

	//Interface
	void run_command(std::string command);

	void map_function(std::string key, SHELL_FUNC func);

	void map_error(int key, std::string msg);

	void raise_error(int error_code);

	void run();

	void run_file(std::string filename);

	//Constructors
	Shell();
	Shell(unsigned int flags);

};

//POSSIBLE UPGRADES
//Allow for users to link error handling functions
//Let error handling get the tokens that caused an error, perhaps with an error class
//Allow for a 'default' function to execute when no funciton is found in the map, before raising an error.
//This can be useful yet can be easily achieved with a wrapper class fitted for any pourpose of the user.
#endif