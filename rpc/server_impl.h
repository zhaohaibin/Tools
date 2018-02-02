#ifndef _SERVER_IMPL_H
#define _SERVER_IMPL_H
#include <string>
#include "RCF/RCF.hpp"
#include "service/service.h"

namespace rpc
{
	class rpc_server_impl
	{
	public:
		rpc_server_impl(const std::string& ip, int port);
		~rpc_server_impl();
	public:
		void bindService(rpc_service& se, const std::string& name = "");
		void setThreadMaxCount(unsigned int count);
		void start();
		void stop();
	private:
		RCF::RcfServer m_rcfServer;
	};
}
#endif