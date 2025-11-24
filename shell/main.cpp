#include <iostream>
#include <string>
#include <vector>
#include "shell.h"

std::map<std::string, unsigned long> variables;

void echo(SHELL_ARGS args, Shell &shell) {

	std::vector<std::string> words(args.begin() + 1, args.end());

	for (std::string s : words) {

		std::cout << s << ' ';
	}

	std::cout << '\n';
}

void sum(SHELL_ARGS args, Shell &shell) {
	
	int sum = 0;
	
	std::vector<std::string> nums(args.begin()+1, args.end());

	for (std::string s : nums) {
		
		int n = std::stoi(s);

		sum += n;
	}

	std::cout << sum << '\n';
}

void set_var(std::string name, long val) {

	variables[name] = val;

}

void new_var(SHELL_ARGS args, Shell &shell) {

	set_var( args[0], stol(args[1]) );

}

void get_var(SHELL_ARGS args, Shell &shell) {
	
	if ( args.size() == 3 ) {

		long val = variables[args[0]];
		set_var(args[2], val);

	}

	else std::cout << variables[args[0]] << '\n';
}

int main(int argc, char *argv[]){

	Shell myShell;

	myShell.set_flags(SHELL_FLAG_ON_ERROR_PRINT);

	myShell.map_function("echo", echo);
	myShell.map_function("sum", sum);
	myShell.map_function("new", new_var);
	myShell.map_function("get", get_var);
	
	myShell.run_file("test.txt");
	myShell.run_file("test.tx");
	myShell.run();

	return 0;
}