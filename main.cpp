#include "classes.cpp"

#include "libcurl.cpp"

using namespace std;

int main()
{

	Phrases p;
	Sites s;
	Configuration c;

	c.readFromFile("configuration.txt");

	p.readFromFile(c.data["SEARCH_FILE"]);
	s.readFromFile(c.data["SITE_FILE"]);

	//check each word on each site
	deque<string>::iterator i;
	for(i = s.q.begin(); i != s.q.end(); i++)
	{
		libcurl(*i, p.q);
	}

}
