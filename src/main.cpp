//============================================================================
// Name        : main.cpp
// Author      : Grupo Capcom
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "tools/json/ConfigFileParser/ConfigFileParser.h"

using namespace std;

json parseConfigFile() {
	ConfigFileParser parser = ConfigFileParser("config/config.json");
	parser.parse();
	return parser.getConfig();
}

int main() {
	json config = parseConfigFile();
	cout << config.dump(4) << endl;

	return 0;
}
