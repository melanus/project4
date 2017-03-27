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

#include "libcurl.cpp"

using namespace std;

//This file contains several classes used in main.cpp

template <typename T>
class SafeQueue {
	private:
		queue<T> q;
		mutex m;
		condition_variable cv;

	public:
		SafeQueue() {}

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
	//SafeQueue q;	

	static void readFromFile(string filename) {
		string line;
		ifstream file;
		file.open(filename.c_str());
		if(file.is_open())
		{
			while(getline(file, line)) 
			{
				sites.push(line);   //push_front for deque
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
	//deque<string> q;
	//SafeQueue q;	

	void readFromFile(string filename) {
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
		}
	}
};

/*class Fetcher {
	
	public:
		//SafeQueue data;	//this should be global
						//the site queue should also be global

	void * fetch() {
		string site = sites.pop();
		string html_data = libcurl(site);
		siteData.push(html_data);
	}

};*/

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
		for(i = phrases.begin(); i != phrases.end(); i++)
		{
			target = *i;
			pos = 0;
			count = 0;
			while(pos != -1)
			{
				pos = data.find(target, pos);
				if (pos == -1) break;
				pos++;
				count++;
			}
			fileLock.lock();
			outputFile << "Time," << target << "," << site << "," << count << endl;
			fileLock.unlock();
		}
	}
};
	
void libcurl(string site) {
  CURL *curl_handle;
  CURLcode res;

  size_t pos = 0;
  int count = 0;

  struct MemoryStruct chunk;

  chunk.memory = (char *)malloc(1);  /* will be grown as needed by the realloc above */
  chunk.size = 0;    /* no data at this point */

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl_handle = curl_easy_init();

  /* specify URL to get */
  curl_easy_setopt(curl_handle, CURLOPT_URL, site.c_str());

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
  else {
	string dat = (string)chunk.memory;
	siteData.push(make_pair(site, dat));
	/*string filename = to_string(run) + ".csv";
	ofstream outputFile;
	outputFile.open(filename, fstream::app);
	string data = (string)chunk.memory;
	string target;
	deque<string>::iterator i;
	for(i = phrases.begin(); i != phrases.end(); i++)
	{
		target = *i;
		pos = 0;
		count = 0;
		while(pos != -1)
		{
			pos = data.find(target, pos);
			if(pos == -1) { break; }
			pos++;
			count++;
		}
		outputFile << "Time," << target << "," << site << "," << count << endl;
		//cout << target << " found " << count << " times";
		//cout << " on site " << site << endl;
    	//printf("%lu bytes retrieved\n", (long)chunk.size);*/
	//}
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
		//SafeQueue data;	//this should be global
						//the site queue should also be global

	void * fetch() {
		string site = sites.pop();
		libcurl(site);	//this will push onto sitesData
	}

};
	


