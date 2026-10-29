FROM debian:bookworm-slim
RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ make ca-certificates libsqlite3-dev libcpp-httplib-dev nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY backend.cpp .
RUN g++ backend.cpp -std=c++17 -O2 -o backend -lsqlite3 -pthread
ENV PORT=10000
EXPOSE 10000
CMD ["./backend"]
