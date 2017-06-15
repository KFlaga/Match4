#include "Server.h"
#include <vector>
#include <algorithm>
#include <thread>
#include "AiRandom.h"
#include "AiAlfaBeta.h"
#include "Game.h"

namespace Match4
{
	Server* getServerFromHandle(ServerHandle handle)
	{
		return reinterpret_cast<Server*>(handle);
	}

	ServerHandle getHandleFromServer(Server* server)
	{
		return reinterpret_cast<ServerHandle>(server);
	}

	Server::Server() :
		game_(new Game(this)),
		serverThread_(new std::thread([this] { this->run(); }))
	{
		messageProcessors_ = MessageHandlerMap{
			{ MessageType::ReqHover, [](Game* game, const Message& msg) { game->processHoverRequest(msg); } },
			{ MessageType::ReqSelect, [](Game* game, const Message& msg) { game->processSelectRequest(msg); } },
			{ MessageType::ReqSetGameSettings, [](Game* game, const Message& msg) { game->processSettingsRequest(msg); } },
			{ MessageType::ReqNextPlayer, [](Game* game, const Message& msg) { game->processNextPlayerRequest(msg); } }
		};
	}

	Server::~Server()
	{
		running_ = false;
		if (serverThread_->joinable())
			serverThread_->join();
	}

	void Server::run()
	{
		while (running_)
		{
			update();
			std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(20));
		}
	}

	void Server::update()
	{
		// If have any request - process it
		while (internalQueue_.haveMore())
		{
			Message req = internalQueue_.pop();
			processRequest(req);
		}

		while (requestQueue_.haveMore())
		{
			Message req = requestQueue_.pop();
			processRequest(req);
		}
	}

	void Server::processRequest(const Message& request)
	{
		auto processor = messageProcessors_.find((MessageType)request.type);
		if (processor != messageProcessors_.end())
		{
			processor->second(game_.get(), request);
		}
	}

	void Server::pushRequest(const Message& request)
	{
		requestQueue_.push(request);
	}

	void Server::pushInternal(const Message& request)
	{
		internalQueue_.push(request);
	}

	void Server::pushResponse(const Message& request)
	{
		responseQueue_.push(request);
	}

	bool Server::haveResponesPending()
	{
		return responseQueue_.haveMore();
	}

	Message Server::getNextResponse()
	{
		return responseQueue_.pop();
	}

	ServerHandle createServer()
	{
		Server* server = new Server();

		ServerHandle handle = getHandleFromServer(server);
		return handle;
	}

	void destroyServer(ServerHandle handle)
	{
		Server* server = getServerFromHandle(handle);
		delete server;
	}

	void awaitResponse(ServerHandle handle, int* msg)
	{
		Server* server = getServerFromHandle(handle);

		while (!server->haveResponesPending())
		{
			std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(20));
		}
		Message response = server->getNextResponse();
		Message::toByteStream(&response, (char*)msg);
	}

	void sendMessage(ServerHandle handle, int* msg)
	{
		Server* server = getServerFromHandle(handle);
		Message request;
		Message::fromByteStream(&request, (char*)msg);
		server->pushRequest(request);
	}

	int getMessageType(const char* typeName)
	{
		return (int)Message::getTypeFromName(typeName);
	}

	namespace MessageNames
	{
		std::map<std::string, MessageType>& GetNamesMap()
		{
			static std::map<std::string, MessageType>  map;
			map["NoMessage"] = MessageType::NoMessage;
			map["Unknown"] = MessageType::Unknown;
			map["ReqHover"] = MessageType::ReqHover;
			map["ReqSelect"] = MessageType::ReqSelect;
			map["ReqSetGameSettings"] = MessageType::ReqSetGameSettings;
			map["ReqNextPlayer"] = MessageType::ReqNextPlayer;
			map["ReqEndGame"] = MessageType::ReqEndGame;
			map["RespConfirm"] = MessageType::RespConfirm;
			map["RespNextPlayer_Human"] = MessageType::RespNextPlayer_Human;
			map["RespNextPlayer_Cpu"] = MessageType::RespNextPlayer_Cpu;
			map["RespGoodMove"] = MessageType::RespGoodMove;
			map["RespBadMove"] = MessageType::RespBadMove;
			map["RespWinMove"] = MessageType::RespWinMove;
			map["RespField"] = MessageType::RespField;
			map["RespDraw"] = MessageType::RespDraw;
			return map;
		}
	}

	MessageType Message::getTypeFromName(const char* typeName)
	{
		static auto& map = MessageNames::GetNamesMap();
		auto typeIt = map.find(std::string(typeName));
		if (typeIt != map.end())
		{
			return typeIt->second;
		}
		return MessageType::Unknown;
	}
}