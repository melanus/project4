#include "classes.cpp"

#include "libcurl.cpp"

using namespace std;

void libcurl(string site, deque<string> phrases) {
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
	string data = (string)chunk.memory;
	//cout << "data size is " << data.size() << endl;
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

		cout << target << " found " << count << " times";
		cout << " on site " << site << endl;
    	//printf("%lu bytes retrieved\n", (long)chunk.size);
	}
  }

  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);

  free(chunk.memory);

  /* we're done with libcurl, so clean it up */
  curl_global_cleanup();

  //return 0;

}

int main()
{

	Phrases p;
	Sites s;
	Configuration c;

	p.readFromFile("phrases.txt");
	s.readFromFile("sites.txt");
	c.readFromFile("configuration.txt");

	//check each word on each site
	deque<string>::iterator i;
	for(i = s.q.begin(); i != s.q.end(); i++)
	{
		libcurl(*i, p.q);
	}

	/*deque<string>::iterator i;
	for(i = p.q.begin(); i != p.q.end(); i++)
		cout << *i << endl;

	cout << endl;

	for(i = s.q.begin(); i != s.q.end(); i++)
		cout << *i << endl;

	cout << endl;

	map<string, string>::iterator it;
	for(it = c.data.begin(); it != c.data.end(); it++)
		cout << it->first << " " << it->second << endl;
	*/
}
