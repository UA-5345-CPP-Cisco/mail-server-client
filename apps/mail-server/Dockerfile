FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        ca-certificates \
        cmake \
        g++ \
        git \
        libsqlite3-dev \
        ninja-build \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

COPY . .

RUN cmake \
        -S . \
        -B /src/cmake-build-release \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DMAIL_BUILD_TESTS=OFF \
        -DMAIL_BUILD_QT_CLIENT=OFF \
    && cmake --build /src/cmake-build-release --target mail_server_neo

WORKDIR /app

RUN mkdir -p /app/data \
    && cp /src/cmake-build-release/apps/mail-server-neo/mail_server_neo /app/mail_server_neo \
    && cp -R /src/config /app/config \
    && mkdir -p /app/libs/mail-storage \
    && cp -R /src/libs/mail-storage/migrations /app/libs/mail-storage/migrations

EXPOSE 8080

CMD ["./mail_server_neo", "config/mail_server.docker.json"]
