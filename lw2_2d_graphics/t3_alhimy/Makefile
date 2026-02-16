# Компилятор и флаги
CXX := g++
CXXFLAGS := -std=c++20 -Iinclude -Wno-unused-parameter -Wno-narrowing
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# Флаги для покрытия
COVERAGE_FLAGS := -fprofile-arcs -ftest-coverage -g
COVERAGE_LDFLAGS := -fprofile-arcs -ftest-coverage

# Директории
SRC_DIR := src
TEST_DIR := Unit
BUILD_DIR := build
BIN_DIR := bin
TESTS_BIN_DIR := $(BIN_DIR)/tests
COVERAGE_DIR := coverage_report
VSCODE_COV_DIR := ~/Volgatech/OOD/.vscode/coverage

# Находим все исходные файлы проекта
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
MAIN_SRCS := main.cpp
SRCS += $(MAIN_SRCS)

# Тестовые файлы
TEST_SRCS := \
	$(TEST_DIR)/TestShape.cpp \
	$(TEST_DIR)/TestPicture.cpp

# Объектные файлы для основной программы
MAIN_OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(MAIN_SRCS))

# Объектные файлы для библиотеки
LIB_OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Генерация списка тестовых целей
TEST_TARGETS := $(patsubst $(TEST_DIR)/%.cpp,$(TESTS_BIN_DIR)/%_test,$(TEST_SRCS))

# Основная цель
all: $(BIN_DIR)/main

# ============================================================================
# ЦЕЛИ ДЛЯ VSCODE COVERAGE GUTTERS
# ============================================================================

# Основная цель для VSCode - генерирует LCOV файл для Coverage Gutters
vscode-coverage: CXXFLAGS += $(COVERAGE_FLAGS)
vscode-coverage: LDFLAGS += $(COVERAGE_LDFLAGS)
vscode-coverage: $(VSCODE_COV_DIR) test-coverage-run generate-lcov
	@echo ""
	@echo "✅ Coverage data generated for VSCode"
	@echo "📊 LCOV file: $(VSCODE_COV_DIR)/coverage.lcov"
	@echo "📈 HTML report: $(VSCODE_COV_DIR)/index.html"
	@echo ""
	@echo "🎯 Now in VSCode:"
	@echo "   1. Install 'Coverage Gutters' extension"
	@echo "   2. Ctrl+Shift+P → 'Coverage Gutters: Watch'"
	@echo "   3. Select '$(VSCODE_COV_DIR)/coverage.lcov'"
	@echo "   4. Ctrl+Shift+P → 'Coverage Gutters: Display Coverage'"
	@echo "   5. Open any source file to see coverage highlighting"

# Запуск тестов с покрытием
test-coverage-run: $(TEST_TARGETS)
	@echo "🧪 Running tests with coverage instrumentation..."
	@for test in $^; do \
		echo "Running $$test..."; \
		$$test && echo "✅ $$test PASSED" || echo "❌ $$test FAILED"; \
		echo; \
	done

# Генерация LCOV и HTML отчетов
generate-lcov: $(VSCODE_COV_DIR)
	@echo "📊 Generating coverage reports..."
	@gcovr -r . \
		--exclude="$(TEST_DIR)/.*" \
		--exclude=".*catch.*" \
		--exclude=".*test.*" \
		--exclude=".*/tests?/.*" \
		--lcov -o $(VSCODE_COV_DIR)/coverage.lcov
	@gcovr -r . \
		--exclude="$(TEST_DIR)/.*" \
		--exclude=".*catch.*" \
		--exclude=".*test.*" \
		--exclude=".*/tests?/.*" \
		--html --html-details -o $(VSCODE_COV_DIR)/index.html
	@echo "📈 Generated HTML report: $(VSCODE_COV_DIR)/index.html"

# Быстрый просмотр покрытия в VSCode (только LCOV)
vscode-quick: CXXFLAGS += $(COVERAGE_FLAGS)
vscode-quick: LDFLAGS += $(COVERAGE_LDFLAGS)
vscode-quick: $(VSCODE_COV_DIR) test-coverage-run
	@gcovr -r . \
		--exclude="$(TEST_DIR)/.*" \
		--exclude=".*catch.*" \
		--lcov -o $(VSCODE_COV_DIR)/coverage.lcov
	@echo "✅ Quick LCOV generated for VSCode: $(VSCODE_COV_DIR)/coverage.lcov"

# ============================================================================
# ОБЩИЕ ЦЕЛИ ПОКРЫТИЯ
# ============================================================================

# Полное покрытие с детальным отчетом
coverage: CXXFLAGS += $(COVERAGE_FLAGS)
coverage: LDFLAGS += $(COVERAGE_LDFLAGS)
coverage: test-coverage-run
	@echo "📊 Generating detailed coverage report..."
	@gcovr -r . \
		--exclude="$(TEST_DIR)/.*" \
		--exclude=".*catch.*" \
		--exclude=".*test.*" \
		--html --html-details -o $(COVERAGE_DIR)/index.html
	@gcovr -r . \
		--exclude="$(TEST_DIR)/.*" \
		--exclude=".*catch.*" \
		--print-summary
	@echo "📁 Full report: $(COVERAGE_DIR)/index.html"

# Покрытие с анализом ветвей
coverage-branches: CXXFLAGS += $(COVERAGE_FLAGS)
coverage-branches: LDFLAGS += $(COVERAGE_LDFLAGS)
coverage-branches: test-coverage-run
	@echo "🌿 Generating branch coverage report..."
	@gcovr -r . \
		--exclude="$(TEST_DIR)/.*" \
		--exclude=".*catch.*" \
		--branches \
		--html --html-details -o $(COVERAGE_DIR)/branches.html
	@gcovr -r . \
		--exclude="$(TEST_DIR)/.*" \
		--exclude=".*catch.*" \
		--branches --print-summary
	@echo "📁 Branch report: $(COVERAGE_DIR)/branches.html"

# Консольный отчет
coverage-console: CXXFLAGS += $(COVERAGE_FLAGS)
coverage-console: LDFLAGS += $(COVERAGE_LDFLAGS)
coverage-console: test-coverage-run
	@echo "=== 📊 CODE COVERAGE SUMMARY ==="
	@gcovr -r . \
		--exclude="$(TEST_DIR)/.*" \
		--exclude=".*catch.*" \
		--print-summary
	@echo ""
	@echo "=== 🌿 BRANCH COVERAGE ==="
	@gcovr -r . \
		--exclude="$(TEST_DIR)/.*" \
		--exclude=".*catch.*" \
		--branches --print-summary

# ============================================================================
# СБОРКА ПРОГРАММ И ТЕСТОВ
# ============================================================================

# Основная программа
$(BIN_DIR)/main: $(MAIN_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Правило для сборки тестов
$(TESTS_BIN_DIR)/%_test: $(BUILD_DIR)/$(TEST_DIR)/%.o $(filter-out $(BUILD_DIR)/main.o, $(LIB_OBJS)) | $(TESTS_BIN_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Правило для объектных файлов основной программы
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для объектных файлов библиотеки
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для объектных файлов тестов
$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ============================================================================
# СЛУЖЕБНЫЕ ЦЕЛИ
# ============================================================================

# Создание директорий
$(BUILD_DIR) $(BIN_DIR) $(TESTS_BIN_DIR) $(COVERAGE_DIR) $(VSCODE_COV_DIR):
	mkdir -p $@

# Запуск всех тестов (без покрытия)
test: $(TEST_TARGETS)
	@echo "🧪 Running tests..."
	@for test in $^; do \
		echo "Running $$test..."; \
		$$test && echo "✅ $$test PASSED" || echo "❌ $$test FAILED"; \
		echo; \
	done

# Показать информацию о файлах для покрытия
coverage-info:
	@echo "=== 📁 SOURCE FILES FOR COVERAGE ANALYSIS ==="
	@for file in $(SRCS); do \
		echo "$$file"; \
	done
	@echo ""
	@echo "=== 🧪 TEST FILES (EXCLUDED) ==="
	@for file in $(TEST_SRCS); do \
		echo "$$file"; \
	done

# Очистка
clean:
	rm -rf ./$(BUILD_DIR) ./$(BIN_DIR) ./$(COVERAGE_DIR) ./$(VSCODE_COV_DIR)
	find . -name "*.gcda" -delete
	find . -name "*.gcno" -delete
	find . -name "*.gcov" -delete
	find . -name "*.lcov" -delete

# Очистка только покрытия
clean-coverage:
	rm -rf ./$(COVERAGE_DIR) ./$(VSCODE_COV_DIR)
	find . -name "*.gcda" -delete
	find . -name "*.gcno" -delete
	find . -name "*.gcov" -delete
	find . -name "*.lcov" -delete

# Помощь
help:
	@echo "=== 🎯 AVAILABLE TARGETS ==="
	@echo "📦 all              - Build main program"
	@echo "🧪 test             - Run tests without coverage"
	@echo ""
	@echo "=== 🔬 COVERAGE TARGETS ==="
	@echo "💚 vscode-coverage  - Generate LCOV for VSCode Coverage Gutters (RECOMMENDED)"
	@echo "💨 vscode-quick     - Quick LCOV generation for VSCode"
	@echo "📊 coverage         - Full HTML coverage report"
	@echo "🌿 coverage-branches - Coverage with branch analysis"
	@echo "📟 coverage-console  - Console-only coverage summary"
	@echo ""
	@echo "=== 🧹 CLEANING ==="
	@echo "🧹 clean            - Clean everything"
	@echo "🧽 clean-coverage   - Clean only coverage data"
	@echo ""
	@echo "=== ℹ️  INFO ==="
	@echo "📋 coverage-info    - Show which files are analyzed"
	@echo "❓ help             - Show this help"

.PHONY: all test test-coverage-run generate-lcov coverage coverage-branches \
        coverage-console vscode-coverage vscode-quick clean clean-coverage \
        help coverage-info

# Автоматический вывод помощи по умолчанию
.DEFAULT_GOAL := help