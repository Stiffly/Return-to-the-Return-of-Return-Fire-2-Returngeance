#ifndef MessageRelay_h__
#define MessageRelay_h__

#include <functional>
#include <unordered_map>
#include <list>

struct Event
{
protected:
	Event() { }
};

class EventRelay
{
private:
	typedef std::string MessageType_t;
	typedef std::function<bool(Event)> MessageCallback_t;

public:
	void Subscribe(std::string messageType, MessageCallback_t callback)
	{
		m_Callbacks[messageType].push_back(callback);
	}

	void Raise(std::string messageType, Event message)
	{
		auto callbackIt = m_Callbacks.find(messageType);
		if (callbackIt == m_Callbacks.end())
			return;

		for (auto &callback : callbackIt->second)
		{
			callback(message);
		}
	}

private:
	std::unordered_map<MessageType_t, std::list<MessageCallback_t>> m_Callbacks;

};

#endif // MessageRelay_h__
