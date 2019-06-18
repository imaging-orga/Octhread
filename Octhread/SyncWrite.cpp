#include "SyncWrite.h"



SyncWrite::SyncWrite(std::string& filename) : m_wFilename(filename)
{
	createFile();
	open();
}


SyncWrite::~SyncWrite()
{
	m_wStream.close();
}

void SyncWrite::open()
{
	m_wStream.open(m_wFilename, std::ios::out | std::ios::app);
}

void SyncWrite::close()
{
	m_wStream.close();
}

void SyncWrite::LockedWrite(std::string & line){
	boost::upgrade_lock<boost::shared_mutex> lock(_access);
	m_wStream << line << "\n";
}

void SyncWrite::createFile()
{
	m_wStream.open(m_wFilename, std::ios::out);
	m_wStream.close();
}
