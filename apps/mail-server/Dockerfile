# syntax=docker/dockerfile:1.7

FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive
ENV CCACHE_DIR=/root/.cache/ccache

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        ca-certificates \
        ccache \
        cmake \
        g++ \
        git \
        libsqlite3-dev \
        ninja-build \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

COPY . .

RUN --mount=type=cache,id=mail-server-client-ccache,target=/root/.cache/ccache,sharing=locked \
    --mount=type=cache,id=mail-server-client-fetchcontent,target=/root/.cache/cmake-fetchcontent,sharing=locked \
    cmake \
        -S . \
        -B /src/cmake-build-release \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_COMPILER_LAUNCHER=ccache \
        -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
        -DFETCHCONTENT_BASE_DIR=/root/.cache/cmake-fetchcontent \
        -DMAIL_BUILD_TESTS=OFF \
        -DMAIL_BUILD_QT_CLIENT=OFF \
    && cmake --build /src/cmake-build-release --target mail_server

FROM ubuntu:24.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        ca-certificates \
        libgcc-s1 \
        libsqlite3-0 \
        libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

RUN mkdir -p /app/data \
    && mkdir -p /app/libs/mail-storage

COPY --from=builder /src/cmake-build-release/apps/mail-server/mail_server /app/mail_server
COPY --from=builder /src/config /app/config
COPY --from=builder /src/libs/mail-storage/migrations /app/libs/mail-storage/migrations

EXPOSE 8080

CMD ["./mail_server", "config/mail_server.docker.json"]
