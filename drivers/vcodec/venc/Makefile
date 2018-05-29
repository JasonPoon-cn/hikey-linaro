# Add your debugging flag (or not) to CFLAGS
include drivers/vcodec/venc/drv_venc_make.cfg

EXTRA_CFLAGS += $(VENC_CFLAGS) -fno-pic
EXTRA_CFLAGS += -Idrivers/vcodec/venc

obj-$(CONFIG_HI_VCODEC_VENC) += hi_omxvenc.o
hi_omxvenc-objs :=  venc_regulator.o     \
                    drv_venc_intf.o      \
                    drv_venc_osal.o      \
                    hal_venc.o           \
                    hi_drv_mem.o

