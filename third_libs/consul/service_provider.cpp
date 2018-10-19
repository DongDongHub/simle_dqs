#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <stdio.h>
#include <stdlib.h>

#include"service_provider.h"

using namespace ppconsul;

ServiceProvider::ServiceProvider( const string strConsulUrl,
                                  const string strSvrName,
                                  int nPort,
                                  int nChkSec,
                                  const string strIfName
                                ):
    m_strConsulUrl(strConsulUrl),
    m_strSvrName(strSvrName),
    m_nSvrPort(nPort),
    m_nChkSec(nChkSec),
    m_strIfName(strIfName)
{
    m_nStat = CONSUL_SVR_STAT_UNINIT;
    init();
}


bool ServiceProvider::register1( std::unordered_set<string> tags)
{
    if( m_nStat == CONSUL_SVR_STAT_REG ) {
        return true;
    }

    if( m_nStat == CONSUL_SVR_STAT_UNINIT ) {
        init();
    }

    if( m_nStat != CONSUL_SVR_STAT_INIT ) {
        return false;
    }

    if ( !unregister() ) {
        return false;
    }

    try {
	cout<<m_strConsulUrl<<endl;
        Consul consul(m_strConsulUrl);
        Agent agent(consul);

        agent.registerService(
            m_strSvrName,
            TcpCheck{m_strChkUrl, std::chrono::seconds(m_nChkSec)},
            agent::kw::id = m_strSvrId,
            agent::kw::port = m_nSvrPort,
            agent::kw::tags = tags,
            agent::kw::address = m_strSvrAddr
        );
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



void ServiceProvider::init()
{
    if ( m_nStat != CONSUL_SVR_STAT_UNINIT ) {
        return;
    }

    if ( getIp() ) { // get local ip fail
        string tmp = m_strSvrAddr + ":" + std::to_string(m_nSvrPort);
        m_strSvrId = m_strSvrName + "-" + tmp;
        m_strChkId = "service:" + m_strSvrId;
        m_strChkUrl = tmp;
        m_nStat = CONSUL_SVR_STAT_INIT;
    }
}


bool ServiceProvider::getIp()
{
    int fd;
    bool xRet = false;
    struct ifconf ifconf;
    struct ifreq *ifreq;
    char buf[1024];//缓冲区
    //初始化ifconf
    ifconf.ifc_len = 1024;
    ifconf.ifc_buf = buf;

    if ( (fd = socket(AF_INET,SOCK_STREAM,0)) < 0 ) {
        m_strErrMsg = "getIp create fd failed";
        return xRet;
    }
    ioctl(fd, SIOCGIFCONF, &ifconf); //获取所有接口信息

    //接下来一个一个的获取IP地址
    ifreq = (struct ifreq*)ifconf.ifc_buf;
//  printf("ifconf.ifc_len:%d\n",ifconf.ifc_len);
//  printf("sizeof (struct ifreq):%d\n",sizeof (struct ifreq));

    for (int i = (ifconf.ifc_len/sizeof (struct ifreq));  i > 0; i--, ++ifreq) {
        if( ifreq->ifr_flags == AF_INET ) {
            //for ipv4
            //printf("name =[%s]\n" , ifreq->ifr_name);
            //printf("local addr = [%s]\n" ,inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr));
            string strIfName = ifreq->ifr_name;
            if( m_strIfName.length() > 0 && strIfName != m_strIfName ) { //if if name non-empty first filiter interface name
                continue;
            }

            if ( ioctl(fd, SIOCGIFFLAGS, ifreq) < 0 ) {
                m_strErrMsg = "getIp get if flag err";
                xRet = false;
            }

            if( ifreq->ifr_flags & IFF_UP ) {
                string tmpIp = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
                if( m_strIfName.length() > 0 ) {
                    m_strSvrAddr = tmpIp;
                    xRet = true;
                    break;
                }

                if( tmpIp.find("127.0.0.1") == string::npos ) {
                    m_strSvrAddr = tmpIp;
                    xRet = true;
                    break;
                }
            }
        }
        ;
    }

    close(fd);
    return  xRet;
}

bool ServiceProvider::unregister()
{
    if( m_nStat != CONSUL_SVR_STAT_INIT && m_nStat != CONSUL_SVR_STAT_REG ) {
        return false;
    }

    try {
        Consul consul(m_strConsulUrl);
        Agent agent(consul);
        agent.deregisterService( m_strSvrId );
        agent.deregisterCheck( m_strChkId );
        return true;
    } catch( std::runtime_error &e ) {
        cout<<"catch std::runtime_error" <<e.what()<<endl;
        m_strErrMsg = "catch std::runtime_error" + string(e.what());
    } catch( std::exception &e  ) {
        cout<<"catch std::exception " <<e.what()<<endl;
        m_strErrMsg = "catch std::exception" + string(e.what());
    } catch(...) {
        cout<<"catch std::unidentify_error" <<endl;
        m_strErrMsg = "catch std::unidentify_error";
    }
    return false;
}




