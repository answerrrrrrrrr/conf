# Path to your oh-my-zsh installation.
export ZSH=$HOME/.oh-my-zsh
ZSH_THEME="air9"

# DISABLE_AUTO_UPDATE="true"
export UPDATE_ZSH_DAYS=30
ENABLE_CORRECTION="true"

# Uncomment the following line if you want to disable marking untracked files
# under VCS as dirty. This makes repository status check for large repositories
# much, much faster.
# DISABLE_UNTRACKED_FILES_DIRTY="true"

# Uncomment the following line if you want to change the command execution time
# stamp shown in the history command output.
# The optional three formats: "mm/dd/yyyy"|"dd.mm.yyyy"|"yyyy-mm-dd"
# HIST_STAMPS="mm/dd/yyyy"

# Custom plugins may be added to ~/.oh-my-zsh/custom/plugins/
plugins=(git autojump colored-man-pages sudo zsh-syntax-highlighting !history)
source $ZSH/oh-my-zsh.sh

# export MANPATH="/usr/local/man:$MANPATH"
export PATH="/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/bin:/sbin"
export DOTFILES=~/Documents/Nutstore/dotfiles

source $DOTFILES/tmuxinator.zsh
export EDITOR='vim'

# You may need to manually set your language environment
# export LANG=en_US.UTF-8

# Compilation flags
# export ARCHFLAGS="-arch x86_64"

# ssh
# export SSH_KEY_PATH="~/.ssh/rsa_id"

alias p='python'
alias mv='mv -v'
alias cp='cp -rv'
alias tmux='tmux -2'
alias pg='ps -ef | grep -v grep | grep'
alias op='dolphin . 2>/dev/null || nautilus . 2>/dev/null || open .'
alias path='echo $path | python -c "for i in raw_input().split(): print i"'

alias vv='vim $DOTFILES/vimrc'
alias vt='vim $DOTFILES/tmux.conf'
alias vz='vim $DOTFILES/zshrc'
alias vzz='vim ~/.zshrc'
alias sz='source ~/.zshrc'

alias sv='source ~/venv/bin/activate'
alias sv3='source ~/venv3/bin/activate'
alias glg='glol'
alias gitzsh='less ~/.oh-my-zsh/plugins/git/git.plugin.zsh'
alias gitdotfiles='sh $DOTFILES/gitdotfiles.sh'

alias ttc='tmux show-buffer | xclip -selection clipboard'
alias tb='tmux show-buffer'

alias -s zip='unzip'
alias -s gz='tar -xzvf'
alias -s tgz='tar -xzvf'
alias -s bz2='tar -xjvf'

# Safe rm
mkdir -p ~/.trash
alias rm=rmt
rmt() {     # trash
    mv $@ ~/.trash/
}
rml() {     # list
    ls -lAh ~/.trash
}
rme() {     # empty
    #read -p "Clear trash?[y/N]" confirm
    #[ $confirm = 'y' ] || [ $confirm = 'Y' ] && /bin/rm -rfv ~/.trash/.* ~/.trash/*
    /bin/rm -rfv ~/.trash/.* ~/.trash/*
}

# fuzz finder, source after set PATH
[ -f ~/.fzf.zsh ] && source ~/.fzf.zsh
