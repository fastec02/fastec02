#!/bin/sh

git remote set-url --add origin git@github.com:fastec02/fastec02
git remote set-url --delete origin git://github.com/fastec02/fastec02.git
git add *
git commit -a -m "update $(date)"
git push
