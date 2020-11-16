FROM alpine:latest AS build

RUN apk add --no-cache -q -f cmake make g++

ADD atomic-list/src /home
WORKDIR /home
RUN mkdir build && cd build && cmake .. && make && cp libZPR-2020Z-atomic-list.so /lib/

FROM build AS tests

RUN apk add --no-cache -q -f git

RUN git clone -q https://github.com/google/googletest.git /googletest \
  && mkdir -p /googletest/build \
  && cd /googletest/build \
  && cmake .. && make && make install \
  && cd / && rm -rf /googletest

ADD atomic-list/test /home
WORKDIR /home

RUN cd build && cmake .. && make

FROM tests AS coverage

RUN apk add --update --no-cache python3-dev py3-pip libxml2-dev libxslt-dev && ln -sf python3 /usr/bin/python
RUN pip3 install gcovr

RUN ls -al /home

ENTRYPOINT /home/run-tests-and-coverage.sh
