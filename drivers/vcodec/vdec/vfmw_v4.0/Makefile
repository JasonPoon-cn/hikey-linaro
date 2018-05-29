TOP      := drivers/../..
VFMW_DIR := drivers/vcodec/vdec

include $(VFMW_DIR)/vfmw_v4.0/vfmw_make.cfg

EXTRA_CFLAGS += -Idrivers/vcodec/vdec/include
EXTRA_CFLAGS += -Idrivers/vcodec/vdec/omxvdec
EXTRA_CFLAGS += $(VFMW_CFLAGS) -fno-pic

#build in
obj-$(CONFIG_HI_VCODEC_VDEC) += hi_vfmw.o
hi_vfmw-objs := $(VFMW_CFILES)

