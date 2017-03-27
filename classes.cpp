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
#include <ctime>
#include <algorithm>

#include "libcurl.cpp"

using namespace std;

//This file contains several classes used in main.cpp

mutex fileLock;

//Queue to prevent threading from interferring 
template <typename T>
class SafeQueue {
	public:
		queue<T> q;
		mutex m;
		condition_variable cv;

	public:
		SafeQueue() {}

		/*void print() {
			queue<T>::iterator i;
			for(i = q.begin(); i != q.end(); i++)
			{
				cout << *i << " ";
			}
			cout << endl;
		}*/

		void push(T elem) {
			unique_lock<mutex> lock(m);
			q.push(elem);
			cv.notify_one();
		}

		T pop() {
			T s;
			
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

/*global variables*/
SafeQueue<string> sites;
deque<string> phrases;
SafeQueue<pair<string, string> > siteData;
int run = 1;
/* end global variables*/

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

	bool readFromFile(string filename) {
		setDefaults();
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
			return false;
		}
		return true;
	}
};

class Sites {
	
  public:
	//deque<string> q;
	//SafeQueue q;	

	bool readFromFile(string filename) {
		cout << "reading sites" << endl;
		string line;
		ifstream file;
		file.open(filename.c_str());
		if(file.is_open())
		{
			while(getline(file, line)) 
			{
				sites.push(line);   //push_front for deque
				cout << line << endl;
			}
		}
		else
		{ 
			cout << "Unable to open file " << filename << endl;
			return false;
		}
		return true;
	}
};

class Phrases {

  public:
	//deque<string> q;
	//SafeQueue q;	

	bool readFromFile(string filename) {
		string line;
		ifstream file;
		file.open(filename.c_str());
		if(file.is_open())
		{
			while(getline(file, line)) 
			{
				phrases.push_front(line);
			}
		}
		else
		{ 
			cout << "Unable to open file " << filename << endl;
			return false;
		}
		return true;
	}
};


class Parser {
	
  public:
	mutex fileLock;

	void * parse() {
		size_t pos, count;
		string filename = to_string(run) + ".csv";
		ofstream outputFile;
		outputFile.open(filename, fstream::app);
		pair<string, string> p = siteData.pop();
		string site = p.first;
		string data = p.second;
		string target;

		deque<string>::iterator i;
		// Iterate through each prhase and check if it is in HTML
		for(i = phrases.begin(); i != phrases.end(); i++) {
			time_t current = time(0);
			// display time in a readable format and not just numbers
			string legibletime = ctime(&current);
			legibletime.erase(std::remove(legibletime.begin(), legibletime.end(), '\n'), legibletime.end()); // getting rid of newline at end
			target = *i;
			pos = 0;
			count = 0;
			// check if phrases is in the data
			while(pos != -1) {
				pos = data.find(target, pos);
				if (pos == -1) break;
				pos++;
				count++;
			}
			fileLock.lock();
			outputFile << legibletime <<"," << target << "," << site << "," << count << endl;
			fileLock.unlock();
		}
	}
};
	
// function that receives HTML from specified site
void libcurl(string site) {
  CURL *curl_handle;
  CURLcode res;


  struct MemoryStruct chunk;

  chunk.memory = (char *)malloc(1);  /* will be grown as needed by the realloc above */
  chunk.size = 0;    /* no data at this point */

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl_handle = curl_easy_init();

  /* specify URL to get */
  curl_easy_setopt(curl_handle, CURLOPT_URL, site.c_str());

  //Checking if a timeout has occured
  curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, site.c_str());

  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  /* get it! */
  res = curl_easy_perform(curl_handle);

  /* check for errors */
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
  if(CURLE_OPERATION_TIMEDOUT == res)
  {
	  printf("Timeout has occurred\n");
	  string dat = "";
	  siteData.push(make_pair(site, dat));

  }
  else {
	string dat = (string)chunk.memory;
	siteData.push(make_pair(site, dat));
	cout << endl << "pushing data from site " << site << endl; 
  }

  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);

  free(chunk.memory);

  /* we're done with libcurl, so clean it up */
  curl_global_cleanup();

  //return 0;

}


class Fetcher {
	
	public:

	void * fetch() {
		string site = sites.pop();
		libcurl(site);	//this will push onto sitesData
	}

};
	


// function the pthread calls to fetch the site data
void * fetch(void * psomething) {
	string site = sites.pop();
	cout << "fetching " << site << endl;
	libcurl(site);	//this will push onto sitesData
}

// function that the pthread calls to parse the data from libcurl
void * parse(void * psomething) {
	cout << "PARSING SOON LOOK FOR SITE NAME " << endl;
	size_t pos, count;
	string filename = to_string(run) + ".csv";
	ofstream outputFile;
	outputFile.open(filename, fstream::app);
	pair<string, string> p = siteData.pop();
	string site = p.first;
	cout << "parsing " << site << endl;
	string data = p.second;
	string target;

	deque<string>::iterator i;
	// Iterate through each phrase and check if it is in HTML
	for(i = phrases.begin(); i != phrases.end(); i++) {
		time_t current = time(0);
		string legibletime = ctime(&current);
		// display time in a readable format and not just numbers
		legibletime.erase(std::remove(legibletime.begin(), legibletime.end(), '\n'), legibletime.end()); // getting rid of newline at end
		target = *i;
		pos = 0;
		count = 0;
		// check if phrases is in the data
		while(pos != -1) {
			pos = data.find(target, pos);
			if (pos == -1) break;
			pos++;
			count++;
		}
		fileLock.lock();
		cout << "writing " << site << " results to file" << endl;
		outputFile << legibletime <<"," << target << "," << site << "," << count << endl;
		fileLock.unlock();
	}
}
