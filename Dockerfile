FROM ubuntu:20.4
RUN apt updata && apt install -y g++
COPY main.cpp