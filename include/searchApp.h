#pragma once

#include "common.h"
#include "..\include\nodeRecord.h"

class searchApp
{
public:
	
	searchApp();

	enum eIndexType { USERS=1, TICKETS, ORGS };

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method will create index for each of the data types and relate objects.
	// ------------------------------------------------------------------------------------
	// Input Parameters
	// userJsonFilePath -> Path to JSON file having user records
	// ticketsJsonFilePath -> Path to JSON file having Ticket records
	// orgJsonFilePath -> Path to JSON file having organization records
	//
	// return 
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int buildSearchIndexData(string userJsonFilePath, string ticketsJsonFilePath, string orgJsonFilePath);

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method will search an object to match user search criterias.
	// It will print to command line matching records.
	// It will print to command line related object for each of the matching records.
	// ------------------------------------------------------------------------------------
	// Input Parameters
	// searchCriteria -> value of an attribute in JSON record to be searched
	// searchField -> the attribute name in JSON record to be searched
	// idxType -> Data type in which record needs to be searched
	// bGetRelatedIdInfo -> Flag to decide if related records are to fetched
	//
	// return 
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int searchAndDisplayIndexedData(string searchCriteria, string searchField, eIndexType idxType, bool bGetRelatedIdInfo);
	
	/*************************************************************************************/
	// Usage:
	// This method will return number of user records matching user criteria.
	// This does not include the number of related objects.
	// return 
	// Number of records found.
	//-----------------------------------------------------------------------------------*/
	int getUsersRecordCount();

	/*************************************************************************************/
	// Usage:
	// This method will return number of user records matching user criteria.
	// This does not include the number of related objects.
	// return 
	// Number of records found.
	//-----------------------------------------------------------------------------------*/
	int getTicketsRecordCount();

	/*************************************************************************************/
	// Usage:
	// This method will return number of organization records matching user criteria.
	// This does not include the number of related objects.
	// return 
	// Number of records found.
	//-----------------------------------------------------------------------------------*/
	int getOrgsRecordCount();
	
	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method will list all available attributes or data fields on a data type.
	// ------------------------------------------------------------------------------------
	// Input Parameters
	// idxType -> -> Data type on which attributes or data fields are to be displayed
	//
	// return
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int DisplayDataFields(eIndexType idxType);

	~searchApp();

	vector<string> listOfUserDataFields;	// Store attributes or data fields on a user records
	vector<string> listOfTicketDataFields;	// Store attributes or data fields on a ticket records
	vector<string> listOfOrgDataFields;		// Store attributes or data fields on a organization records

private:

	int _usersRecords;						// Keep count of users records matching user search criteria
	int _ticketsRecords;					// Keep count of tickets records matching user search criteria
	int _orgsRecords;						// Keep count of orgs records matching user search criteria

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method will eliminate any duplicates records having same value as that of the searchfield.
	// It will print to command line matching records.
	// ------------------------------------------------------------------------------------
	// Input Parameters
	// sSearchField -> value of an attribute in JSON record to be searched
	// searchCriteria -> the attribute name in JSON record to be searched
	// iItr -> Iterator to traverse duplicate records
	// idxType -> Data type in which record needs to be searched
	// bGetRelatedIdInfo -> Flag to decide if related records are to fetched
	//
	// return
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int CheckForAskedSearchField(int iDupResults,
		string sSearchField,
		string searchCriteria,
		unordered_multimap<string, nodeRecord *>::iterator iItr,
		eIndexType idxType,
		bool bGetRelatedIdInfo);

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method will search records for user/ticket/org/ having matching user search criteria.
	// It will print to command line matching records.
	// ------------------------------------------------------------------------------------
	// Input Parameters
	// map - Map with records for user/ticket/organization
	// searchField -> value of an attribute in JSON record to be searched
	// searchCriteria -> the attribute name in JSON record to be searched
	// idxType -> Data type in which record needs to be searched
	// bGetRelatedIdInfo -> Flag to decide if related records are to fetched
	//
	// return
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int searchMapAndDisplayIndexedData(unordered_multimap<string, nodeRecord *> &iMap, string searchCriteria, string searchField, eIndexType idxType, bool bGetRelatedIdInfo);

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method will build an index of related objects of an user.
	// ------------------------------------------------------------------------------------
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int buildUserRelationsIndexData();

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method will build index for tickets and orgs related to an user.
	// ------------------------------------------------------------------------------------
	// Input Parameters
	// map -> Map with records of all the users
	// searchField -> value of an attribute in JSON record to be searched
	// searchCriteria -> the attribute name in JSON record to be searched
	// iItr -> Iterator to traverse duplicate records
	// idxType -> Data type in which record needs to be searched
	// sUserKey -> key to match when a record is found
	//
	// return
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int buildUserRelIdxData(unordered_multimap<string, nodeRecord *> &map, string searchCriteria, string searchField, eIndexType idxType, string sUserKey);

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method will build an index of related objects of a ticket.
	// ------------------------------------------------------------------------------------
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int buildTicketRelationsIndexData();

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method will build index for user and orgs related to a ticket.
	// ------------------------------------------------------------------------------------
	// Input Parameters
	// map -> Map with records of all the tickets
	// searchCriteria -> the attribute name in JSON record to be searched
	// sTicketKey -> key to match when a record is found
	//
	// return
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int buildTicketRelIdxData(unordered_multimap<string, nodeRecord *> &map, string searchCriteria, string sTicketKey);

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method will build an index of related objects of a organization.
	// ------------------------------------------------------------------------------------
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int buildOrgRelationsIndexData();
	
	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method will build index for user and tickets related to a org.
	// ------------------------------------------------------------------------------------
	// Input Parameters
	// map -> Map with records of all the tickets
	// searchCriteria -> the attribute name in JSON record to be searched
	// sTicketKey -> key to match when a record is found
	//
	// return
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int buildOrgRelIdxData(unordered_multimap<string, nodeRecord *> &map, string searchCriteria);

	// Map to store user records for exampe {userid, _id, <_id:"01",name:"John",...>}
	unordered_multimap<string, nodeRecord *> _mapUsers;
	// Map to store ticket records
	unordered_multimap<string, nodeRecord *> _mapTickets;
	// Map to store ticket records
	unordered_multimap<string, nodeRecord *> _mapOrganization;

	// User relations
	// User _id from User, _id from Ticket
	unordered_multimap<string, nodeRecord *> _mapUserAndTicketRelations;
	// User _id from User, _id from Organization
	unordered_multimap<string, nodeRecord *> _mapUserAndOrgRelations;

	// Ticket relations
	// Ticket _id from Ticket, assignee_id from User
	unordered_multimap<string, nodeRecord *> _mapTicketAndUserRelations;
	// Ticket _id from Ticket, _id from Organization
	unordered_multimap<string, nodeRecord *> _mapTicketAndOrgRelations;
	
	// Organization relations
	// Organization _id from Organization, user _id from User
	unordered_multimap<string, nodeRecord *> _mapOrgAndUserRelations;
	// Organization _id from Organization, ticket _id from Ticket
	unordered_multimap<string, nodeRecord *> _mapOrgAndTicketRelations;

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method will create map based on file input and catergory - user, ticket, organization.
	// ------------------------------------------------------------------------------------
	// Input Parameters
	// idxType -> Data type in which record needs to be searched
	// isFilePath -> JSON file for which flat records are to be stored in map
	//
	// return
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int createMap(eIndexType idxType ,string isFilePath);

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method is for debugging a map
	// ------------------------------------------------------------------------------------
	// Input Parameters
	// umTest -> Map containig records
	// sMapName -> This is a comment for print
	//
	// return
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	void printMap(unordered_multimap<string, nodeRecord *> &umTest, string sMapName);

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method is to be print related objects for record searched for user criteria
	// ------------------------------------------------------------------------------------
	// Input Parameters
	// _map -> Map containig records to be printed
	// sKey -> Related objects are searched for this key
	// sComment -> This is a comment for print
	//
	// return
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int printRelatedIndexData(unordered_multimap<string, nodeRecord *>&_map, string sKey, string sComment);

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method creates a list of available search/data fields from the MAP that can be searched for by user a value
	// ------------------------------------------------------------------------------------
	// Input Parameters
	// _map -> Map containig records to be scanned to populate search/data fields 
	// vlistOfDataFields -> Store search/data fields 
	//
	// return
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int createDataFieldList(unordered_multimap<string, nodeRecord *>_map, vector<string> &vlistOfDataFields);

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method print search/data fields to user to choose from
	// ------------------------------------------------------------------------------------
	// Input Parameters
	// vlistOfDataFields -> Store search/data fields
	//
	// return
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int DisplayFields(vector<string> vlistOfDataFields);

	/*-------------------------------------------------------------------------------------
	// Usage:
	// This method deletes all the memory on heap occupied by nodeRecords
	// ------------------------------------------------------------------------------------
	// Input Parameters
	// _map -> Map containig records
	//
	// return
	// 0 - successful execution of method, else - method execution failed.
	//-----------------------------------------------------------------------------------*/
	int DeleteMapData(unordered_multimap<string, nodeRecord *> &_map);
};

