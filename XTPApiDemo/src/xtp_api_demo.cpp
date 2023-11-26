// testTradeApi.cpp : 定义控制台应用程序的入口点。
//
#define __STDC_WANT_LIB_EXT1__ 1

#include <string>
#include <map>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32

#include "FileUtils.h"
#include "xtp_quote_api.h"
#include "quote_spi.h"

bool is_connected_ = false;
std::string trade_server_ip;
int trade_server_port;
uint64_t session_id_ = 0;
std::map<uint64_t,uint64_t> map_session;
uint64_t* session_arrary = NULL;
FileUtils* fileUtils = NULL;
XTPOrderInsertInfo* orderList = NULL;
std::string quote_server_ip;
int quote_server_port;
std::string quote_username;
std::string quote_password;
XTP_PROTOCOL_TYPE quote_protocol = XTP_PROTOCOL_UDP;

int main()
{

	fileUtils = new FileUtils();
	if (!fileUtils->init())
	{
		std::cout << "The config.json file parse error." << std::endl;
#ifdef _WIN32
		system("pause");
#endif

		return 0;
	}

	//读取交易配置
	trade_server_ip = fileUtils->stdStringForKey("trade_ip");
	trade_server_port = fileUtils->intForKey("trade_port");
	int out_count = fileUtils->intForKey("out_count");
	bool auto_save = fileUtils->boolForKey("auto_save");
	int client_id = fileUtils->intForKey("client_id");
	int account_count = fileUtils->countForKey("account");
	int resume_type = fileUtils->intForKey("resume_type");
	std::string account_key = fileUtils->stdStringForKey("account_key");
#ifdef _WIN32
	std::string filepath = fileUtils->stdStringForKey("path");
#else
	std::string filepath = fileUtils->stdStringForKey("path_linux");
#endif // _WIN32
	bool ping_pong_test = fileUtils->boolForKey("ping_pong_test");//demo是否开启乒乓测试，默认是不开启的，如果需要开启，请在配置文件中修改

	//读取行情配置
	quote_server_ip = fileUtils->stdStringForKey("quote_ip");
	quote_server_port = fileUtils->intForKey("quote_port");
	quote_username = fileUtils->stdStringForKey("quote_user");
	quote_password = fileUtils->stdStringForKey("quote_password");
	quote_protocol = (XTP_PROTOCOL_TYPE)(fileUtils->intForKey("quote_protocol"));
	int32_t quote_buffer_size = fileUtils->intForKey("quote_buffer_size");

	//读取心跳超时配置
	int32_t heat_beat_interval = fileUtils->intForKey("hb_interval");


	//初始化行情api
	XTP::API::QuoteApi* pQuoteApi = XTP::API::QuoteApi::CreateQuoteApi(client_id, filepath.c_str(), XTP_LOG_LEVEL_DEBUG);//log日志级别可以调整
	if (!pQuoteApi)
	{
		std::cout << "Failed to create quote api, please check the input parameters." << std::endl;
		return 0;
	}
	MyQuoteSpi* pQuoteSpi = new MyQuoteSpi();
	if (!pQuoteSpi)
	{
		std::cout << "Failed to create quote spi, please check the input parameters." << std::endl;
		return 0;
	}
	pQuoteApi->RegisterSpi(pQuoteSpi);

	//设定行情服务器超时时间，单位为秒
	pQuoteApi->SetHeartBeatInterval(heat_beat_interval); //此为1.1.16新增接口
	//设定行情本地缓存大小，单位为MB
	pQuoteApi->SetUDPBufferSize(quote_buffer_size);//此为1.1.16新增接口

	int loginResult_quote = -1;
	//登录行情服务器,自1.1.16开始，行情服务器支持UDP连接，推荐使用UDP
	loginResult_quote = pQuoteApi->Login(quote_server_ip.c_str(), quote_server_port, quote_username.c_str(), quote_password.c_str(), quote_protocol); 
	if (loginResult_quote == 0)
	{
		//登录行情服务器成功后，订阅行情
		int instrument_count = fileUtils->countForKey("quote_ticker.instrument");
		int quote_exchange = fileUtils->intForKey("quote_ticker.exchange");

		//从配置文件中读取需要订阅的股票
		char* *allInstruments = new char*[instrument_count];
		for (int i = 0; i < instrument_count; i++) {
			allInstruments[i] = new char[XTP_TICKER_LEN];
			std::string instrument = fileUtils->stdStringForKey("quote_ticker.instrument[%d]", i);
			// strcpy_s(allInstruments[i], XTP_TICKER_LEN, instrument.c_str());
			strncpy(allInstruments[i], instrument.c_str(), XTP_TICKER_LEN);
		}

		//开始订阅,注意公网测试环境仅支持TCP方式，如果使用UDP方式会没有行情数据，实盘大多数使用UDP连接
		pQuoteApi->SubscribeMarketData(allInstruments, instrument_count, (XTP_EXCHANGE_TYPE)quote_exchange);


		//释放
		for (int i = 0; i < instrument_count; i++) {
			delete[] allInstruments[i];
			allInstruments[i] = NULL;
		}

		delete[] allInstruments;
		allInstruments = NULL;
	}
	else
	{
		//登录失败，获取失败原因
		XTPRI* error_info = pQuoteApi->GetApiLastError();
		std::cout << "Login to server error, " << error_info->error_id << " : " << error_info->error_msg << std::endl;
	
	}


	if (account_count > 0)
	{
		//针对多用户的情况
		orderList = new XTPOrderInsertInfo[account_count];
		memset(orderList, 0, sizeof(XTPOrderInsertInfo)*account_count);
	}
	

		//主线程循环，防止进程退出
		while (true)
		{
#ifdef _WIN32
			Sleep(1000);
#else
			sleep(1);
#endif // WIN32

		}
	

	

	if (orderList)
	{
		delete[] orderList;
		orderList = NULL;
	}

	if (session_arrary)
	{
		delete[] session_arrary;
		session_arrary = NULL;
	}

	return 0;
}
