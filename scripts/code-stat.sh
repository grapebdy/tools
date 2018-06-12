#!/bin/bash

function is_author() {
	count=`git log --author $1 | wc -l`
	return $count
}


function result_summary() {
	echo name added removed total | awk '{printf "\t%8s \t%10s \t%10s \t%10s\n", $1, $2, $3, $4}'
}

function caculate_specific() {

	is_author $1
	if [ $? -eq 0 ]
	then
		echo "author("$1") is not existed"
		return -1
	fi

	echo -n $1 | awk '{printf "%16s ", $1}'

	git log --author $1 --pretty=tformat: --numstat  |
		awk '{add += $1; subs += $2; loc += $1 - $2 }
		END{printf "\t%10s \t%10s \t%10s\n", add, subs, loc}'
	return 0
}

function author_all() {

	author=`git shortlog --numbered --summary | awk '{printf $2"\n"}'`
	for au in $author
	do

	caculate_specific $au

	done
}


if [ $# -le 0 ]
then
	echo "all author summary:"
	result_summary
	author_all
else
	result_summary
	caculate_specific $1
fi
