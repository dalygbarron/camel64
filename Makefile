V=1
SOURCE_DIR=src
BUILD_DIR=build
N64_ROM_REGIONFREE=true
N64_ROM_SAVETYPE=eeprom4k
include $(N64_INST)/include/n64.mk

all: main.z64
.PHONY: all

sources = $(wildcard src/*.cpp) $(wildcard src/coru/*.c)
OBJS = $(addprefix $(BUILD_DIR)/,$(patsubst src/%.cpp,%.o,$(sources:%.c=%.cpp)))

assets_font = $(addprefix fonts/,$(notdir $(wildcard assets/fonts/*.ttf)))
assets_i_png = $(addprefix pics/,$(notdir $(wildcard assets/pics/*.i.png)))
assets_png = $(filter-out $(assets_i_png),$(addprefix pics/,$(notdir $(wildcard assets/pics/*.png))))
assets_xm = $(addprefix music/,$(notdir $(wildcard assets/music/*.xm)))
assets_model = $(addprefix models/,$(notdir $(wildcard assets/models/*.glb)))
assets_conv = $(addprefix filesystem/,game.ini game.toke\
                                     $(assets_png:.png=.sprite)\
									 $(assets_font:%.ttf=%.font64)\
									 $(assets_xm:%.xm=%.xm64)\
									 $(assets_model:%.glb=%.model64)\
						             $(assets_i_png:.i.png=.sprite))

main.z64: N64_ROM_TITLE="Idiot Timer"
main.z64: $(BUILD_DIR)/main.dfs

$(BUILD_DIR)/main.dfs: $(assets_conv)
$(BUILD_DIR)/main.elf: $(OBJS)

AUDIOCONV_FLAGS ?=

filesystem/game.ini: assets/game.ini
	@mkdir -p $(dir $@)
	@echo "    [GAME] $@"
	@cp $< $@

filesystem/game.toke: assets/game.toke
	@mkdir -p $(dir $@)
	@echo "    [GAME] $@"
	@cp $< $@

filesystem/music/%.xm64: assets/music/%.xm
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -o filesystem/music "$<"

filesystem/pics/%.sprite: assets/pics/%.i.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE_I] $@"
	@mksprite -f I8 $<
	@mv $(patsubst %.png, %.sprite, $(notdir $<)) $@

filesystem/pics/%.sprite: assets/pics/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@mksprite -f RGBA16 -o filesystem/pics $<

filesystem/fonts/%.font64: assets/fonts/%.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	@mkfont -o filesystem/fonts -s 24 $<

filesystem/models/%.model64: assets/models/%.glb
	@mkdir -p $(dir $@)
	@echo "    [MODEL] $@"
	@mkmodel -o filesystem/models "$<"

clean:
	rm -rf $(BUILD_DIR) main.z64 filesystem
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
