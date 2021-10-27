FROM archlinux:base-devel

RUN echo "[multilib]" >> /etc/pacman.conf
RUN echo "Include = /etc/pacman.d/mirrorlist" >> /etc/pacman.conf
RUN echo "Server = http://mirror.ufscar.br/archlinux/$repo/os/$arch" >> /etc/pacman.d/mirrorlist
RUN pacman -Sy --noconfirm lib32-glibc

RUN useradd -m workshop
RUN usermod -a -G kvm workshop
RUN mkdir /workshop
WORKDIR /workshop

USER workshop
