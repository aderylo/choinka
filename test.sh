#!/bin/bash
files=$(ls $2 | grep .in)

if [[ "$2" == "." ]]; then
    tests_location=$(pwd)
else
    tests_location=$2
fi

echo $1 

for file in $files
do
    file_name=$(basename $file)
    file_name_witout_ext=${file_name%%.*}

    tput sgr 0
    echo -n $file_name
    echo -n ' '

    $1 < $tests_location/$file_name > out 2> err

    stdout=$(diff out $tests_location/$file_name_witout_ext.out)
    # stderr=$(diff err $tests_location/$file_name_witout_ext.err)
    
    rm out
    rm err 

    if [ "$stdout" == "" ]
    then
        tput setaf 2
        echo OK
    else
        tput setaf 1
        echo ANS
        break
    fi
done