#!/bin/bash

# 이미지 이름
IMAGE_NAME="udp-client"

# 10개의 컨테이너 생성
for i in $(seq 1 10); do
    CONTAINER_NAME="udp_client_$i"
    docker run --name "$CONTAINER_NAME" -d "$IMAGE_NAME"
done
