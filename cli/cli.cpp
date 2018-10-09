#include "dqsService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TDebugProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransportUtils.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;


using boost::shared_ptr;

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;
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

