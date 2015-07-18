// bisign2bikey v1.01
// Copyright © 2015 AgentRev
// Licensed under GNU GPL v3

#include <fstream>
#include <iostream>
#include <string>

#ifdef _MSC_VER
#pragma warning (disable : 4996)
#endif

#define BIKEY_LENGTH 0x99

using namespace std;

int main(int argc, char* argv[])
{
	string strBisignPath;
	string strBikeyDest;

	if (argc < 2)
	{
		cout << "Usage: bisign2bikey bisign_path [bikey_destination]" << "\n\n"
		     << "bisign2bikey v1.01 - Copyright (c) 2015 AgentRev - Licensed under GNU GPL v3" << "\n";
		return 1;
	}
	else
		strBisignPath = argv[1];

	if (argc > 2)
		strBikeyDest = argv[2];

	if (strBisignPath.compare(strBisignPath.length() - 7, 7, ".bisign") != 0)
	{
		cout << "Error: Not a .bisign file" << "\n";
		return 10;
	}

	fstream fsBisign;
	fsBisign.open(strBisignPath, ios::in | ios::binary);

	if (fsBisign.fail())
	{
		cout << "Error: Unable to read .bisign" << "\n";
		return 20;
	}

	string strAuthority;

	while (fsBisign.peek() != 0 && !fsBisign.eof() && !fsBisign.fail())
		strAuthority += fsBisign.get();

	int iAuthLen = strAuthority.length();
	int p_cBikeyLen = iAuthLen + BIKEY_LENGTH;
	char* p_cBikey = new char[p_cBikeyLen];

	strncpy(p_cBikey, strAuthority.c_str(), iAuthLen);

	fsBisign.read(p_cBikey + iAuthLen, BIKEY_LENGTH);
	streamsize iReadLen = fsBisign.gcount();
	fsBisign.close();

	if (iReadLen < BIKEY_LENGTH)
	{
		cout << "Error: File too short" << "\n";
		return 30;
	}

	if (strBikeyDest.empty())
		strBikeyDest = strAuthority + ".bikey";

	fstream fsBikey;
	fsBikey.open(strBikeyDest.c_str(), ios::out | ios::trunc | ios::binary);

	if (fsBikey.fail())
	{
		cout << "Error: Unable to write .bikey" << "\n";
		return 40;
	}

	fsBikey.write(p_cBikey, p_cBikeyLen);
	bool bWriteFail = fsBikey.fail();
	fsBikey.close();

	if (bWriteFail)
	{
		cout << "Error: Problem writing .bikey" << "\n";
		return 50;
	}

	cout << "bikey successfully written to \"" << strBikeyDest << "\"" << "\n";
	return 0;
}
