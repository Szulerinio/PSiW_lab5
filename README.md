# PSiW_lab5

Project for System and Concurrent programming class at Poznan University of Technology. 

Linux program to split .jpg to 3 .jpg containing 1 RGB color each on separate threads


## compilation

you will need gcc compiler and jpeglib pthread libraries

compile with
``` c
    gcc zad5.c -o out -ljpeg -pthread

```

## running
```c
    ./out [jpg filename]
```
example

```c
    ./out "oko_cut.jpg"
```

tested on manjaro 5.13 only
