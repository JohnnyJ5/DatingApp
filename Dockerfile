# ── Build stage ───────────────────────────────────────────────────────────────
FROM debian:bookworm-slim AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
        g++ \
        make \
        curl \
        tar \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

# Download standalone Asio 1.28.2 (header-only, not committed to the repo)
RUN curl -fsSL https://sourceforge.net/projects/asio/files/asio/1.28.2%20%28Stable%29/asio-1.28.2.tar.gz/download \
        -o asio.tar.gz \
    && tar -xzf asio.tar.gz \
    && cp -r asio-1.28.2/include/asio    third_party/asio \
    && cp    asio-1.28.2/include/asio.hpp third_party/asio.hpp \
    && rm -rf asio-1.28.2 asio.tar.gz

# Always build from scratch inside the container to avoid stale host artefacts
RUN make clean && make

# ── Runtime stage ─────────────────────────────────────────────────────────────
FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
        libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build/server ./build/server
COPY --from=builder /app/server/static ./server/static

EXPOSE 9090

CMD ["./build/server"]
