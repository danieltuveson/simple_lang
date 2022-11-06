#!/usr/bin/env zsh
# Script used to create a new .c and .h file

if [[ -z 1 ]]; then
    echo "Error: must supply name of header file"
    exit 1
fi

header_name=$1
ucase_header_name=$(echo $header_name | tr "[:lower:]" "[:upper:]")
lcase_header_name=$(echo $header_name | tr "[:upper:]" "[:lower:]")

file_name=$lcase_header_name.h

if [[ -e $file_name ]]; then
    echo "Error: a header file named $file_name already exists"
    exit 1
fi

write_template() {
    echo "#ifndef $ucase_header_name"
    echo "#define $ucase_header_name"
    echo ""
    echo "#include \"$lcase_header_name.c.generated.h\""
    echo "#endif"
}

write_template > $file_name

c_file_name=$lcase_header_name.c

if [[ -e $c_file_name ]]; then
    echo "A file named $c_file_name already exists, skipping generating $c_file_name"
else
    echo "#include \"$file_name\"" > $c_file_name
    echo "" >> $c_file_name
    mv $lcase_header_name.* src
fi

exit 0
