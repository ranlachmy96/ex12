rm -f tmp
find $1 -type f -exec ./my.sh {} \;
cat tmp | uniq -d > tmp2

best_line=""
best_num=0
while read line; do
	for file in $(find $1 -name $line);do
		wc=$(echo $file | tr "/" "\n" | wc -l)	
		if [ $wc -gt $best_num ];then
			best_num=$wc
			best_line=$file		
		fi
	done 
done<tmp2
echo $best_line
