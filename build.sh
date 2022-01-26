#!/bin/sh

DOCKER_REGISTRY=local
DOCKER_IMAGE_VERSION=0.1.0

mkdir build 2> /dev/null

docker run -it --rm --privileged \
       --network="host" \
       --workdir="/out" \
       --user="$(id -u):$(id -g)" \
       --volume="$(pwd):/in" \
       --volume="$(pwd)/build:/out:rw" \
       $DOCKER_REGISTRY/stm32-cross-build-gcc:$DOCKER_IMAGE_VERSION \
       cmake -DCMAKE_MAKE_PROGRAM=make -DCMAKE_TOOLCHAIN_FILE=/in/toolchain.cmake \
       -G"Unix Makefiles" /in

docker run -it --rm --privileged \
       --network="host" \
       --workdir="/out" \
       --user="$(id -u):$(id -g)" \
       --volume="$(pwd):/in" \
       --volume="$(pwd)/build:/out:rw" \
       $DOCKER_REGISTRY/stm32-cross-build-gcc:$DOCKER_IMAGE_VERSION \
       make -f /out/Makefile
