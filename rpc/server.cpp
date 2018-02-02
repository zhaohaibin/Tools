#include "server.h"
#include "server_impl.h"
namespace rpc
{
	rpc_server::rpc_server(const std::string& ip, int port)
		: m_pImpl(new rpc_server_impl(ip, port))
	{}

	rpc_server::~rpc_server()
	{
		if(m_pImpl)
		{
			delete m_pImpl;
			m_pImpl = NULL;
		}
	}

	void rpc_server::bindService( rpc_service& se, const std::string& name /*= ""*/)
	{
		m_pImpl->bindService(se, name);
	}

	void rpc_server::setThreadMaxCount( unsigned int count )
	{

	}

	void rpc_server::start()
	{
		m_pImpl->start();
	}

	void rpc_server::stop()
	{
		m_pImpl->stop();
	}

}

