#!/bin/bash -x

if test "$(uname -s)" == Linux; then
	TIME_FLAG=-v
	RELOU_FLAG="-rpath ."
else
	TIME_FLAG=-l
	RELOU_FLAG=
fi

nm libft_malloc.so | grep -E 'free|alloc'

for i in 0 1 2 3 3_bis 4 5 6 7 8; do
    clang -Wall -Wextra -Iinc -pthread -o "build/test$i" "test/test$i.c" $RELOU_FLAG -L. -lft_malloc \
        && clang -pthread -o "build/ctrl$i" "test/test$i.c" -D CTRL \
        && diff -y \
                <(./run.sh /usr/bin/time "$TIME_FLAG" "./build/test$i" 2>&1) \
                <(/usr/bin/time "$TIME_FLAG" "./build/ctrl$i" 2>&1)
done

PAGE_TEST0="$(./run.sh /usr/bin/time $TIME_FLAG ./build/test0 2>&1 | grep 'page rec' | cut -dp -f1)"
PAGE_TEST1="$(./run.sh /usr/bin/time $TIME_FLAG ./build/test1 2>&1 | grep 'page rec' | cut -dp -f1)"
echo "$PAGE_TEST1 - $PAGE_TEST0" | bc

exit 0 # eheh
