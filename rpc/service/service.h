#ifndef _SERVICE_H
#define _SERVICE_H
#include <map>
#include <string>
#include <functional>
#include "../../include/config.h"
#include "../../include/client.h"
using namespace std;
namespace rpc
{
	class rpc_service_impl;
	class RPC_API rpc_service
	{
		typedef std::function<void(const rpc::rpc_request& rpcRequest, rpc::rpc_response& rpcResponse)> handler_type;
	public:
		rpc_service();
		~rpc_service();
	public:
		void rpcCall(const std::string& funcName,const rpc::rpc_request& rpcRequest, rpc::rpc_response& rpcResponse);
		bool regFunction(const std::string& funcName, handler_type func);
		void removeFunction(const std::string& funcName);
	private:
		rpc_service_impl* m_pImpl;
	};
}
#endif