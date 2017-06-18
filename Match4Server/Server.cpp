#include "Server.h"
#include <vector>
#include <algorithm>
#include <thread>
#include "AiRandom.h"
#include "AiAlfaBeta.h"
#include "Game.h"

#ifdef _WIN32
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#endif

namespace Match4
{
#ifdef _WIN32
	void showConsole()
	{
		 AllocConsole();

		HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
		int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
		FILE* hf_out = _fdopen(hCrt, "w");
		setvbuf(hf_out, NULL, _IONBF, 1);
		*stdout = *hf_out;

		HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
		hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
		FILE* hf_in = _fdopen(hCrt, "r");
		setvbuf(hf_in, NULL, _IONBF, 128);
		*stdin = *hf_in;
	}
#endif
	
	Server* getServerFromHandle(ServerHandle handle)
	{
		return reinterpret_cast<Server*>(handle);
	}

	ServerHandle getHandleFromServer(Server* server)
	{
		return reinterpret_cast<ServerHandle>(server);
	}

	Server::Server(bool enableLog) :
		game_(new Game(this)),
		enableLog_(enableLog),
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
		logMessage("server deleted!");
	}

	void Server::run()
	{
		logMessage("server run!");
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
		logMessage(std::string("Processing request: ") + std::to_string(request.type));
		auto processor = messageProcessors_.find((MessageType)request.type);
		if (processor != messageProcessors_.end())
		{
			processor->second(game_.get(), request);
		}
		else
		{
			logMessage(std::string("Processor not found!"));
		}
	}

	void Server::pushRequest(const Message& request)
	{
		requestQueue_.push(request);
		logMessage(std::string("Request pushed: ") + std::to_string(request.type));
	}

	void Server::pushInternal(const Message& request)
	{
		internalQueue_.push(request);
		logMessage(std::string("Internal request pushed: ") + std::to_string(request.type));
	}

	void Server::pushResponse(const Message& request)
	{
		responseQueue_.push(request);
		logMessage(std::string("Response pushed: ") + std::to_string(request.type));
	}

	bool Server::haveResponesPending()
	{
		return responseQueue_.haveMore();
	}

	Message Server::getNextResponse()
	{
		return responseQueue_.pop();
	}
	
	void Server::logMessage(const char* msg)
	{
		if(enableLog_)
		{
			printf(msg);
			printf("\n");
		}
	}
	
	void Server::logMessage(const std::string& msg)
	{
		if(enableLog_)
		{
			printf(msg.c_str());
			printf("\n");
		}
	}

	ServerHandle createServer()
	{
		Server* server = new Server();
		server->logMessage(std::string("Server created"));

		ServerHandle handle = getHandleFromServer(server);
		return handle;
	}

	void destroyServer(ServerHandle handle)
	{
		Server* server = getServerFromHandle(handle);
		server->logMessage(std::string("Server destroyed"));
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
	
	bool checkResponse(ServerHandle handle, int* msg)
	{
		Server* server = getServerFromHandle(handle);

		if (!server->haveResponesPending())
		{
			return false;
		}
		Message response = server->getNextResponse();
		Message::toByteStream(&response, (char*)msg);
		return true;
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