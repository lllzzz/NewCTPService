#include "ShareData.h"

void ShareData::setLastPrice(string iid, double price)
{
    Cache::getInstance()->set("CACHE_KEY_IID_LAST_PRICE_" + iid, Tool::d2s(price));
}

double ShareData::getLastPrice(string iid)
{
    string price = Cache::getInstance()->get("CACHE_KEY_IID_LAST_PRICE_" + iid);
    return Tool::s2d(price);
}

void ShareData::setUpperPrice(string iid, double price)
{
    Cache::getInstance()->set("CACHE_KEY_IID_UPPER_PRICE_" + iid, Tool::d2s(price));
}

double ShareData::getUpperPrice(string iid)
{
    string price = Cache::getInstance()->get("CACHE_KEY_IID_UPPER_PRICE_" + iid);
    return Tool::s2d(price);
}

void ShareData::setLowerPrice(string iid, double price)
{
    Cache::getInstance()->set("CACHE_KEY_IID_LOWER_PRICE_" + iid, Tool::d2s(price));
}

double ShareData::getLowerPrice(string iid)
{
    string price = Cache::getInstance()->get("CACHE_KEY_IID_LOWER_PRICE_" + iid);
    return Tool::s2d(price);
}