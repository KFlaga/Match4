#pragma once

#ifndef MATCH4_MESSAGEQUEUE_H
#define MATCH4_MESSAGEQUEUE_H

#include <queue>
#include "Message.h"

namespace Match4
{
	class MessageQueue
	{
		std::queue<Message> list_;
		int size_ = 0;

	public:
		void push(const Message& val)
		{
			list_.push(val);
			++size_;
		}

		Message pop()
		{
			--size_;
			Message v = list_.front();
			list_.pop();
			return v;
		}

		bool haveMore()
		{
			return size_ > 0;
		}

		void clear()
		{
			size_ = 0;
			while(!list_.empty())
				list_.pop();
		}
	};
}

#endif