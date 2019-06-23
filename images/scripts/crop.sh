#!/bin/bash
var=$(ls -v | cat -n)



for d in */ ;do 
	cd "$d";
       	mogrify -crop 320x240+160+120 +repage *.png;
       	cd ..;done

