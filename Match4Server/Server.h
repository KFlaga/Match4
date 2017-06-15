#pragma once

#ifndef MATCH4_SERVER_H
#define MATCH4_SERVER_H

#include "MessageQueue.h"
#include "Board.h"
#include "ServerModule.h"
#include <map>
#include <memory>

namespace std
{
	class thread;
}

namespace Match4
{
	class Game;
	class Server
	{
	private:
		typedef std::map <MessageType, void(*)(Game*, const Message&)> MessageHandlerMap;
		MessageHandlerMap messageProcessors_;

		MessageQueue requestQueue_;
		MessageQueue responseQueue_;
		MessageQueue internalQueue_;

		std::unique_ptr<Game> game_;
		std::unique_ptr<std::thread> serverThread_;
		bool running_ = true;
	public:
		~Server();
		Server();

		void pushRequest(const Message& request);
		void pushResponse(const Message& response);
		void pushInternal(const Message& request);
		bool haveResponesPending();
		Message getNextResponse();

		Game* getGame() { return game_.get(); }

	private:
		void run();
		void update();
		void processRequest(const Message& request);
	};
}

#endif