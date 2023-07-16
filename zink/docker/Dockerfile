FROM artifacts.devops.knsb.ru/jenkins/cppbuilder:astra1.6-v7

ARG UNAME=builder
ARG UID=1000
ARG GID=1000

RUN apt-get -qq update                    \
    && apt-get install sudo git vim apt-transport-https gdb -qq -y --no-install-recommends --no-install-suggests \
    && groupadd $UNAME -g $GID             \
    && useradd -u $UID -g $UNAME $UNAME     \
    && mkhomedir_helper $UNAME             \
    && printf "$UNAME:$UNAME" | chpasswd    \
    && adduser $UNAME sudo                 \
    && printf \"$UNAME ALL= NOPASSWD: ALL  \
    \" >> /etc/sudoers                    \
    && echo "deb https://artifacts.devops.knsb.ru/repository/astra-release-local smolensk main contrib non-free" >> /etc/apt/sources.list \
    && echo "deb https://artifacts.devops.knsb.ru/repository/astra-local smolensk main contrib non-free" >> /etc/apt/sources.list \
    && wget -q --no-check-certificate https://artifacts.devops.knsb.ru/repository/binrepo/apt-local/public.gpg.key \
    && apt-key add public.gpg.key

USER $UNAME
RUN conan profile new default --detect                                          \
    && conan profile update settings.compiler.libcxx=libstdc++11 default        \
    && conan profile update settings.compiler.cppstd=14 default                 \
    && cd && git clone https://github.com/Kitware/CMake.git                     \
    && cd CMake && ./bootstrap && make -j`nproc` && sudo make -j`nproc` install