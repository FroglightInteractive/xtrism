#! /bin/zsh -f

rm -f allofem.txt
touch allofem.txt

for a in *.H
do
    echo --------------------------------------------------------------------------- >> allofem.txt
    echo '|||||||||||          ' $a >> allofem.txt
    echo >> allofem.txt
    cat $a >> allofem.txt
    b=`basename $a .H`.C
    if [[ -f $b ]]; then
	echo >> allofem.txt
	echo --------------------------------------------------------------------------- >> allofem.txt
	echo '|||||||||||          ' $b >> allofem.txt
	echo >> allofem.txt
	cat $b >> allofem.txt
	fi
    echo -e '\014' >> allofem.txt
done
