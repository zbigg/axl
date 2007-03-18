#!/bin/sh
############################################################
#
#  Update static configuration files in build/*/config.h
#
#    	Id: $Id: cfg_update.sh 1.1 03/10/16 00:03:01+02:00 zbigg@marl.(none) $
#
#  Author: Zbigniew Zagorski <zzbigg@o2.pl>
#
############################################################

PACKAGE_VERSION=`cat VERSION`

for FILE in build/*/config.h ; do
    diff3 -m $FILE ./misc/config_in_last config.h.in > $FILE.merge
    CONFLICT=$?
    cat $FILE.merge | sed -e "s/VERSION \([0-9.\"]\+\)/VERSION \"$PACKAGE_VERSION\"/" > $FILE.new
    if test "$CONFLICT" != "0" ; then
	cp -f $FILE $FILE.orig
	echo "$0: $FILE: conflicts during merging config.h.in with $FILE"
	echo "$0: $FILE:    please check it (copy of original: $FILE.orig)"
    fi
    ( diff -q $FILE $FILE.new > /dev/null) || ( echo "$0: merged $FILE" ; cp -f $FILE.new $FILE ; )
    #rm -f $FILE.new $FILE.merge
done
cp -f config.h.in ./misc/config_in_last
