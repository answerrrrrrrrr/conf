#/bin/sh

# src=`pwd`
dst=~/Documents/Github/conf/

cp -rf $DOTFILES/* $dst

cd $dst

git add --all
git commit -m "update dotfiles."
git push
