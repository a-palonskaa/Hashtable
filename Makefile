CC = g++
CFLAGS = -Wall -O2 -g -msse4.1 -std=c++17 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations   \
         -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy     \
         -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2     \
         -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith       \
         -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo           \
         -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef   \
         -Wunreachable-code -Wunused -Wvariadic-macros -Wno-literal-range                \
         -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast              \
         -Wno-varargs -Wstack-protector -Wsuggest-override                               \
         -Wlong-long -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow \
         -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-protector -fPIE -Werror=vla
COMMON_DIR = build/libs
LIBS_DIR = $(COMMON_DIR)

BUILD_DIR = build

INCLUDES = include common/logger
SOURCES = main.cpp bucket.cpp hash.cpp hashtable.cpp parse_text.cpp
OBJECTS = $(addprefix $(BUILD_DIR)/src/, $(SOURCES:%.cpp=%.o))
DEPS = $(OBJECTS:%.o=%.d)

EXECUTABLE = build/hasht
CFLAGS += $(addprefix -I, $(INCLUDES))
LDFLAGS = -L$(LIBS_DIR) -lcommon

.PHONY: all libs hasht clean

all: hasht

hasht: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@$(CC) $^ $(LDFLAGS) -o $@

$(OBJECTS): $(BUILD_DIR)/%.o:%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -MP -MMD -c $< -o $@

libs:
	@for dir in $(LIBS_DIR); do \
		$(MAKE) -C $$dir all;   \
	done

clean:
	@for dir in $(SUBDIRS); do  \
		$(MAKE) -C $$dir clean; \
	done
	@rm -f $(OBJECTS) $(EXECUTABLE)

echo:
	echo $(OBJECTS)

asm:
	@mkdir -p $(BUILD_DIR)/asm
	@$(CC) $(CFLAGS) -S $(FILE).cpp -o $(BUILD_DIR)/asm/$(notdir $(FILE)).s
	@echo "Assembler output saved to $(BUILD_DIR)/asm/$(notdir $(FILE)).s"
