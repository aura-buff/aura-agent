# Этап сборки
FROM ubuntu:22.04 AS builde
RUN apt-get update && apt-get install -y \
    build-essential cmake git libcurl4-openssl-dev zlib1g-dev

# Копируем и собираем prometheus-cpp (как мы делали вручную)
WORKDIR /libs
RUN git clone https://github.com/jupp0r/prometheus-cpp.git && \
    cd prometheus-cpp && git submodule update --init && \
    mkdir _build && cd _build && cmake .. -DBUILD_SHARED_LIBS=ON && \
    make -j$(nproc) && make install

# Копируем твой проект
WORKDIR /app
COPY . .
RUN mkdir build && cd build && cmake .. && make -j$(nproc)

# Этап запуска (чтобы образ был маленьким)
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y libcurl4 zlib1g && rm -rf /var/lib/apt/lists/*
COPY --from=builder /usr/local/lib /usr/local/lib
COPY --from=builder /app/build/aura-agent /usr/local/bin/aura-agent
RUN ldconfig

EXPOSE 3100
CMD ["aura-agent"]r
