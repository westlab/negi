#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [ -z "$@" ]
  then
    python $DIR/cpplint.py --root=src ./src/*.cpp ./src/*.h
    OUT=$?
    if [ $OUT -eq 0 ];then
        echo "Lint is passed"
        exit 0
    else
        echo "Plase fix cpp lint"
        exit $OUT
    fi
fi

python $DIR/cpplint.py --root=src $@
