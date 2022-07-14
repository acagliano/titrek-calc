# ----------------------------
# Makefile Options
# ----------------------------

NAME = TITREK
ICON = icon.png
DESCRIPTION = "TI-Trek Multiplayer"
COMPRESSED = NO
ARCHIVED = NO
HAS_PRINTF := NO


CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk
