#!/bin/sh

cd `dirname $0`
echo "clear log file:"$(pwd)"/*"

rm *.log

touch debug.log
touch error.log
touch info.log
touch warn.log
touch fatal.log