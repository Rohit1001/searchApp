#pragma once

#include "common.h"
#include "..\include\nodeRecord.h"
using boost::property_tree::ptree;
using boost::property_tree::read_json;

class jsonParser
{

public:

	vector<pair<string, string>> v;
	nodeRecord *nr;

	void setJsonFilePath(string sFilePath);

	int readJsonFile(const char *cptrFilePath);

	int getPropertyTree(ptree &propertyTree);

	void ParseTreeCreateIndex(ptree &pt, unordered_multimap<string, nodeRecord *> &map, int level);

private:

	string indent(int level);

	string sJsonFilePath;

	string sJsonStream;

};


