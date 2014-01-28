# macros to include all files with same extension

define all-files-under
$(patsubst ./%,%, \
  $(shell cd $(LOCAL_PATH) ; \
          find $(1) -name "$(2)" -and -not -name ".*") \
 )
endef

define all-cpp-files-under
$(call all-files-under,$(1),*.cpp)
endef

define all-c-files-under
$(call all-files-under,$(1),*.c)
endef

# module

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

COCOS2DX_ROOT=$(LOCAL_PATH)/../../cocos2d-x
COCOS2DX_EXT_ROOT=$(COCOS2DX_ROOT)/extensions
COCOSDENSHION_ROOT=$(COCOS2DX_ROOT)/CocosDenshion
LOCAL_MODULE := cocos2dx-better
LOCAL_SRC_FILES := $(call all-cpp-files-under,src) \
	$(call all-cpp-files-under,thirdparty) \
	$(call all-c-files-under,thirdparty/) \
	$(call all-cpp-files-under,$(COCOS2DX_EXT_ROOT)/GUI/CCControlExtension) \
	$(call all-cpp-files-under,$(COCOS2DX_EXT_ROOT)/GUI/CCEditBox) \
	$(COCOS2DX_EXT_ROOT)/CCBReader/CCData.cpp \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/GUI/Layouts/UILayoutDefine.cpp \
	$(call all-cpp-files-under,$(COCOS2DX_EXT_ROOT)/CocoStudio/Armature) \
	$(call all-cpp-files-under,$(COCOS2DX_EXT_ROOT)/CocoStudio/Json)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include \
	$(LOCAL_PATH)/include/platform/android \
	$(COCOS2DX_ROOT)/cocos2dx/platform/android \
	$(COCOS2DX_ROOT)/cocos2dx/platform/android/jni \
	$(COCOSDENSHION_ROOT)/include \
	$(COCOS2DX_EXT_ROOT) \
	$(COCOS2DX_EXT_ROOT)/GUI/CCControlExtension \
	$(COCOS2DX_EXT_ROOT)/GUI/CCEditBox \
	$(COCOS2DX_EXT_ROOT)/CCBReader \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Armature \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Armature/animation \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Armature/datas \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Armature/display \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Armature/physics \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Armature/utils
LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES) \
	$(COCOS2DX_ROOT)/cocos2dx/support/tinyxml2 \
	$(LOCAL_PATH)/thirdparty/aosp
LOCAL_EXPORT_LDLIBS := -L$(LOCAL_PATH)/thirdparty/system_libs/$(TARGET_ARCH) -lsqlite -llog
LOCAL_LDLIBS := $(LOCAL_EXPORT_LDLIBS)
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static yajl

include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,cocos2dx-better/thirdparty/yajl)
