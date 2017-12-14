# venv

venv is a command line tool that helps you create different local file repositories where you can store files and switch working copies between them easily

venv is similar to "git stash". however it does not diff internal file contents. instead it swaps out the entire file. this prevents horrific merge conflicts in certain xml (plist) files. and it can all be added to your source control with no issues.


tutorial

to see the commands, run:
```
$ venv -help
```
to initialize venv, run:
```
$ venv -init
$ venv -create conf1
$ venv -create conf2
```
when you create a new venv repo, it's automatically set to be the active venv. in this case it's conf2.

to switch to conf1 run:
```
$ venv -select conf1
```
to add a file:
```
$ echo conf > test
$ ./venv -add test -to conf1
test -> .venv/a8d5c723-655e-4eb6-98be-453fe8c85a7f/a747bed0-298b-4ffe-8bfd-158ab7bd8cda
$ ./venv -add test -to conf2
test -> .venv/a295705e-2874-45c2-a66a-7f7de274e6d7/056397d7-221e-4b27-8cda-a7798122ea26
```
the file is automatically copied into the venv.

let's try modifying it:
```
$ echo changes for conf1 > test
$ ./venv -select conf2
pushing...
test -> .venv/a8d5c723-655e-4eb6-98be-453fe8c85a7f/a747bed0-298b-4ffe-8bfd-158ab7bd8cda
pulling...
.venv/a295705e-2874-45c2-a66a-7f7de274e6d7/056397d7-221e-4b27-8cda-a7798122ea26 -> test
$ cat test
conf
$ ./venv -select conf1
pushing...
test -> .venv/a295705e-2874-45c2-a66a-7f7de274e6d7/056397d7-221e-4b27-8cda-a7798122ea26
pulling...
.venv/a8d5c723-655e-4eb6-98be-453fe8c85a7f/a747bed0-298b-4ffe-8bfd-158ab7bd8cda -> test
$ cat test
changes for conf1

```
to see status of current working copy:
```
$ ./venv

   venv list:                                               
   ------------------------------------   -----   -----   --------  
   id                                     files   name    selected  
                                                                    
   a8d5c723-655e-4eb6-98be-453fe8c85a7f   1       conf1   *         
   a295705e-2874-45c2-a66a-7f7de274e6d7   1       conf2             
   ------------------------------------   -----   -----   --------  

Files test and .venv/a8d5c723-655e-4eb6-98be-453fe8c85a7f/a747bed0-298b-4ffe-8bfd-158ab7bd8cda differ
```
to push your changes:
```
$ venv -push
```
or alternatively to pull a fresh copy:
```
$ venv -pull
```
finally, to remove everything venv related and keep the current working copy:
```
$ venv -deinit
```

that's all