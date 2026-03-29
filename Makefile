BUILD    := build

# ── Top-level targets ─────────────────────────────────────────────────────────
DOCKER_IMAGE := matching-algorithm
DOCKER_NAME  := matching-algorithm

TEST_BINS := $(BUILD)/test_gale_shapley \
             $(BUILD)/test_hopcroft_karp \
             $(BUILD)/test_hungarian \
             $(BUILD)/test_blossom

.PHONY: all tests run_tests run run_server clean \
        docker-build docker-start docker-stop

all: _cmake
	cmake --build $(BUILD) --parallel

tests: _cmake
	cmake --build $(BUILD) --parallel --target test_gale_shapley test_hopcroft_karp test_hungarian test_blossom

# ── Run main driver ───────────────────────────────────────────────────────────
run: all
	./$(BUILD)/main

run_server: all
	./$(BUILD)/server

# ── Run all tests ─────────────────────────────────────────────────────────────
run_tests: tests
	@for t in $(TEST_BINS); do \
	    echo "--- $$t ---"; \
	    ./$$t; \
	    echo; \
	done

# ── Configure (run once, or when CMakeLists.txt changes) ─────────────────────
_cmake: | $(BUILD)
	@if [ ! -f $(BUILD)/CMakeCache.txt ]; then \
	    cmake -S . -B $(BUILD); \
	fi

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)

# ── Docker ────────────────────────────────────────────────────────────────────
docker-build:
	docker build -t $(DOCKER_IMAGE) .

docker-start:
	@if docker ps -aq -f name=^$(DOCKER_NAME)$$ | grep -q .; then \
	    docker stop $(DOCKER_NAME); \
	    docker start $(DOCKER_NAME); \
	else \
	    $(MAKE) docker-build; \
	    docker run -d --name $(DOCKER_NAME) -p 9090:9090 $(DOCKER_IMAGE); \
	fi
	@echo "Server running at http://localhost:9090"

docker-stop:
	docker stop $(DOCKER_NAME)
