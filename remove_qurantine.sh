#!/bin/sh

#  remove_qurantine.sh
#  Created by Denis Perevalov on 16/11/2018.
#  This file removes "quarantine flag", which OS sets after downloading an app.
#  You need to remove, else app will crash at start.
#  See more info here:
#  https://fabianmoronzirfas.me/application/2018/04/06/of-app-crash.html

#
BASEDIR=$(dirname "$0")
#echo "$BASEDIR"
xattr -dr com.apple.quarantine "$BASEDIR"
echo "Quarantine flag for apps in this folder was removed"
