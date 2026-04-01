# --- BUILDER ---
FROM ubuntu:22.04 AS builder
RUN apt-get update && apt-get install -y \
    build-essential cmake git libcurl4-openssl-dev zlib1g-dev nlohmann-json3-dev
WORKDIR /libs
RUN git clone https://github.com/jupp0r/prometheus-cpp.git && \
    cd prometheus-cpp && git submodule update --init && \
    mkdir _build && cd _build && cmake .. -DBUILD_SHARED_LIBS=ON && \
    make -j$(nproc) && make install

WORKDIR /app
COPY . .
# Убеждаемся, что папка build чистая перед сборкой
RUN rm -rf build && mkdir build && cd build && cmake .. && make -j$(nproc)

# --- RUNTIME ---
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
    libcurl4 zlib1g python3 python3-pip curl && \
    rm -rf /var/lib/apt/lists/*
RUN pip3 install aiogram

WORKDIR /app
# Копируем бинарник и библиотеки
COPY --from=builder /usr/local/lib /usr/local/lib
COPY --from=builder /app/build/aura-agent /usr/local/bin/aura-agent
RUN ldconfig

# Копируем ВСЕ файлы проекта (включая register.py и entrypoint.sh)
COPY . .

# Даем права на запуск
RUN chmod +x entrypoint.sh bot_handlers/register.py

EXPOSE 3100

# Используем абсолютный путь для надежности
ENTRYPOINT ["/bin/bash", "/app/entrypoint.sh"]
