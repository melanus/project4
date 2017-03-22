#include <iostream>
#include <string>
#include <deque>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <mutex>
#include <queue>
#include <condition_variable>

using namespace std;

//This file contains several classes used in main.cpp

class SafeQueue {
	private:
		queue<string> q;
		mutex m;
		condition_variable cv;

	public:
		SafeQueue() {}

		void push(string elem) {
			unique_lock<mutex> lock(m);
			q.push(elem);
			cv.notify_one();
		}

		string pop() {
			string s;
			
			unique_lock<mutex> lock(m);
			while(q.empty())
				cv.wait(lock);

			if(!q.empty()) 
			{
				s = q.front();
				q.pop();
			}

			return s;
		}

		bool empty() {
			return q.empty();
		}
};

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
	//deque<string> q;
	SafeQueue q;	

	static void readFromFile(string filename) {
		string line;
		ifstream file;
		file.open(filename.c_str());
		if(file.is_open())
		{
			while(getline(file, line)) 
			{
				q.push(line);   //push_front for deque
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
	//SafeQueue q;	

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



