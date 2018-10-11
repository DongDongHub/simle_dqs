#include "dqsService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TDebugProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransportUtils.h>

#include"service_consuer.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

#define ROOT_DIR "/jiot"
#define SVR_DIR  "dqs"


using boost::shared_ptr;

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;
	string strIp;
	int nPort;
	vector<Host> hosts;
	if( !ServiceProvider::parseHosts("192.168.33.10:2379", hosts) )
	{
		cout<<"parse etcd hosts error"<<endl;	
	}	
	ServiceConsumer svrConsumer( hosts, ROOT_DIR, SVR_DIR );
	svrConsumer.getServiceProvider( SVR_DIR , string & ip, int & nPort)
	
	shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
	shared_ptr<TTransport> transport(new TBufferedTransport(socket));
//	shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	//shared_ptr<TProtocol> protocol(new TDebugProtocol(transport));
	shared_ptr<TProtocol> protocol(new TJSONProtocol(transport));

	dqsServiceClient client(protocol);
	try {
		transport->open();

		std::string req = "hellodds";
		std::string reply;

		client.QueryDeviceInfo(reply, req);
		printf("req %s rsp : %s\n", req.c_str(), reply.c_str());

		transport->close();
	} catch (TException &tx) {
		printf("ERROR: %s\n", tx.what());
	}
}

