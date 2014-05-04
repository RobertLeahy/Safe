SUFFIXES+=.mk


#OPTIMIZATION=-O3
OPTIMIZATION:=-O0 -g -fno-inline -fno-omit-frame-pointer
OPTS_SHARED:=-Wall -Wpedantic -Werror -fno-rtti -std=c++11 -I include -fPIC
GPP:=clang++ $(OPTS_SHARED) $(OPTIMIZATION)
comma:=,
LINK=-Wl,-rpath,'$$ORIGIN' -Wl,-rpath-link,bin -Wl,-rpath-link,bin/mods $(if $(1),-Wl$(comma)-soname$(comma)$(notdir $(1)))
MKDIR=mkdir -p $(dir $(1))


#	DEFAULT

.PHONY: all
all:

.PHONY: clean
clean: cleandeps
	-@rm -r -f obj

.PHONY: cleanall
cleanall: clean
	-@rm -r -f bin
	
.PHONY: cleandeps
cleandeps:
	-@rm -r -f makefiles
	
	
#	DIRECTORIES

bin:
	-@mkdir bin
	
	
NODEPS:=clean cleanall cleandeps
	
	
ifeq (0,$(words $(findstring $(MAKECMDGOALS),$(NODEPS))))

	-include $(patsubst src/%.cpp,makefiles/%.mk,$(shell find src/ -name "*.cpp"))

endif
	
	
obj/%.o:
	$(call MKDIR,$(patsubst obj/%.o,makefiles/%.mk,$@))
	$(GPP) -MM -MT "$@" $(patsubst obj/%.o,src/%.cpp,$@) -MF $(patsubst obj/%.o,makefiles/%.mk,$@)
	$(call MKDIR,$@)
	$(GPP) -c -o $@ $(patsubst obj/%.o,src/%.cpp,$@)
	
	
include testshared.mk
include test.mk
