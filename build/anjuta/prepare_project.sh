#!/bin/sh
#
#
# prepare project to work with anjuta
#
# must be run in 
# $AXLSRC/build/anjuta
#
#

[ -f .tm_project.cache ] || echo -n "" > .tm_project.cache
ln -s build/anjuta/.tm_project.cache ../../
ln -s build/anjuta/axl.prj ../../