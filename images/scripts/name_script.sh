#!/bin/bash
var=$(ls -v | cat -n)



for d in */ ;do 
	cd "$d";
	number=0
       	for f in `ls -v *.png`; do
	mv -n "$f" "MVC2_Ryu_"$number".png"
	((number=number+1));
	done
	cd ..;
done

