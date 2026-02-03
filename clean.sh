#!/bin/bash
echo "WARNING: This will delete all work inside the container."
read -p "Are you sure? (yes/no): " confirm
if [ "$confirm" = "yes" ]; then
    docker compose down -v
    docker rmi devos:latest 2>/dev/null
    echo "Cleaned."
else
    echo "Cancelled."
fi
