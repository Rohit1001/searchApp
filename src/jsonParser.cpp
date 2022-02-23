#include "..\include\jsonParser.h"

void jsonParser::setJsonFilePath(string sFilePath)
{
	sJsonFilePath = sFilePath;
}

int jsonParser::getPropertyTree(ptree &propertyTree)
{
	try
	{
		readJsonFile(sJsonFilePath.c_str());

		istringstream isStreamJson(sJsonStream);

		propertyTree.clear();

		read_json(isStreamJson, propertyTree);
	}
	catch (...)
	{
		cout << "ERROR: In Parsing JSON file: " << sJsonFilePath << endl;
		cout << "INFO: Please validate JSON file: " << endl;
		return -1;
	}

	return 0;
}

int jsonParser::readJsonFile(const char *cptrFilePath)
{
	sJsonStream = "";

	ifstream is;
	is.open(cptrFilePath, ios::binary);

	// get length of file:
	is.seekg(0, ios::end);
	streamoff length = is.tellg();
	is.seekg(0, ios::beg);

#ifdef DEBUG_PRINT_INTERNAL_2
	cout << "File size: " << length << endl;
#endif

	// allocate memory:
	char *buffer = new char[length + 1];
	memset(buffer, '\0', sizeof(char)*(length + 1));

	// read data as a block:
	is.read(buffer, length);

	// create string stream of memory contents
	// NOTE: this ends up copying the buffer!!!
	sJsonStream = buffer;

	// delete temporary buffer
	delete[] buffer;

	// close filestream
	is.close();

	return 0;
}

string jsonParser::indent(int level) {
	string s;
	for (int i = 0; i<level; i++) s += "  ";
	return s;
}

void jsonParser::ParseTreeCreateIndex(ptree &pt, unordered_multimap<string, nodeRecord *> &map, int level) {
	if (pt.empty()) {

	#ifdef DEBUG_PRINT_INTERNAL_1
			cerr << "\"" << pt.data() << "\"";
	#endif 
	}

	else {

		#ifdef DEBUG_PRINT_INTERNAL_1
				if (level) cerr << endl;
				cerr << indent(level) << "{" << endl;
		#endif 

		for (ptree::iterator pos = pt.begin(); pos != pt.end();) {

#ifdef DEBUG_PRINT_INTERNAL_1
			cerr << indent(level + 1) << "\"" << pos->first << "\": ";
#endif 
			string sKey = pos->first.data();
			string sKeyValue = pos->second.data();

#ifdef DEBUG_PRINT_INTERNAL_1
			if ((sKey == "" && sKeyValue != "")) {
				cerr << endl << "key empty Value not" << endl;
			}
#endif

			int looper = 0;
			if ((sKey != "" && sKeyValue == "")) {
				//cerr << endl << "key not Value empty" << endl;
				ptree &local = pt.get_child(sKey);
				//cerr << "[" << endl;
				for (ptree::iterator itr = local.begin(); itr != local.end(); ++itr) {
					string sKey1 = itr->first.data();
					string sKeyValue1 = itr->second.data();
					//cerr << sKey1 << "   " << sKeyValue1 << endl;
					//cerr << sKey1 << "   " << sKeyValue1 << endl;

#ifdef DEBUG_PRINT_INTERNAL_1
					if (looper == 0)
						cerr << endl;
					cerr << indent(level + 3) << "\"" << sKey << "\": ";
					cerr << "\"" << sKeyValue1 << "\"," << endl;
#endif					
					nr->AddKeyValueData(sKey, sKeyValue1);

					++looper;
					if (sKey1 == "" && sKeyValue1 == "")
						break;
				}
				if (looper > 0) {
					//cerr << indent(level + 1) << " ]";
					++pos;

#ifdef DEBUG_PRINT_INTERNAL_1
					if (pos != pt.end()) {
						cerr << ",";
					}
					cerr << endl;
#endif
				}
			}
			if (looper == 0)
			{
				if (sKey == "" && sKeyValue == "")
				{
					nr = new nodeRecord();
				}

				if (sKey != ""/* && sKeyValue != ""*/)
				{
					nr->AddKeyValueData(sKey, sKeyValue);
				}

				ParseTreeCreateIndex(pos->second, map, level + 1);

				++pos;

#ifdef DEBUG_PRINT_INTERNAL_1
				if (pos != pt.end()) {
					cerr << ",";
				}
				cerr << endl;
#endif
			}
		}
#ifdef DEBUG_PRINT_INTERNAL_1
		cerr << indent(level) << " }";
#endif
		if (nr != NULL)
		{
			nr->AddNodeRecordToMap(map);

			delete nr; nr = NULL;
		}
	}

	return;
}
