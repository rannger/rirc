#include "channel.h"
#include "macro.h"
#include "PrivateMsg.h"
#include <assert.h>
#include <map>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>


typedef std::map<str_t,rirc::Channel*> channel_map_t;
static channel_map_t *g_channelMap = NULL;

namespace rirc
{
	Channel::Channel() 
	{
		assert(0);	
	}

	Channel::Channel(const str_t& name)
	:m_name(name)
	{
	}

	Channel::~Channel()
	{
		for(int i=0;i<m_messageList.size();++i) {
			PrivateMessage* msg = m_messageList.at(i);
			delete msg;
		}
	}


	void Channel::addMessage(rirc::PrivateMessage* msg)
	{
		m_messageList.push_back(msg);
	}

	static channel_map_t* channeMapSingleInstance(void) 
	{
		__ASSERT_MAIN_THREAD
		if (NULL == g_channelMap) {
			g_channelMap = new channel_map_t();
		}
		return g_channelMap;
	}

	Channel* channel4Name(const str_t& name)
	{
		__ASSERT_MAIN_THREAD
		if (channeMapSingleInstance()->count(name)==0) {
			Channel* channel = new Channel(name);
			channeMapSingleInstance()->insert(std::pair<str_t,rirc::Channel*>(name,channel));
		}
		return channeMapSingleInstance()->at(name);
	}

	void clearChannelMap(void)
	{
		__ASSERT_MAIN_THREAD
		std::vector<rirc::Channel*> v(channeMapSingleInstance()->size());
		for(auto const &ent : *channeMapSingleInstance()) {
			  v.push_back(ent.second);
		}
		for(int i = 0;i<v.size();++i) {
			rirc::Channel* ch = v.at(i);
			delete ch;
		}
		channeMapSingleInstance()->clear();
		delete channeMapSingleInstance();
	}
}



