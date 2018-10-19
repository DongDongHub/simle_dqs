#ifndef CONSUL_SERVICE_CONSUMER_H
#define CONSUL_SERVICE_CONSUMER_H

#include<string>
#include<map>
#include "ppconsul/ppconsul.h" //consul client
#include "ppconsul/health.h"

using namespace ppconsul::health;
namespace ppconsul
{

    class ServiceConsumer
    {
    public:
        ServiceConsumer( const string strConsulUrl);

    public:
        bool get(const string& strSvrName,   string& strIp, int& nPort, const string& strTag = "", const string& strDc = "");

    private:
        size_t nextIndex(const string& strSvrName, const string& strTag, const string& strDc, size_t size);

    private:
        string  m_strConsulUrl;     // consul address
        string  m_strErrMsg;
        map<string, int> m_mIndex;

    };

}
#endif

