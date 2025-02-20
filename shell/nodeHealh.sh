#!/bin/bash


# Author: benphamdev
#Date: 15/12/2024

#this script outputs the node health

#version: v1

set -x # debug mode

set -e #exit the script when there is an error

set -o pipefail

#set -exo pipefail

df -h

free -g

nproc

ps -ef
