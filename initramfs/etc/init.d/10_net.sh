#!/bin/sh

ip a add 192.168.23.233/24 dev eth0
ip l set eth0 up
ip l set lo up
