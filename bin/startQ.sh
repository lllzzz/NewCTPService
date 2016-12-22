#!/bin/bash

nohup python ../src/market/queue.py ctp $1 > /tmp/mq.log &
nohup python ../src/market/queue.py ctp $1 > /tmp/mq.log &
nohup python ../src/market/queue.py ctp $1 > /tmp/mq.log &
nohup python ../src/market/queue.py ctp $1 > /tmp/mq.log &
nohup python ../src/market/queue.py ctp $1 > /tmp/mq.log &
nohup python ../src/market/queue.py ctp $1 > /tmp/mq.log &
nohup python ../src/trade/queue.py ctp $1 > /tmp/tq.log &

