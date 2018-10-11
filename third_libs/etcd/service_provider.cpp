#include"service_provider.h"

#include<exception>
#include <thread>
#include <boost/algorithm/string.hpp>
#include<iostream>
using std::cout;
using std::endl;

ServiceProvider::ServiceProvider( vector<Host>	hosts, 
		int nPort,	
		const string& strRootDir,
		const string& strSvrDir,
		int nKeyTtl,
		int nNormalSec,
		int nShortSec
		) : m_session(hosts),
	            m_nPort(nPort),
	            m_strRootDir(strRootDir),
	            m_strSvrDir(strSvrDir),
	            m_nKeyTtl(nKeyTtl),
	            m_nNormalSec(nNormalSec),
	            m_nShortSec(nShortSec)
{
	m_xThread = false;
	m_nEtcdHosts = hosts.size();
}

bool ServiceProvider::parseHosts( const string& strHosts, vector<Host>& hosts){
	vector<string> result;
	try{
		if( strHosts.find(',') == std::string::npos ) //no find , in strHosts
		{
			result.push_back(strHosts);
		}
		else
		{
			boost::split(result, strHosts, [](char c){return c == ',';});
			if( result.size() == 0 )
			{
				return false;
			}
		}
		for( auto &str : result ){
			Host host;
			if( Host::parseHostFromString( str, host) )
			{
				hosts.push_back( Host(items[0], nPort) );				
			}
		}
	}
	catch(...)
	{
		cout<< "boost split invoke error please check "<< strHosts << endl;
	}
	return hosts.size() == 0 ? false : true;
}


void ServiceProvider::registService(bool xOneTime )
{
	if ( m_nEtcdHosts <= 0 ) //check etcd hosts not empty
	{
		cout<<" no avaliable etcd hosts back thread exit normal" <<endl;
		return;
	}

	bool xRootDir = false , xSvrDir = false, xLastResult = false;		
	do 
	{
		try
		{
			xLastResult = false;
			if( (!xRootDir) && (! m_session.detectAssignedDir(m_strRootDir)) ) 
			{
				std::cout<< "check servce root	dir /jiot failed : " <<std::endl;
			}
			else
			{
				xRootDir = true;
				std::cout<<"detect root dir "<<endl;
			}

			if( (!xSvrDir) && (! m_session.detectAssignedDir(m_strSvrDir)) ) 
			{
				std::cout<< "check servce dir /jiot/dqs failed : " <<std::endl;
			}
			else
			{
				xSvrDir = true;
				std::cout<<"detect svr dir "<<endl;
			}

			if( xRootDir && xSvrDir )
			{
				if( !registerServiceImpl() )
				{
					std::cout<< "register service info failed" <<std::endl;
				}
				else
				{
					xLastResult = true;
					std::cout<< "register service info success" <<std::endl;
				}
			}

			if ( !xOneTime ) { //周期性的时候才需要去 sleep 去更新 ttl
				if( xLastResult ) {
					sleep(12);
				} else {
					sleep(2);
				}
			}
		}
		catch( std::runtime_error &e ){
			cout<<"catch std::runtime_error" <<e.what()<<endl;
		}
		catch( std::exception &e  ){
			cout<<"catch std::exception " <<e.what()<<endl;
		}
		catch(...){
			cout<<"catch std::unidentify_error" <<endl;
		}
	}
	while( !xOneTime );

}

bool ServiceProvider::registerServiceImpl()
{
	string str = "10.0.2.15";
	str.append(":");
	str.append(to_string(m_nPort));
	string strTmp = "dds123";
	cout << "key : "<<str <<" val : " << strTmp <<endl;
	unique_ptr<PutResponse> rsp = m_session.put(m_strSvrDir + str, strTmp, 60);
	//	unique_ptr<PutResponse> rsp = s.put( str, strTmp, 15);
	if( rsp->getError() != nullptr ) // mkdir err
	{
		return false;	
	}
	return true;
}

bool ServiceProvider::retrieveHostIp()
{
	return true;
}

void ServiceProvider::registerServicePeriodly(   )
{
	if( m_xThread ){
		cout<<" back thread already running."<<endl;
	}

	auto fn = [this](){
		this->registService(false);
	};

	std::thread t1(fn);
	t1.detach();	
	m_xThread = true;
}


void ServiceProvider::registerServiceOnce()
{
	registService( true );
}

