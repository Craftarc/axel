#!/bin/bash

# Start nginx (automatically starts in background)
nginx
if [[ $? -ne 1 ]]
then
    echo "nginx started"
else
    echo "nginx failed to start"
fi

# Start axel
/app/main


