#include"service_consumer.h"
#include<iostream>

using namespace ppconsul;

ServiceConsumer::ServiceConsumer(const std::vector< std::string> vecConsuls):m_vecConsuls(vecConsuls)
{
    m_nCurrIndex = 0;
    m_nMaxRetryTime = 3;
}

bool ServiceConsumer::get(const std::string& svrName,  std::string& strIp, int& nPort, const std::string& tag, const std::string& dc)
{

    if( m_vecConsuls.size() == 0 ) {
        m_strErrMsg = "vec consuls no available host";
        return false;
    }

    for( size_t i = 0; i < m_nMaxRetryTime; ++i ) {

        try {

            if( i != 0 ) { //first time failed try second, change the consul addr to next
                m_nCurrIndex =  (m_nCurrIndex + 1) / m_vecConsuls.size();
                std::cout<<" try failed incr m_nCurrIndex " << m_nCurrIndex << std::endl;
            }
            Consul consul("http://" + m_vecConsuls[m_nCurrIndex]);
	    Health  health(consul);
	    auto services = health.service(svrName, health::kw::passing=true, health::kw::tag = tag, health::kw::dc = dc);

            size_t size = services.size();
            std::cout<<"get svr prodvider size :"<<size<<std::endl;
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
            std::cout<<"catch std::runtime_error" <<e.what()<<std::endl;
        } catch( std::exception &e  ) {
            std::cout<<"catch std::exception " <<e.what()<<std::endl;
        } catch(...) {
            std::cout<<"catch std::unidentify_error" <<std::endl;
        }
    }
    return false;
}


size_t ServiceConsumer::nextIndex(const std::string& svrName, const std::string& tag, const std::string& dc, size_t size)
{
    std::string tmp = svrName;  //format key like  dc-svr1-tag
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
