#include "RequestClose.h"
#include "WSServer.h"

void RequestClose::process(){
	this->server->closeConnection(this->con);
}