FROM ubuntu:16.04

#config resources
COPY ./engine /app/engine
COPY ./script.sh /app/script.sh

#install requirements
RUN apt-get update && apt-get install -y openssh-server


RUN apt install -y build-essential checkinstall
#RUN apt install -y libreadline-gplv2-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev
#RUN wget https://www.python.org/ftp/python/3.6.6/Python-3.6.6.tar.xz
#RUN tar xvf Python-3.6.6.tar.xz
#RUN ./Python-3.6.6/configure
#RUN make altinstall
RUN apt -y install sqlite3 libsqlite3-dev


RUN apt-get install -y python-pip 
RUN pip install -U Flask
RUN apt-get install -y g++ make

#ssh config
RUN mkdir /var/run/sshd
RUN echo 'root:root' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

EXPOSE 22 5000

#compile
RUN make dll -C /app/engine/prediction/
#RUN echo 'LD_LIBRARY_PATH=/app/engine' >> etc/environment
RUN export 'LD_LIBRARY_PATH=/app/engine'
CMD ["/bin/bash", "/app/script.sh"]