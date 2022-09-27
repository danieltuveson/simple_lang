#!/bin/zsh
# File for autogenerating c header files
# For now just grabs lines starting with two words and a group of stuff in parentheses
# Will probably make this more sophisticated in the future, but works well
# enough for properly indented K&R style declarations

if [[ -z $1 ]]; then
    echo "Error: must supply name of header file"
    exit 1
fi

header_name=$1
lcase_header_name=$(echo $header_name | tr "[:upper:]" "[:lower:]")
file_name=$lcase_header_name.c

if [[ ! -e $file_name ]]; then
    echo "Error: could not find a header file named \"$file_name\""
    exit 1
fi

cat $file_name | egrep '^((\w|\d|\_|\*)+\s+)+(\w|\d|\_|\*)+\([^\)]*\)' | sed -r 's/$/;/g' > $file_name.generated.h
