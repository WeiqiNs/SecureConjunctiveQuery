FROM ubuntu:latest

# Update libraries.
RUN apt update && apt upgrade -y
# Install needed libraries.
RUN apt install -y git build-essential libgmp-dev libmpfr-dev libssl-dev libgtest-dev gdb cmake
# Clean up.
RUN apt clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Download the RELIC library and the RBP library.
RUN git clone https://github.com/relic-toolkit/relic.git
RUN git clone https://github.com/Weiqi97/LibRBP.git

# RELIC library installation.
RUN mkdir /relic/build
WORKDIR "/relic/build"
RUN ../preset/gmp-pbc-bls381.sh .. && make && make install

# RBP library installation.
RUN mkdir /LibRBP/build
WORKDIR "/LibRBP/build"
RUN cmake .. && make && make install

# Copy the files over to working directory.
RUN mkdir -p /EEJ/build
COPY . /EEJ
WORKDIR "/EEJ/build"
RUN cmake .. && make && ctest
