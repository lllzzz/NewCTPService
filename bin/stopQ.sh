#!/bin/bash

ps -ef | grep queue | grep python | grep ctp | grep "$1" | grep -v grep | cut -c 9-15 | xargs kill -9
