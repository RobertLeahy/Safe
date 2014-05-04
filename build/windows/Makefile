SUFFIXES+=.mk


#OPTIMIZATION=-O3
OPTIMIZATION:=-O0 -g -fno-inline -fno-omit-frame-pointer
OPTS_SHARED:=-D_WIN32_WINNT=0x0600 -Wall -Wpedantic -Werror -fno-rtti -std=gnu++11 -I include
GPP:=g++ $(OPTS_SHARED) $(OPTIMIZATION)
MKDIR=@mkdir_nofail.bat $(subst /,\,$(dir $(1)))


#	DEFAULT

.PHONY: all
all:

.PHONY: clean
clean: cleandeps
	@rmdir_nofail.bat obj

.PHONY: cleanall
cleanall: clean
	@rmdir_nofail.bat bin
	
.PHONY: cleandeps
cleandeps:
	@rmdir_nofail.bat makefiles
	
	
#	DIRECTORIES

bin:
	@mkdir_nofail.bat bin
	
	
NODEPS:=clean cleanall cleandeps
	
	
ifeq (0,$(words $(findstring $(MAKECMDGOALS),$(NODEPS))))

	-include $(subst .cpp,.mk,$(subst src,makefiles,$(subst \,/,$(subst $(shell echo %CD%)\,,$(shell dir /b /s src\*.cpp)))))

endif
	
	
obj/%.o:
	$(call MKDIR,$(patsubst obj/%.o,makefiles/%.mk,$@))
	$(GPP) -MM -MT "$@" $(patsubst obj/%.o,src/%.cpp,$@) -MF $(patsubst obj/%.o,makefiles/%.mk,$@)
	$(call MKDIR,$@)
	$(GPP) -c -o $@ $(patsubst obj/%.o,src/%.cpp,$@)
	

include testshared.mk
include test.mk