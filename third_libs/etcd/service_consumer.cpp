#include"service_consumer.h"
#include<iostream>
#include<thread>
#include <boost/algorithm/string.hpp>

using namespace etcd;

ServiceConsumer::ServiceConsumer(vector<Host> hosts, 
				 const string strRootDir,
				 const vector<string> & vecSvrDirs):
				 m_session(hosts),
				 m_strRootDir(strRootDir),
				 m_vecSvrDir(vecSvrDirs)
{
	m_xInit = false;
	m_xThread = false;
	m_nRootIndex = -1;
	m_nStat = UNSYNC;

	if( ( hosts.size() > 0 )  && 
		( m_vecSvrDir.size() > 0 )    &&
		( m_strRootDir.length() > 0 )  
	  )
	{
		m_xInit = true;
	}

	for( auto &item : m_vecSvrDir)
	{
		vector<Host> vec;
		if( item.length() > 0 )
		{
			item = m_strRootDir + "/" + item;
		}
		m_mProviders[item] = vec; // /jiot/dqs
	}
}


bool ServiceConsumer::getServiceProvider( const string& strSvrDir, string & ip, int& nPort )
{
	if( m_nStat == UNSYNC )
	{
		cout<<" basic svr dir "<< strSvrDir<<endl;
		if( !updateServiceProvider( strSvrDir ) )
		{
			cout<< " update svr dir failed. " <<endl;
			return false;
		}
		
//		if( !m_xThread )
//		{
//			auto fn = [this](){
//				this->updateServiceProvider();
//			};
//
//			std::thread t1(fn);
//			t1.detach();	
//			m_xThread = true;			
//		}
	}

	string strFullDir = m_strRootDir + "/" + strSvrDir;
	if ( m_mProviders.find(strFullDir) == m_mProviders.end() )
	{
		return false;
	}

	vector<Host>& vecSvr = m_mProviders[strFullDir];
	if( vecSvr.size() == 0 )
	{
		return false;
	}

	int nIndex = m_mProviderIndex[strFullDir];
	m_mProviderIndex[strFullDir] = nIndex + 1;
	nIndex = nIndex % vecSvr.size();

	Host host = vecSvr[nIndex];
	
	ip = host.getHost();
	nPort = host.getPort();
	return true;
}


void  ServiceConsumer::updateServiceProvider()
{
	while(1)
	{
		if( UNSYNC == m_nStat )
		{
			bool xSync = true;
			for( auto& dir : m_vecSvrDir )
			{
				if(! updateServiceProvider( dir ) )
				{
					xSync = false;
				}
			}
			if( xSync )
			{
				m_nStat = PSYNC;
			}
		}
		else if( m_nStat == PSYNC )
		{
			watchServiceProdivers();
		}
	}
	
}

void ServiceConsumer::watchServiceProdivers()
{
	while( m_nStat == PSYNC )
	{

			vector<Node> children ;	
			int i = 0;
			for(  ; i < 3; ++i )
			{
				unique_ptr<GetResponse> rsp;
				if( m_nRootIndex == -1 )
				{
					rsp = m_session.wait(m_strRootDir, true);
					
				}
				else
				{
					rsp = m_session.wait(m_strRootDir, true, m_nRootIndex);
				}
				if ( rsp->getError() != NULL ) {
					//ResponseError* err =  r.getError();
					cout << "get  dir occur error  " << m_strRootDir <<endl;					
					continue;
				}
				children =  rsp->getNode()->getNodes();				
				m_nRootIndex = rsp->getNode()->getModifiedIndex();
			}

			if ( i == 3 )
			{
				m_nStat = UNSYNC;
				m_nRootIndex = -1;
				continue;
			}

			for( auto &node : children )
			{
				cout<<"watch key : "<< node.getKey()<< " value : " << node.getValue()<<endl;
			}
	}
}


bool  ServiceConsumer::updateServiceProvider(const string& strSvrDir)
{
	bool xRet = true;
	string strFullDir = m_strRootDir ;
	if( strSvrDir.length() > 0 )
	{
		strFullDir +=  "/" + strSvrDir;
	}

	vector<Node> children;
	int i = 0;
	bool xExistDir = false;
	for( ; i < 3; ++i )  // 重试 3 次
	{
		if( !xExistDir ) 
		{
			int nRet = m_session.existDirImpl(strFullDir);
			if( nRet == 0  )
			{
				xExistDir = true;
			}
			else if( nRet == EXIST_DIR_TYPE_ERR )
			{
				cout << " dir not exist  " << strFullDir <<endl;
				return false;
			}
			//cout << " dir not exist  " << strFullDir << " svr dir  "<< strSvrDir<<" m_strRootDir "<< m_strRootDir <<endl;
			continue;
		}		
		unique_ptr<GetResponse> r = m_session.get(strFullDir);
		if( r->getError() != NULL ) {
			cout << "get  dir occur error  " << strFullDir <<endl;					
			continue;
		}
		children = r->getNode()->getNodes();
		break;
	}
	
	if( i == 3 )
	{
		return false;
	}

	vector<Host> & vec = m_mProviders[strFullDir];
	vec.clear();
	m_mProviderIndex[strFullDir] = 0;
	for( auto & node : children )
	{
		if( node.isDirectory() )
		{
			continue;
		}

		Host host;
		if( Host::parseHostFromString(node.getValue(), host) )
		{	
			vec.push_back( host );
			xRet = true;			
		}	
	}
	return xRet;
}







