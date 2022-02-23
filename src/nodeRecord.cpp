#include "..\include\nodeRecord.h"

nodeRecord::nodeRecord()
{
	sSearchField = "";
	sActSearchValue = "";

	v = new  vector<pair<string, string>>;
	if (v != NULL)
	{
		//cout << "Vector initial capacity is " << v->capacity() << endl;
		v->reserve(MAX_KEYS_NODE);
		//cout << "Vector new capacity " << v->capacity() << endl;
	}

}

nodeRecord::nodeRecord(nodeRecord &vCopyFrom)
{
	sSearchField = vCopyFrom.sSearchField;
	sActSearchValue = vCopyFrom.sActSearchValue;
	
	v = new  vector<pair<string, string>>;
	if (v != NULL)
	{
		//cout << "Vector initial capacity is " << v->capacity() << endl;			
		//cout << "Vector new capacity " << v->capacity() << endl;
		v = vCopyFrom.v;
	}
}

int nodeRecord::AddKeyValueData(string sKey, string sValue)
{
	v->emplace_back(make_pair(sKey, sValue));

	return 0;
}

nodeRecord * nodeRecord::CreateNodeRecord(string sSearchField, string sSearchValue, vector <pair<string, string>> *v)
{
	nodeRecord *nr = new nodeRecord;

	if (nr != NULL)
	{
		nr->sSearchField = sSearchField;
		nr->sActSearchValue = sSearchValue;
		nr->v = v;
	}

	//nr->v = new vector <pair<string, string>>(*v);

	return nr;
}

int nodeRecord::AddNodeRecordToMap(unordered_multimap<string, nodeRecord *> &umTest)
{
	vector<pair<string, string>>::iterator it;

	//cout << "Current number of elements in Map: " << umTest.size() << endl;

	for (it = v->begin(); it != v->end(); it++)
	{
		pair<string, string> p = *it;
		//cout << "key: " << p.first << "\t" << "keyValue: " << p.second << endl;

		string sSearchField = p.first;

		string sSearchValue = p.second;

		nodeRecord *nr = CreateNodeRecord(sSearchField, sSearchValue, v);

		//printnodeRecord(nr);

		pair<string, nodeRecord *> myrecord(sSearchValue, nr);

		umTest.insert(myrecord);
	}

	//cout << "Mumber of elements in Map: " << umTest.size() << endl;

	return 0;
}

void nodeRecord::printNodeRecord(nodeRecord *nr, string sComment)
{
#ifdef DEBUG_PRINT_INTERNAL_1
	//cout << "printnodeRecord" << endl;
	cout << "Search Field: " << nr->sSearchField << endl;
	cout << "Acutal Search Value: " << nr->sActSearchValue << endl;
#endif

	vector<pair<string, string>> *v = nr->v;

	printVector(v, sComment);
}

void nodeRecord::printVector(vector<pair<string, string>> *v, string sComment)
{
#ifdef DEBUG_PRINT_INTERNAL_1
	cout << "Print Vector" << endl;
	cout << "Number of entries in Vector: " << v->size() << endl;
#endif

	cout << endl << "--------------------------------------------------" << endl;
	cout << sComment << endl;
	cout << "--------------------------------------------------" << endl;
	vector<pair<string, string>>::iterator it;
	for (it = v->begin(); it != v->end(); it++)
	{
		pair<string, string> p = *it;
		//cout << "key: " << p.first << "\tkeyValue: " << p.second << endl;
		cout << p.first << "\t : " << p.second << endl;
	}
	cout << "--------------------------------------------------" << endl;
}

void nodeRecord::clearVector()
{
	v->clear();
}

nodeRecord::~nodeRecord()
{
}
