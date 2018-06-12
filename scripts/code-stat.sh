#!/bin/bash

function is_author() {
	count=`git log --author $1 | wc -l`
	return $count
}

function caculate_code() {
	is_author $1
	if [ $? -eq 0 ]
	then
		echo "author("$1") is not existed"
		return -1
	fi

	echo -n $1": "
	git log --author $1 --pretty=tformat: --numstat  |
		awk '{add += $1; subs += $2; loc += $1 - $2 }
		END{printf "added %s, removed %s, total %s\n", add, subs, loc}'
	return 0
}

author=`git shortlog --numbered --summary | awk '{printf $2"\n"}'`

if [ $# -le 0 ]
then
	echo "all author summary:"
else
	caculate_code $1
	exit 0
fi


for au in $author
do

caculate_code $au

done
