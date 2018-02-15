#ifndef HTTP_H
#define HTTP_H

#pragma comment(lib, "Ws2_32")
#pragma comment(lib, "fwpuclnt.lib")
#pragma comment(lib, "ntdsapi.lib")

#include <iostream>
#include <string>
#include <winsock2.h>
#include <mstcpip.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <fstream>
#include <locale>
#include <rpc.h>
#include <ntdsapi.h>
#include "List.h"
#include "TemporaryTerm.h"

class Http
{
private:
	WSADATA wsaData;
	Http *parent; // the parent that spawned this object from a link(may be null if there is no parent)
	std::string hostname, request, port = "80"; //the server hostname, the request to the server and the port with default value 80
	std::string _url;// the url to the server
	const char *serverName; //
	struct addrinfo hints, *res = NULL, *ptr = NULL; //structs for the socket
	SOCKADDR_IN socketAddr; // the socket struct
	SOCKET sock = INVALID_SOCKET; // the socket used to connect to the server
	void indexTerms(List<TemporaryTerm>, int); // function to push the temporary terms into the main tree

public:
	int _depth = 0; // current depth of the document
	int id = 0; // id of the document
	std::string textHTML; // the text of the document
	List<std::string> links; // list of links in the document
	List<std::string> words; // all the words in the document
	List<TemporaryTerm> temporaryTerms;
	Http(); //default constructor
	Http(std::string url, Http *parent, int depth); // constructor with parameters to spawn new indexed pages
	addrinfo *getAddressIp(); //function to get the server ip address
	void connectSocket(); // fuction to connect the socket to the server
	void getHTML(); // get the html from the server
	void splitURL(); // parse the url into different components (hostname, request, port)
	void getTextInfo(std::string text); // parse the text (links, words)
};
#endif