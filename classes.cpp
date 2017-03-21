#include <iostream>
#include <string>
#include <deque>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

using namespace std;

//This file contains several classes used in main.cpp

class Configuration {

  public:
	map<string, string> data;
		
	void setDefaults() {
		data["PERIOD_FETCH"] = "180";
		data["NUM_FETCH"] = "1";
		data["NUM_PARSE"] = "1";
		data["SEARCH_FILE"] = "Search.txt";
		data["SITE_FILE"] = "Sites.txt";
	}

	void readFromFile(string filename) {
		setDefaults();
		size_t pos = 0;
		string line;
		string token;
		ifstream file;
		file.open(filename.c_str());
		if(file.is_open())
		{
			while(getline(file, line)) 
			{
				token = line.substr(0, line.find("="));
				line.erase(0, line.find("=") + 1);
				data[token] = line;
			}
		}
		else
		{ 
			cout << "Unable to open file " << filename << endl;
		}
	}
};

class Sites {
	
  public:
	deque<string> q;

	void readFromFile(string filename) {
		string line;
		ifstream file;
		file.open(filename.c_str());
		if(file.is_open())
		{
			while(getline(file, line)) 
			{
				q.push_front(line);
			}
		}
		else
		{ 
			cout << "Unable to open file " << filename << endl;
		}
	}
};

class Phrases {

  public:
	deque<string> q;
	
	void readFromFile(string filename) {
		string line;
		ifstream file;
		file.open(filename.c_str());
		if(file.is_open())
		{
			while(getline(file, line)) 
			{
				q.push_front(line);
			}
		}
		else
		{ 
			cout << "Unable to open file " << filename << endl;
		}
	}
};

