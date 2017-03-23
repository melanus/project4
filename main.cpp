#include "classes.cpp"
#include <fstream>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

using namespace std;

bool flag = true;
int period = 180;

void setflag(int s)
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

	period = atoi(c.data["PERIOD_FETCH"].c_str());

	signal(SIGALRM, setflag);
	alarm(period);

	p.readFromFile(c.data["SEARCH_FILE"]);

	while(1){

		if (flag){
			s.readFromFile(c.data["SITE_FILE"]);
			
			while(!sites.empty())
			{
				fetcher.fetch();
				parser.parse();

			}
			flag = false;
			++run;
		}
	}
}


