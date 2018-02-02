#include "client.h"
#include "client_impl.h"

namespace rpc
{
	rpc_client::rpc_client( const std::string& ip, int port )
		: m_impl(new rpc_client_impl(ip, port))
	{}

	rpc_client::~rpc_client()
	{
		if(m_impl)
		{
			delete m_impl;
			m_impl = NULL;
		}
	}

	void rpc_client::invoke( const std::string& funcName, 
		const rpc_request& rpcRequest, rpc_response& rpcResponse , 
		int timeOutSec)
	{
		m_impl->invoke(funcName, rpcRequest, rpcResponse, timeOutSec);
	}

	rpc_request::rpc_request()
		: m_pContentStr(new std::string()) 
	{
	}

	rpc_request::rpc_request( const std::string& str ) 
		: m_pContentStr(new std::string())
	{
		setContent(str);
	}

	rpc_request::~rpc_request()
	{
		_clear();
	}

	void rpc_request::setContent( const std::string& str )
	{
		m_pContentStr->clear();
		m_pContentStr->reserve(str.length()+1);
		m_pContentStr->append(str.c_str(), str.length());
	}

	const string& rpc_request::getContent() const
	{
		return *m_pContentStr;
	}

	void rpc_request::_clear()
	{
		if(m_pContentStr)
		{
			delete m_pContentStr;
			m_pContentStr = NULL;
		}
	}

	rpc_response::rpc_response()
		: m_pContentStr(new std::string())
	{}

	rpc_response::~rpc_response()
	{
		_clear();
	}

	void rpc_response::setContent( const std::string& str )
	{
		m_pContentStr->clear();
		m_pContentStr->reserve(str.length()+1);
		m_pContentStr->append(str.c_str(), str.length());
	}

	const string& rpc_response::getContent() const
	{
		return *m_pContentStr;
	}

	void rpc_response::_clear()
	{
		if(m_pContentStr)
		{
			delete m_pContentStr;
			m_pContentStr = NULL;
		}
	}

}

