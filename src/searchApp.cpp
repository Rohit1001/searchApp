#include "searchApp.h"
#include "jsonParser.h"

searchApp::searchApp()
{
	int _usersRecords = 0;
	int _ticketsRecords = 0;
	int _orgsRecords = 0;
}

int searchApp::buildSearchIndexData(string userJsonFilePath, string ticketsJsonFilePath, string orgJsonFilePath)
{
	int rc = createMap(USERS, userJsonFilePath);
	if (rc != 0)
	{
		cout << "Error in creating map for USERS." << endl; 
		return	0;
	}

#ifdef DEBUG_PRINT_INTERNAL_1
	printMap(_mapUsers, "Users Map");
#endif

	int rc2 = createMap(TICKETS, ticketsJsonFilePath);
	if (rc != 0)
	{
		cout << "Error in creating map for TICKETS." << endl;
		return	0;
	}

#ifdef DEBUG_PRINT_INTERNAL_1
	printMap(_mapUsers, "Tickets Map");
#endif

	int rc3 = createMap(ORGS, orgJsonFilePath);
	if (rc != 0)
	{
		cout << "Error in creating map for ORGS." << endl;
		return	0;
	}

#ifdef DEBUG_PRINT_INTERNAL_1
	printMap(_mapUsers, "Org Map");
#endif

	rc = createDataFieldList(_mapUsers, listOfUserDataFields);
	if (rc != 0)
	{
		cout << "Error in creating tags from map of USERS." << endl;
		return	0;
	}

	rc = createDataFieldList(_mapTickets, listOfTicketDataFields);
	if (rc != 0)
	{
		cout << "Error in creating tags from map of TICKETS." << endl;
		return	0;
	}

	rc = createDataFieldList(_mapOrganization, listOfOrgDataFields);
	if (rc != 0)
	{
		cout << "Error in creating tags from map of ORGS." << endl;
		return	0;
	}

	rc = buildUserRelationsIndexData();
	if (rc != 0)
	{
		cout << "Error in creating user relations." << endl;
		return	0;
	}

	rc = buildTicketRelationsIndexData();
	if (rc != 0)
	{
		cout << "Error in creating ticket relations." << endl;
		return	0;
	}

	rc = buildOrgRelationsIndexData();
	if (rc != 0)
	{
		cout << "Error in creating org relations." << endl;
		return	0;
	}


	return 0;
}

int searchApp::CheckForAskedSearchField(int iDupResults,
	string sSearchField,
	string searchCriteria,
	unordered_multimap<string, nodeRecord *>::iterator iItr,
	eIndexType idxType,
	bool bGetRelatedIdInfo)
{
	int nfilterRec = 1;

	for (int i = 0; i < iDupResults; i++)
	{
		nodeRecord *noderec = iItr->second;

		string s = noderec->sSearchField;

		if ((s == sSearchField) && (noderec->sActSearchValue == searchCriteria))
		{
			string sComment = "";
			if (idxType == USERS)
			{
				_usersRecords = nfilterRec++;
				sComment = "User";
			}
			else if (idxType == TICKETS)
			{
				_ticketsRecords = nfilterRec++;
				sComment = "Ticket";
			}
			else if (idxType == ORGS)
			{
				sComment = "Organization";
				_orgsRecords = nfilterRec++;
			}

			vector<pair<string, string>> *v = noderec->v;
			
#ifndef DISABLE_PRINT_FOR_PERF_MEAS
			string sPrint = sComment + " record - " + to_string(i+1);
			noderec->printNodeRecord(noderec, sPrint);
#endif
			if (bGetRelatedIdInfo)
			{
				vector<pair<string, string>>::iterator it;
				for (it = v->begin(); it != v->end(); it++)
				{
					pair<string, string> p = *it;

					if (p.first == "_id")
					{
						if (idxType == USERS)
						{
							string sComment = "Ticket";
							printRelatedIndexData(_mapUserAndTicketRelations, p.second, sComment);

							sComment = "Organization";
							printRelatedIndexData(_mapUserAndOrgRelations, p.second, sComment);
							break;
						}
						else if (idxType == TICKETS)
						{
							string sComment = "User";
							printRelatedIndexData(_mapTicketAndUserRelations, p.second, sComment);

							sComment = "Organization";
							printRelatedIndexData(_mapTicketAndOrgRelations, p.second, sComment);
							break;
						}
						else if (idxType == ORGS)
						{
							string sComment = "User";
							printRelatedIndexData(_mapOrgAndUserRelations, p.second, sComment);

							sComment = "Ticket";
							printRelatedIndexData(_mapOrgAndTicketRelations, p.second, sComment);
							break;
						}
					}
				}
			}			
		}		
		iItr++;
	}
	return 0;
}

int searchApp::searchMapAndDisplayIndexedData(unordered_multimap<string, nodeRecord *> &iMap, string searchCriteria, string searchField, eIndexType idxType, bool bGetRelatedIdInfo)
{
	int dupResults = 0;

	auto itr{ iMap.find(searchCriteria) };

	dupResults = iMap.count(searchCriteria);

	if (dupResults > 0)
	{
		CheckForAskedSearchField(dupResults, searchField, searchCriteria, itr, idxType, bGetRelatedIdInfo);
	}

	return 0;
}


int searchApp::searchAndDisplayIndexedData(string searchCriteria, string searchField, eIndexType idxType, bool bGetRelatedIdInfo)
{
	if (idxType == USERS)
	{
		int rc = searchMapAndDisplayIndexedData(_mapUsers, searchCriteria, searchField, idxType, bGetRelatedIdInfo);
	}
	else if (idxType == TICKETS)
	{
		int rc = searchMapAndDisplayIndexedData(_mapTickets, searchCriteria, searchField, idxType, bGetRelatedIdInfo);
	}
	else if (idxType == ORGS)
	{
		int rc = searchMapAndDisplayIndexedData(_mapOrganization, searchCriteria, searchField, idxType, bGetRelatedIdInfo);
	}

	return 0;
}

int searchApp::createMap(eIndexType idxType, string isFilePath)
{	
	int rc = 0;
	jsonParser jParser;
	
	jParser.setJsonFilePath(isFilePath);
	
	ptree propertyTree;
	
	rc = jParser.getPropertyTree(propertyTree);
	if (rc != 0) return -1;
	
	if (idxType == USERS)
	{
		jParser.ParseTreeCreateIndex(propertyTree, _mapUsers, 0);
#ifdef DEBUG_PRINT_INTERNAL_2
		cout << "Users Map Size: " << _mapUsers.size() << endl;		
#endif
	}
	else
		if (idxType == TICKETS)
		{
			jParser.ParseTreeCreateIndex(propertyTree, _mapTickets, 0);
#ifdef DEBUG_PRINT_INTERNAL_2
			cout << "Tickets Map Size: " << _mapTickets.size() << endl;
#endif
		}
	else
		if (idxType == ORGS)
		{
			jParser.ParseTreeCreateIndex(propertyTree, _mapOrganization, 0);
#ifdef DEBUG_PRINT_INTERNAL_2
			cout << "Organizations Map Size: " << _mapOrganization.size() << endl;
#endif
		}
	
	return rc;
}

void searchApp::printMap(unordered_multimap<string, nodeRecord *> &umTest, string sMapName)
{
	unordered_multimap<string, nodeRecord *>::const_iterator it;

	int i = 1;
	cout << "Print Multi Map" << endl;

	for (it = umTest.begin(); it != umTest.end(); it++)
	{
		cout << "---------------------------" << endl;

		string s = it->first;

		cout << "key: " << s << endl;

		nodeRecord *nr = it->second;

		string sComment = sMapName + " Map Record";
		sComment = sComment  + " - " + to_string(i);
		nr->printNodeRecord(nr, sComment);
		i++;
	}
}

int searchApp::getUsersRecordCount()
{
	return _usersRecords;
}

int searchApp::getTicketsRecordCount()
{
	return _ticketsRecords;
}

int searchApp::getOrgsRecordCount()
{
	return _orgsRecords;
}

searchApp::~searchApp()
{
	DeleteMapData(_mapUsers);
	DeleteMapData(_mapTickets);
	DeleteMapData(_mapOrganization);

	DeleteMapData(_mapUserAndTicketRelations);
	DeleteMapData(_mapUserAndOrgRelations);

	DeleteMapData(_mapTicketAndUserRelations);
	DeleteMapData(_mapTicketAndOrgRelations);

	DeleteMapData(_mapOrgAndUserRelations);
	DeleteMapData(_mapOrgAndTicketRelations);
}

int searchApp::buildUserRelIdxData(unordered_multimap<string, nodeRecord *> &map, string searchCriteria, string searchField, eIndexType idxType, string sUserKey)
{
	int dupResultsUsers, dupResultsTickets = 0;

	auto itr1{ map.find(searchCriteria) };

	dupResultsUsers = map.count(searchCriteria);

	for (int i = 0; i < dupResultsUsers; i++)
	{
		nodeRecord *noderec = itr1->second;
		string s = noderec->sSearchField;

		// Insert user info -> ticket_id, user_id, info vector
		if ((s == searchField) && (noderec->sActSearchValue == searchCriteria))
		{
			vector<pair<string, string>> *v = noderec->v;
			vector<pair<string, string>>::iterator it;
			for (it = v->begin(); it != v->end(); it++)
			{
				pair<string, string> p = *it;
				//cout << "key: " << p.first << " keyValue: " << p.second << endl;

				if (p.first == "_id")
				{
					nodeRecord *newNodeRec = new nodeRecord(*noderec);
					if (newNodeRec != NULL)
					{
						newNodeRec->sSearchField = p.second;
						newNodeRec->sActSearchValue = sUserKey;
						pair<string, nodeRecord *> myrecord(sUserKey, newNodeRec); // UserKey, <_id,<,,,,,>>

						if (idxType == TICKETS)
							_mapUserAndTicketRelations.insert(myrecord);
						else
							if (idxType == ORGS)
								_mapUserAndOrgRelations.insert(myrecord);
					}
				}
			}
		}
		itr1++;
	}
	return 0;
}

int searchApp::buildUserRelationsIndexData()
{
	// User relations
	// User _id from User, _id from Ticket
	// User _id from User, _id from Organization

	unordered_multimap<string, nodeRecord *>::const_iterator itUser;

	for (itUser = _mapUsers.begin(); itUser != _mapUsers.end(); itUser++)
	{
		nodeRecord *noderec = itUser->second;
		string sDataField = noderec->sSearchField;

		if ((sDataField == "_id") && noderec != NULL)
		{
			string sCurrUserKey = itUser->first;
			//cout << "key: " << sDataField << "key Value: " << sUserKey << endl;

			vector<pair<string, string>> *v = noderec->v;
			vector<pair<string, string>>::iterator it;
			for (it = v->begin(); it != v->end(); it++)
			{
				pair<string, string> p = *it;
				if (p.first == "_id")
				{
					string sUserKey = p.second;
					//cout << "key: " << sDataField << "key Value: " << sOrgKey << endl;
					searchApp::eIndexType idxType = searchApp::TICKETS;
					buildUserRelIdxData(_mapTickets, sUserKey, "assignee_id", idxType, sCurrUserKey);
				}
				else
					if (p.first == "organization_id")
					{
						string sOrgKey = p.second;
						//cout << "key: " << sDataField << "key Value: " << sOrgKey << endl;
						searchApp::eIndexType idxType = searchApp::ORGS;
						buildUserRelIdxData(_mapOrganization, sOrgKey, "_id", idxType, sCurrUserKey);
					}
			}
		}
	}

#ifdef DEBUG_PRINT_INTERNAL_1
	printMap(_mapUserAndTicketRelations, "User Tickets Relations Map");
	printMap(_mapUserAndOrgRelations, "User Orgs Relations Map");
#endif

#ifdef DEBUG_PRINT_INTERNAL_2
	cout << "Users and Tickets Relations Map Size: " << _mapUserAndTicketRelations.size() << endl;
	cout << "Users and Orgs Relations Map Size: " << _mapUserAndOrgRelations.size() << endl;
#endif

	return 0;
}

int searchApp::buildTicketRelIdxData(unordered_multimap<string, nodeRecord *> &map, string searchCriteria, string sTicketKey)
{
	int dupResultsUsers, dupResultsTickets = 0;

	auto itr1{ map.find(searchCriteria) };

	dupResultsUsers = map.count(searchCriteria);
	
	for (int i = 0; i < dupResultsUsers; i++)
	{
		nodeRecord *noderec = itr1->second;
		string s = noderec->sSearchField;

		// Insert user info -> ticket_id, user_id, info vector
		if ((s == "_id") && (noderec->sActSearchValue == searchCriteria))
		{
			vector<pair<string, string>> *v = noderec->v;
			vector<pair<string, string>>::iterator it;
			for (it = v->begin(); it != v->end(); it++)
			{
				pair<string, string> p = *it;
				//cout << "key: " << p.first << " keyValue: " << p.second << endl;

				if (p.first == "_id")
				{
					nodeRecord *newNodeRec = new nodeRecord(*noderec);
					if (newNodeRec != NULL)
					{
						newNodeRec->sSearchField = p.second;
						pair<string, nodeRecord *> myrecord(sTicketKey, newNodeRec); // organization_id, <_id,<,,,,,>>
						_mapTicketAndUserRelations.insert(myrecord);
					}
				}
				else
				if (p.first == "organization_id")
				{
					auto itr2{ _mapOrganization.find(p.second) };

					dupResultsTickets = _mapOrganization.count(p.second);

					for (int ii = 0; ii < dupResultsTickets; ii++)
					{
						nodeRecord *noderec = itr2->second;
						string s = noderec->sSearchField;

						// Insert user info -> org_id, user_id, info vector
						if ((s == "_id") && (itr2->first == p.second))
						{
							vector<pair<string, string>> *v = noderec->v;
							vector<pair<string, string>>::iterator it;
							for (it = v->begin(); it != v->end(); it++)
							{
								pair<string, string> p = *it;
								if (p.first == "_id")
								{
									nodeRecord *newNodeRec = new nodeRecord(*noderec);
									if (newNodeRec != NULL)
									{
										newNodeRec->sSearchField = p.second;
										pair<string, nodeRecord *> myrecord(sTicketKey, newNodeRec); // organization_id, <_id,<,,,,,>>
										_mapTicketAndOrgRelations.insert(myrecord);
									}
									break;
								}
							}
						}
						itr2++;
					}
				}
			}
		}
		itr1++;
	}
	return 0;
}

int searchApp::buildTicketRelationsIndexData()
{
	// Ticket relations
	// Ticket _id from Ticket, assignee_id from User
	// Ticket _id from Ticket, _id from Organization

	unordered_multimap<string, nodeRecord *>::const_iterator itTicket1;
	
	for (itTicket1 = _mapTickets.begin(); itTicket1 != _mapTickets.end(); itTicket1++)
	{
		nodeRecord *noderec = itTicket1->second;
		string sDataField = noderec->sSearchField;

		if ((sDataField == "_id") && noderec != NULL)
		{
			string sTicketKey = itTicket1->first;
			//cout << "key: " << sDataField << "key Value: " << sTicketKey << endl;

			vector<pair<string, string>> *v = noderec->v;
			vector<pair<string, string>>::iterator it;
			for (it = v->begin(); it != v->end(); it++)
			{
				pair<string, string> p = *it;
				if (p.first == "assignee_id")
				{
					string sUserKey = p.second;
					//cout << "key: " << sDataField << "key Value: " << sOrgKey << endl;
					buildTicketRelIdxData(_mapUsers, sUserKey, sTicketKey);
				}
			}
		}
	}

#ifdef DEBUG_PRINT_INTERNAL_1
	printMap(_mapTicketAndUserRelations, "Ticket Users Relations Map");
	printMap(_mapTicketAndOrgRelations, "Ticket Orgs Relations Map");
#endif

#ifdef DEBUG_PRINT_INTERNAL_2
	cout << "Tickets and Users Relations Map Size: " << _mapTicketAndUserRelations.size() << endl;
	cout << "Tickets and Orgs Relations Map Size: " << _mapTicketAndOrgRelations.size() << endl;
#endif

	return 0;
}

int searchApp::buildOrgRelIdxData(unordered_multimap<string, nodeRecord *> &map, string searchCriteria)
{
	int dupResultsUsers, dupResultsTickets = 0;

	auto itr1{ map.find(searchCriteria) };

	dupResultsUsers = map.count(searchCriteria);

	for (int i = 0; i < dupResultsUsers; i++)
	{
		nodeRecord *noderec = itr1->second;
		string s = noderec->sSearchField;

		// Insert user info -> org_id, user_id, info vector
		if ((s == "organization_id") && (noderec->sActSearchValue == searchCriteria))
		{
			vector<pair<string, string>> *v = noderec->v;
			vector<pair<string, string>>::iterator it;
			for (it = v->begin(); it != v->end(); it++)
			{
				pair<string, string> p = *it;
				//cout << "key: " << p.first << " keyValue: " << p.second << endl;

				if (p.first == "_id")
				{
					nodeRecord *newNodeRec = new nodeRecord(*noderec);
					if (newNodeRec != NULL)
					{
						newNodeRec->sSearchField = p.second;
						pair<string, nodeRecord *> myrecord(searchCriteria, newNodeRec); // organization_id, <_id,<,,,,,>>
						_mapOrgAndUserRelations.insert(myrecord);
					}

					auto itr2{ _mapTickets.find(p.second) };

					dupResultsTickets = _mapTickets.count(p.second);

					for (int ii = 0; ii < dupResultsTickets; ii++)
					{
						nodeRecord *noderec = itr2->second;
						string s = noderec->sSearchField;

						// Insert user info -> org_id, user_id, info vector
						if ((s == "assignee_id") && (itr2->first == p.second))
						{
							vector<pair<string, string>> *v = noderec->v;
							vector<pair<string, string>>::iterator it;
							for (it = v->begin(); it != v->end(); it++)
							{
								pair<string, string> p = *it;
								if (p.first == "_id")
								{
									nodeRecord *newNodeRec = new nodeRecord(*noderec);
									if (newNodeRec != NULL)
									{
										newNodeRec->sSearchField = p.second;
										pair<string, nodeRecord *> myrecord(searchCriteria, newNodeRec); // organization_id, <_id,<,,,,,>>
										_mapOrgAndTicketRelations.insert(myrecord);
									}
									break;
								}
							}
						}
						itr2++;
					}
				}
			}
		}
		itr1++;
	}
	return 0;
}

int searchApp::buildOrgRelationsIndexData()
{
	// Organization relations
	// Organization _id from Organization, user _id from Users

	// Organization _id from Organization, ticket _id from Tickets
	//unordered_map<int, unordered_multimap<string, nodeRecord *>::iterator *> _mapOrgAndTicketRelations;

	unordered_multimap<string, nodeRecord *>::const_iterator itOrg;

	for (itOrg = _mapOrganization.begin(); itOrg != _mapOrganization.end(); itOrg++)
	{
		nodeRecord *noderec = itOrg->second;
		string sDataField = noderec->sSearchField;

		if ((sDataField == "_id") && noderec != NULL)
		{
			string sOrgKey = itOrg->first;
			//cout << "key: " << sDataField << "key Value: " << sOrgKey << endl;
			
			buildOrgRelIdxData(_mapUsers, sOrgKey);
		}
	}

#ifdef DEBUG_PRINT_INTERNAL_1
	printMap(_mapOrgAndUserRelations, "Org Users Relations Map");
	printMap(_mapOrgAndTicketRelations, "Org Tickets Relations Map");
#endif

#ifdef DEBUG_PRINT_INTERNAL_2
	cout << "Orgs and Users Relations Map Size: " << _mapOrgAndUserRelations.size() << endl;
	cout << "Orgs and Tickets Relations Map Size: " << _mapOrgAndTicketRelations.size() << endl;
#endif

	return 0;
}

int searchApp::printRelatedIndexData(unordered_multimap<string, nodeRecord *>&_map, string sKey, string sComment)
{
	int dupResults = 0;
	
	auto itr{ _map.find(sKey) };

	dupResults = _map.count(sKey);

#ifndef DISABLE_PRINT_FOR_PERF_MEAS

	cout << endl << "Number of related records for " << sComment << " : " << dupResults;
	for (int i = 0; i < dupResults; i++)
	{
		nodeRecord *noderec = itr->second;
		string s = noderec->sSearchField;

		string sPrint = sComment + " record - " + to_string(i + 1);
		noderec->printNodeRecord(noderec, sPrint);
		itr++;
	}
#endif
	return 0;
}

int searchApp::createDataFieldList(unordered_multimap<string, nodeRecord *>_map, vector<string> &vlistOfDataFields)
{
	unordered_multimap<string, nodeRecord *>::const_iterator itr1;

	for (itr1 = _map.begin(); itr1 != _map.end(); itr1++)
	{
		nodeRecord *noderec = itr1->second;
		
		vector<pair<string, string>> *v = noderec->v;

			vector<pair<string, string>>::iterator itr2;
			for (itr2 = v->begin(); itr2 != v->end(); itr2++)
			{
				pair<string, string> p = *itr2;

				vector<string>::iterator itr3;
				itr3 = find(vlistOfDataFields.begin(), vlistOfDataFields.end(), p.first);
				if(itr3 == vlistOfDataFields.end())
					vlistOfDataFields.emplace_back(p.first);
			}
	}

	return 0;
}

int searchApp::DisplayFields(vector<string> vlistOfDataFields)
{
	vector<string>::iterator itr;
	//int i = 1;
	for (itr = vlistOfDataFields.begin(); itr != vlistOfDataFields.end(); itr++)
	{
		cout << *itr << endl;
	}

	return 0;
}

int searchApp::DisplayDataFields(eIndexType idxType)
{
	if (idxType == USERS)
	{
		DisplayFields(listOfUserDataFields);
	}
	else
		if (idxType == TICKETS)
		{
			DisplayFields(listOfTicketDataFields);
		}
		else
			if (idxType == ORGS)
			{
				DisplayFields(listOfOrgDataFields);
			}

	return 0;
}

int searchApp::DeleteMapData(unordered_multimap<string, nodeRecord *> &_map)
{
	unordered_multimap<string, nodeRecord *>::const_iterator itr1;

	for (itr1 = _map.begin(); itr1 != _map.end(); itr1++)
	{
		nodeRecord *noderec = itr1->second;
		delete noderec; noderec = NULL;
	}

	return 0;
}
