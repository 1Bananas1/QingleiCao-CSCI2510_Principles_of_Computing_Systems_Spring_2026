#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 <logfile>"
    exit 1
fi

logfile=$1

lines=$(wc -l < "$logfile")
echo "LINES: $lines"

echo "STATUS_COUNTS:"
awk '{print $2}' "$logfile" | sort | uniq -c | sort -rn | awk '{print $2, $1}'
