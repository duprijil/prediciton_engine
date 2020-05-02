all: build run

build:
	docker build -t enimg .

run:
	docker run -d -P -p 5000:5000 -p 22:22 --name encon enimg
	docker logs encon -f

clear:
	-docker stop encon
	docker system prune -f --volumes
