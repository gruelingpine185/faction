project_name := faction
project = $(bin_dir)/$(project_name)

# paths
inc_dir := inc/$(project_name)
src_dir := src
bin_dir := bin
vnd_dir := vendor
code_dirs := renderer utils
bin_dirs := $(code_dirs)

# files
main := $(src_dir)/main.c
c_headers := $(foreach path, $(code_dirs), $(wildcard $(inc_dir)/$(path)/*.h)) 	\
	$(wildcard $(inc_dir)/*.h)
c_sources := $(foreach path, $(code_dirs), $(wildcard $(src_dir)/$(path)/*.c)) 	\
	$(wildcard $(src_dir)/*.c)
c_objects := $(patsubst $(src_dir)/%.c, $(bin_dir)/%.o, \
	$(filter-out $(main), $(c_sources)))

# compiler flags
c_std = -std=c11
c_opt = -O2
c_wrn = -Wall -Wextra -pedantic
c_def = -DF_DEBUG_MODE=1
c_inc = -I$(inc_dir) -I$(vnd_dir)
CC ?= clang
CFLAGS := $(strip $(c_std) $(c_opt) $(c_wrn) $(c_def) $(c_inc))


.PHONY: all clean glfw
all:
	@$(MAKE) $(project)
clean:
	-rm -rf $(bin_dir)
glfw:
	cd $(vnd_dir)/glfw; \
	cmake -S . -B build
	@$(MAKE) -C $(vnd_dir)/glfw/build

$(project): $(c_objects) $(vnd_dir)/volk/volk.h
	$(CC) $(CFLAGS) $(main) $(vnd_dir)/volk/volk.c $(c_objects) -o $@ $(shell pkg-config --static --libs glfw3)

$(bin_dir)/%.o: $(src_dir)/%.c $(wildcard $(inc_dir)/%.h) $(bin_dirs)
	$(CC) $(CFLAGS) -fpic -c $< -o $@

$(bin_dir):
	-mkdir $@

$(bin_dirs): $(bin_dir)
	-$(foreach path, $@, mkdir $</$(path))
