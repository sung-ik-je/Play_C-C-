#!/bin/bash

# 10개의 컨테이너 종료
for i in $(seq 1 10); do
    CONTAINER_NAME="udp_client_$i"
    # 컨테이너가 실행 중인 경우에만 종료
    if [ $(docker ps -q -f name="$CONTAINER_NAME") ]; then
        docker stop "$CONTAINER_NAME"
        echo "$CONTAINER_NAME stopped."
    else
        echo "$CONTAINER_NAME is not running."
    fi
done
