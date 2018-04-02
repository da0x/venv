# venv

venv is file stashing command line tool. it's designed to be a very simple file stashing mechanism that does not impose any rules. it helps you create different local file repositories (each repo is called a venv). you can store files and switch working copies between them easily.

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
$ venv -add test -to conf1
test -> .venv/conf1/1c44ff32-b377-4f62-9c2a-81734ee35fc4
$ venv -add test -to conf2
test -> .venv/conf2/b59fb797-12e9-4087-a38c-44e0a002ff59
```
the file is automatically copied into the venv.

let's try modifying it:
```
$ echo changes for conf1 > test
```

now we can start switching between them. you can use the explicit option -select or simply type the name of your venv

```
$ venv conf2
pushing [conf1]
test -> .venv/conf1/1c44ff32-b377-4f62-9c2a-81734ee35fc4
pulling [conf2]
.venv/conf2/b59fb797-12e9-4087-a38c-44e0a002ff59 -> test
$ cat test
conf


$ venv conf1
pushing [conf2]
test -> .venv/conf2/b59fb797-12e9-4087-a38c-44e0a002ff59
pulling [conf1]
.venv/conf1/1c44ff32-b377-4f62-9c2a-81734ee35fc4 -> test


$ cat test
changes for conf1

```
to see status of current working copy:
```
$ venv

   list                                                                 
   -   -----   ----   ------------------------------------------------  
       label   path   internal path                                     
                                                                        
   ☒   conf1   test   .venv/conf1/1c44ff32-b377-4f62-9c2a-81734ee35fc4  
   ☐   conf2   test   .venv/conf2/b59fb797-12e9-4087-a38c-44e0a002ff59  
   -   -----   ----   ------------------------------------------------  


```
to push your changes without switching the current venv:
```
$ venv -push
```
or alternatively to pull a fresh copy to reset your copy:
```
$ venv -pull
```
finally, finally you can remove a specific venv with -remove, you can rename a venv with -rename, or you can remove everything venv related with -deinit
```
$ venv -rename conf2 conf_2
.venv/conf2 -> .venv/conf_2

$ venv -remove conf_2
removing [conf_2]
.venv/conf_2/b59fb797-12e9-4087-a38c-44e0a002ff59
.venv/conf_2

$ venv -deinit
removing all venvs
.venv/venv
.venv/conf1/1c44ff32-b377-4f62-9c2a-81734ee35fc4
.venv/conf1
.venv

```

that's all
