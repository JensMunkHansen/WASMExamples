#!/bin/bash

# Default Chrome binary path (adjust as needed)
CHROME_PATH="/usr/bin/google-chrome"

# Check if Chrome binary exists
if [ ! -f "$CHROME_PATH" ]; then
    echo "Chrome not found at $CHROME_PATH. Please update the script with the correct path."
    exit 1
fi

# Default user data directory (can be customized)
USER_DATA_DIR="/tmp/chrome_dev"

# Ensure the user data directory exists
mkdir -p "$USER_DATA_DIR"

# Ensure a URL is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <URL>"
    exit 1
fi

# Extract the URL from the arguments
URL=$1

# Launch Chrome with the specified arguments
"$CHROME_PATH" --user-data-dir="$USER_DATA_DIR" \
               --no-default-browser-check \
               --no-first-run \
               --disable-features=MetricsReporting \
               "$URL"
