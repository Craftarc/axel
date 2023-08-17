#!/bin/bash


# Script that starts the RIE and main executable, and writes logs to the bind-mounted project directory
echo ">> Inside container"

# Make the logs directory if it does not exist
mkdir -p /app/axel/logs

# Start RIE in background
echo ">> Starting RIE and passing the handler..."
/aws-lambda/aws-lambda-rie /app/axel/build/main > /app/axel/logs/tmp.log 2>&1 &

# Save PID of RIE
rie_pid=$!

# Get progress updates by printing out the tail end of logs
tail -f /app/axel/logs/tmp.log &


# Once RIE stops, clean up logs to only show output from spdlog
# Loop that waits for a specific string
echo -e "\n>> To stop the RIE, enter 'stop'\n"

read -r input

if [ "$input" == "stop" ]; then
  echo ">> Stopping RIE with PID: ${rie_pid}"
  kill -TERM $rie_pid
fi

logfile=/app/axel/logs/"$(date +"%Y-%m-%dT%H:%M:%SZ")".log
grep -e '\[[0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\}[[:space:]][0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\}\.[0-9]\{3\}\][[:space:]]\[[a-z]\+\]' /app/axel/logs/tmp.log > "${logfile}"
rm /app/axel/tmp.log

echo ">> Logs cleaned up. Logfile at: ${logfile}"


