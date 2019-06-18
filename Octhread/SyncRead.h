#pragma once
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <string>

typedef boost::shared_mutex Lock;
typedef boost::shared_lock<Lock> ReadLock;
class SyncRead
{
public:
	SyncRead();
	~SyncRead();


private:
	std::string m_filename;
};

