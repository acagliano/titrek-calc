# ----------------------------
# Program Options
# ----------------------------

NAME         ?= TITREK
ICON         ?= icon.png
DESCRIPTION  ?= "TI-Trek Multiplayer"
MAIN_ARGS    ?= NO
COMPRESSED   ?= NO
ARCHIVED     ?= NO

L ?= graphx fileioc keypadc srldrvce usbdrvce

# ----------------------------
# Compile Options
# ----------------------------

OPT_MODE     ?= -Oz
EXTRA_CFLAGS ?= -Wall -Wextra
HAS_PRINTF := NO

# ----------------------------
# Debug Options
# ----------------------------

OUTPUT_MAP   ?= NO

include $(CEDEV)/meta/makefile.mk

