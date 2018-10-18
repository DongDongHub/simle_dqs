#ifndef SERVICE_CONSUMER_H
#define SERVICE_CONSUMER_H


//etcd client
#include "etcd_cli.h"
#include<string>
#include<map>
#include<vector>
#include<mutex>

namespace etcd {

#define UNSYNC    0
#define SYNC      1  //全部获取 get all
#define PSYNC     2  //有变更部分的同步


#define DIR_NO_EXIST
#define DIR_GET_ERR

class ServiceConsumer {
public:
	ServiceConsumer(vector<Host> hosts, 
				 const string strRootDir,
				 const vector<string> & vecSvrDirs);

	private:
		void updateServiceProvider();
		bool updateServiceProvider( const  string&  strSvrDir);
		void watchServiceProdivers();

	public:
		bool getServiceProvider( const string& strSvrDir, string & ip, int& nPort );
		

	private:
		Session m_session;     // etcd client
		string  m_strRootDir;
		vector<string> m_vecSvrDir;
		map<string, vector<Host>> m_mProviders;
		map<string, unsigned int>m_mProviderIndex;
		bool m_xThread;
		bool m_xInit;

		mutex m_mutex;

		int  m_nStat;
		int  m_nRootIndex;
};

}
#endif

