FROM alpine:latest

RUN apk add --no-cache -q -f git cmake make g++ boost-dev

RUN git clone -q https://github.com/google/googletest.git /googletest \
  && mkdir -p /googletest/build \
  && cd /googletest/build \
  && cmake .. && make && make install \
  && cd / && rm -rf /googletest

COPY src /home/atomic/src
COPY ./test /home
WORKDIR /home

RUN mkdir build && cd build && cmake .. && make

ENTRYPOINT /home/build/ZPR-2020Z-atomic-list-test
