# Simple http server using only `C` and Unix syscalls

## Build
### Linux, MacOS

- Compile
```bash
make server
```

- Compile and run
```bash
make run
```

### Windows
Only using `Docker`

1. Build the image:
```bash
docker build -t server-example .
```

2. Run it:
```bash
docker run -it -p 8080:8080 server-example
```

If everything went well server can be accessed on: <http://localhost:8080>
