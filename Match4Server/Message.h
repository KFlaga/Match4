#pragma once

#ifndef MATCH4_MESSAGE_H
#define MATCH4_MESSAGE_H

#include <string>

namespace Match4
{
	enum MessageType
	{
		Unknown = 0,
		NoMessage = 1,

		ReqHover, // msg data: { column, 0, 0 } -> response is good/bad move
		ReqSelect, // msg data: { column, 0, 0 } -> response is good/bad move and win/draw/next_player
		ReqSetGameSettings, // message data: { P1:(-1, 0:n), P2:(-1, 0:n), 0 } -> Px:(-1, 0:n) is difficulty setting for cpu x-th player or -1 if its human, followed by 'RespConfirm'
		ReqNextPlayer, // msg data: { player, 0, 0 }
		ReqEndGame, // msg data: { 0, 0, 0 }

		RespConfirm, // msg data: { 0, 0, 0 }
		RespNextPlayer_Human,  // msg data: { player, 0, 0 }
		RespNextPlayer_Cpu,  // msg data: { player, 0, 0 } -> followed by 'RespGoodMove' with cpu move and win/draw/next_player
		RespGoodMove, // msg data: { column, 0, 0 }
		RespBadMove, // msg data: { column, 0, 0 }
		RespWinMove, // msg data: { player, 0, 0 } -> after win_move comes 2 'RespField' messages with match start/end
		RespField, // msg data: { row, column, 0 }
		RespDraw, // msg data: { 0, 0, 0 }
	};

	struct Message
	{
		int type;
		union 
		{
			int data[3];
			struct
			{
				int column;
			private:
				int unused[2];
			} columnData;
		};

		static void toByteStream(const Message* msg, char* stream)
		{
			std::memcpy(stream, (char*)msg, sizeof(Message));
		}

		static void fromByteStream(Message* msg, const char* stream)
		{
			std::memcpy((char*)msg, stream, sizeof(Message));
		}

		static MessageType getTypeFromName(const char* name);
	};
}

#endif
