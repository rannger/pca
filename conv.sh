#/bin/zsh

file_list=$(ls|sort)

for file_name in ${file_list[@]}
do
	iconv -f utf-8 -t gb2312 ./$file_name > ./"${file_name}.tmp"
	rm ./$file_name
	mv "./${file_name}.tmp" "./$file_name"
done
