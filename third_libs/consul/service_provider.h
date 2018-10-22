#ifndef CONSUL_SERVICE_PROVIDER_H
#define CONSUL_SERVICE_PROVIDER_H

#include<string>
#include<unordered_set>

//consul client
#include "ppconsul/agent.h"
using namespace ppconsul::agent;

#define CONSUL_SVR_STAT_UNINIT 0
#define CONSUL_SVR_STAT_INIT   1
#define CONSUL_SVR_STAT_REG    2


namespace ppconsul
{

    class ServiceProvider
    {
    public:
        ServiceProvider( const std::vector<std::string> vecConsuls,
                         const std::string strSvrName,
                         int nPort,
                         int nChkSec = 10,
                         const std::string strIfName = ""
                       );

    public:
        bool register1( std::unordered_set<std::std::string>  tags );
        bool unregister();

    private:
        bool unregisterService();
        bool unregisterCheck();

        void init();

        bool getIp();


    private:
        std::vector<std::string> m_vecConsuls; // consul address vector

        std::string  m_strSvrName;       //

        int         m_nSvrPort;

        std::string  m_strSvrAddr;         // app host

        std::string  m_strChkUrl;  // use for tcp check
        std::string  m_strChkId;
        std::string  m_strSvrId;

        int     m_nChkSec;    // tcp check periodly

        int     m_nStat;

        std::string  m_strErrMsg;

        std::string  m_strIfName;  //interface name

        int     m_nCurrIndex;      //consul address index current use.
        size_t     m_nMaxRetryTime;

    };

}
#endif

