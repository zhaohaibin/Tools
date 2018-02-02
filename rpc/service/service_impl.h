#ifndef _SERVICE_IMPL_H
#define _SERVICE_IMPL_H
#include <map>
#include <string>
#include <functional>
#include "../../include/client.h"
using namespace std;
namespace rpc
{
	typedef std::function<void(const rpc::rpc_request& rpcRequest, rpc::rpc_response& rpcResponse)> handler_type;
	typedef std::map<std::string, handler_type> functions_type;

	class rpc_functions
	{
	public:
		rpc_functions();
		~rpc_functions();
	public:
		string doRun(const std::string& funcName, const std::string& str);
		void doRun(const std::string& funcName, const rpc::rpc_request& rpcRequest, rpc::rpc_response& rpcResponse);
		bool regFunction(const std::string& funcName, handler_type func);
		void removeFunction(const std::string& funcName);
	private:
		functions_type m_functions;
	};

	class rpc_service_impl
	{
	public:
		rpc_service_impl();
		~rpc_service_impl();
	public:
		void handler(const std::string& funcName, const rpc::rpc_request& rpcRequest, rpc::rpc_response& rpcResponse);
		bool regFunction(const std::string& funcName, handler_type func);
		void removeFunction(const std::string& funcName);
	private:
		rpc_functions m_regFunctions;
	};
}
#endif