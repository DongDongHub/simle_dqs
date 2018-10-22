#include "dqsService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TDebugProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransportUtils.h>

#include "service_consumer.h"
using ppconsul::ServiceConsumer;


using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

#define SVR_NAME "jiot_dqs"
#define SVR_TAG  ""
#define SVR_DC   ""

using boost::shared_ptr;

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	std::string strIp;
	int nPort;

	ServiceConsumer svrConsumer({"127.0.0.1:8500"});
	if( !svrConsumer.get(SVR_NAME, strIp, nPort, SVR_TAG, SVR_DC) ) {
		std::cout<<"get failed please check it"<<std::endl;
	}
	std::cout<<"get ip :"<<strIp << " port :"<<nPort<<std::endl;

	if( !svrConsumer.get(SVR_NAME, strIp, nPort, SVR_TAG, SVR_DC) ) {
		std::cout<<"get failed please check it"<<std::endl;
	}
	std::cout<<"get ip :"<<strIp << " port :"<<nPort<<std::endl;


	boost::shared_ptr<TTransport> socket(new TSocket(strIp, nPort));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	//  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	//shared_ptr<TProtocol> protocol(new TDebugProtocol(transport));
	boost::shared_ptr<TProtocol> protocol(new TJSONProtocol(transport));

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
