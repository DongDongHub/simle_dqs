#ifndef SERVICE_PROVIDER_H
#define SERVICE_PROVIDER_H


#include<string>

//etcd client
#include "etcd_cli.h"

namespace etcd {

class ServiceProvider {
	public:
		ServiceProvider(vector<Host> hosts, 
				int nPort,
				const string& strRootDir = "/jiot",
				const string& strSvrDir = "/jiot/dqs",
				int nKeyTtl = 15,
				int nNormalSec = 12,
				int nShortSec = 2
			       );
	private:
		void registService(bool xOneTime  );
		bool registerServiceImpl();
		bool retrieveHostIp();

	public:
		static bool parseHosts(const string& strHosts, vector<Host>& hosts);

	public:
		void registerServicePeriodly();
		void registerServiceOnce();


	private:
		Session m_session;     // etcd client 

		int     m_nPort;

		string  m_strRootDir;  // service path in etcd
		string  m_strSvrDir;

		int     m_nKeyTtl;
		int     m_nNormalSec;  // thread sleep time section
		int     m_nShortSec;

		string  m_strKey;      // the key val register in etcd
		string  m_strVal;

		bool    m_xThread;
		int     m_nEtcdHosts;
};

}
#endif

