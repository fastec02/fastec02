#!/bin/sh

git add *
git commit -a -m "update $(date)"
git push
