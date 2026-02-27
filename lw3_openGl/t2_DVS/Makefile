# Компилятор и флаги
CXX := g++
CXXFLAGS := -std=c++20 -Iinclude -Ilib/include -Wno-unused-parameter -Wno-narrowing
LDFLAGS := -lglfw -lGL

# Директории
SRC_DIR := src
TEST_DIR := Unit
LIB_DIR := lib
BUILD_DIR := build
BIN_DIR := bin
TESTS_BIN_DIR := $(BIN_DIR)/tests

# GLAD файлы
GLAD_SRC := $(LIB_DIR)/src/glad.c
GLAD_OBJ := $(BUILD_DIR)/glad.o

# Находим все исходные файлы проекта
SRCS := $(shell find $(SRC_DIR) -name "*.cpp" 2>/dev/null || true)
MAIN_SRCS := main.cpp
SRCS += $(MAIN_SRCS)

# Тестовые файлы
TEST_SRCS := \
	$(TEST_DIR)/TestShape.cpp \
	$(TEST_DIR)/TestPicture.cpp

# Объектные файлы для основной программы
MAIN_OBJS := $(BUILD_DIR)/main.o $(GLAD_OBJ)

# Объектные файлы для библиотеки (без main.o)
LIB_OBJS := $(filter-out $(BUILD_DIR)/main.o, $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS)))
LIB_OBJS += $(GLAD_OBJ)

# Генерация списка тестовых целей
TEST_TARGETS := $(patsubst $(TEST_DIR)/%.cpp,$(TESTS_BIN_DIR)/%_test,$(TEST_SRCS))

# ============================================================================
# ОСНОВНЫЕ ЦЕЛИ
# ============================================================================

# Сборка программы
all: $(BIN_DIR)/main

# Пересборка (clean + all)
reall: clean all

# Очистка
clean:
	rm -rf ./$(BUILD_DIR) ./$(BIN_DIR)

# Запуск программы
run: $(BIN_DIR)/main
	./$(BIN_DIR)/main

# Запуск тестов
test: $(TEST_TARGETS)
	@for test in $^; do \
		$$test; \
	done

# Справка
help:
	@echo "=== ДОСТУПНЫЕ КОМАНДЫ ==="
	@echo "make all      - собрать программу"
	@echo "make reall    - пересобрать (clean + all)"
	@echo "make clean    - очистить сборку"
	@echo "make run      - запустить программу"
	@echo "make test     - запустить тесты"
	@echo "make help     - показать эту справку"

# ============================================================================
# ПРАВИЛА СОЗДАНИЯ ДИРЕКТОРИЙ
# ============================================================================

$(BIN_DIR) $(TESTS_BIN_DIR):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@

# ============================================================================
# ПРАВИЛА СБОРКИ
# ============================================================================

# Сборка главного исполняемого файла
$(BIN_DIR)/main: $(MAIN_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Сборка тестов
$(TESTS_BIN_DIR)/%_test: $(BUILD_DIR)/$(TEST_DIR)/%.o $(LIB_OBJS) | $(TESTS_BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Компиляция main.cpp (папка уже создана через | $(BUILD_DIR))
$(BUILD_DIR)/main.o: main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция glad.c (папка уже создана через | $(BUILD_DIR))
$(GLAD_OBJ): $(GLAD_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция файлов из src/ (папка уже создана через | $(BUILD_DIR))
# Создаем необходимые поддиректории в build
$(BUILD_DIR)/src/%.o: src/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция тестовых файлов (папка уже создана через | $(BUILD_DIR))
$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ============================================================================
# PHONY ЦЕЛИ
# ============================================================================

.PHONY: all reall clean run test help

# ============================================================================
# ЦЕЛЬ ПО УМОЛЧАНИЮ
# ============================================================================

.DEFAULT_GOAL := help