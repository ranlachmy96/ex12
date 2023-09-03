langs=$(cat $1 | cut -d":" -f2 | tr "," "\n" | sort | uniq)

#while read line
for line in $langs
do
	echo -n $line ":" 
	x=$(egrep $line $1 | cut -d":" -f1)
	echo $x 
done > result
 
