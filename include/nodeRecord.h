#pragma once

#include "common.h"

#define MAX_KEYS_NODE 50

class nodeRecord
{
public:
	
	nodeRecord();

	nodeRecord(nodeRecord &CopyFrom);

	~nodeRecord();

	vector<pair<string, string>> *v;

	string sSearchField;	// for example "_id":"2" -> _id
	string sActSearchValue; // Value that is indexed, for example "_id":"2" -> 2

	int AddKeyValueData(string sKey, string sValue);

	nodeRecord * CreateNodeRecord(string sSearchField, string sSearchValue, vector <pair<string, string>> *v);

	int AddNodeRecordToMap(unordered_multimap<string, nodeRecord *> &umTest);

	void printNodeRecord(nodeRecord *nr, string sComment);

	void printVector(vector<pair<string, string>> *v, string sComment);

	void clearVector();
};

