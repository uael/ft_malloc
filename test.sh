#!/bin/bash -x

if test "$(uname -s)" == Linux; then
	TIME_FLAG=-v
	RELOU_FLAG="-rpath ."
else
	TIME_FLAG=-l
	RELOU_FLAG=
fi

nm libft_malloc.so | grep -E 'free|alloc'

for i in 0 1 2 3 3_bis 4 6 8; do
    clang -Wall -Wextra -Iinclude -pthread -o "build/test$i" "test/test$i.c" $RELOU_FLAG
done
for i in 5 7 9; do
    clang -Wall -Wextra -Iinclude -pthread -L. -lft_malloc -o "build/test$i" "test/test$i.c" $RELOU_FLAG
done

exit 0 # eheh
