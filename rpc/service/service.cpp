#include "service.h"
#include "service_impl.h"

namespace rpc
{
	rpc_service::rpc_service()
		: m_pImpl(new rpc_service_impl())
	{}

	rpc_service::~rpc_service()
	{
		delete m_pImpl;
		m_pImpl = NULL;
	}

	void rpc_service::rpcCall( const std::string& funcName,const rpc::rpc_request& rpcRequest, rpc::rpc_response& rpcResponse )
	{
		m_pImpl->handler(funcName, rpcRequest, rpcResponse);
	}

	bool rpc_service::regFunction( const std::string& funcName, handler_type func )
	{
		return m_pImpl->regFunction(funcName, func);
	}

	void rpc_service::removeFunction( const std::string& funcName )
	{
		m_pImpl->removeFunction(funcName);
	}

}