# DevOS Container Environment

Isolated development environment for building OS components.

## Setup
```bash
chmod +x build.sh start.sh stop.sh shell.sh clean.sh
./build.sh
./start.sh
```

## Commands

- `./build.sh` - Build the Docker container
- `./start.sh` - Start and enter DevOS environment
- `./stop.sh` - Stop the environment
- `./shell.sh` - Open another terminal in the running container
- `./clean.sh` - Delete container and volume

## Directory Structure
```
/devos/
├── src/
├── include/
├── lib/
├── tests/
└── build/
```

Work persists in Docker volume between sessions.
