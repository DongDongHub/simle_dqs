#include"service_consumer.h"
#include<iostream>

using namespace ppconsul;

ServiceConsumer::ServiceConsumer(const string strConsulUrl):m_strConsulUrl(strConsulUrl)
{

}

bool ServiceConsumer::get(const string& svrName,  string& strIp, int& nPort, const string& tag, const string& dc)
{
    try {

        Consul consul(m_strConsulUrl);
        Health  health(consul);
        auto services = health.service(svrName, health::kw::passing=true, health::kw::tag = tag, health::kw::dc = dc);

        size_t size = services.size();
        cout<<"get svr prodvider size :"<<size<<endl;
        if( size == 0 ) {
            m_strErrMsg = "no available service provider to use";
            return false;
        }
        auto &svr = services[ nextIndex(svrName, tag, dc, size) ];
        auto &item = std::get<1>(svr);
        strIp = item.address;
        nPort = item.port;
        return true;
    } catch( std::runtime_error &e ) {
        cout<<"catch std::runtime_error" <<e.what()<<endl;
    } catch( std::exception &e  ) {
        cout<<"catch std::exception " <<e.what()<<endl;
    } catch(...) {
        cout<<"catch std::unidentify_error" <<endl;
    }
    return false;
}


size_t ServiceConsumer::nextIndex(const string& svrName, const string& tag, const string& dc, size_t size)
{
    string tmp = svrName;  //format key like  dc-svr1-tag
    size_t index = 0;
    if(dc.length() > 0) {
        tmp = dc + "-" + svrName;
    }

    if( tag.length() > 0 ) {
        tmp += "-" + tag;
    }

    if( m_mIndex.find(tmp) != m_mIndex.end() ) {
        index = m_mIndex[tmp];
    }
    index = index % size;
    m_mIndex[tmp] = index + 1;
    return index;  // 从 0 开始
}




