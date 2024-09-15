.PHONY: format

format:
	find include src tests examples -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -i
