FROM artifacts.devops.knsb.ru/jenkins/cppbuilder:astra1.6-v7

ARG UNAME=builder
ARG UID=1000
ARG GID=1000

RUN apt-get -qq update                                                          \
    && apt-get install sudo git vim ssh apt-transport-https gdb libzmq3-dev     \
        libczmq-dev -qq -y --no-install-recommends --no-install-suggests        \
    && groupadd $UNAME -g $GID                                                  \
    && useradd -u $UID -g $UNAME $UNAME                                         \
    && mkhomedir_helper $UNAME                                                  \
    && printf "$UNAME:$UNAME" | chpasswd                                        \
    && adduser $UNAME sudo                                                      \
    && echo "$UNAME ALL= NOPASSWD: ALL\n" >> /etc/sudoers                       \
    && echo "deb https://artifacts.devops.knsb.ru/repository/astra-release-local smolensk main contrib non-free" >> /etc/apt/sources.list \
    && echo "deb https://artifacts.devops.knsb.ru/repository/astra-local smolensk main contrib non-free" >> /etc/apt/sources.list \
    && wget -q --no-check-certificate https://artifacts.devops.knsb.ru/repository/binrepo/apt-local/public.gpg.key \
    && apt-key add public.gpg.key

USER $UNAME
RUN conan profile new default --detect                                          \
    && conan profile update settings.compiler.libcxx=libstdc++11 default        \
    && conan profile update settings.compiler.cppstd=14 default

RUN cd && git clone --branch zink https://github.com/drug007/zyre.git zyre-zink \
    && cd zyre-zink/zink && cmake -B./build -H. -DCMAKE_BUILD_TYPE=Debug        \
    && cd build && make -j`nproc` && ln -s ./zink /home/builder/zink 

WORKDIR /home/builder/zyre-zink/zink/build/
