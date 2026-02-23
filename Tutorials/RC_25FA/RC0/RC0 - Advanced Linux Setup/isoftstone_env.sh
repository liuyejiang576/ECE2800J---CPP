#!/bin/bash
new_user=$1
PASSWD=$2
if [ ! -f "/home/$new_user/install_vim.sh" ]; then
    echo "install_vim.sh file not exist"
else
    echo "install_vim.sh file exists, delete."
    rm -f /home/$new_user/install_vim.sh*
fi
sudo mv /root/install_vim.sh /home/$new_user/
bash install_vim.sh
echo "Set vim successfully\n" >> isoftstone_env_log
echo "Set zsh\n" >> isoftstone_env_log
if [ ! -f "/home/$new_user/install_zsh.sh" ]; then
    echo "install_zsh.sh not exist"
else
    echo "install_zsh.sh file exists, delete."
    rm -f  /home/$new_user/install_zsh.sh*
fi
sudo mv /root/install_zsh.sh /home/$new_user/
bash install_zsh.sh ${PASSWD}
git clone https://gitee.com/song_df/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting
sed -i 's/plugins=(git)/plugins=(git zsh-syntax-highlighting)/' ~/.zshrc
echo "Set zsh successfully，content replaced.">> isoftstone_env_log
echo "Set automatically filling tools...\n">> isoftstone_env_log
mkdir ~/.oh-my-zsh/plugins/incr
wget wiki.haizeix.com/courses_resource/cloud_usage/incr-0.2.zsh -O ~/.oh-my-zsh/plugins/incr/incr.plugin.zsh
echo "Automatically filling tools done\n" >> isoftstone_env_log
echo "jump tools setting...\n" >> isoftstone_env_log
echo 'autoload -U colors && colors'  >> ~/.zshrc
echo 'PROMPT="%{$fg[red]%}%n%{$reset_color%}@%{$fg[blue]%}%m %{$fg[yellow]%}%1~ %{$reset_color%}%# "' >> ~/.zshrc
echo 'RPROMPT="[%{$fg[yellow]%}%?%{$reset_color%}]"' >> ~/.zshrc
echo '[ -r "/etc/zshrc_$TERM_PROGRAM" ] && . "/etc/zshrc_$TERM_PROGRAM"' >> ~/.zshrc
echo 'source /usr/share/autojump/autojump.sh' >> ~/.zshrc
echo 'source ~/.oh-my-zsh/plugins/incr/incr*.zsh' >> ~/.zshrc
ctags -I __THROW -I __attribute_pure__ -I __nonnull -I __attribute__ --file-scope=yes --langmap=c:+.h --languages=c,c++ --links=yes --c-kinds=+p --c++-kinds=+p --fields=+iaS --extra=+q -R -f ~/.vim/systags /usr/include/ /usr/local/include
echo 'set tags+=~/.vim/systags' >> ~/.vimrc
echo "All Done!"
exit
