#CTPService

封装CTP接口，并提供Web版模型开发


####依赖

*   Linux（CentOS7）
*   Redis
*   MySQL

####安装

* git clone xxxxx
* cd CTPService/
* cp src/ctp/thostmduserapi.so /usr/local/lib/libthostmduserapi.so
* cp src/ctp/thosttraderapi.so /usr/local/lib/libthosttraderapi.so
* make market
* make trade
* make clean

####启动
* 行情服务：./bin/marketService etc/config.dev.json
* 交易服务：./bin/tradeService etc/config.dev.json
* WEB服务：CTP_CONFIG_PATH=etc/config.model.dev.json python src/py/run.py
