while read line
do
	name=$line
	OIFS=$IFS
	IFS=';'
	let i=1
	for w in $name
		do
	    if [ $i = 3 ] ; then
        	IFS=':'
		let j=1
		for w2 in $w	
			do
			if [ $j = 2 ] ; then
				confidence=$w2
			fi
		let j=$[$j+1]
		done
            fi
	    let i=$[$i+1]
	done
	IFS=$OIFS
done < 'final_output.txt'
echo $confidence
