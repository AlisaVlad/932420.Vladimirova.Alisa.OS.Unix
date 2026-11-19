FROM alpine:3.20

WORKDIR /app
COPY worker.sh .
RUN chmod +x worker.sh && mkdir -p /shared

CMD ["/app/worker.sh"]