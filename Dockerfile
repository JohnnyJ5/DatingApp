# ── Build stage ───────────────────────────────────────────────────────────────
FROM debian:bookworm-slim AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
        g++ \
        make \
        cmake \
        libasio-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

# Always build from scratch inside the container to avoid stale host artefacts
RUN cmake -S . -B build && cmake --build build --parallel

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
