#pragma once

#ifndef MATCH4_SERVERMODULE_H
#define MATCH4_SERVERMODULE_H

namespace Match4
{
	typedef size_t ServerHandle;
	ServerHandle createServer();
	void destroyServer(ServerHandle handle);
	void awaitResponse(ServerHandle handle, int* msg);
	bool checkResponse(ServerHandle handle, int* msg);
	void sendMessage(ServerHandle handle, int* msg);
	int getMessageType(const char* typeName);
}

#endif // !MATCH4_SERVERMODULE_H
