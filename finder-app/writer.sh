#!/bin/bash

# exit value 1, print statement for missing parameters
if [ ! $# -eq 2 ]
then
    echo "ERROR: Invalid number of arugments $#"
    echo "Usage: writer.sh /path/to/new/file <write_string>"
    exit 1
fi

# first argument is a path to directoy on filesystem
writefile=$1

# second argument is text string to be search for within files
writestr=$2

# create path if it doesn't exist
if [ ! -e $writefile ]
then
    # remove filename and check if directory exists
    dir_name="${writefile%/*}/"
    if [ ! -d $dir_name ]
    then
        mkdir -p $dir_name
    fi
fi

# overwrite or create new file
echo "$writestr" > $writefile

# exits value 1, print statement if file can't be created
if [ $? -ne 0 ]
then
    echo "ERROR: Failed to write to $writefile"
    exit 1
fi
