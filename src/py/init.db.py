#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
from common.MySQL import MySQL
from common.Config import Config

db = MySQL()

config = Config.get()
iids = config['iids'];

for iid in iids:
    iid = filter(lambda x:x not in '0123456789', iid)

    # tick
    sql = '''
        CREATE TABLE IF NOT EXISTS `tick_%s` (
            `id` bigint(20) NOT NULL AUTO_INCREMENT,
            `token` char(32) NOT NULL DEFAULT '',
            `iid` varchar(50) NOT NULL DEFAULT '',
            `time` char(17) NOT NULL DEFAULT '',
            `msec` int(11) NOT NULL DEFAULT '0',
            `price` decimal(10,2) NOT NULL DEFAULT '0.00',
            `volume` int(11) NOT NULL DEFAULT '0',
            `bid_price1` decimal(10,2) NOT NULL DEFAULT '0.00',
            `bid_volume1` int(11) NOT NULL DEFAULT '0',
            `ask_price1` decimal(10,2) NOT NULL DEFAULT '0.00',
            `ask_volume1` int(11) NOT NULL DEFAULT '0',
            `mtime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
            PRIMARY KEY (`id`),
            UNIQUE KEY idx_token(`token`),
            KEY idx_time(`time`)
            ) ENGINE=InnoDB CHARSET=utf8;''' % (iid)
    db.insert(sql)


sql = '''
CREATE TABLE IF NOT EXISTS `trade` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `from` varchar(50) NOT NULL DEFAULT '',
    `iid` varchar(50) NOT NULL DEFAULT '',
    `trade_id` varchar(50) NOT NULL DEFAULT '',
    `action` varchar(50) NOT NULL DEFAULT '',

    `ctp_front_id` int(11) NOT NULL DEFAULT '0',
    `ctp_session_id` int(11) NOT NULL DEFAULT '0',
    `ctp_exchange_id` varchar(50) NOT NULL DEFAULT '',
    `ctp_sys_order_id` int(11) NOT NULL DEFAULT 0,
    `ctp_td_req_id` int(11) NOT NULL DEFAULT '0',
    `ctp_trade_id` int(11) NOT NULL DEFAULT '0',

    `price` decimal(10,2) NOT NULL DEFAULT '0.00',
    `volume` int(11) NOT NULL DEFAULT 0,
    `is_buy` int(4) NOT NULL DEFAULT '-1',
    `is_open` int(4) NOT NULL DEFAULT '-1',
    `is_today` int(4) NOT NULL DEFAULT '-1',
    `send_time` char(17) NOT NULL DEFAULT '',

    `deal_price` decimal(10,2) NOT NULL DEFAULT '0.00',
    `deal_volume` int(11) NOT NULL DEFAULT 0,
    `deal_time` char(17) NOT NULL DEFAULT '',

    `status` int(11) NOT NULL DEFAULT '0',

    `mtime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (`id`),
    UNIQUE KEY idx_trade_id(`trade_id`)
) ENGINE=InnoDB CHARSET=utf8;
'''
db.insert(sql)


sql = '''
CREATE TABLE IF NOT EXISTS `model` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `name` varchar(50) NOT NULL DEFAULT '',
    `class_name` varchar(50) NOT NULL DEFAULT '',
    `source` text,
    `status` int(11) NOT NULL DEFAULT '',
    `mtime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB CHARSET=utf8;
'''
db.insert(sql)
