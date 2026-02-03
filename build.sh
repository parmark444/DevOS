#!/bin/bash
echo "Building DevOS container..."
docker compose build
echo ""
echo "Container built."
echo ""
echo "Run './start.sh' to enter the DevOS environment."
