#!/bin/bash
setup_color() {
    if [ -t 1 ]; then
        RED=$(printf '\033[31m')
        GREEN=$(printf '\033[32m')
        YELLOW=$(printf '\033[33m')
        BLUE=$(printf '\033[34m')
        BOLD=$(printf '\033[1m')
        RESET=$(printf '\033[m')
    else
        RED=""
        GREEN=""
        YELLOW=""
        BLUE=""
        BOLD=""
        RESET=""
    fi
}
setup_color
username=`whoami`
if [[ ! ${username} == "root" ]];then
    echo "${RED}Please use root user to execute this script.${RESET}"
    exit
fi
n=`sudo grep -n "ClientAliveInterval " /etc/ssh/sshd_config | awk -F':' '{print $1}'`
TMPn='ClientAliveInterval 60'
m=`sudo grep -n "ClientAliveCountMax " /etc/ssh/sshd_config | awk -F':' '{print $1}'`
TMPm='ClientAliveCountMax 3'
sudo sed -i "$[ n ]c $TMPn" /etc/ssh/sshd_config
sudo sed -i "$[ m ]c $TMPm" /etc/ssh/sshd_config
echo "Updating..."
apt update
echo "Downloading..."
apt install -y git zsh gcc g++ glibc-doc autojump universal-ctags
regex="^[a-zA-Z]+$"
while [[ 1 ]];do
    read -p "Initialize ${RED}machine name${RESET}(${YELLOW}in English${RESET}) :" host_name
    if [[ ! ${host_name} =~ ${regex} ]];then
        echo "${RED}illegal, again.${RESET}"
        continue
    else
        break
    fi
done
hostnamectl set-hostname ${host_name}
echo "successfully change name"
host_ip=`ifconfig eth0 |grep inet|grep -v 127.0.0.1|grep -v inet6|awk '{print $2}'|tr -d "addr:"`
host_isoftstone_xiaokai=`grep -n "$host_ip" /etc/hosts | head -1 | awk -F':' '{print $1}'`
host_isoftstone_xiaokai=${host_isoftstone_xiaokai[0]}
if [ ! -n "$host_isoftstone_xiaokai" ]; then
    echo "not found, append"
    echo "$host_ip	${host_name}	${host_name}" >> /etc/hosts
else 
    echo "found, cover"
    host_isoftstone_xiaokai_TMP="$host_ip	${host_name}	${host_name}"
    sed -i "$[ host_isoftstone_xiaokai ]c $host_isoftstone_xiaokai_TMP" /etc/hosts
fi
echo "${host_name} welcome!"
while [[ 1 ]];do
    read -p "Initialize ${RED}user name${RESET}（${YELLOW}in English${RESET}） :" username
    if [[ ! ${username} =~ ${regex} ]];then
        echo "${RED}illegal, again.${RESET}"
        continue
    else
        break
    fi
done
while [[ 1 ]];do
    read -p "Initialize for machine ${BLUE}${username}${RESET} ${RED}password${RESET} : " USER_PASSWD
    read -p "Your password is ${GREEN}${USER_PASSWD}${RESET}, please enter ${YELLOW}y${RESET} to confirm, any other characters will allow you to set the password again [y/n]:" in_tmp
    if [[ ${in_tmp} == 'y' ]];then
        break
    else
        continue
    fi
done
useradd  ${username} -G sudo -m && echo "Add user successfully" ||( userdel -rf ${username}; echo "User del ${username}" && useradd ${username} -G sudo -m  && echo "Add user successfully")
sleep 1
(
    sleep 1
    echo ${USER_PASSWD}
    sleep 1
    echo ${USER_PASSWD}
)|passwd ${username}
if [ $? -eq 0 ];
    then
    echo "PASSWD changed successfully"
    else
    echo "PASSWD change failed"
    exit
fi
echo "Finish set the user!"
cp isoftstone_env.sh /home/${username}/
chown ${username} /home/${username}/isoftstone_env.sh
chgrp ${username} /home/${username}/isoftstone_env.sh
chmod a+x /home/${username}/isoftstone_env.sh
q=`grep -n '%sudo	ALL=(ALL:ALL) ALL' /etc/sudoers | awk -F':' '{print $1}'`
if [ ! -n "$q" ]; then
    echo "sudo right has changed"
else
    TMPq='%sudo	ALL=(ALL:ALL) NOPASSWD: ALL'
    sed -i "$[ q ]c $TMPq" /etc/sudoers
    echo "sudo right finish change"
fi
num=`grep -n 'Defaults   visiblepw' /etc/sudoers | awk -F':' '{print $1}'`
if [ ! -n "$num" ]; then
  echo "Defaults visiblepw IS NULL"
  sudo echo 'Defaults   visiblepw' >> /etc/sudoers
else
  echo "Defaults visiblepw NOT NULL"
fi
su - $username -c "bash isoftstone_env.sh $username ${USER_PASSWD}"
rm /home/$username/install_vim.sh*
rm /home/$username/isoftstone_env.sh*
rm /home/$username/install_zsh.sh*
l=`grep -n '%sudo	ALL=(ALL:ALL) NOPASSWD: ALL' /etc/sudoers | awk -F':' '{print $1}'`
if [ ! -n "$l" ]; then
    echo "sudo command do not need to change or check the original file."
else
    TMPl='%sudo	ALL=(ALL:ALL) ALL'
    sudo sed -i "$[ l ]c $TMPl" /etc/sudoers
    echo "sudo content recovers"
fi
cd
rm ./init_env.sh
rm ./isoftstone_env.sh
echo -e "Your username : ${BLUE}${username}${RESET}, password : ${GREEN}${USER_PASSWD}${RESET}\nPlease log in with the new account."
su - ${username}
