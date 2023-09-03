x=$(tr " " "\n" < $1| sort | uniq -c | sort -n | wc -l)
tr " " "\n" < $1 | sort | uniq -c | sort -n | head -$[x/2+1] | tail -1 | cut -d" " -f5

