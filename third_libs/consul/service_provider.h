#ifndef CONSUL_SERVICE_PROVIDER_H
#define CONSUL_SERVICE_PROVIDER_H

#include<string>
#include<unordered_set>

//consul client
#include "ppconsul/agent.h"
using namespace ppconsul::agent;
using std::string;
using std::cout;
using std::endl;

#define CONSUL_SVR_STAT_UNINIT 0
#define CONSUL_SVR_STAT_INIT   1
#define CONSUL_SVR_STAT_REG    2


namespace ppconsul
{

    class ServiceProvider
    {
    public:
        ServiceProvider( const string strConsulUrl,
                         const string strSvrName,
                         int nPort,
                         int nChkSec = 10,
                         const string strIfName = ""
                       );

    public:
        bool register1( std::unordered_set<std::string>  tags );
        bool unregister();

    private:
        bool unregisterService();
        bool unregisterCheck();

        void init();

        bool getIp();


    private:
        string  m_strConsulUrl;     // consul address
        string  m_strSvrName;

        int     m_nSvrPort;

        string  m_strSvrAddr;         // app host

        string  m_strChkUrl;  // use for tcp check
        string  m_strChkId;
        string  m_strSvrId;

        int     m_nChkSec;    // tcp check periodly

        int     m_nStat;

        string  m_strErrMsg;

        string  m_strIfName;  // interface name
    };

}
#endif

