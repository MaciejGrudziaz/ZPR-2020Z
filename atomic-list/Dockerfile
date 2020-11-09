FROM alpine:latest AS build

RUN apk add --no-cache -q -f cmake make g++

ADD ./src /home
WORKDIR /home
RUN mkdir build && cd build && cmake .. && make && cp libZPR-2020Z-atomic-list.so /lib/

FROM build AS tests

RUN apk add --no-cache -q -f git

RUN git clone -q https://github.com/google/googletest.git /googletest \
  && mkdir -p /googletest/build \
  && cd /googletest/build \
  && cmake .. && make && make install \
  && cd / && rm -rf /googletest

ADD ./test /home
WORKDIR /home

RUN cd build && cmake .. && make

ENTRYPOINT /home/build/ZPR-2020Z-atomic-list-test
