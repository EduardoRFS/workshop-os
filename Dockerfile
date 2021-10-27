FROM archlinux:base-devel

RUN useradd -m workshop
RUN usermod -a -G kvm workshop
RUN mkdir /workshop
WORKDIR /workshop

USER workshop
