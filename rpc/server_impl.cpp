#include "server_impl.h"
#include "service/i_rpc_service.hpp"

namespace rpc
{
	rpc_server_impl::rpc_server_impl( const std::string& ip, int port ) 
		: m_rcfServer(RCF::TcpEndpoint(ip, port))
	{}

	rpc_server_impl::~rpc_server_impl()
	{}

	void rpc_server_impl::bindService( rpc_service& se, const std::string& name /*= ""*/ )
	{
		m_rcfServer.bind<i_rpc_service>(se, name);
	}

	void rpc_server_impl::setThreadMaxCount( unsigned int count )
	{
		m_rcfServer.getThreadPool()->setThreadMaxCount(count);
	}

	void rpc_server_impl::start()
	{
		m_rcfServer.getServerTransport().setMaxIncomingMessageLength(1024*1024*10);
		m_rcfServer.start();
	}

	void rpc_server_impl::stop()
	{
		m_rcfServer.stop();
	}

}

