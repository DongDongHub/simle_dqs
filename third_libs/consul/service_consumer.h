#ifndef CONSUL_SERVICE_CONSUMER_H
#define CONSUL_SERVICE_CONSUMER_H

#include<string>
#include<map>
#include<vector>
#include "ppconsul/ppconsul.h" //consul client
#include "ppconsul/health.h"

using namespace ppconsul::health;
namespace ppconsul
{

    class ServiceConsumer
    {
    public:
        ServiceConsumer(const std::vector< std::string> vecConsuls);

    public:
        bool get(const std::string& strSvrName,   std::string& strIp, int& nPort, const std::string& strTag = "", const std::string& strDc = "");

    private:
        size_t nextIndex(const std::string& strSvrName, const std::string& strTag, const std::string& strDc, size_t size);

    private:
        std::vector<std::string>  m_vecConsuls;     // consul address
        std::string  m_strErrMsg;
		size_t       m_nCurrIndex;
		int 		 m_nMaxRetryTime;
        std::map<std::string, int> m_mIndex;

    };

}
#endif

