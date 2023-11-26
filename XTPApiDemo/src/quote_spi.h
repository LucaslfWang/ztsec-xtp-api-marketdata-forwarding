#pragma once
#include "xtp_quote_api.h"
#include <fstream>
#include <time.h>
#include <sys/timeb.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32

#define NUM_OF_ROUND 1

using namespace XTP::API;

class MyQuoteSpi : public QuoteSpi
{
public:
	MyQuoteSpi();
	~MyQuoteSpi();

	virtual void OnDisconnected(int reason);

	virtual void OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last);
	virtual void OnUnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last);
	virtual void OnDepthMarketData(XTPMD *market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count, int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count);
	virtual void OnSubOrderBook(XTPST *ticker, XTPRI *error_info, bool is_last);
	virtual void OnUnSubOrderBook(XTPST *ticker, XTPRI *error_info, bool is_last);
	virtual void OnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last);
	virtual void OnUnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last);
	virtual void OnOrderBook(XTPOB *order_book);
	virtual void OnTickByTick(XTPTBT *tbt_data);
	virtual void OnQueryAllTickers(XTPQSI* ticker_info, XTPRI *error_info, bool is_last);
	virtual void OnQueryTickersPriceInfo(XTPTPI* ticker_info, XTPRI *error_info, bool is_last);
	virtual void OnSubscribeAllMarketData(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnUnSubscribeAllMarketData(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnSubscribeAllOrderBook(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnUnSubscribeAllOrderBook(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnSubscribeAllTickByTick(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnUnSubscribeAllTickByTick(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnSubscribeAllOptionMarketData(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnUnSubscribeAllOptionMarketData(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnSubscribeAllOptionOrderBook(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnUnSubscribeAllOptionOrderBook(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnSubscribeAllOptionTickByTick(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnUnSubscribeAllOptionTickByTick(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);

	virtual void OnQueryAllTickersFullInfo(XTPQFI* ticker_info, XTPRI *error_info, bool is_last);

	//以下为行情回补涉及的回调函数
	virtual void OnRebuildQuoteServerDisconnected(int reason);
	virtual void OnRequestRebuildQuote(XTPQuoteRebuildResultRsp* rebuild_result);
	virtual void OnRebuildTickByTick(XTPTBT *tbt_data);
	virtual void OnRebuildMarketData(XTPMD *md_data);


private:


	bool IsErrorRspInfo(XTPRI *pRspInfo);

};