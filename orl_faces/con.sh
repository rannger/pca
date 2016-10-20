#/bin/sh

file_list=$(ls|grep '^s'|sort)



for dir_name in ${file_list[@]}
do
	files=$(ls $dir_name|sort)
	declare -i i=1
	for file_name in ${files[@]}
	do
		if test $i -lt 10 
		then
			mv $dir_name/$i".pgm" $dir_name"0"$i".pgm"
		else
			mv $dir_name/$i".pgm" $dir_name$i".pgm"
		fi 
		
		i=$i+1
	done
done