#include "classes.cpp"

#include "libcurl.cpp"

using namespace std;

int main()
{
	string site;

	Phrases p;
	Sites s;
	Configuration c;

	c.readFromFile("configuration.txt");

	p.readFromFile(c.data["SEARCH_FILE"]);
	s.readFromFile(c.data["SITE_FILE"]);

	//check each word on each site
	//deque<string>::iterator i;
	while(!s.q.empty()) 
	{
		site = s.q.pop();
		libcurl(site, p.q);
	}

	//for(i = s.q.begin(); i != s.q.end(); i++)
	//{
	//	libcurl(*i, p.q);
	//}

}
