FROM ubuntu:14.04

MAINTAINER Damien Zammit <damien@zamaudio.com>

ENV PATH=$PATH:/opt/mingw64/bin:/opt/local/bin

WORKDIR ~/

RUN apt-get update -qq -y && apt-get install python-software-properties -y
RUN apt-get install software-properties-common -y && add-apt-repository ppa:kxstudio-debian/mingw -y && add-apt-repository ppa:ubuntu-toolchain-r/test -y && apt-get update -qq -y
RUN apt-get install wget -y

RUN wget -c https://launchpad.net/~flosoft/+archive/ubuntu/cross-apple/+files/apple-uni-sdk-10.5_20110407-0.flosoft1_amd64.deb --no-check-certificate

RUN wget http://zamaudio.com/mbox2/apple-macports-fftw-3_3.3.5-1_all.deb
RUN wget http://zamaudio.com/mbox2/apple-macports-libiconv_1.14-0_all.deb
RUN wget http://zamaudio.com/mbox2/apple-macports-liblo_0.26-1_all.deb
RUN wget http://zamaudio.com/mbox2/apple-macports-libogg_1.3.0-1_all.deb
RUN wget http://zamaudio.com/mbox2/apple-macports-libsamplerate_0.1.9-1_all.deb
RUN wget http://zamaudio.com/mbox2/apple-macports-libsndfile_1.0.25-0_all.deb
RUN wget http://zamaudio.com/mbox2/apple-macports-libvorbis_1.3.3-0_all.deb
RUN wget http://zamaudio.com/mbox2/apple-x86-gcc_4.2.1~5646-1kxstudio2_amd64.deb
RUN wget http://zamaudio.com/mbox2/apple-x86-odcctools_758.159-0kxstudio2_amd64.deb 
RUN wget http://zamaudio.com/mbox2/apple-x86-setup_2_amd64.deb
RUN wget http://zamaudio.com/mbox2/apple-macports-flac_1.2.1-1_all.deb

# Apple cross compiler && build deps
RUN dpkg -i apple-x86-odcctools_758.159-0kxstudio2_amd64.deb && \
	dpkg -i apple-x86-gcc_4.2.1~5646-1kxstudio2_amd64.deb && \
	dpkg -i apple-uni-sdk-10.5_20110407-0.flosoft1_amd64.deb && \
	dpkg -i apple-x86-setup_2_amd64.deb && \
	dpkg -i apple-macports-liblo_0.26-1_all.deb && \
	dpkg -i apple-macports-fftw-3_3.3.5-1_all.deb && \
	dpkg -i apple-macports-libiconv_1.14-0_all.deb && \
	dpkg -i apple-macports-libogg_1.3.0-1_all.deb && \
	dpkg -i apple-macports-libvorbis_1.3.3-0_all.deb && \
	dpkg -i apple-macports-flac_1.2.1-1_all.deb && \
	dpkg -i apple-macports-libsndfile_1.0.25-0_all.deb && \
	dpkg -i apple-macports-libsamplerate_0.1.9-1_all.deb

# Windows cross-compiler && build deps
RUN apt-get install -y \
	mingw64-x-gcc \
	mingw64-x-binutils \
	mingw64-x-pkgconfig \
	mingw64-x-liblo \
	mingw64-x-libsndfile
#libsamplerate and fftw3f

RUN wget http://zamaudio.com/mbox2/mingw64-x-fftw-3_3.3.5-1_all.deb
RUN dpkg -i mingw64-x-fftw-3_3.3.5-1_all.deb
RUN wget http://zamaudio.com/mbox2/mingw64-x-libsamplerate_0.1.9-1_all.deb
RUN dpkg -i mingw64-x-libsamplerate_0.1.9-1_all.deb
RUN wget http://zamaudio.com/mbox2/mingw64-x-pthread_0.0.1-3_all.deb
RUN dpkg -i mingw64-x-pthread_0.0.1-3_all.deb

# GNU/Linux compiler && build deps
RUN apt-get install -y \
	g++-4.8 \
	binutils \
	pkg-config \
	libx11-dev \
	libgl1-mesa-dev \
	liblo-dev \
	fftw3-dev \
	libsndfile-dev \
	libsamplerate0-dev \
	libjack-dev \
	ladspa-sdk \
	libpthread-stubs0-dev

RUN apt-get install -y make zip

CMD []
