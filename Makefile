vnd_dir := vendor


.PHONY: glfw
glfw:
	cd $(vnd_dir)/glfw; \
	cmake -S . -B build
	@$(MAKE) -C $(vnd_dir)/glfw/build

