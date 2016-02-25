//===========Tweebaa==============
//DesignLayer.h	--Jly 30 2015	
//Version 1.1 by Raymond Zhuang at Toronto
// last modified at
#pragma once

#include <cstdlib>
#include <fstream>
#include "define.h"
#include "HttpConnection.h"
#include "PopupLayer.h"
#include "Collage_Template.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace std;
using namespace cocos2d::ui;

USING_NS_CC;

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)  
    #include <dirent.h>  
    #include <sys/stat.h>  
#else  
    #include <io.h>
#endif  

typedef struct{
			Vec2 	_pos;
			int		_ZOrder;
			float	_angle;
			float	_scale;
			string	_text;
			string	_font;
			Color3B	_color;
		}ORIG_INFO;

typedef struct{
			unsigned int	id;
			PicType			_Cat;
			unsigned int	_seat;
			ElementType		_type;
			Vec2 	_position;
			int		nZorder;
			string	_filename;
			char	uid[64];
			ORIG_INFO	_oInfo;
		}SPRITE_INFO;

enum OperationType
{
	 kOTypeAdd = 1,
	 kOTypeDel,
	 kOTypeMove,
	 kOTypeFlipX,
	 kOTypeFlipY,
	 kOTypeText,
	 kOTypeColor,
	 kOTypeFont,
	 kOTypeZOrder,
};

class Operation
{
	public:
		SPRITE_INFO	m_item;
		unsigned short	m_nSaved;		//0-false,1-saved,2-published
		OperationType	m_Operation;

		Sprite* m_sprite;
		Label*	m_label;

};


class DesignLayer :public Layer
{
	private:
		
		bool   lessmark(const   SPRITE_INFO&   s1,const   SPRITE_INFO&   s2);
		bool isMoved,isColored,isFonted;

		ListView	*_listColor,*_listFont;

		std::vector<std::string>	v_Font_List;

		Vec2	StartPos,CurPos;
		float	preScale;

	public:
			
		static Sprite *	selSprite;	//selected sprite
		static Label *	selLabel;
		static SPRITE_INFO selItem_info;

		static Vector<Sprite *> sp_vec;//¾«ÁéÈÝÆ÷
		static Vector<Label *> lb_vec;
		static vector<SPRITE_INFO> sp_info_vec;//¾«ÁéÊôÐÔÈÝÆ÷

		static vector<Operation>	v_undo;
		static vector<Operation>	v_redo;

		bool Undo(void);
		bool Redo(void);
		static bool UndoAdd(OperationType,SPRITE_INFO);

		static bool UndoSave(void);		//after saved or published, set flag
		static void UndoClear(void);
		static void RedoClear(void);

		static void	MsgBox_Doit(cocos2d::Object *,Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent,const char * button_ok,const char * button_no,void*,cocos2d::SEL_CallFuncND callback);
		static void	MsgBox(Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent);
		void	MsgBoxCallback(cocos2d::Node *pNode,void*);
		static bool LoadDraft(const char*);

		int	nCurInfoPos,nCurItemPos;
		DesignLayer(cocos2d::Node*);
		~DesignLayer(void);
		static Node* designArea;
		static Node* _rightMenu;
		Node* designMenu, *colorMenu, *fontMenu;
		
		static bool	bLongPressed;
		LayerColor* layerColor;

		virtual bool init();
		void initialize(cocos2d::Node* pNode);
		void FindAllFile(string folderPath);
		//std::vector<std::string> getFilePathAtVec(std::string filePath);

		virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
		virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
		virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
		virtual void onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event);

		void selectSpriteForTouch(Vec2 touchLocation);
		void panForTranslation(Vec2 translation);

		void longPress(float dt);

		void touchForward(Node* object,TouchEventType type);
		void touchBack(Node* object,TouchEventType type);
		void touchFlipX(Node* object,TouchEventType type);
		void touchMax(Node* object,TouchEventType type);
		void touchRemove(Node* object,TouchEventType type);
		void touchColor(Node* object,TouchEventType type);
		void touchFont(Node* object,TouchEventType type);
		void touchText(Node* object,TouchEventType type);

		void popupLayer(Node*,Vec2 popPos,const char *,const char *);
		void buttonCallback(cocos2d::Node *pNode,void*);
		void clickFontCallback(cocos2d::Ref *pSender, TouchEventType event);
		void clickColorCallback(cocos2d::Ref *pSender, TouchEventType event);
};

inline void SetButtonEnabled(ui::Button * paramButton, bool paramEnabled)
{
	if (paramButton==NULL) return;
	if (paramEnabled)
	{
		paramButton->setBright(true);
		paramButton->setEnabled(true);
		paramButton->setTouchEnabled(true);
	}
	else
	{
		paramButton->setBright(false);
		paramButton->setEnabled(false);
		paramButton->setTouchEnabled(false);
	}
}

inline	int RGB2I(Color3B color)
{
	return color.b << 16 | color.g << 8 | color.r;
}

inline	Color3B I2RGB(int color)
{
	Color3B	c;
	c.b = (color>>16) & 0xff;
	c.g = (color>>8) & 0xff;
	c.r = color & 0xff;

	return(c);
}