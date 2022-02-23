# searchApp
Search App is implemented to search records and related objects from JSON files.

-----------------------------------------------------------------------------------------------------------------
A. Compile and Build
-----------------------------------------------------------------------------------------------------------------
1. This project was compiled and build in VS 2017
2. To build the code you will need Boost library. Download boost library from https://www.boost.org/doc/libs/1_66_0/more/getting_started/windows.html#get-boost
3. Configure VS 2017 with settings at above link in section "4.1   Build From the Visual Studio IDE"

To build the files in this project
1. Create a new Windows 32 console project in VS 2017.
2. Copy the content of main.cpp to your project file containing int main() method.
3. Copy the include and src folder to you project directory.
4. Compile and build the code.

-----------------------------------------------------------------------------------------------------------------
B. To run the standalone exe in folder Application-EXE
-----------------------------------------------------------------------------------------------------------------
1. Download the exe from folder Application-EXE.
2. Download Coding-Challenge-Solution.docx file from docs folder.
3. Go to Test Cases section and download the input JSON files and store them in a folder.
4. Run the EXE. Run the test case Test-1. You will be prompted for inputs as shown below.

--------------------------------------------------------------------------
--------------------------- SEARCH APPLICATION ---------------------------
Enter folder path for JSON records:
c:\temp\jsonfiles\file\input-01
Path entered by user: c:\temp\jsonfiles\file\input-01
Building search index.
JSON file with user data. c:\temp\jsonfiles\file\input-01\users.json
JSON file with ticket data. c:\temp\jsonfiles\file\input-01\tickets.json
JSON file with organization data. c:\temp\jsonfiles\file\input-01\organizations.json
Search index is built successfully. Please proceed with search of records.
--------------------------------------------------------------------------
Enter search record type -> 1 for User, 2 for Ticket, 3 for Organization
1
Select one of the below data fields to get user records:
_id
url
external_id
name
alias
created_at
active
verified
shared
locale
timezone
last_login_at
email
phone
signature
organization_id
tags
suspended
role
_id
Select a value for sSearchField:
74
--------------------------------------------------------------------------

Search for value "74" against field "_id" in record of type 1.

--------------------------------------------------
User record - 1
--------------------------------------------------
_id      : 74
url      : http://initech.zendesk.com/api/v2/users/74.json
external_id      : 8fa4f74b-e690-4478-bf09-40fed1ebc417
name     : Melissa Bishop
alias    : Mr Katharine
....
