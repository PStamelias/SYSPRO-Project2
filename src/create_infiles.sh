#!/bin/bash
if  [ $2 -lt 0 ]
then
    echo "Negative Given Num_of_files"
    exit
fi
if [ $2 -eq 0 ]
then
	echo "Error:Cannot have zero num of files"
	exit
fi
if  [ $3 -lt 0 ]
then
    echo "Negative Given Num_of_Dirs"
    exit
fi

if  [ $4 -lt 0 ]
then
    echo "Negative Given levels"
    exit
fi
#Check if directory exists,if not create it
if [ ! -d $1 ]; then
	mkdir -p $1
fi
#Creation of Directory Names
for ((i=1;i<=$3;i++)); do
	Name[$i]=""
	rand=$((1 + RANDOM%(1+8-1)))
	#echo $rand
	for ((j=1;j<=$rand;j++)); do
		NEW_UUID=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 1 | head -n 1)
		Name[$i]+=$NEW_UUID
	done
done
#Creation of Directories
end=$4
start=$1
val=0
for ((i=1;i<=$3;i++)); do
	if (($val == 0)); then
		Dest=$start
	fi
	Path=$Dest
	Path+=/
	Path+=${Name[$i]}
	Dest=$Path
	Fakelos[$i]=$Path
	mkdir -p $Path
	val=$((val+1))
	if (($val == $end)); then
		val=0
	fi
done 
#Creation of FileNames
for ((i=1;i<=$2;i++)); do
	File[$i]=""
	rand=$((1 + RANDOM%(1+8-1)))
	for ((j=1;j<=$rand;j++)); do
		NEW_UUID=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 1 | head -n 1)
		File[$i]+=$NEW_UUID
	done
done
#Add dir_name/
size=$(($3+1))
for ((i=1;i<=$size;i++)); do
	last=$((i-1))
	if (($i == 1)); then
		Table[$i]=$1
	else  
		Table[$i]=${Fakelos[$last]}
	fi
done

#Creation of Paths
j=1
for((i=1;i<=$2;i++)); do
	Folder[$i]=""
	Arxi=${Table[$j]}/
	Arxi+=${File[$i]}
	Folder[$i]=$Arxi
	if (($j == $size)); then
		j=1
	fi
	j=$((j+1))
done
#Write Characters
t="$PWD"
for((i=1;i<=$2;i++)); do
	rand=$((1000 + RANDOM%(1000+128000-1000)))
	number=$rand
	g=$t
	g+=/${Folder[$i]};
	echo $number
	echo "g="$g
	for((j=1;j<=$number;j++)); do
		NEW_UUID=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 1 | head -n 1)
		echo  -n $NEW_UUID >> "$g"
	done
done
