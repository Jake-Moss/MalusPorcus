# Building
```
git clone https://github.com/Jake-Moss/MalusPorcus
git submodule init
git submodule update
```
## Desktop
```
make raylib -j8
make 
make run
```

## Web
```
make emsdk
make raylib -j8 P=WEB

make P=WEB
make host
```

# Things done broke
- Something something linker error. Rebuild raylib for either web or desktop, you can't have both at the same time
- Can't find `emcc`, you should `source ./emsdk/emsdk_env.sh`, or use `direnv`

