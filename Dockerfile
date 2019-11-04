FROM ubuntu
EXPOSE 8181

RUN useradd -ms /bin/bash srv
ADD . /home/srv/code

WORKDIR /home/srv/code
RUN apt-get update -y && apt-get install -y build-essential && gcc webhello.c -o webhello && chmod +x webhello && chown -R srv:srv .

USER srv
CMD /home/srv/code/webhello