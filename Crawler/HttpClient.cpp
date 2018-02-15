#include "HttpClient.h"
#include "List.h"
#include "Tree.h"
#include "Term.h"
#include "TermInFile.h"
#include "TemporaryTerm.h"
#include "ListNode.h"
#include "Main.h"
#include <cctype>

Http::Http() {}

Http::Http(std::string url, Http *parent, int depth)
{
	if (!(url.substr(url.length() - 3, url.length() - 1) == "pdf")) //check if file is pdf format
	{
		_depth = depth - 1; //decrease the depth by 1
		this->parent = parent; //set the parent
		if (_depth >= 0) //check for depth
		{
			_url = url; //set the url
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			{
				std::cout << "WSAStartup failed.\n";
				throw("fail");
			}

			bool pageIndexed = false;
			ListNode<IndexedPage> *tempLink = Main::indexedPages.startPtr; //set iterator for indexed pages and check if this page 
			while (tempLink != NULL)										// is already indexed			
			{
				if (tempLink->data.pageName == url)
				{
					pageIndexed = true;
				}
				tempLink = tempLink->nextPtr;
			}
			if (!pageIndexed) // if page isnt indexed, index it
			{
				Main::Pages.insert(*this);
				this->id = ++Main::id;
				IndexedPage *indexedPage = new IndexedPage();
				indexedPage->id = this->id;
				indexedPage->pageName = url;
				Main::indexedPages.insert(*indexedPage);
				splitURL();
				connectSocket();
			}
		}
	}
}

void Http::splitURL()
{
	/*
		move index if theres http or www
	*/
	bool isServNameFinished = false;
	bool isHttpBegin = false;
	std::string tempHost = "";
	std::string httpString = "http://";
	std::string wwwString = "www.";
	int index = 0;

	//check if theres http string at the beginning
	if (httpString == _url.substr(index, httpString.length()))
	{
		index += httpString.length();
		isHttpBegin = true;
	}
	//check if theres www string
	if (wwwString == _url.substr(index, wwwString.length()))
	{
		index += wwwString.length();
	}

	//if it has http beginning change the hostname
	if (isHttpBegin)
	{
		while ((_url[index] != '\0') && (_url[index] != ':') && (_url[index] != '/'))
		{
			tempHost += _url[index];
			index++;
		}
		hostname = tempHost;

		//get port if its specified
		if (_url[index] == ':')
		{
			index++;
			port = "";
			while ((_url[index] != '/') && (_url[index] != '\0'))
			{
				port += _url[index];
				index++;
			}
		}
		//current symbol should be '/' so we increment to get the GET request
		if (_url[index] == '/')
		{
			index++;
		}
		
		while (_url[index] != '\0')
		{
			request += _url[index];
			index++;
		}
	}
	else // if no http beginning get the request and set hostname and port from parent
	{
		while (_url[index] != '\0')
		{
			request += _url[index];
			index++;
		}
		hostname = parent->hostname;
		port = parent->port;
	}
}

addrinfo* Http::getAddressIp()
{
	struct addrinfo hints, *res = NULL, *resFail = NULL;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	const char *serverName = hostname.c_str();
	int result = getaddrinfo(serverName, port.c_str(), &hints, &res); //get the server ip address

	if (result != 0)
	{
		printf("getaddrinfo failed with error: %d\n", result);
		WSACleanup();
		return resFail;
		//throw std::invalid_argument("null address");
	}

	return res;
}

void Http::connectSocket()
{
	addrinfo *res, *ptr;
	res = getAddressIp();
	int result;

	for (ptr = res; ptr != NULL; ptr = ptr->ai_next)
	{
		sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol); //create socket
		//sock = WSASocket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol, NULL, 0, 0);

		if (sock == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			throw std::invalid_argument("socket failed");
		}

		result = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen); //connect to server
		if (result == SOCKET_ERROR)
		{
			closesocket(sock);
			sock = INVALID_SOCKET;
			continue;
		}
		else //if everything is ok socket will connect
		{
			std::cout << "connected";
			getHTML(); //start getting text from server
		}
		break;
	}

	if (sock == INVALID_SOCKET) //socket could not connect
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
	}
}

void Http::getHTML()
{
	int bufSize = 1024; //size of a received msg
	int result;
	
	std::string requestDeli = "\r\n";
	std::string get = "GET /";
	std::string version = " HTTP/1.1";
	std::string hostString = "Host: ";

	//request sent to the server
	std::string msgString = get + request + version + requestDeli + hostString + hostname + requestDeli + "Accept-Charset: iso-8859-1, unicode-1-1;q=0.8\r\n" + "Connection: close\r\n\r\n";
	result = send(sock, msgString.c_str(), msgString.size(), 0); // result from the server

	if (result == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		throw("socket failed");
	}
	//continue to receive msg from the server until the result is 0
	do {
		char *recvbuf = new char[bufSize];
		int a = strlen(recvbuf);
		result = recv(sock, recvbuf, bufSize, 0);
		if (result > 0)
		{
			std::string tempstring(recvbuf, result);
			textHTML += tempstring; //concatinate the server response
			printf("Bytes received: %d\n", result);
			
		}
		else if (result == 0)
		{
			printf("Connection closed\n");
			break;
		}
		else
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
		}

		delete[] recvbuf;
	} while (result > 0);

	getTextInfo(textHTML);
}

void Http::getTextInfo(std::string text)
{
	// predefine some search strings
	std::string href = "href=";
	std::string html = "<html";
	std::string keywords = "keywords";
	std::string content = "content";
	std::string meta = "meta";
	std::string word = "";
	int lines = 0;
	int wordCount = 0; //word count in this document
	int index = 0; //current char index of the string

	index = text.find(html); //find the start index of the html document
	if (index == -1)
	{
		index = 0;
	}
	while (text[index] != '\0') //start iterating through the document
	{
		if (text[index] == '\n') //count lines
		{
			lines++;
		}
		if (text[index] == '<') //check if a tag starts
		{
			if (text[index + 1] == '!') //check if a comment starts
			{
				while (1) // iterate through comment
				{
					if (text[index] == '-' && text[index + 1] == '-' && text[index + 2] == '>')
					{
						index = index + 3;
						break;
					}
					index++;
				}
				continue;
			}
			bool linkFound = false;
			while (text[index] != '>') // run until a '>' is reached
			{
				if (linkFound) //if a link is found iterate until '>' is found
				{
					index++;
					continue;
				}
				else if (text.substr(index, href.length()) == href) //check for link in this tag
				{
					index += href.length() + 1; // move the index
					std::string link = "";
					while (text[index] != '"') // get the link
					{
						link += text[index];
						index++;
					}
					links.insert(link); //push the link into the list with links
					linkFound = true; //set that a link is found in this tag
				}
				else if (text.substr(index, meta.length()) == meta)
				{
					index += meta.length() + 1;
					bool keywordsFound = false;
					while (text[index] != '>')
					{
						if (!keywordsFound && text.substr(index, keywords.length()) == keywords)
						{
							index += keywords.length() + 1;
							keywordsFound = true;
						}
						if (keywordsFound)
						{
							//if (text.substr(index, content.length()) == content)
							//{
							//	index += content.length() + 1;
							//	
							//	while (text[index] != '>')
							//	{
							//		if (text[index] == ' ' || text[index] == '\"' || text[index] == ',')
							//		{
							//			index++;

							//			word = "";
							//		}
							//		else
							//		{
							//			word += text[index];
							//		}
							//	}
							//}
						}
						index++;
					}
					break;
				}
				index++;
			}
		}

		//WORD SEPARATORS
		if (text[index] == ' ' || text[index] == '\n' || text[index] == '\t' || text[index] == ':' || text[index] == ';' ||
			text[index] == ',' || text[index] == '.' || text[index] == '\r' || text[index] == '\<' || text[index] == '\>' ||
			text[index] == '\&' || text[index] == '-' ||text[index] == '(' || text[index] == ')' || text[index] == '!' || text[index] == '/' ||
			text[index] == '?' || text[index] == '"')
			{
				if (word.length() > 0) // if there is a word -> push it into the list
				{
					for (int i = 0; i < word.length(); i++)
					{
						word[i] = std::tolower(word[i]); // set the word to lower
					}
					wordCount++; // increment the word count
					TemporaryTerm *tempTerm = new TemporaryTerm(); //create a temporary term for this document that will be inserted into the tree
					tempTerm->term = word;
					tempTerm->fileName = _url;
					tempTerm->positions.insert(wordCount);
					
					TemporaryTerm *termFound = (TemporaryTerm*)temporaryTerms.search(*tempTerm); //check if the term exists already in the list
					if (termFound != NULL)
					{
						termFound->positions.insert(wordCount);
						termFound->termCount++;
					}
					else
					{
						temporaryTerms.insert(*tempTerm);
					}
					words.insert(word);
				}
				word = "";
				index++;
				continue;
			}

		word += text[index];
		index++;
	}

	ListNode<std::string> *currentLink = links.startPtr; //create links iterator
	//create new http objects to index the links in this page
	while (currentLink != NULL)
	{
		std::string link = currentLink->data;
		Http *http = new Http(currentLink->data, this, _depth);
		currentLink = currentLink->nextPtr;
	}

	indexTerms(temporaryTerms, this->id); //put temporary terms from this document in the tree

}

void Http::indexTerms(List<TemporaryTerm> tempTerms, int id)
{
	
	ListNode<TemporaryTerm> *tempTerm = tempTerms.startPtr; //create iterator
	while (tempTerm != NULL)
	{
		TemporaryTerm insertTerm = tempTerm->data; //set data
		insertTerm.id = id;
		Main::tree.insert(Main::tree.rootTerm, &insertTerm); //push to tree
		tempTerm = tempTerm->nextPtr; //move iterator
	}
}

