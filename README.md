# Marvel vs Capcom
Taller de Programación I (Grupo Capcom)

### Steps to compile
You can use the script that will do all the commands:
```
$ ./setUp.sh
```
#### The script does the following commands:
- install make and cmake 
```
$ sudo apt install make 
$ sudo apt install cmake
```
- install g++
```
$ sudo apt install g++
``` 
- install sdl2 
```
$ sudo apt install libsdl2-dev
```
- install sdl2-image x
```
$ sudo apt install libsdl2-image-dev
```
- install mixer 
```
$ sudo apt install libsdl2-mixer-dev
```
- compile 
```
$ cmake CmakeLists.txt"
$ make
```

### To run the game:



Running server: 
```
./server.sh
```
Running client:
```
.client.sh 
```
##### Notes: 
1- The client is set as local by default, if you want to use a different IP you can change it in the shell script "client.sh".

2- You can change the quantity of players from 2 to 4 in the config.json file located on the folder config.