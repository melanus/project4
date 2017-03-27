# project4
Matthew Lanus - mlanus
Matthew Flanagan - mflanag6


Description:

In a very simplistic sense, our code goes through a websites searching for various words and counting how many times that phrase occurs in a website. This is accomplished through a variety of file and operations. Given a *configuartion* file with parameters (Period fetch, number of fetching threads, number of parsing threads, and the two search files of website and phrases) we parse through the config file and assign the values to a config class. Then we signal a timer that will throw a signal every time period and allow the program to execute the reading of search files and phrases as well as fetching and parsing the website data. 

Before we can read the site file, we create an array of two sets of threads, one for fetching the sites, and another for parsing the website data. Then we read the site file and perform the fetching and parsing. Finally we output it to a file and wait for the next period to execute the same fetching and parsing with a potentially updated website.

We created multiple classes - one for a safe threaded queue, for the configuartion file, the sites, the parser and the fetcher. We created a thread safe queue such that it would allow us to use threads without accidentally modifying the queue. We made it thread safe by creating mutex locks and signalling certain threads on certain conditions. We get the website data through a function and library called libcurl, which loads the HTML (the fetching part) and we parse through that by going through the whole HTML and comparing it to the phrases. We store the websites in a thread safe queue so the threads can read mutliple sites at once. We also put the HTML data into a thread safe queue so the second set of threads can also read that at the same time, thus increasing effeciency. Specifically, we called the threads on the fetch and parse functions.


The program does have a few parameters, particularly on threading. It will not take 0 and more than 8 threads. If it receives that in the configuration file, the program will exit and display an error message.


