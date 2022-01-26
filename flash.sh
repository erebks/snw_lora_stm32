#!/bin/sh

DOCKER_REGISTRY=local
DOCKER_IMAGE_VERSION=0.1.0

docker run -it --rm --privileged \
       --network="host" \
       --workdir="/out" \
       --volume="$(pwd):/out:ro" \
       $DOCKER_REGISTRY/stm32-cross-build-openocd:$DOCKER_IMAGE_VERSION \
       openocd -f openocd.cfg -c "program build/lorawan_at_slave.elf" -c "reset" -c "shutdown"

#       --user="$(id -u):$(id -g)" \
