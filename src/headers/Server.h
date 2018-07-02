#ifndef SERVER_H
#define SERVER_H
#include <list>
#include <iostream>
#include <random>


class Server
{
private:
	unsigned short wait;
	
public:
	Server();
	~Server();

	void add() {if(wait < 10) wait++;}
	void finish() {if(wait > 0) wait--;}
	short getWait() {return wait;}
};
#endif