#include "service_impl.h"

namespace rpc
{

	rpc_functions::rpc_functions()
	{}

	rpc_functions::~rpc_functions()
	{}

	string rpc_functions::doRun( const std::string& funcName, const std::string& str )
	{
		rpc::rpc_request rpcRequest(str);
		rpc::rpc_response rpcResponse;
		functions_type::iterator it = m_functions.find(funcName);
		if(it != m_functions.end())
		{
			(it->second)(rpcRequest, rpcResponse);
		}
		string strRe = rpcResponse.getContent();
		return strRe;
	}

	void rpc_functions::doRun( const std::string& funcName, const rpc::rpc_request& rpcRequest, rpc::rpc_response& rpcResponse )
	{
		functions_type::iterator it = m_functions.find(funcName);
		if(it != m_functions.end())
		{
			(it->second)(rpcRequest, rpcResponse);
		}
	}

	bool rpc_functions::regFunction( const std::string& funcName,handler_type func )
	{
		if(m_functions.find(funcName) != m_functions.end())
			return false;
		m_functions.insert(std::make_pair(funcName, func));
		return true;
	}

	void rpc_functions::removeFunction( const std::string& funcName )
	{
		m_functions.erase(funcName);
	}

	rpc_service_impl::rpc_service_impl()
	{}

	rpc_service_impl::~rpc_service_impl()
	{}

	void rpc_service_impl::handler( const std::string& funcName, const rpc::rpc_request& rpcRequest, rpc::rpc_response& rpcResponse )
	{
		m_regFunctions.doRun(funcName, rpcRequest, rpcResponse);
	}

	bool rpc_service_impl::regFunction( const std::string& funcName, handler_type func )
	{
		return m_regFunctions.regFunction(funcName, func);
	}

	void rpc_service_impl::removeFunction( const std::string& funcName )
	{
		m_regFunctions.removeFunction(funcName);
	}

}