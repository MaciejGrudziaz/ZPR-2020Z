FROM zpr-test AS test

RUN apk add --update --no-cache python3-dev py3-pip libxml2-dev libxslt-dev && ln -sf python3 /usr/bin/python
RUN pip3 install gcovr

COPY run-tests-and-coverage.sh /home

ENTRYPOINT ./run-tests-and-coverage.sh
