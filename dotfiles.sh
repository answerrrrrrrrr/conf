#/bin/bash

# set -x    # Display commands

filepath=$(cd "$(dirname "$0")"; pwd)



if [ `whoami` != "root" ];then
    SUDO="sudo"
fi
$SUDO apt-get install -y zsh tmux autojump wget curl dnsutils ranger build-essential
# $SUDO apt-get install -y guake
# $SUDO apt-get install -y open-vm-tools-desktop fuse



# vim
if [ -f ~/.vimrc ]; then
    mv ~/.vimrc ~/.vimrc.bak
fi
echo "source $filepath/vimrc" > ~/.vimrc

if [ ! -d ~/.vim/autoload ]; then
    curl -fLo ~/.vim/autoload/plug.vim --create-dirs \
            https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
else
    echo "[*] vim-plug exists."
fi



# tmux
if [ -f ~/.tmux.conf ]; then
    mv ~/.tmux.conf ~/.tmux.conf.bak
fi
echo "source $filepath/tmux.conf" > ~/.tmux.conf



# zsh
if [ ! -d ~/.oh-my-zsh ]
then
    wget https://github.com/robbyrussell/oh-my-zsh/raw/master/tools/install.sh -O - | sh
    git clone git://github.com/zsh-users/zsh-syntax-highlighting ~/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting
else
    echo "[*] Oh-my-zsh exists."
fi
echo "source $filepath/air9.zsh-theme" > ~/.oh-my-zsh/themes/air9.zsh-theme

if [ -f ~/.zshrc ]; then
    mv ~/.zshrc ~/.zshrc.bak
fi
echo "source $filepath/zshrc" > ~/.zshrc



# Source Code Pro
#if [ ! -d ~/.fonts ]; then
    #mkdir ~/.fonts
    #cp -r OTF/* ~/.fonts
    #fc-cache -f -v
#fi



# fzf
if [ ! -d ~/.fzf ]
then
    git clone --depth 1 https://github.com/junegunn/fzf.git ~/.fzf
    ~/.fzf/install
else
    echo "[*] fzf exists."
fi



# $SUDO apt-get install python-pip
# pip install powerline-status
# pip install shadowsocks

# lantern
# $SUDO apt-get install -y gdebi-core
# $SUDO gdebi ../lantern-installer-beta-64-bit.deb



chsh -s /bin/zsh

echo -n "[*] Ready to reboot? [y/N]"
read choice
if [ "$choice" = "y" -o "$choice" = "Y" ]
then
    echo "[*] Rebooting..."
    $SUDO reboot
fi
