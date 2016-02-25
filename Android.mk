LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
				   ../../Classes/CheckoutLayer.cpp \
				  ../../Classes/CheckoutScene.cpp \
				  ../../Classes/GameFailedScene.cpp \
				  ../../Classes/GameWinScene.cpp \
				  ../../Classes/GameRoundWinScene.cpp \
				  ../../Classes/ChooseLayer.cpp \
				  ../../Classes/WelcomeLayer.cpp \
				   ../../Classes/WelcomeScene.cpp \
				   ../../Classes/TweebaaMobileLayer.cpp \
				   ../../Classes/TwitterWrapper.cpp \
				   ../../Classes/VisibleRect.cpp \
				    ../../Classes/Login.cpp \
					../../Classes/TwbIntroScene.cpp \
					../../Classes/LeaderBoardLayer.cpp \
					../../Classes/DataAccess/DataAccess.cpp \
					../../Classes/find_diff/FindDiffLayer.cpp \
					../../Classes/find_diff/FindDiffScene.cpp \
					../../Classes/zoom/ZoomGameScene.cpp \
					../../Classes/zoom/ZoomPhotoLayer.cpp \
					../../Classes/Gif/gif_lib/dgif_lib.c \
					../../Classes/Gif/gif_lib/gifalloc.c \
					../../Classes/Gif/gif_lib/gif_err.c \
					../../Classes/Gif/gif_lib/gif_font.c \
					../../Classes/Gif/gif_lib/gif_hash.c \
					../../Classes/Gif/gif_lib/quantize.c \
					../../Classes/Gif/Bitmap.cpp \
					../../Classes/Gif/CacheGif.cpp \
					../../Classes/Gif/GifBase.cpp \
					../../Classes/Gif/GifMovie.cpp \
					../../Classes/Gif/InstantGif.cpp \
					../../Classes/Gif/Movie.cpp \
					../../Classes/Collage/BrowseLayer.cpp \
					../../Classes/Collage/category.cpp \
					../../Classes/Collage/Collage_Template.cpp \
					../../Classes/Collage/CollageScene.cpp \
					../../Classes/Collage/CreateLayer.cpp \
					../../Classes/Collage/DesignLayer.cpp \
					../../Classes/Collage/FavoritLayer.cpp \
					../../Classes/Collage/HttpConnection.cpp \
					../../Classes/Collage/PopupLayer.cpp \
					../../Classes/Collage/MainMenu.cpp \
					../../Classes/Collage/PublishLayer.cpp \
					../../Classes/Collage/TTWaitting.cpp \
					../../Classes/Collage/ProfileLayer.cpp \
					../../Classes/sqlite/sqlite3.c \
					../../Classes/EventBoard/EventBoard.cpp \
					../../Classes/LeaderBoard/LeaderBoard.cpp \
					../../Classes/LeaderBoard/LeaderBoardItemAdapter.cpp \
					../../Classes/ZhuanPanSystem/GlobalInfo.cpp \
					../../Classes/ZhuanPanSystem/Gzi.cpp \
					../../Classes/ZhuanPanSystem/TurntableScene.cpp \
					../../Classes/ZhuanPanSystem/TurntableSystem.cpp \
					../../Classes/ZhuanPanSystem/SpinLucky.cpp \
					../../Classes/ZhuanPanSystem/SpinLuckyLayer.cpp \
					../../Classes/ZhuanPanSystem/SpinLuckyItemAdapter.cpp \
					../../Classes/MemberCenter/RegisterTabScene.cpp \
					../../Classes/MemberCenter/RegisterView.cpp \
					../../Classes/MemberCenter/ForgotpassLayer.cpp \
					../../Classes/MemberCenter/MyAccountLayer.cpp \
					../../Classes/MultiPriceMatch/MultiPriceMatch.cpp  \
					../../Classes/MultiPriceMatch/MultiPriceMatchScene.cpp  \
					../../Classes/utilits/CCMD5.cpp \
					../../Classes/utilits/base64.c \
					../../Classes/utilits/xxtea.c \
					../../Classes/utilits/DailyCheckIn.cpp \
					../../Classes/utilits/DailyCheckInLayer.cpp \
					../../Classes/utilits/DataCenter.cpp \
					../../Classes/utilits/CCSVParse.cpp \
					../../Classes/utilits/NetworkUtil.cpp \
					../../Classes/utilits/pugixml.cpp \
					../../Classes/utilits/SettingScene.cpp \
					../../Classes/utilits/Tutorial.cpp \
					../../Classes/utilits/VideoPlayer.cpp \
					../../Classes/Msgbox/Dialogbox.cpp \
					../../Classes/Msgbox/DialogLayer.cpp \
					../../Classes/Msgbox/MsgBox.cpp \
					../../Classes/theme/GameRound.cpp \
					../../Classes/theme/ThemeLayer.cpp \
					../../Classes/theme/ThemeTextLayer.cpp \
					 ../../Classes/memory_match/GameLayer.cpp \
					 ../../Classes/memory_match/GameScene.cpp \
					 ../../Classes/memory_match/TouchSprite.cpp \
					 ../../Classes/jigsaw/CCNodeExt.cpp \
					 ../../Classes/jigsaw/jigSawScene.cpp \
					 ../../Classes/jigsaw/MainActivity.cpp \
					 ../../Classes/jigsaw/MenuLayer.cpp \
					 ../../Classes/jigsaw/PictureGameLayer.cpp \
					 ../../Classes/jigsaw/PictureGameScene.cpp \
					 ../../Classes/jigsaw/SlidingMenuLayer.cpp \
					 ../../Classes/PriceMatch/PriceMatchLayer.cpp \
					 ../../Classes/PriceMatch/PriceMatchScene.cpp \
					 ../../Classes/PriceMatch/PriceMatchQuestion.cpp \
					 ../../Classes/C2DXShareSDK/C2DXShareSDK.cpp \
					 ../../Classes/C2DXShareSDK/Android/ShareSDKUtils.cpp \
					../../Classes/C2DXShareSDK/Android/JSON/CCJSONConverter.cpp \
					../../Classes/C2DXShareSDK/Android/JSON/cJSON/cJSON.c	
				   
					 
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../cocos2d/extensions \
$(LOCAL_PATH)/../../Classes/memory_match \
$(LOCAL_PATH)/../../Classes/jigsaw \
$(LOCAL_PATH)/../../Classes/find_diff \
$(LOCAL_PATH)/../../Classes/zoom \
$(LOCAL_PATH)/../../Classes/spot_it \
$(LOCAL_PATH)/../../Classes/price_the_pic \
$(LOCAL_PATH)/../../Classes/Login \
$(LOCAL_PATH)/../../Classes/Login_again \
$(LOCAL_PATH)/../../Classes/game_menu \
$(LOCAL_PATH)/../../Classes/game_passed \
$(LOCAL_PATH)/../../Classes/PriceMatch \
$(LOCAL_PATH)/../../Classes/LevelSelect \
$(LOCAL_PATH)/../../Classes/MemberCenter \
$(LOCAL_PATH)/../../Classes/theme \
$(LOCAL_PATH)/../../Classes/ZhuanPanSystem \
$(LOCAL_PATH)/../../Classes/MultiPriceMatch \
$(LOCAL_PATH)/../../Classes/utilits \
$(LOCAL_PATH)/../../Classes/CollageGUI \
$(LOCAL_PATH)/../../Classes/C2DXShareSDK \
$(LOCAL_PATH)/../../Classes/C2DXShareSDK/Android \
$(LOCAL_PATH)/../../Classes/C2DXShareSDK/Android/JSON \
$(LOCAL_PATH)/../../Classes/C2DXShareSDK/Android/JSON/cJSON

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)


# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END