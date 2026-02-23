#!/bin/bash
echo "This script will install and configure vim and zsh automatic."
echo "The time this takes is related to the network conditions, please wait patiently."
if [[ `whoami` == "root" ]];then
    echo -e "\033[31mYou are  running this script with Root\033[0m"
    echo -e "\033[31mGenerally, we do not recommend using root for programming or directly controlling your Linux OS, especially when you are a beginner \033[0m"
    echo -e "\033[31mSo, There is no necessary for you to configure with root."
    read -p "Do you really want to do this?[N/y]" choice
    if [[ ${choice} != y ]];then
    	echo "Bye."
    	exit 1
    fi
fi
if which apt-get >/dev/null; then
	sudo apt-get install -y vim  universal-ctags  xclip astyle python-setuptools  git wget
elif which yum >/dev/null; then
	sudo yum install -y gcc vim git ctags xclip astyle python-setuptools python-devel wget	
fi
if which brew >/dev/null;then
    echo "You are using HomeBrew tool"
    brew install vim ctags git astyle
fi
sudo easy_install -ZU autopep8 
sudo ln -s /usr/bin/ctags /usr/local/bin/ctags
rm -rf ~/vim* 2>&1 >/dev/null
rm -rf ~/.vim* 2>&1 >/dev/null
mv -f ~/vim ~/vim_old
cd ~/ && git clone https://gitee.com/hzx_3/vim.git
mv -f ~/.vim ~/.vim_old 2>&1 >/dev/null
mv -f ~/vim ~/.vim 2>&1 >/dev/null
mv -f ~/.vimrc ~/.vimrc_old 2>&1 >/dev/null
mv -f ~/.vim/.vimrc ~/ 
git clone https://gitee.com/hzx_3/vundle.git ~/.vim/bundle/vundle
echo "Downloading..." > haizei_log
echo "Command-t takes time, do not worry" >> haizei_log
echo "DO NOT exit" >> haizei_log
echo "Automatic exit will happen after done" >> haizei_log
echo "Please wait patiently" >> haizei_log
vim haizei_log -c "BundleInstall" -c "q" -c "q"
rm haizei_log
echo "Finish installation."
