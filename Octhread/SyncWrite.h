#pragma once

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <string>
#include <fstream>

class SyncWrite
{
public:
	SyncWrite(std::string& filename);
	~SyncWrite();

	void open();
	void close();
	void LockedWrite(std::string& line);


private:
	std::string m_wFilename;
	std::ofstream m_wStream;
	boost::shared_mutex _access;


	void createFile();

};


