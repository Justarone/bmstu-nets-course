FROM archlinux:latest

RUN pacman -Sy && yes | pacman -S make gcc boost

CMD ["/bin/bash"]
