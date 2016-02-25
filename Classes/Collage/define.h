//===========Tweebaa==============
//define.h	--Jly 23 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at
#pragma once

//#define _CRTDBG_MAP_ALLOC 
//#include<stdlib.h> 
//#include<crtdbg.h> 

#define Window_Width	960
#define Window_Heigh	640
#define	Main_Categories	15
#define Default_Draft_Design	"temp_design.png"
#define Default_Publish_Design	"temp_publish.jpg"
#define Default_Font	"fonts/Humanst521 BT.ttf"
#define Default_Title_Font	"fonts/HUM521B.TTF"
#define Default_Chinese_Font	"fonts/baidu.ttf"
#define	Default_FontSize	50
#define	Static_Resources	22	// xml files + color pattern
//#define	Default_FontColor	

#define MenuItem_Profile 1005
#define MenuItem_Trends 1001
#define MenuItem_Home 1000
#define MenuItem_Create 1002
#define MenuItem_Feed 1003
#define MenuItem_Shop 1004

#define MenuItem_AddItem 2001
#define MenuItem_Fit 2002
#define MenuItem_Clear 2003
#define MenuItem_Save 2004
#define MenuItem_Publish 2005
#define MenuItem_Facebook 2011
#define MenuItem_Twitter 2012
#define MenuItem_Pinterest 2013
#define MenuItem_GPlus 2014

#define MenuItem_OK 3001
#define MenuItem_CANCEL 3002

#define	Template_URL	"http://67.224.94.82/Collage/TempImg/"
//#define Design_URL	"http://67.224.94.82/images801"
#define Design_URL		"http://67.224.94.82/upload/UploadImg"
#define Product_URL		"http://67.224.94.82/upload/UploadImg/mid2/"
#define Req_URL			"http://67.224.94.82/mobileApp"
//#define Req_URL		"http://tweebaa.com:8081"

#define Decoration_URL	"http://67.224.94.82/Collage/DesignDecorationImg/"
//#define	Article_URL		"http://67.224.94.82/Collage/DesignDecorationImg/"
#define	Article_URL		"http://67.224.94.82/Collage/DesignDecorationImg/"

#define Design_Template_Time	"/CollageAPI.ashx?action=load_sort_publish&sort_type=1"
#define Design_Total	"/CollageAPI.ashx?action=load_publish_total"
#define Remove_Design	"/CollageAPI.ashx?action=remove_my_collage&user_id=%s&design_id=%s"
#define Template_List	"/CollageAPI.ashx?action=load_template&page=1&pageDiv=12"
//#define Product_List	"/CollageAPI.ashx?action=load_product&page=1"
//#define Product_List	"/CollageAPI.ashx?action=load_product&page=1&pageDiv=40"
#define Product_List	"/CollageAPI.ashx?action=load_product&page=1&pageDiv=9999&cate_guid="
#define Like_List	"/CollageAPI.ashx?action=load_all_publish&page=1&pageDiv=9999"
#define Published_List	"/CollageAPI.ashx?action=load_my_publish&user_id=%s&page=1&pageDiv=9999"
#define Search_Product	"/CollageAPI.ashx?action=load_search_product&page=1&pageDiv=9999&keyword="

#define Background_List	"/CollageAPI.ashx?action=load_decoration_Background"
#define Event_List	"/CollageAPI.ashx?action=load_decoration_Christmas"
#define Decoration_List	"/CollageAPI.ashx?action=load_decoration&page=1&pageDiv=9999"
#define Article_List	"/CollageAPI.ashx?action=load_artise&page=1&pageDiv=9999"
#define Post_FileName		"/CollageFileAPI.ashx"
#define Post_Parameter	"/CollageAPI.ashx?action=Save_Collage"
//#define	Post_Like	"/CollageAPI.ashx?action=save_favour&user_id=eff4dca6-fedc-4bd9-bbe5-79dec2bb39d8&design_id=20"
//#define	Post_Unlike	"/CollageAPI.ashx?action=remove_favour&user_id=eff4dca6-fedc-4bd9-bbe5-79dec2bb39d8&design_id=20"
#define	Post_Like	"/CollageAPI.ashx?action=save_favour"
#define	Post_Unlike	"/CollageAPI.ashx?action=remove_favour"
#define COllage_Share	"https://www.tweebaa.com/product/collageReview.aspx?design_id="

#define	MAX_UNDO	9
#define	MAX_PIC	9			// max pics in edit area
#define MAX_TEXT	30
#define PIC_MATRIX 600		//save design size
#define MAX_SIDE_LENGTH 425
#define PIC_MIN_MATRIX 50		
#define Detail_MATRIX	360
#define Category_Width 380	//category pop-up window size
#define Detail_Width 375
#define L_Menu_Width 200
#define R_Menu_Width 132
#define D_Menu_Width 60
#define	C_Menu_Width	100
#define	Color_Num	21
#define Color_Pattern_Width	78
#define Color_Pattern_Height	55
#define Products_Size  110  //pic frame size -> category window 
#define Products_number 5
#define Items_Margin	15
#define MAX_CacheFile 18
#define Items_InaRow	3	//category product&decoration list
#define Items_MaxRow	12
#define	CriticalRows	3	//距离边界临界行数
#define Product_ID_Flag	5000	//decoration ID += 5000

#define Current_Item_Tag	9999
#define List_Products_Tag	9001
#define Design_Tag	9002
#define L_Menu_Tag	9003
#define D_Menu_Tag	9004
#define P_Detail_Tag	9005
#define C_Menu_Tag	9006
#define F_Menu_Tag	9007
#define B_TRENDS	9101

#define Design_Pos_X	L_Menu_Width+PIC_MATRIX/2+15

#define TIME_OUT 10

enum AppendFlag { HEAD=false, END=true};

enum GDirection        //当前手势方向;
{
	 kGDirectionUp = 0,
	 kGDirectionDown,
	 kGDirectionLeft,
	 kGDirectionRight,
	 kGDirectionNo
};

enum PicType
{
	kPTypeNone = 0,
	kPTypeDecoration = 1,
	kPTypeArticle,
	kPTypeProduct,
};

enum ElementType
{
	 kETypeSprite = 0,
	 kETypeLabel,
};

enum OpSaveType
{
	 kNOSave = 0,
	 kSaved,
	 kPublished
};

enum menuItem{Profile=MenuItem_Profile,Home=MenuItem_Home,Trends=MenuItem_Trends,Create=MenuItem_Create,Feed=MenuItem_Feed,Shop=MenuItem_Shop};

#define	Cat_Electronics	"1608AC1B-786A-4596-9DEE-04FC45131332"
#define	Cat_Apparel		"3EE6A6A4-933A-42BA-9C53-07F6AEE27ED2"
#define	Cat_Garden		"4036D9BF-F178-45BB-94F0-1693923AE9B5"
#define	Cat_Toys		"C9590D3C-F179-413A-AC5B-24DB7EEB2A1A"
#define	Cat_Tools		"5D01EB4D-7E33-4381-B993-45988262CB92"
#define	Cat_Children	"F66C02B2-3441-4D71-8328-5A5129E73D2A"
#define	Cat_Arts		"B984BDA6-A0A7-4CD2-9EDD-6D1B33EFC3AD"
#define	Cat_Household	"1382D416-6E2D-4708-8557-7F226EDBDC13"
#define	Cat_Health		"C781386B-DBD9-4257-9EB6-8D74ED61A9C1"
#define	Cat_Automotive	"F5527740-ED0E-4F9B-B7B9-A8222E5EA31C"
#define	Cat_Pets		"1F728092-0771-431D-81B2-A9B7D3DDE7FE"
#define	Cat_Sports		"EA786CE8-9664-48CC-A22D-B1BB5957F0E2"
#define	Cat_Office		"1E12E85A-F86C-4243-8446-C5F33B19E454"
#define	Cat_Green		"FDAC6A4C-86D4-4DBB-857C-F52831F846B1"
#define	Cat_Clean		"7DEE69E4-6103-4E16-A0CC-FB065473183D"
