#include "searchApp.h"
#include <chrono>
#include <time.h>
#include <ctime>
#include <ios> 
#include <algorithm>

using chrono::duration_cast;
using chrono::milliseconds;
using chrono::seconds;
using chrono::system_clock;

/*-------------------------------------------------------------------------------------
// Tool INFO:
// This command line utility is to search records from JSON files users.json, tickets.json and organizations.json.
// User will be presented with the below choices:
//	1. Folder or Dir path where above 3 json files are stored
//	2. 1/2/3 - To select user / ticket / org record to search
//	3. A attribute or data field to search in selected data
//	4. A value for selected attribute or data field to search in user / ticket / org
// ------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------
// Usage:
// This method will take input from user.
// ------------------------------------------------------------------------------------
// Input Parameters
// indexData -> Object to build, search and fetch available data fields.
// sSearchField -> A attribute or data field to search in selected data
// sSearchValue -> A value for selected attribute or data field to search in user / ticket / org
//
// return
// 0 - successful execution of method, else - method execution failed.
//-----------------------------------------------------------------------------------*/
int takeUserInput(searchApp &indexData, searchApp::eIndexType &idxType, string &sSearchField, string &sSearchValue)
{
	int rc = 0, catChoice = 0;
	string sChoice, inputStr = "";
	
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "Enter search record type -> 1 for User, 2 for Ticket, 3 for Organization" << endl;
	getline(cin, sChoice);
	catChoice = stoi(sChoice);
	
	if (catChoice == searchApp::USERS)
	{
		cout << "Select one of the below data fields to get user records:" << endl;
		indexData.DisplayDataFields(searchApp::USERS);
		idxType = searchApp::USERS;
	}
	else
		if (catChoice == searchApp::TICKETS)
		{
			cout << "Select one of the below data fields to get ticket records:" << endl;
			indexData.DisplayDataFields(searchApp::TICKETS);
			idxType = searchApp::TICKETS;
		}
		else
			if (catChoice == searchApp::ORGS)
			{
				cout << "Select one of the below data fields to get organization records:" << endl;
				indexData.DisplayDataFields(searchApp::ORGS);
				idxType = searchApp::ORGS;
			}
			else
			{
				if ((catChoice < 1) || (catChoice > 3))
				{
					rc = -1;
					return rc;
				}
			}

	getline(cin, sSearchField);
	
	cout << "Select a value for sSearchField:" << endl;
	
	getline(cin, sSearchValue);
	
	if (sSearchValue == "\"\"")
		sSearchValue = "";
	
	if ((catChoice < 1) || (catChoice > 3) || (sSearchField == ""))
		rc = -1;
		
	cout << "--------------------------------------------------------------------------" << endl;

	return rc;
}

int main()
{
	searchApp indexData;
	int rc = 0, resultCount = 0;
	string sFolderPath;
	searchApp::eIndexType idxType;
	string sSearchField, sSearchValue;
	string sContinueChoice;
	
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "--------------------------- SEARCH APPLICATION ---------------------------" << endl;

	// Take input from user for JSON file folder

	cout << "Enter folder path for JSON records: " << endl;
	getline(cin, sFolderPath);
	cout << "Path entered by user: " << sFolderPath << endl;

	string sUserJSONFile = sFolderPath + "\\users.json";
	string sTicketSONFile = sFolderPath + "\\tickets.json";
	string sOrganizationJSONFile = sFolderPath + "\\organizations.json";

	cout << "Building search index. " << endl;
	cout << "JSON file with user data. " << sUserJSONFile  << endl;
	cout << "JSON file with ticket data. " << sTicketSONFile << endl;
	cout << "JSON file with organization data. " << sOrganizationJSONFile << endl;

	// Step-1: Build data and relations Index

	rc = indexData.buildSearchIndexData(sUserJSONFile, sTicketSONFile, sOrganizationJSONFile);
		
	if (rc == 0)
	{
		cout << "Search index is built successfully. Please proceed with search of records." << endl;
	}
	else
	{
		cout << "ERROR: Error encountered in building the search index." << endl;

		return -1;
	}

	do
	{
		// Build data and related objects Index

		rc = takeUserInput(indexData, idxType, sSearchField, sSearchValue);
		if (rc != 0)
		{
			cout << "Incorrect choice or search field. Please try again." << endl;
			continue;
		}
		
		cout << endl << "Search for value \"" << sSearchValue << "\" against field \"" << sSearchField << "\" in record of type " << idxType << "." << endl;
	
		auto begin = chrono::high_resolution_clock::now();

		// Step-2: Search for data and related objects

		rc = indexData.searchAndDisplayIndexedData(sSearchValue, sSearchField, idxType, true);

		auto end = chrono::high_resolution_clock::now();
		auto elapsedmicro = chrono::duration_cast<chrono::microseconds>(end - begin);
		auto elapsedmilli = chrono::duration_cast<chrono::milliseconds>(end - begin);
		
		cout << "Time taken to fetch search results " << (double)(elapsedmicro.count()) << " microseconds" << endl;
		cout << "Time taken to fetch search results " << (double)(elapsedmilli.count()) << " milliseconds" << endl;
	
		if (idxType == searchApp::USERS)
			resultCount = indexData.getUsersRecordCount();
		else if (idxType == searchApp::TICKETS)
			resultCount = indexData.getTicketsRecordCount();
		else if (idxType == searchApp::ORGS)
			resultCount = indexData.getOrgsRecordCount();

		if (resultCount > 0)
		{
			cout << "Number of records found for give search criterias: " << resultCount << endl;
		}
		else
		{
			cout << "No records were found for give search criterias." << endl;
		}
	
		cout << endl << "To continue press 'c', to exit press 'q'" << endl;
		getline(cin, sContinueChoice);

	} while (sContinueChoice[0] != 'q');
	
	return 0;
}
