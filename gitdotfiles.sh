#/bin/sh

# src=`pwd`

dst=~/Documents/Github/conf

cp -rf $DOTFILES/* $dst/
cp ~/.zshrc $dst/zshrc-mac

cd $dst
git add --all
git commit -m "updated."
git push
