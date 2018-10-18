#include "dqsService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TDebugProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransportUtils.h>

#include "ppconsul/ppconsul.h"
#include "ppconsul/health.h"
using namespace ppconsul;
using namespace ppconsul::health;


using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

#define SVR_DIR  "dqs"


using boost::shared_ptr;

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;
	int nPort=9092;
	(void)nPort;
	Consul consul("http://172.17.0.5:8500");
	Health  health(consul);
	auto services = health.service("jiot_dqs", health::kw::passing=true);
	
	std::cout<<services.size()<<std::endl;
	for( auto &val : services )
	{
		auto &v1 = std::get<1>(val);
		std::cout<<v1.id<<"  "<<v1.name<<" "<<v1.address<<v1.port<<std::endl;
	}

/*
       http::Status s;
        auto r = consul.get(s, "jiot_dqs");
        std::cout << s.code() << ' ' << s.message() << '\n';
        if (r.headers())
        {
            std::cout
                << "Index: " << r.headers().index() << '\n'
                << "Known leader: " << (r.headers().knownLeader() ? "true" : "false") << '\n'
                << "Lastcontact: " << r.headers().lastContact().count() << "\n\n";
        }
        else
        {
            std::cout << '\n';
        }
        std::cout << r.data() << std::endl;
	std::string strIp = "127.0.0.1";
*/

/*	
	boost::shared_ptr<TTransport> socket(new TSocket(strIp, nPort));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
//	shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
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
	sleep(100);
*/
}

