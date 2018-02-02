#include <string>
#include <iostream>
#include "..\include\rpc_init.h"
#include "..\include\client.h"
using namespace std;

int main()
{
	rpc::rpc_init rpcInit;
	rpc::rpc_client rpcClient("127.0.0.1", 50001);
	string str;
	for(int i = 0; i < 1000; ++i)
	{
		for(int i = 0; i < 1024; ++i)
		{
			str += "A";
		}
	}

	for(int i = 0; i < 10000000; ++i)
	{	
		rpc::rpc_request rpcRequest(str);
		rpc::rpc_response rpcResponst;
		rpcClient.invoke("test", rpcRequest, rpcResponst);
		string content = rpcResponst.getContent();
		cout << content << endl;
	}
	int n;
	cin >> n;
}