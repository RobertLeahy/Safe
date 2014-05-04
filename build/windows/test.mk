test: \
bin/tests.exe


bin/tests.exe: $(TESTS_DEPENDENCIES) | bin
	$(GPP) -o $@ $^
	bin/tests.exe
