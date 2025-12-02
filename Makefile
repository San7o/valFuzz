.PHONY: format, docs

format:
	find include src tests -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -i --style=file:./utils/.clang-format

docs:
	doxygen docs/doxygen.conf
