#ifndef _CLIENT_IMPL_H
#define _CLIENT_IMPL_H
#include "RCF/RCF.hpp"
#include "./service/i_rpc_service.hpp"

namespace rpc
{
	class rpc_request;
	class rpc_response;
	class rpc_client_impl
	{
	public:
		rpc_client_impl(const std::string& ip, int port);
		~rpc_client_impl();
		void invoke( const std::string& funcName, 
			const rpc_request& rpcRequest, rpc_response& rpcResponse ,
			int timeOutSec);
	private:
		std::string m_ip;
		int m_port;
	};
}
#endif