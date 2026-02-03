#!/bin/bash
echo "Starting DevOS environment..."
docker compose up -d
docker compose exec devos /bin/bash
