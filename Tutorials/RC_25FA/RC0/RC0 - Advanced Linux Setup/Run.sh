#!/bin/bash
apt update
apt upgrade -y
apt autoremove
apt install wget -y
bash init_env.sh
