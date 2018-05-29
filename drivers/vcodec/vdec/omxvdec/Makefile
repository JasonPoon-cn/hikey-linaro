
TOP := drivers/..


EXTRA_CFLAGS += -DHIVDEC_SMMU_SUPPORT

EXTRA_CFLAGS += -Idrivers/vcodec/vdec/omxvdec
EXTRA_CFLAGS += -Idrivers/vcodec/vdec/vfmw_v4.0
EXTRA_CFLAGS += -Idrivers/vcodec/vdec/vfmw_v4.0/format
EXTRA_CFLAGS += -fno-pic
EXTRA_CFLAGS += -DOMXVDEC_TVP_CONFLICT

ifneq ($(TARGET_BUILD_VARIANT), user)
EXTRA_CFLAGS += -DUSER_DISABLE_VDEC_PROC
endif

#build in
obj-$(CONFIG_HI_VCODEC_VDEC) += hi_omxvdec.o
hi_omxvdec-objs += omxvdec.o regulator.o


