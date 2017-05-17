#/bin/sh

filepath=$(cd "$(dirname "$0")"; pwd)

sudo apt-get install zsh tmux autojump wget curl
# sudo apt-get install guake
# sudo apt-get install open-vm-tools-desktop fuse



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



# git
if [ -f ~/.gitconfig ]; then
    mv ~/.gitconfig ~/.gitconfig.bak
fi
cp $filepath/gitconfig ~/.gitconfig



# zsh
if [ -f ~/.zshrc ]; then
    mv ~/.zshrc ~/.zshrc.bak
fi
echo "source $filepath/zshrc" > ~/.zshrc

if [ ! -d ~/.oh-my-zsh ]
then
    wget https://github.com/robbyrussell/oh-my-zsh/raw/master/tools/install.sh -O - | sh
else
    echo "[*] Oh-my-zsh exists."
fi
# cp amber.zsh-theme  ~/.oh-my-zsh/themes



# Source Code Pro
if [ ! -d ~/.fonts ]; then
    mkdir ~/.fonts
fi
cp -r OTF/* ~/.fonts
fc-cache -f -v



# fzf
if [ ! -d ~/.fzf ]
then
    git clone --depth 1 https://github.com/junegunn/fzf.git ~/.fzf
    ~/.fzf/install
else
    echo "[*] fzf exists."
fi



# shadowsocks
# sudo apt-get install python-pip
# pip install shadowsocks



# lantern
# sudo apt install gdebi-core
# sudo gdebi ../lantern-installer-beta-64-bit.deb



# chsh & reboot
chsh -s /bin/zsh

echo -n "[*] Ready to reboot? [y/N]"
read choice
if [ $choice = "y" -o $choice = "Y" ]
then
    echo "[*] Rebooting..."
    sudo reboot
else
    echo "[*] Reboot later..."
fi
