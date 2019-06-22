#!/bin/bash
var=$(ls -v | cat -n)


for d in */ ;do
        cd "$d";
        number=0
	$(rename 's/ /_/g' *)
	for file in $(ls -v *.png); do
	mv -n "$file" "IMAGEee_"$number".png"
        ((number=number+1));
	done
        cd ..;
done

