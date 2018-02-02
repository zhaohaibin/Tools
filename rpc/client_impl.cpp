#include "client_impl.h"
#include "client.h"

namespace rpc
{
	rpc_client_impl::rpc_client_impl( const std::string& ip, int port )
		: m_ip(ip), m_port(port)
	{}

	rpc_client_impl::~rpc_client_impl()
	{}

	void do_invoke(const string& ip, int port,
		const std::string& funcName, 
		const rpc_request& rpcRequest, 
		rpc_response& rpcResponse,
		int timeOutSec)
	{
		RcfClient<i_rpc_service> rcfClient(RCF::TcpEndpoint(ip, port));
		RCF::ClientStub stub = rcfClient.getClientStub();
		rcfClient.getClientStub().setConnectTimeoutMs(10*1000);
		rcfClient.getClientStub().setRemoteCallTimeoutMs(timeOutSec*1000);
		rcfClient.rpcCall(funcName, rpcRequest, rpcResponse);
	}

	void rpc_client_impl::invoke( const std::string& funcName, 
		const rpc_request& rpcRequest, rpc_response& rpcResponse,
		int timeOutSec)
	{
		std::string re;
		//RcfClient<i_rpc_service> rcfClient(RCF::TcpEndpoint(m_ip, m_port));
		try
		{
// 			RCF::ClientStub stub = rcfClient.getClientStub();
// 			rcfClient.getClientStub().setConnectTimeoutMs(10*1000);
// 			rcfClient.getClientStub().setRemoteCallTimeoutMs(60*1000);
// 			rcfClient.rpcCall(funcName, rpcRequest, rpcResponse);
			do_invoke(m_ip, m_port, funcName, rpcRequest, rpcResponse, timeOutSec);
		}catch(const RCF::Exception & e)
		{
			re = e.getError().getErrorString();
			string strJson = "{\"code\": -1 ,\"data\": \"" + re + "\""+ "}";
			rpcResponse.setContent(strJson);
		}catch(...)
		{
			string strJson = "{\"code\": -1 ,\"msg\": \"exception\"\"}";
		}
	}

}

