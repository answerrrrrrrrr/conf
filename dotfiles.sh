#/bin/sh

filepath=$(cd "$(dirname "$0")"; pwd)

sudo apt-get install zsh tmux autojump wget guake
# sudo apt-get install open-vm-tools-desktop fuse


if [ -f ~/.vimrc ]; then
    mv ~/.vimrc ~/.vimrc.bak
fi
echo "source $filepath/vimrc" > ~/.vimrc

#if [ ! -d ~/.vim/bundle ]; then
    #git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim
#else
    #echo "[*] Vundle exists."
#fi


if [ ! -d ~/.vim/autoload ]; then
    curl -fLo ~/.vim/autoload/plug.vim --create-dirs \
            https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
else
    echo "[*] vim-plug exists."
fi


if [ -f ~/.tmux.conf ]; then
    mv ~/.tmux.conf ~/.tmux.conf.bak
fi
echo "source $filepath/tmux.conf" > ~/.tmux.conf


if [ -f ~/.gitconfig ]; then
    mv ~/.gitconfig ~/.gitconfig.bak
fi
cp $filepath/gitconfig ~/.gitconfig


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
cp amber.zsh-theme  ~/.oh-my-zsh/themes


#if [ ! -d ~/.fzf ]
#then
    #git clone --depth 1 https://github.com/junegunn/fzf.git ~/.fzf
    #~/.fzf/install
#else
    #echo "[*] fzf exists."
#fi


chsh -s /bin/zsh
#echo "export DOTFILES=$filepath" >> $filepath/zshrc


echo -n "[*] Ready to reboot? [y/N]"
read choice
if [ $choice = "y" -o $choice = "Y" ]
then
    echo "[*] Rebooting..."
    sudo reboot
else
    echo "[*] Reboot later..."
fi
