#include "classes.cpp"
#include <fstream>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

using namespace std;

bool flag = false;
int period = 180;

void setflag()
{
	flag = true;
	alarm(period);
}
	
int main()
{
	Phrases p;
	Sites s;
	Configuration c;
	Fetcher fetcher;
	Parser parser;

	c.readFromFile("configuration.txt");

	signal(SIGALRM, setflag);
	alarm(c.data["PERIOD_FETCH"]);

	p.readFromFile(c.data["SEARCH_FILE"]);

	while(1){

		if (flag){
			s.readFromFile(c.data["SITE_FILE"]);
			
			run = 1;   //this will increment every time the timer goes
			
			while(!sites.empty())
			{
				fetcher.fetch();
				parser.parse();

			}
			flag = false;
		}
	}
}


