test: \
bin/tests


bin/tests: $(TESTS_DEPENDENCIES) | bin
	$(GPP) -o $@ $^
	bin/tests
