#!/bin/sh
# sets up URLs

HOSTIP=`echo $DOCKER_HOST | sed 's#^tcp://##' | sed 's/:[0-9]*$//'`
if [ -z "${HOSTIP}" ] 
then
  HOSTIP=127.0.0.1
fi

# Where is this grep from? Where can I change it to NOT be afrl_cloudbleed.curtail?
SENSORNAME=`docker ps --format='{{ .Names }}' | grep afrl_cloudbleed.curtail.`
UINAME=`docker ps --format='{{ .Names }}' | grep afrl_cloudbleed.curtui.`

# Why do we care about establishing demoport vs admin port? is this leftover code?
DEMOPORT=`docker port ${SENSORNAME} | sed 's/^.*://'`
ADMINPORT=`docker port ${UINAME} | sed 's/^.*://'`

echo '*****'  Make sure to update current config   '*****'
echo '*****' with the file at /private/config.json '*****'
echo
echo '#' === CloudBleed Demo ===
echo '#' Generate traffic by running traffic.sh
echo
echo '#' Admin Service at: https://${HOSTIP}:${ADMINPORT}

