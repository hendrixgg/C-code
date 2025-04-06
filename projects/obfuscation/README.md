Switched to C89 to get strict aliasing, this removes the need for union types.


build:
```
gcc -o example example.c obfuscation.c -Wall -Wextra -Wpedantic -std=c89 -O3
```

run:
```
.\example.exe
```