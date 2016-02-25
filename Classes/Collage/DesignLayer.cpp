//===========Tweebaa==============
//DesignLayer.cpp	--Jly 30 2015	
//Version 1.2 by Raymond Zhuang at Toronto
// last modified at Nov 2015

#include "DesignLayer.h"

vector <SPRITE_INFO> DesignLayer::sp_info_vec;
Vector <Sprite *> DesignLayer::sp_vec;
Vector <Label *> DesignLayer::lb_vec;

vector<Operation>	DesignLayer::v_undo;
vector<Operation>	DesignLayer::v_redo;

SPRITE_INFO DesignLayer::selItem_info;
Node* DesignLayer::designArea;
//Node* DesignLayer::designMenu;
Node* DesignLayer::_rightMenu;
		
bool DesignLayer::bLongPressed=false;
Sprite * DesignLayer::selSprite;
Label * DesignLayer::selLabel;

DesignLayer::DesignLayer(cocos2d::Node* pNode)
{
	isMoved=false;
	isColored=false;
	isFonted=false;

	StartPos=Vec2::ZERO;

	initialize(pNode);
}


DesignLayer::~DesignLayer(void)
{
	Layer::onExit();
	designArea->removeFromParent();
	designArea=nullptr;
	_rightMenu->removeFromParent();
	_rightMenu=nullptr;
	designMenu->removeFromParent();
	designMenu=nullptr;
	colorMenu->removeFromParent();
	colorMenu=nullptr;
	fontMenu->removeFromParent();
	fontMenu=nullptr;
}


bool DesignLayer::init(){
	if(!Layer::init()){
		return false;
	}
	
	return true;

}

void DesignLayer::initialize(cocos2d::Node* pNode)
{
	
		designArea = pNode->getParent()->getChildByTag(Design_Tag);

		selSprite = NULL;
		bLongPressed=false;

		// redisplay collage
		if(sp_info_vec.size()>0)
		{
			int i=0;
			for(auto sp_info:sp_info_vec)
			{
				if(sp_info._type==ElementType::kETypeSprite)
				{
					sp_vec.at(sp_info._seat)->setPosition(sp_info._position);
					if (designArea)
						designArea->addChild(sp_vec.at(sp_info._seat), sp_info.nZorder);
				}
				else
				{
					lb_vec.at(sp_info._seat)->setPosition(sp_info._position);
					if (designArea)
						designArea->addChild(lb_vec.at(sp_info._seat), sp_info.nZorder);
				}
				i++;
			}
			
		}
		
		FindAllFile("fonts");

		fontMenu = CSLoader::createNode("Collage/ColorLayer.csb");
		fontMenu->setPosition(Vec2(Window_Width,0));
		designArea->getParent()->addChild(fontMenu);
		fontMenu->setTag(F_Menu_Tag);

		_listFont = static_cast<ListView*>(fontMenu->getChildByName("ListView_Color"));
		//static_cast<ScrollView*>(_listProduct)->addEventListener(CC_CALLBACK_2(Category::ProductViewMoveCallback, this));
		Size _listFontSize = _listFont->getSize();
	
		_listFont->setContentSize(Size(_listFontSize.width, _listFontSize.height));

		_listFont->setTouchEnabled(true);
		_listFont->setDirection(SCROLLVIEW_DIR_VERTICAL);
		_listFont->setBounceEnabled(true);
		_listFont->setClippingEnabled(true);		//hide off by window size
		_listFont->setItemsMargin(5);

		for(auto sFontName : v_Font_List)
		{
			Layout* custom_item = Layout::create();
			custom_item->setSize(Size(Color_Pattern_Width,Color_Pattern_Height));
			//cocos2d::__String s1 = "FONT中" ;
			//std::wstring s1 = "FONT中" ;
			//auto s = CollageTemplateList::WStrToUTF8(s1);
			//auto custom_label = Label::createWithTTF(s1, sFontName, 20);
			auto custom_label = Label::createWithTTF("FONT", sFontName, 24);
			if(!custom_label)
				continue;
			custom_label->setColor(Color3B::BLUE);
			custom_label->setAnchorPoint(Vec2(.5,.5));
			custom_label->setPosition(Color_Pattern_Width/2,Color_Pattern_Height/2);
			auto custom_button = ImageView::create("Collage/text_square.png");
			custom_button->setName(sFontName);
			custom_button->setScaleX(Color_Pattern_Width/custom_button->getContentSize().width);
			custom_button->setScaleY(Color_Pattern_Height/custom_button->getContentSize().height);
			custom_button->setAnchorPoint(Vec2::ZERO);
			custom_button->setPosition(Vec2(0,0));
			custom_button->setTouchEnabled(true);
			custom_button->addTouchEventListener(this,toucheventselector(DesignLayer::clickFontCallback));
			custom_button->addChild(custom_label);
			custom_item->addChild(custom_button);

			_listFont->pushBackCustomItem(custom_item);
		}

	//响应拖动事件
		
		auto listener = EventListenerTouchAllAtOnce::create();      //多点触摸
		this->setTouchEnabled(true);
	
		listener->onTouchesBegan = CC_CALLBACK_2(DesignLayer::onTouchesBegan, this);
		listener->onTouchesMoved = CC_CALLBACK_2(DesignLayer::onTouchesMoved, this);
		listener->onTouchesEnded = CC_CALLBACK_2(DesignLayer::onTouchesEnded, this);
		listener->onTouchesCancelled = CC_CALLBACK_2(DesignLayer::onTouchesCancelled, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, designArea);

		
		
		colorMenu = CSLoader::createNode("Collage/ColorLayer.csb");
		colorMenu->setPosition(Vec2(Window_Width,0));
		designArea->getParent()->addChild(colorMenu);
		colorMenu->setTag(C_Menu_Tag);

		_listColor = static_cast<ListView*>(colorMenu->getChildByName("ListView_Color"));
		//static_cast<ScrollView*>(_listProduct)->addEventListener(CC_CALLBACK_2(Category::ProductViewMoveCallback, this));
		Size _listColorSize = _listColor->getSize();
	
		_listColor->setContentSize(Size(_listColorSize.width, _listColorSize.height));

		_listColor->setTouchEnabled(true);
		_listColor->setDirection(SCROLLVIEW_DIR_VERTICAL);
		_listColor->setBounceEnabled(true);
		_listColor->setClippingEnabled(true);		//hide off by window size
		_listColor->setItemsMargin(5);

		for(int j=0;j<Color_Num;j++)
		{
			Layout* custom_item = Layout::create();
			custom_item->setSize(Size(Color_Pattern_Width,Color_Pattern_Height));
			//custom_item->setColor(Color3B(158,61,150));

			auto custom_button = ImageView::create("Collage/shade1.jpg");
			custom_button->setColor(Color3B(HttpConnection::v_color_list.at(j).R,HttpConnection::v_color_list.at(j).G,HttpConnection::v_color_list.at(j).B));
			custom_button->setScaleX(Color_Pattern_Width/custom_button->getContentSize().width);
			custom_button->setScaleY(Color_Pattern_Height/custom_button->getContentSize().height);
			custom_button->setAnchorPoint(Vec2::ZERO);
			custom_button->setPosition(Vec2(0,0));
			custom_button->setTouchEnabled(true);
			custom_button->addTouchEventListener(this,toucheventselector(DesignLayer::clickColorCallback));
			custom_item->addChild(custom_button);
			_listColor->pushBackCustomItem(custom_item);
		}
		
		designMenu = CSLoader::createNode("Collage/Designer.csb");
		designMenu->setPosition(Vec2(Window_Width,0));
		designArea->getParent()->addChild(designMenu);
		designMenu->setTag(D_Menu_Tag);

		auto buttonForward = dynamic_cast<Button*>(designMenu->getChildByName("Button_Forward"));
		auto buttonBack = dynamic_cast<Button*>(designMenu->getChildByName("Button_Back"));
		auto buttonFlipX = dynamic_cast<Button*>(designMenu->getChildByName("Button_FlipX"));
		auto buttonMax = dynamic_cast<Button*>(designMenu->getChildByName("Button_FlipY"));
		auto buttonRemove = dynamic_cast<Button*>(designMenu->getChildByName("Button_Remove"));
		auto buttonColor = dynamic_cast<Button*>(designMenu->getChildByName("Button_Color"));
		auto buttonText = dynamic_cast<Button*>(designMenu->getChildByName("Button_Text"));
		auto buttonFont = dynamic_cast<Button*>(designMenu->getChildByName("Button_Font"));
		
		//buttonFlipY->setVisible(false);

		buttonFlipX->addTouchEventListener(this,toucheventselector(DesignLayer::touchFlipX));
		buttonMax->addTouchEventListener(this,toucheventselector(DesignLayer::touchMax));
		buttonRemove->addTouchEventListener(this,toucheventselector(DesignLayer::touchRemove));
		buttonForward->addTouchEventListener(this,toucheventselector(DesignLayer::touchForward));
		buttonBack->addTouchEventListener(this,toucheventselector(DesignLayer::touchBack));
		buttonColor->addTouchEventListener(this,toucheventselector(DesignLayer::touchColor));
		buttonFont->addTouchEventListener(this,toucheventselector(DesignLayer::touchFont));
		buttonText->addTouchEventListener(this,toucheventselector(DesignLayer::touchText));

		Node::onEnter();
	
}

void DesignLayer::selectSpriteForTouch(Vec2 touchLocation)
{
    Sprite * newSprite = NULL;
	Label * newLabel = NULL;
	SPRITE_INFO item_info;
	//int j,k;
		
	if(sp_vec.empty()&&lb_vec.empty()) return;

	if(bLongPressed)
	{
		if (selSprite)
		{
			if ( selSprite->getBoundingBox().containsPoint(touchLocation) )			//editing sprite has highest priority
				this->schedule(schedule_selector(DesignLayer::longPress),1);
		}
		else if(selLabel)
		{
			if ( selLabel->getBoundingBox().containsPoint(touchLocation) )
				this->schedule(schedule_selector(DesignLayer::longPress),1);
		}

		return;
	}

	/*for ( int i=sp_vec.size()-1;i>=0;i--)
    {
		Sprite *sprite = sp_vec.at(i);
		sprite_info = sp_info_vec.at(i);

		if ( sprite->getBoundingBox().containsPoint(touchLocation) )
        {
            
			newSprite = sprite;
			this->schedule(schedule_selector(DesignLayer::longPress),1);
			nCurPos=i;
			SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Clone"),true);
			break;
        }
		
    }*/

	//j=sp_vec.size()-1;
	//k=lb_vec.size()-1;
	for ( int i=sp_info_vec.size()-1;i>=0;i--)
    {
		Sprite *sprite;
		Label	*label;

		item_info = sp_info_vec.at(i);
		if(item_info._type == ElementType::kETypeSprite)
		{
			sprite = sp_vec.at(item_info._seat);
			if ( sprite->getBoundingBox().containsPoint(touchLocation) )
			{
				newSprite = sprite;
				this->schedule(schedule_selector(DesignLayer::longPress),1);
				nCurItemPos=item_info._seat;
				nCurInfoPos=i;
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Clone"),true);
				
				//record start pos and status
				item_info._oInfo._pos=newSprite->getPosition();
				item_info._oInfo._scale=newSprite->getScale();
				item_info._oInfo._angle=newSprite->getRotation();
				item_info._oInfo._ZOrder=newSprite->getZOrder();
				
				break;
			}
			//j--;
		}	
		else if(item_info._type == ElementType::kETypeLabel)
		{
			label = lb_vec.at(item_info._seat);
			if ( label->getBoundingBox().containsPoint(touchLocation) )
			{
				newLabel = label;
				this->schedule(schedule_selector(DesignLayer::longPress),1);
				nCurItemPos=item_info._seat;
				nCurInfoPos=i;
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Clone"),false);
				
				//record start pos and status
				item_info._oInfo._pos=newLabel->getPosition();
				item_info._oInfo._scale=newLabel->getScale();
				item_info._oInfo._angle=newLabel->getRotation();
				item_info._oInfo._ZOrder=newLabel->getZOrder();
				item_info._oInfo._text=newLabel->getString();
				item_info._oInfo._color=newLabel->getColor();
				item_info._oInfo._font=newLabel->getTTFConfig().fontFilePath;
				
				break;
			}
			//k--;
		}
		if(newSprite || newLabel)
		{
			selItem_info._oInfo=item_info._oInfo;
		//if(selItem_info._oInfo._scale==0)
			//log("error get info from item!!!!!!!");
		}
    }

	//replace selected item
    if (newSprite != selSprite && NULL != newSprite)
    {
        if (NULL != selSprite)
        {
            selSprite->stopAllActions();
            //selSprite->runAction(RotateTo::create(0.1, 0));
			selSprite->runAction(FadeTo::create(0.5f, 255));
        }
        /*RotateBy * rotLeft = RotateBy::create(0.1, -14.0);
        RotateBy * rotCenter = RotateBy::create(0.1, 0.0);
        RotateBy * rotRight = RotateBy::create(0.1, 14.0);
        Sequence * rotSeq = Sequence::create(rotLeft, rotCenter, rotRight, rotCenter, NULL);*/
        //newSprite->runAction(RepeatForever::create(rotSeq));
		auto *fadeout = FadeOut::create(0.2f);
		auto *backout = fadeout->reverse();
		Sequence * rotSeq = Sequence::create(fadeout,backout,NULL);
		newSprite->runAction(Repeat::create(rotSeq,1));
        selSprite = newSprite;
		selLabel = nullptr;
		
    }
	else if(newLabel != selLabel && NULL != newLabel)
	{
		if (NULL != selLabel)
        {
            selLabel->stopAllActions();
			selLabel->runAction(FadeTo::create(0.5f, 255));
        }
		auto *fadeout = FadeOut::create(0.2f);
		auto *backout = fadeout->reverse();
		Sequence * rotSeq = Sequence::create(fadeout,backout,NULL);
		newLabel->runAction(Repeat::create(rotSeq,1));
        selLabel = newLabel;
		selSprite = nullptr;
	}
	if(newSprite || newLabel)
	{
		selItem_info = item_info;
//if(item_info._oInfo._scale==0)
	//log("error2 item info: %f,%f",item_info._oInfo._pos.x,item_info._oInfo._pos.y);
	}
	isMoved=false;
}


void DesignLayer::panForTranslation(Vec2 translation)
{
	if (selSprite)
    {
        Vec2 oldPos = selSprite->getPosition();
		Vec2 newPos = oldPos + translation;
		auto spriteWidth = selSprite->getBoundingBox().size.width;
		auto spriteHeight = selSprite->getBoundingBox().size.height;
		auto frameMaxX = designArea->getBoundingBox().getMaxX();
		auto frameMaxY = designArea->getBoundingBox().getMaxY();

		if(newPos.x+spriteWidth/2 > frameMaxX-215)
			newPos.x=frameMaxX-215-spriteWidth/2;
		if(newPos.x-spriteWidth/2<0) 
			newPos.x=spriteWidth/2;
		if(newPos.y + spriteHeight/2 > frameMaxY-20)
			newPos.y=frameMaxY-20-spriteHeight/2;
		if(newPos.y - spriteHeight/2<0)
			newPos.y=spriteHeight/2;

        selSprite->setPosition(newPos);
		selItem_info._position=newPos;
		sp_info_vec.at(nCurInfoPos)._position = newPos;
//log("current : x=%f,y=%f,scale=%f,rotation=%f",newPos.x,newPos.y,selSprite->getScale(),selSprite->getRotation());
    }
	else if (selLabel)
    {
        Vec2 oldPos = selLabel->getPosition();
		Vec2 newPos = oldPos + translation;
		auto spriteWidth = selLabel->getBoundingBox().size.width;
		auto spriteHeight = selLabel->getBoundingBox().size.height;
		auto frameMaxX = designArea->getBoundingBox().getMaxX();
		auto frameMaxY = designArea->getBoundingBox().getMaxY();

		if(newPos.x+spriteWidth/2 > frameMaxX-215)
			newPos.x=frameMaxX-215-spriteWidth/2;
		if(newPos.x-spriteWidth/2<0) 
			newPos.x=spriteWidth/2;
		if(newPos.y + spriteHeight/2 > frameMaxY-20)
			newPos.y=frameMaxY-20-spriteHeight/2;
		if(newPos.y - spriteHeight/2<0)
			newPos.y=spriteHeight/2;

        selLabel->setPosition(newPos);
		selItem_info._position=newPos;
		sp_info_vec.at(nCurInfoPos)._position = newPos;
//log("current : x=%f,y=%f,scale=%f,rotation=%f",newPos.x,newPos.y,selLabel->getScale(),selLabel->getRotation());
    }
}

void DesignLayer::longPress(float dt)
{
	auto visiableSize = Director::getInstance()->getVisibleSize();

	this->unschedule(schedule_selector(DesignLayer::longPress));

	if(!selSprite && !selLabel)	return;

	if(StartPos.distance(CurPos)>10 && dt)	//dt==0 : cancel selected
		return;
	
	if(!bLongPressed)
	{
		SetButtonEnabled((Button*)designArea->getParent()->getChildByName("Button_Trends"),false);
		SetButtonEnabled((Button*)designArea->getParent()->getChildByName("Button_Feed"),false);
		SetButtonEnabled((Button*)designArea->getParent()->getChildByName("Button_Shop"),false);
		
		if(selLabel)
		{
			SetButtonEnabled(dynamic_cast<Button*>(designMenu->getChildByName("Button_FlipX")),false);
			SetButtonEnabled(dynamic_cast<Button*>(designMenu->getChildByName("Button_FlipY")),false);
			SetButtonEnabled(dynamic_cast<Button*>(designMenu->getChildByName("Button_Text")),true);
			SetButtonEnabled(dynamic_cast<Button*>(designMenu->getChildByName("Button_Font")),true);
			SetButtonEnabled(dynamic_cast<Button*>(designMenu->getChildByName("Button_Color")),true);
		}
		else if(selSprite)
		{
			SetButtonEnabled(dynamic_cast<Button*>(designMenu->getChildByName("Button_FlipX")),true);
			SetButtonEnabled(dynamic_cast<Button*>(designMenu->getChildByName("Button_FlipY")),true);
			SetButtonEnabled(dynamic_cast<Button*>(designMenu->getChildByName("Button_Text")),false);
			SetButtonEnabled(dynamic_cast<Button*>(designMenu->getChildByName("Button_Font")),false);
			SetButtonEnabled(dynamic_cast<Button*>(designMenu->getChildByName("Button_Color")),false);
		}
		
		
		layerColor = LayerColor::create();
		layerColor->setColor(Color3B(0, 0, 0));  
		layerColor->setOpacity(50);  
		layerColor->setContentSize(Size(PIC_MATRIX, PIC_MATRIX));
		layerColor->setPosition(Vec2(Design_Pos_X-PIC_MATRIX/2,visiableSize.height/2-PIC_MATRIX/2));
		designArea->getParent()->addChild(layerColor); 
		layerColor->setTag(Current_Item_Tag);
		
		if(selSprite)
		{
			selSprite->removeFromParent();
			layerColor->addChild(selSprite);
		}
		else if(selLabel)
		{
			selLabel->removeFromParent();
			layerColor->addChild(selLabel);
		}
		bLongPressed=true;

		//designArea->getParent()->removeChildByTag(MenuItem_Create,true);
		auto actionBy1 = MoveBy::create(0.5,Vec2(R_Menu_Width,0));
		designArea->getParent()->getChildByTag(MenuItem_Create)->runAction(actionBy1);

		char s[3]; sprintf(s,"%d",selItem_info.nZorder+1);
		static_cast<Text*>(designMenu->getChildByName("Text_ZOrder"))->setText(s);
		
		auto actionBy2 = MoveBy::create(0.3,Vec2(-D_Menu_Width,0));
		designMenu->runAction(actionBy2);
	}
	else
	{
		//if ( ! selSprite->getBoundingBox().containsPoint(touchLocation) ) return;
		
		if(!isMoved)
		{
			if(selSprite)
			{
				layerColor->removeChild(selSprite);
	//log("reorder:%d",selSprite_info.nZorder);
				designArea->addChild(selSprite,selItem_info.nZorder);
				selSprite=NULL;
			}
			else if(selLabel)
			{
				layerColor->removeChild(selLabel);
	//log("reorder:%d",selSprite_info.nZorder);
				designArea->addChild(selLabel,selItem_info.nZorder);
				selLabel=NULL;
			}

			layerColor->removeFromParent();
			layerColor=nullptr;
		
			if(isColored)
			{
				auto actionByColor = MoveBy::create(0.5,Vec2(D_Menu_Width+C_Menu_Width,0));
				colorMenu->runAction(actionByColor);
				isColored=false;
			}
			if(isFonted)
			{
				auto actionByFont = MoveBy::create(0.5,Vec2(D_Menu_Width+C_Menu_Width,0));
				fontMenu->runAction(actionByFont);
				isFonted=false;
			}

			auto actionBy2 = MoveBy::create(0.3,Vec2(D_Menu_Width,0));
			designMenu->runAction(actionBy2);
	
			auto actionBy1 = MoveBy::create(0.5,Vec2(-R_Menu_Width,0));
			designArea->getParent()->getChildByTag(MenuItem_Create)->runAction(actionBy1);

			bLongPressed=false;

			SetButtonEnabled((Button*)designArea->getParent()->getChildByName("Button_Trends"),true);
			SetButtonEnabled((Button*)designArea->getParent()->getChildByName("Button_Feed"),true);
			SetButtonEnabled((Button*)designArea->getParent()->getChildByName("Button_Shop"),true);
			return;
		}
	
	}

	
}

void DesignLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{

	if(touches.size()==1 && sp_info_vec.size()>0)
	{

		auto touchLocation = designArea->convertTouchToNodeSpace(touches.back());
		if(StartPos==Vec2::ZERO)
		{
			StartPos = touchLocation;
			CurPos	= touchLocation;
		}
		this->selectSpriteForTouch(touchLocation);
		
	}
	else
	{
		//log("multi touches began");
	}
	
}

void DesignLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
	//this->unschedule(schedule_selector(DesignLayer::longPress));

	if(sp_info_vec.empty() || (!selSprite && !selLabel)) return;
	
	//if(!bLongPressed) return;

	if(touches.size()==1)
	{
		auto touchLocation = designArea->convertTouchToNodeSpace(touches.back());
		CurPos = touchLocation;
				
		//if ( !sp_vec.back()->getBoundingBox().containsPoint(touchLocation) )	return; //not in sprite rect
		if(selSprite)
		{
			if ( !selSprite->getBoundingBox().containsPoint(touchLocation) )
				return;
		}
		else if(selLabel)
		{
			if ( !selLabel->getBoundingBox().containsPoint(touchLocation) )
				return;
		}

		auto oldTouchLocation = touches.back()->getPreviousLocation();
		oldTouchLocation = designArea->convertToNodeSpace(oldTouchLocation);
		
 
		auto translation = touchLocation - oldTouchLocation;
		this->panForTranslation(translation/2);

	}
	else
	{
		this->unschedule(schedule_selector(DesignLayer::longPress));
		//zooming and rotating
		auto location1 = touches.at(0)->getLocation();
		auto location2 = touches.at(1)->getLocation();
		auto curDistance = location1.distance(location2);

		auto prev_location1 = touches.at(0)->getPreviousLocation();
		auto prev_location2 = touches.at(1)->getPreviousLocation();
		auto prevDistance = prev_location1.distance(prev_location2);

		if(curDistance>prevDistance)	//放大
		{
			if(selSprite)
			{
				auto ratio = curDistance/prevDistance;
				auto newScale = selSprite->getScale()*ratio;
				auto original_box = selSprite->getBoundingBox();
				auto new_min_x = original_box.getMinX() - original_box.size.width*(ratio-1)/2;
				auto new_max_x = original_box.getMaxX() + original_box.size.width*(ratio-1)/2;
				auto new_min_y = original_box.getMinY() - original_box.size.height*(ratio-1)/2;
				auto new_max_y = original_box.getMaxY() + original_box.size.height*(ratio-1)/2;
					
//log("org min:%f,%f -- max:%f,%f",original_box.getMinX(), original_box.getMinY(),original_box.getMaxX(),original_box.getMaxY());
//log("(%f)box size : %f ,%f",selSprite->getScale()*ratio,original_box.size.width,original_box.size.height);
//log("new min:%f,%f -- max:%f,%f",new_min_x, new_min_y,new_max_x,new_max_y);
				if(new_min_x>0 && new_max_x<PIC_MATRIX && new_min_y>0 && new_max_y<PIC_MATRIX)
				{

					selSprite->setScale(newScale);	//按比例缩放
					preScale=newScale;
				}
				else
				{
					while(new_min_x<0 || new_max_x>PIC_MATRIX || new_min_y<0 || new_max_y>PIC_MATRIX)
					{
						original_box = selSprite->getBoundingBox();
						new_min_x = original_box.getMinX() - original_box.size.width*(ratio-1)/2;
						new_max_x = original_box.getMaxX() + original_box.size.width*(ratio-1)/2;
						new_min_y = original_box.getMinY() - original_box.size.height*(ratio-1)/2;
						new_max_y = original_box.getMaxY() + original_box.size.height*(ratio-1)/2;

						preScale-=0.01;
						selSprite->setScale(preScale);
					}
				
				}
			}
			else if(selLabel)
			{
				auto newScale = selLabel->getScale()*(curDistance/prevDistance);
				auto ratio = curDistance/prevDistance;
				auto original_box = selLabel->getBoundingBox();
				auto new_min_x = original_box.getMinX() - original_box.size.width*(ratio-1)/2;
				auto new_max_x = original_box.getMaxX() + original_box.size.width*(ratio-1)/2;
				auto new_min_y = original_box.getMinY() - original_box.size.height*(ratio-1)/2;
				auto new_max_y = original_box.getMaxY() + original_box.size.height*(ratio-1)/2;

				if(new_min_x>0 && new_max_x<PIC_MATRIX && new_min_y>0 && new_max_y<PIC_MATRIX)
				{
					selLabel->setScale(newScale);
					preScale=newScale;
				}
				else
				{
					while(new_min_x<0 || new_max_x>PIC_MATRIX || new_min_y<0 || new_max_y>PIC_MATRIX)
					{
						original_box = selLabel->getBoundingBox();
						new_min_x = original_box.getMinX() - original_box.size.width*(ratio-1)/2;
						new_max_x = original_box.getMaxX() + original_box.size.width*(ratio-1)/2;
						new_min_y = original_box.getMinY() - original_box.size.height*(ratio-1)/2;
						new_max_y = original_box.getMaxY() + original_box.size.height*(ratio-1)/2;
					
						preScale-=0.01;
						selLabel->setScale(preScale);
					}
				}
			}
		}
		else if(curDistance<prevDistance)	//缩小
		{
			if(selSprite)
			{
				if(selSprite->getBoundingBox().size.height>PIC_MIN_MATRIX && selSprite->getBoundingBox().size.width>PIC_MIN_MATRIX)
				{
					selSprite->setScale(selSprite->getScale()*(curDistance/prevDistance));	//按比例缩放
				}
			}
			else if(selLabel)
			{
				if(selLabel->getBoundingBox().size.height>PIC_MIN_MATRIX && selLabel->getBoundingBox().size.width>PIC_MIN_MATRIX)
					selLabel->setScale(selLabel->getScale()*(curDistance/prevDistance));
			}
		}
		//sp_vec.back()->setScale(sp_vec.back()->getScale()*(curDistance/prevDistance));	//按比例缩放
		
		
			

		//旋转角度等于以第一点为圆心到第二点距离为半径 与 以第二点为圆心到第一点距离为半径的两个角度之和
		auto distance1=location1.distance(prev_location1);
		auto distance2=location2.distance(prev_location2);
		auto angle1 = CC_RADIANS_TO_DEGREES(atanf(distance2/prevDistance));
		auto angle2 = CC_RADIANS_TO_DEGREES(atanf(distance1/location1.distance(prev_location2)));
		auto angle = angle1+angle2;
		if(distance2<distance1)  //切换两点顺序，保证逆时针判断
		{
			auto temp_location = location1;
			location1 = location2;
			location2 = temp_location;

			temp_location = prev_location1;
			prev_location1 = prev_location2;
			prev_location2 = temp_location;
		}
		if(location2.x>location1.x) //1,4象限
		{
			
			if(abs(location2.y-prev_location2.y)>abs(location2.x-prev_location2.x)) //根据位移判断用户主要意图
			{
				if(location2.y>prev_location2.y )
					angle=-angle;	//anticlock
			}
			else
			{
				if(location2.y>location1.y && location2.x<prev_location2.x) //1象限
					angle=-angle;
				else if(location2.y<location1.y && location2.x>prev_location2.x) //4象限
					angle=-angle;
			}
		}
		else if(location2.x<location1.x) //2,3象限
		{
			if(abs(location2.y-prev_location2.y)>abs(location2.x-prev_location2.x)) //根据位移判断用户主要意图
			{
				if(location2.y<prev_location2.y)
					angle=-angle;
			}
			else
			{
				if(location2.y>location1.y && location2.x<prev_location2.x) //2象限
					angle=-angle;
				else if(location2.y<location1.y && location2.x>prev_location2.x) //3象限
					angle=-angle;
			}
		}
		else //x相等 
		{
			if(location2.y>location1.y) //1,2象限
			{
				if(location2.x<prev_location2.x)
					angle=-angle;
			}
			else if(location2.y<location1.y) //3,4象限
			{
				if(location2.x>prev_location2.x)
					angle=-angle;
			}

		} 
//log("angle=%f:(%f + %f)",angle,angle1,angle2);

		//sp_vec.back()->setRotation(sp_vec.back()->getRotation()+angle);
		if(selSprite && selSprite->getBoundingBox().getMaxY()<PIC_MATRIX && selSprite->getBoundingBox().getMinY()>0 &&
			selSprite->getBoundingBox().getMaxX()<PIC_MATRIX &&	selSprite->getBoundingBox().getMinX()>0)
			selSprite->setRotation(selSprite->getRotation()+angle);
		else if(selLabel)
			selLabel->setRotation(selLabel->getRotation()+angle);

		
	}
	isMoved=true;
}

void DesignLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
	StartPos=Vec2::ZERO;

	if(bLongPressed && (selSprite||selLabel) && !isMoved)
	{
		auto touchLocation = designArea->convertTouchToNodeSpace(touches.back());
		if(selSprite)
		{
			if (! selSprite->getBoundingBox().containsPoint(touchLocation) )
			{
				longPress(0);
			}
		}
		else if(selLabel)
		{
			if (! selLabel->getBoundingBox().containsPoint(touchLocation) )
			{
				longPress(0);
			}
		}
	}

	if(isMoved && (selSprite||selLabel))
	{
		if(selItem_info._oInfo._scale!=0)
			UndoAdd(OperationType::kOTypeMove,selItem_info);

	}

	this->unschedule(schedule_selector(DesignLayer::longPress));
	isMoved=false;
}

void DesignLayer::onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event)
{
     this->unschedule(schedule_selector(DesignLayer::longPress));
	 isMoved=false;
	 StartPos=Vec2::ZERO;
}

void DesignLayer::touchRemove(Node* object,TouchEventType type)
{
	MoveBy	*actionBy2,*actionBy1;
	
	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			
			if(selSprite)
			{
				//selSprite->retain();	//avoid auto release
				layerColor->removeChild(selSprite);
				layerColor->removeFromParent();
				layerColor=nullptr;
				
				//sp_vec.erase(nCurItemPos);		
				/*UNDO_INFO	undo_info;
				undo_info.item_info=sp_info_vec.at(nCurInfoPos);
				UndoAdd(OperationType::kOTypeDel,selSprite,undo_info);*/	//save info from sp_info_vec
			}
			else if(selLabel)
			{
				//selLabel->retain();
				layerColor->removeChild(selLabel);
				layerColor->removeFromParent();
				layerColor=nullptr;
				
				//lb_vec.erase(nCurItemPos);
				/*UNDO_INFO	undo_info;
				undo_info.item_info=sp_info_vec.at(nCurInfoPos);
				UndoAdd(OperationType::kOTypeDel,selLabel,undo_info);*/
			}
			UndoAdd(OperationType::kOTypeDel,sp_info_vec.at(nCurInfoPos));
			sp_info_vec.erase(sp_info_vec.begin()+nCurInfoPos);

			//reset ZOrder, seat, label's id
			for(int i=nCurInfoPos;i<sp_info_vec.size();i++)
			{
				sp_info_vec.at(i).nZorder--;
				if(selSprite && sp_info_vec.at(i)._type == ElementType::kETypeSprite)
					sp_info_vec.at(i)._seat--;
				else if(selLabel && sp_info_vec.at(i)._type == ElementType::kETypeLabel)
				{
					sp_info_vec.at(i)._seat--;
					sp_info_vec.at(i).id--;
				}
			}

			selSprite=NULL;
			selLabel=NULL;
			
			SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Clone"),false);
			SetButtonEnabled((Button*)designArea->getParent()->getChildByName("Button_Trends"),true);
			SetButtonEnabled((Button*)designArea->getParent()->getChildByName("Button_Feed"),true);
			SetButtonEnabled((Button*)designArea->getParent()->getChildByName("Button_Shop"),true);
			
			if(!sp_vec.size() && !lb_vec.size())
			{
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Save"),false);
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Publish"),false);
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Clear"),false);
			}
			
			actionBy2 = MoveBy::create(0.3,Vec2(D_Menu_Width,0));
			designMenu->runAction(actionBy2);

			actionBy1 = MoveBy::create(0.5,Vec2(-R_Menu_Width,0));
			designArea->getParent()->getChildByTag(MenuItem_Create)->runAction(actionBy1);
		
			bLongPressed=false;

			break;  
		  
		default:  
			break; 
	}
}
void DesignLayer::touchFlipX(Node* object,TouchEventType type)
{
	
	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			if(selSprite)
			{
				if(selSprite->isFlippedX())
				{
					selSprite->setFlippedX(false);
				}
				else
				{
					selSprite->setFlippedX(true);
				}
				UndoAdd(kOTypeFlipX,sp_info_vec.at(nCurInfoPos));
			}
			break;  
		  
		default:  
			break;  
    }
	
}

void DesignLayer::touchMax(Node* object,TouchEventType type)
{
	
	if(type==TouchEventType::TOUCH_EVENT_ENDED)  
    {  
		
		if(selSprite)
		{
			/*if(selSprite->isFlippedY())
			{
				selSprite->setFlippedY(false);
			}
			else
			{
				selSprite->setFlippedY(true);
			}
			UndoAdd(kOTypeFlipY,sp_info_vec.at(nCurInfoPos));*/
			selSprite->setPosition(PIC_MATRIX/2,PIC_MATRIX/2);
			selSprite->setRotation(0);
			selSprite->setScale(PIC_MATRIX/selSprite->getContentSize().width>PIC_MATRIX/selSprite->getContentSize().height
				?PIC_MATRIX/selSprite->getContentSize().height:PIC_MATRIX/selSprite->getContentSize().width);
			UndoAdd(OperationType::kOTypeMove,selItem_info);

			while(true)
			{
				if(selSprite->getZOrder()>0)
					touchBack(object,type);
				else
					break;
			}
		}
		 
    }
	
}

void DesignLayer::touchColor(Node* object,TouchEventType type)
{
	MoveBy * actionBy;
	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			
			if(!isColored)
			{
				if(isFonted)
				{
					actionBy = MoveBy::create(0.5,Vec2((D_Menu_Width+C_Menu_Width),0));
					fontMenu->runAction(actionBy);
					isFonted=false;
				}
				actionBy = MoveBy::create(0.5,Vec2(-(D_Menu_Width+C_Menu_Width),0));
				colorMenu->runAction(actionBy);
				isColored=true;
			}
			else
			{
				actionBy = MoveBy::create(0.5,Vec2((D_Menu_Width+C_Menu_Width),0));
				colorMenu->runAction(actionBy);
				isColored=false;
			}

			break;  
		  
		default:  
			break;  
    }
	
}

void DesignLayer::touchFont(Node* object,TouchEventType type)
{
	MoveBy * actionBy;
	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:

			if(!isFonted)
			{
				if(isColored)
				{
					actionBy = MoveBy::create(0.5,Vec2((D_Menu_Width+C_Menu_Width),0));
					colorMenu->runAction(actionBy);
					isColored=false;
				}
				actionBy = MoveBy::create(0.5,Vec2(-(D_Menu_Width+C_Menu_Width),0));
				fontMenu->runAction(actionBy);
				isFonted=true;
			}
			else
			{
				actionBy = MoveBy::create(0.5,Vec2((D_Menu_Width+C_Menu_Width),0));
				fontMenu->runAction(actionBy);
				isFonted=false;
			}

			break;  
		  
		default:  
			break;  
    }
	
}

void DesignLayer::touchText(Node* object,TouchEventType type)
{
	MoveBy * actionBy;
	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			
			//cocos2d::log("Text biu....");
			if(!selLabel)
				popupLayer(layerColor,Vec2(-180,0),"Less than 20 characters","");
			else
				popupLayer(layerColor,Vec2(-180,0),selLabel->getString().c_str(),selLabel->getString().c_str());

			break;  
		  
		default:  
			break;  
    }
	
}

 bool   DesignLayer::lessmark(const   SPRITE_INFO&   s1,const   SPRITE_INFO&   s2)
 {  
      return   s1.nZorder   <   s2.nZorder;  
 }

void DesignLayer::touchBack(Node* object,TouchEventType type)
{
	
	int nMinOrder;
	SPRITE_INFO tmp_info;

	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
	
			nMinOrder=sp_info_vec.at(0).nZorder;

			if(sp_info_vec.at(nCurInfoPos).nZorder>nMinOrder)
			{
				tmp_info.nZorder=sp_info_vec.at (nCurInfoPos).nZorder;	//save original ZOrder
				selItem_info._oInfo._ZOrder=tmp_info.nZorder;	//save for undo

				tmp_info.nZorder--;

				for(int i =0;i<sp_info_vec.size();i++)
				{
					if(i==nCurInfoPos)	continue;
					if(sp_info_vec.at(i).nZorder==tmp_info.nZorder)
					{
						//reset current ZOrder for sp_vec.at(i)
						if(sp_info_vec.at(nCurInfoPos)._type==ElementType::kETypeSprite)
							sp_vec.at(sp_info_vec.at(nCurInfoPos)._seat)->setZOrder(sp_info_vec.at(i).nZorder);
						else
							lb_vec.at(sp_info_vec.at(nCurInfoPos)._seat)->setZOrder(sp_info_vec.at(i).nZorder);

						if(sp_info_vec.at(i)._type==ElementType::kETypeSprite)
							sp_vec.at(sp_info_vec.at(i)._seat)->setZOrder(sp_info_vec.at(nCurInfoPos).nZorder);
						else
							lb_vec.at(sp_info_vec.at(i)._seat)->setZOrder(sp_info_vec.at(nCurInfoPos).nZorder);

						//swap sp_vec
						if(sp_info_vec.at(nCurInfoPos)._type==sp_info_vec.at(i)._type)
						{
							if(sp_info_vec.at(nCurInfoPos)._type==ElementType::kETypeSprite)
								sp_vec.swap(sp_info_vec.at(nCurInfoPos)._seat,sp_info_vec.at(i)._seat);
							else
								lb_vec.swap(sp_info_vec.at(nCurInfoPos)._seat,sp_info_vec.at(i)._seat);
						}
						else
						{
							tmp_info._seat=sp_info_vec.at(nCurInfoPos)._seat;
							sp_info_vec.at(nCurInfoPos)._seat=sp_info_vec.at(i)._seat;
							sp_info_vec.at(i)._seat=tmp_info._seat;
						}

						//swap sp_info_vec position
						tmp_info._position=sp_info_vec.at(nCurInfoPos)._position;
						sp_info_vec.at(nCurInfoPos)._position=sp_info_vec.at(i)._position;
						sp_info_vec.at(i)._position=tmp_info._position;

						tmp_info._type=sp_info_vec.at(nCurInfoPos)._type;
						sp_info_vec.at(nCurInfoPos)._type=sp_info_vec.at(i)._type;
						sp_info_vec.at(i)._type=tmp_info._type;

						tmp_info._filename=sp_info_vec.at(nCurInfoPos)._filename;
						sp_info_vec.at(nCurInfoPos)._filename=sp_info_vec.at(i)._filename;
						sp_info_vec.at(i)._filename=tmp_info._filename;

						tmp_info._oInfo=sp_info_vec.at(nCurInfoPos)._oInfo;
						sp_info_vec.at(nCurInfoPos)._oInfo=sp_info_vec.at(i)._oInfo;
						sp_info_vec.at(i)._oInfo=tmp_info._oInfo;

						strcpy(tmp_info.uid,sp_info_vec.at(nCurInfoPos).uid);
						strcpy(sp_info_vec.at(nCurInfoPos).uid,sp_info_vec.at(i).uid);
						strcpy(sp_info_vec.at(i).uid,tmp_info.uid);

						//repoint current position
						nCurInfoPos=i;

						selItem_info.nZorder=sp_info_vec.at(nCurInfoPos).nZorder;
						break;
					}
				}
				//resort
				//sort(sp_info_vec.begin(), sp_info_vec.end(),lessmark);
				if(object != designArea)
				{
					UndoAdd(kOTypeZOrder,selItem_info);
				}
				char s[3]; sprintf(s,"%d",selItem_info.nZorder+1);
				static_cast<Text*>(designMenu->getChildByName("Text_ZOrder"))->setText(s);
			}

			break;  
		  
		default:  
			break;  
    }
	
}
 
 void DesignLayer::touchForward(Node* object,TouchEventType type)
{
	
	int nMaxOrder;
	SPRITE_INFO tmp_info;

	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:

			nMaxOrder=sp_info_vec.back().nZorder;

			if(sp_info_vec.at(nCurInfoPos).nZorder<MAX_PIC && sp_info_vec.at(nCurInfoPos).nZorder<nMaxOrder)
			{
				tmp_info.nZorder=sp_info_vec.at(nCurInfoPos).nZorder;	//save original ZOrder
				selItem_info._oInfo._ZOrder=tmp_info.nZorder;	//save for undo

				tmp_info.nZorder++;
					
				for(int i =0;i<sp_info_vec.size();i++)
				{
					if(i==nCurInfoPos)	continue;
					if(sp_info_vec.at(i).nZorder==tmp_info.nZorder)
					{
						//reset current ZOrder for sp_vec.at(i)
						if(sp_info_vec.at(nCurInfoPos)._type==ElementType::kETypeSprite)
							sp_vec.at(sp_info_vec.at(nCurInfoPos)._seat)->setZOrder(sp_info_vec.at(i).nZorder);
						else
							lb_vec.at(sp_info_vec.at(nCurInfoPos)._seat)->setZOrder(sp_info_vec.at(i).nZorder);

						if(sp_info_vec.at(i)._type==ElementType::kETypeSprite)
							sp_vec.at(sp_info_vec.at(i)._seat)->setZOrder(sp_info_vec.at(nCurInfoPos).nZorder);
						else
							lb_vec.at(sp_info_vec.at(i)._seat)->setZOrder(sp_info_vec.at(nCurInfoPos).nZorder);

						//swap sp_vec
						if(sp_info_vec.at(nCurInfoPos)._type==sp_info_vec.at(i)._type)
						{
							if(sp_info_vec.at(nCurInfoPos)._type==ElementType::kETypeSprite)
								sp_vec.swap(sp_info_vec.at(nCurInfoPos)._seat,sp_info_vec.at(i)._seat);
							else
								lb_vec.swap(sp_info_vec.at(nCurInfoPos)._seat,sp_info_vec.at(i)._seat);
						}
						else
						{
							tmp_info._seat=sp_info_vec.at(nCurInfoPos)._seat;
							sp_info_vec.at(nCurInfoPos)._seat=sp_info_vec.at(i)._seat;
							sp_info_vec.at(i)._seat=tmp_info._seat;
						}

						//swap sp_info_vec position
						tmp_info._position=sp_info_vec.at(nCurInfoPos)._position;
						sp_info_vec.at(nCurInfoPos)._position=sp_info_vec.at(i)._position;
						sp_info_vec.at(i)._position=tmp_info._position;

						tmp_info._type=sp_info_vec.at(nCurInfoPos)._type;
						sp_info_vec.at(nCurInfoPos)._type=sp_info_vec.at(i)._type;
						sp_info_vec.at(i)._type=tmp_info._type;

						tmp_info._filename=sp_info_vec.at(nCurInfoPos)._filename;
						sp_info_vec.at(nCurInfoPos)._filename=sp_info_vec.at(i)._filename;
						sp_info_vec.at(i)._filename=tmp_info._filename;

						tmp_info._oInfo=sp_info_vec.at(nCurInfoPos)._oInfo;
						sp_info_vec.at(nCurInfoPos)._oInfo=sp_info_vec.at(i)._oInfo;
						sp_info_vec.at(i)._oInfo=tmp_info._oInfo;

						strcpy(tmp_info.uid,sp_info_vec.at(nCurInfoPos).uid);
						strcpy(sp_info_vec.at(nCurInfoPos).uid,sp_info_vec.at(i).uid);
						strcpy(sp_info_vec.at(i).uid,tmp_info.uid);

						//repoint current position 
						nCurInfoPos=i;

						selItem_info.nZorder=sp_info_vec.at(nCurInfoPos).nZorder;
						break;
					}
					else if(i>nCurInfoPos && sp_info_vec.at(i).nZorder<sp_info_vec.at(nCurInfoPos).nZorder)
						break;
					else
						continue;
				}
				//resort
				//sort(sp_info_vec.begin(), sp_info_vec.end(),lessmark);
				if(object != designArea)
				{
					UndoAdd(kOTypeZOrder,selItem_info);
				}
				char s[3]; sprintf(s,"%d",selItem_info.nZorder+1);
				static_cast<Text*>(designMenu->getChildByName("Text_ZOrder"))->setText(s);
			}
				

			break;  
		  
		default:  
			break;  
    }
	
}


void DesignLayer::FindAllFile(string folderPath)  
{  
    string	sFilename;

	// Window处理方式  
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
    _finddata_t FileInfo;  
    string strfind = folderPath + "\\*";  
    long Handle = _findfirst(strfind.c_str(), &FileInfo);  
    if (Handle == -1L)  
    {  
        cocos2d::log("can not match the folder path");  
        return;  
    }  
    do{  
        //判断是否有子目录  
        if (FileInfo.attrib & _A_SUBDIR)      
        {  
            //这个语句很重要  
            if( (strcmp(FileInfo.name,".") != 0 ) &&(strcmp(FileInfo.name,"..") != 0))     
            {  
                string newPath = folderPath + "\\" + FileInfo.name;  
                FindAllFile(newPath);  
            }  
        }  
        else    
        {  
            //count++;  
            //log("file:%s\\%s" , folderPath.c_str() , FileInfo.name); 
			sFilename=folderPath.c_str();
			sFilename+="/";
			sFilename+=FileInfo.name;
			if(sFilename.find(".ttf")!= string::npos || sFilename.find(".TTF")!= string::npos)
			{
				v_Font_List.push_back(sFilename);
				//log("file:%s" , sFilename.c_str()); 
			}
        }  
    }while (_findnext(Handle, &FileInfo) == 0);  
    _findclose(Handle);  
#else
/*#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
    log("folder:%s",folderPath.c_str());  
    DIR *dp;  
    struct dirent* dirp;  
  
    if((dp = opendir(folderPath.c_str())) == NULL)  
    {  
        log("can not match the folder path");  
        return;  
    }  
    while ((dirp=readdir(dp))!=NULL)  
    {  
        struct stat buf;  
        stat(folderPath.c_str(), &buf);  
  
        // 如果是目录  
        if (S_ISDIR(buf.st_mode))  
        {  
                string path;  
                if( (strcmp(dirp->d_name,".") != 0 ) &&(strcmp(dirp->d_name,"..") != 0))     
                {  
                    path =  folderPath + "/" + dirp->d_name;  
                }  
                //如果是目录，递归调用  
                FindAllFile(path);     
        }  
        else  
        {  
            // 如果是文件直接打印  
            //log("%s/%s\n",folderPath.c_str(),dirp->d_name); 
			sFilename=folderPath.c_str();
			sFilename+="/";
			sFilename+=dirp->d_name;
			if(sFilename.find(".ttf")!= string::npos)
			{
				v_Font_List.push_back(sFilename);
				//log("file:%s" , sFilename.c_str()); 
			}
        }  
    }  
    log("\n");  
    closedir(dp);  
#else*/	//for Android
	auto pCTL=CollageTemplateList::ParseXMLWithfile("Font_List.xml");
	if(!pCTL) 
	{
		log("Font_list file format error");
		exit(0);
	}
	for(auto font : pCTL->vTemplate)
	{
		v_Font_List.push_back(font.strListImage);
//log("Font:%s",font.strListImage);
	}
	delete pCTL; 
//#endif
#endif  
}

void DesignLayer::popupLayer(Node* baseLayer,Vec2 popPos,const char * pre_text,const char * text){
    // 定义一个弹出层，传入一张背景图
    PopupLayer* pl = PopupLayer::create("popup_sq.png");
	
    // ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩放
    pl->setContentSize(Size(500, 250));
	//auto sizeLayer = baseLayer->getContentSize();

	pl->setAnchorPoint(Vec2(.5,.5));
	pl->setPosition(popPos.x,popPos.y);
	pl->setTitle("ENTER TEXT",Color3B(0,0,0));
    //pl->setContentText("Dialogbox context",Color3B(0,0,0), 20, 60, 250);
	pl->addEditBox("text_square.png", pre_text ,text, Color3B::BLACK,6 ,Size(350,30), 20);
    // 设置回调函数，回调传回一个 Node 以获取 tag 判断点击的按钮
    // 这只是作为一种封装实现，如果使用 delegate 那就能够更灵活的控制参数了
    pl->setCallbackFunc(this, callfuncND_selector(DesignLayer::buttonCallback),NULL);
    // 添加按钮，设置图片，文字，tag 信息
	pl->addButton("button_normal.png", "button_dis.png", "DONE",Color3B(255,255,255), 1);
	pl->addButton("button_normal.png", "button_dis.png", "CANCEL",Color3B(255,255,255), 0);
    // 添加到指定层
	baseLayer->addChild(pl,99);
}

void DesignLayer::buttonCallback(cocos2d::Node *pNode,void *data){
//log("button call back. tag: %d", pNode->getTag());

	if(pNode->getTag())
	{
		auto editText = dynamic_cast<EditBox*>(pNode->getParent()->getParent()->getChildByName("EditBox"));
		string s=editText->getText();
		if(s.size()>0)
		{
			selLabel->setString(s);
		}
		//check bounding of label...
		auto original_box = selLabel->getBoundingBox();
		auto new_min_x = original_box.getMinX();
		auto new_max_x = original_box.getMaxX();
		auto new_min_y = original_box.getMinY();
		auto new_max_y = original_box.getMaxY();
		auto cScale = selLabel->getScale();

		while(new_min_x<0 || new_max_x>PIC_MATRIX || new_min_y<0 || new_max_y>PIC_MATRIX)
		{
			cScale-=0.01;
			selLabel->setScale(cScale);

			original_box = selLabel->getBoundingBox();
			new_min_x = original_box.getMinX();
			new_max_x = original_box.getMaxX();
			new_min_y = original_box.getMinY();
			new_max_y = original_box.getMaxY();
		}
		UndoAdd(kOTypeText,selItem_info);
	}
}

void  DesignLayer::clickFontCallback(cocos2d::Ref *pSender, TouchEventType event) {
   
    auto btn = static_cast<Button *>(pSender);
	TTFConfig LabelConf;

	switch (event)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
//log("click font = %s", btn->getName().c_str());
			LabelConf=selLabel->getTTFConfig();
			LabelConf.fontFilePath=btn->getName();
			LabelConf.fontSize = Default_FontSize;
			selLabel->setTTFConfig(LabelConf);
			UndoAdd(kOTypeFont,selItem_info);
			break;
		default:
			break;
	}
}

void  DesignLayer::clickColorCallback(cocos2d::Ref *pSender, TouchEventType event) {
   
    auto btn = static_cast<Button *>(pSender);
	switch (event)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
//log("click color = %d,%d,%d",btn->getColor().r,btn->getColor().g,btn->getColor().b);
			selLabel->setColor(btn->getColor());
			UndoAdd(kOTypeColor,selItem_info);
			break;
		default:
			break;
	}
}

bool DesignLayer::Redo(void)
{
	TTFConfig LabelConf;
	float	t1,t2;
	Vec2	t3;
	Color3B	tColor;
	string	sTemp;

	if(v_redo.size()<=0)
		return false;
	auto& op = v_redo.back();

	switch(op.m_Operation)
	{
		case kOTypeAdd:
			if(op.m_item._type==kETypeSprite)
			{
				designArea->addChild(op.m_sprite);
				op.m_sprite->release();
				sp_vec.pushBack(op.m_sprite);
			}
			else if(op.m_item._type==kETypeLabel)
			{
				designArea->addChild(op.m_label);
				op.m_label->release();
				lb_vec.pushBack(op.m_label);
			}
			sp_info_vec.push_back(op.m_item);

			if(sp_info_vec.size()>0)
			{
				if(selSprite)
					SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Clone"),true);
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Save"),true);
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Publish"),true);
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Clear"),true);
			}
			break;
		
		case kOTypeDel:
			if(op.m_item._type==kETypeSprite)
			{
				if(selSprite==op.m_sprite)
					selSprite=nullptr;
				op.m_sprite->retain();		//save into undo_list, when undo or undoclear will release it
				op.m_sprite->removeFromParent();
				sp_vec.erase(op.m_item._seat);
				
			}
			else
			{
				if(selLabel==op.m_label)
					selLabel=nullptr;
				op.m_label->retain();
				op.m_label->removeFromParent();
				lb_vec.erase(op.m_item._seat);
			}

			sp_info_vec.erase(sp_info_vec.begin()+op.m_item.nZorder);
			//reset ZOrder, seat, label's id
			for(int i=op.m_item.nZorder;i<sp_info_vec.size();i++)
			{
				sp_info_vec.at(i).nZorder--;
				if(op.m_item._type==kETypeSprite && sp_info_vec.at(i)._type == ElementType::kETypeSprite)
					sp_info_vec.at(i)._seat--;
				else if(op.m_item._type==kETypeLabel && sp_info_vec.at(i)._type == ElementType::kETypeLabel)
				{
					sp_info_vec.at(i)._seat--;
					sp_info_vec.at(i).id--;
				}
			}

			if(sp_info_vec.size()==0)
			{
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Clone"),false);
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Save"),false);
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Publish"),false);
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Clear"),false);
			}
			break;

		case kOTypeMove:
			if(op.m_item._type==kETypeSprite)
			{
				t1 = op.m_sprite->getRotation();
				t2 = op.m_sprite->getScale();
				t3 = op.m_sprite->getPosition();
				op.m_sprite->setPosition(op.m_item._oInfo._pos);
				op.m_sprite->setRotation(op.m_item._oInfo._angle);
				op.m_sprite->setScale(op.m_item._oInfo._scale);
			}
			else
			{
				t1 = op.m_label->getRotation();
				t2 = op.m_label->getScale();
				t3 = op.m_label->getPosition();
				op.m_label->setPosition(op.m_item._oInfo._pos);
				op.m_label->setRotation(op.m_item._oInfo._angle);
				op.m_label->setScale(op.m_item._oInfo._scale);
			}
			op.m_item._oInfo._angle = t1;
			op.m_item._oInfo._scale = t2;
			op.m_item._oInfo._pos = t3;
			break;
		
		case kOTypeZOrder:
			nCurInfoPos = op.m_item._oInfo._ZOrder;
			
			if(op.m_item.nZorder>op.m_item._oInfo._ZOrder)
			{
				touchForward(designArea,TouchEventType::TOUCH_EVENT_ENDED);
			}
			else
			{
				touchBack(designArea,TouchEventType::TOUCH_EVENT_ENDED);
			}
			break;
		
		case kOTypeFlipX:
			if(op.m_item._type==kETypeSprite)
			{
				if(op.m_sprite->isFlippedX())
				{
					op.m_sprite->setFlippedX(false);
				}
				else
				{
					op.m_sprite->setFlippedX(true);
				}
			}
			break;

		case kOTypeFlipY:
			if(op.m_item._type==kETypeSprite)
			{
				if(op.m_sprite->isFlippedY())
				{
					op.m_sprite->setFlippedY(false);
				}
				else
				{
					op.m_sprite->setFlippedY(true);
				}
			}
			break;
		
		case kOTypeText:
			if(op.m_item._type==kETypeLabel)
			{
				sTemp = op.m_label->getString();
				op.m_label->setString(op.m_item._oInfo._text);
				op.m_item._oInfo._text = sTemp;
			}
			break;
		
		case kOTypeColor:
			if(op.m_item._type==kETypeLabel)
			{
				tColor = op.m_label->getColor();
				op.m_label->setColor(op.m_item._oInfo._color);
				op.m_item._oInfo._color = tColor;
			}
			break;
		
		case kOTypeFont:
			if(op.m_item._type==kETypeLabel)
			{
				LabelConf=op.m_label->getTTFConfig();
				sTemp = LabelConf.fontFilePath;
				LabelConf.fontFilePath=op.m_item._oInfo._font;
				op.m_label->setTTFConfig(LabelConf);
				op.m_item._oInfo._font = sTemp;
			}
			break;
		
		default:
			break;
	}
	v_undo.push_back(v_redo.back());
	v_redo.pop_back();
	if(v_redo.size()==0)
		SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Redo"),false);
	if(v_undo.size()>0)
		SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Undo"),true);
	return true;
}

bool DesignLayer::Undo(void)
{
	TTFConfig LabelConf;
	float	t1,t2;
	Vec2	t3;
	Color3B	tColor;
	string	sTemp;

	if(v_undo.size()<=0)
		return false;
	auto& op = v_undo.back();

	switch(op.m_Operation)
	{
		case kOTypeAdd:
			if(op.m_item._type==kETypeSprite)
			{
				if(selSprite==op.m_sprite)
					selSprite=nullptr;
				op.m_sprite->retain();
				op.m_sprite->removeFromParent();
				sp_vec.popBack();		
			}
			else
			{
				if(selLabel==op.m_label)
					selLabel=nullptr;
				op.m_label->retain();
				op.m_label->removeFromParent();
				lb_vec.popBack();
			}

			sp_info_vec.pop_back();
			if(sp_info_vec.size()==0)
			{
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Clone"),false);
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Save"),false);
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Publish"),false);
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Clear"),false);
			}

			break;
		
		case kOTypeDel:
			if(op.m_item._type==kETypeSprite)
			{
				designArea->addChild(op.m_sprite);
				op.m_sprite->release();
				//reset order for sp_vec & sp_info according to Z_order, modify _seat to last of sp_vec
				sp_vec.insert(op.m_item._seat,op.m_sprite);
				sp_info_vec.insert(sp_info_vec.begin()+op.m_item.nZorder,op.m_item);
				if(op.m_item.nZorder+1<sp_info_vec.size())
				{
					for(int i=op.m_item.nZorder+1;i<sp_info_vec.size();i++)
					{
						sp_info_vec.at(i).nZorder++;
						if(sp_info_vec.at(i)._type==kETypeSprite)
							sp_info_vec.at(i)._seat++;
					}
				}
			}
			else if(op.m_item._type==kETypeLabel)
			{
				designArea->addChild(op.m_label);
				op.m_label->release();
				//reset order for lb_vec & lb_info according to Z_order, modify _seat to last of lb_vec
				lb_vec.insert(op.m_item._seat,op.m_label);
				sp_info_vec.insert(sp_info_vec.begin()+op.m_item.nZorder,op.m_item);
				if(op.m_item.nZorder+1<sp_info_vec.size())
				{
					for(int i=op.m_item.nZorder+1;i<sp_info_vec.size();i++)
					{
						sp_info_vec.at(i).nZorder++;
						if(sp_info_vec.at(i)._type==kETypeLabel)
							sp_info_vec.at(i)._seat++;
					}
				}
			}

			if(sp_info_vec.size()>0)
			{
				if(selSprite)
					SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Clone"),true);
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Save"),true);
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Publish"),true);
				SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Clear"),true);
			}
			
			break;
		
		case kOTypeMove:
			
			if(op.m_item._type==kETypeSprite)
			{
				t1 = op.m_sprite->getRotation();
				t2 = op.m_sprite->getScale();
				t3 = op.m_sprite->getPosition();
				op.m_sprite->setPosition(op.m_item._oInfo._pos);
				op.m_sprite->setRotation(op.m_item._oInfo._angle);
				op.m_sprite->setScale(op.m_item._oInfo._scale);
			}
			else
			{
				t1 = op.m_label->getRotation();
				t2 = op.m_label->getScale();
				t3 = op.m_label->getPosition();
				op.m_label->setPosition(op.m_item._oInfo._pos);
				op.m_label->setRotation(op.m_item._oInfo._angle);
				op.m_label->setScale(op.m_item._oInfo._scale);
			}
			op.m_item._oInfo._angle = t1;
			op.m_item._oInfo._scale = t2;
			op.m_item._oInfo._pos	= t3;
			break;

		case kOTypeZOrder:
			nCurInfoPos = op.m_item.nZorder;		//selected item is NULL
			
			if(op.m_item.nZorder<op.m_item._oInfo._ZOrder)
			{
				touchForward(designArea,TouchEventType::TOUCH_EVENT_ENDED);
			}
			else
			{
				touchBack(designArea,TouchEventType::TOUCH_EVENT_ENDED);
			}
			break;
		
		case kOTypeFlipX:
			if(op.m_item._type==kETypeSprite)
			{
				if(op.m_sprite->isFlippedX())
				{
					op.m_sprite->setFlippedX(false);
				}
				else
				{
					op.m_sprite->setFlippedX(true);
				}
			}
			break;

		case kOTypeFlipY:
			if(op.m_item._type==kETypeSprite)
			{
				if(op.m_sprite->isFlippedY())
				{
					op.m_sprite->setFlippedY(false);
				}
				else
				{
					op.m_sprite->setFlippedY(true);
				}
			}
			break;
		
		case kOTypeText:
			if(op.m_item._type==kETypeLabel)
			{
				sTemp = op.m_label->getString();
				op.m_label->setString(op.m_item._oInfo._text);
				op.m_item._oInfo._text = sTemp;
			}
			break;
		
		case kOTypeColor:
			if(op.m_item._type==kETypeLabel)
			{
				tColor = op.m_label->getColor();
				op.m_label->setColor(op.m_item._oInfo._color);
				op.m_item._oInfo._color = tColor;
			}
			break;
		
		case kOTypeFont:
			if(op.m_item._type==kETypeLabel)
			{
				LabelConf=op.m_label->getTTFConfig();
				sTemp = LabelConf.fontFilePath;
				LabelConf.fontFilePath=op.m_item._oInfo._font;
				op.m_label->setTTFConfig(LabelConf);
				op.m_item._oInfo._font = sTemp;
			}
			break;
		
		default:
			break;
	}
	v_redo.push_back(v_undo.back());
	v_undo.pop_back();
	if(v_undo.size()==0)
		SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Undo"),false);
	if(v_redo.size()>0)
		SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Redo"),true);
	return true;
}

void DesignLayer::RedoClear(void)
{
	for(auto v : v_redo)
	{
		if(v.m_Operation==kOTypeAdd)
		{
			switch(v.m_item._type)
			{
				case kETypeSprite:
					v.m_sprite->release();
					break;
				case kETypeLabel:
					v.m_label->release();
					break;
			}
		}
	}
	v_redo.clear();
}

void DesignLayer::UndoClear(void)
{
	for(auto v : v_undo)
	{
		if(v.m_Operation==kOTypeDel)
		{
			switch(v.m_item._type)
			{
				case kETypeSprite:
					v.m_sprite->release();
					break;
				case kETypeLabel:
					v.m_label->release();
					break;
			}
		}
	}
	v_undo.clear();
}


bool DesignLayer::UndoAdd(OperationType o_type,SPRITE_INFO info)
{
	Operation	op;

	op.m_Operation=o_type;
	op.m_item=info;
	op.m_nSaved=OpSaveType::kNOSave;

	if(info._type==kETypeSprite)
		op.m_sprite = sp_vec.at(info._seat);
	else if(info._type==kETypeLabel)
		op.m_label = lb_vec.at(info._seat);

	switch(o_type)
	{
		case OperationType::kOTypeAdd:
			if(v_undo.size()>0 && v_undo.back().m_Operation==o_type && v_undo.back().m_item._type==info._type)
			{
				if(info._type==kETypeSprite)
				{
					if(v_undo.back().m_sprite==op.m_sprite)
						return false;
				}
				else
				{
					if(v_undo.back().m_label==op.m_label)
						return false;
				}
			}
			
			break;

		case OperationType::kOTypeDel:
			if(info._type==kETypeSprite)
			{
				op.m_sprite->retain();
				sp_vec.erase(info._seat);
			}
			else if(info._type==kETypeLabel)
			{
				op.m_label->retain();
				lb_vec.erase(info._seat);
			}
			if(v_undo.size()>0 && v_undo.back().m_Operation==kOTypeDel && v_undo.back().m_item._type==info._type)
			{
				if(info._type==kETypeSprite)
				{
					if(v_undo.back().m_sprite==op.m_sprite)
						return false;
				}
				else
				{
					if(v_undo.back().m_label==op.m_label)
						return false;
				}
			}
			break;

		case OperationType::kOTypeMove:
			if(v_undo.size()>0 && v_undo.back().m_Operation==OperationType::kOTypeMove)
			{
				if(info._oInfo._scale==0 || (v_undo.back().m_item._oInfo._pos==info._oInfo._pos 
					&& v_undo.back().m_item._oInfo._angle==info._oInfo._angle 
					&& v_undo.back().m_item._oInfo._scale==info._oInfo._scale))
				{
//log("duplicated operation!!%f,%f,%f,%f",info._oInfo._pos.x,info._oInfo._pos.y,info._oInfo._angle,info._oInfo._scale);
					return false;
				}
			}
			break;

		case OperationType::kOTypeZOrder:
			if(v_undo.size()>0 && v_undo.back().m_Operation==OperationType::kOTypeZOrder)
			{
				if(v_undo.back().m_item._oInfo._ZOrder==info._oInfo._ZOrder)
					return false;
			}
			break;
		
		case OperationType::kOTypeText:
		case OperationType::kOTypeColor:
		case OperationType::kOTypeFont:
			if(info._type!=kETypeLabel)
				return false;
			break;
		
		default:
			
			break;
	}

	if(v_undo.size()>=MAX_UNDO)
	{
		v_undo.erase(v_undo.begin());
	}

	v_undo.push_back(op);

	SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Undo"),true);
	SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Redo"),false);
	SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Publish"),true);
	SetButtonEnabled((Button*)_rightMenu->getChildByName("Button_Save"),true);
	RedoClear();
	return true;
}

void DesignLayer::MsgBox_Doit(cocos2d::Object *obj,Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent,const char *button_ok,const char *button_no,void *para_data,cocos2d::SEL_CallFuncND callback)
{
    PopupLayer* pl = PopupLayer::create("popup_sq.png");

    pl->setContentSize(Size(460, 280));
	//auto sizeLayer = baseLayer->getContentSize();

	pl->setAnchorPoint(Vec2(.5,.5));
	pl->setPosition(popPos.x,popPos.y);
	pl->setTitle(sTitle,Color3B(106,72,36));
	pl->setContentText(sContent.c_str(),Color3B(106,72,36), 24, 20, 185);
 	pl->setCallbackFunc(obj, callback,para_data);
    // 添加按钮，设置图片，文字，tag 信息
	pl->addButton("button_normal.png", "button_dis.png", button_ok,Color3B(255,255,255), 1);
	pl->addButton("button_normal.png", "button_dis.png", button_no,Color3B(255,255,255), 0);

    // 添加到指定层
	baseLayer->addChild(pl);
}

void DesignLayer::MsgBox(Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent){

    PopupLayer* pl = PopupLayer::create("popup_sq.png");

    pl->setContentSize(Size(450, 250));
	//auto sizeLayer = baseLayer->getContentSize();

	pl->setAnchorPoint(Vec2(.5,.5));
	pl->setPosition(popPos.x,popPos.y);
	pl->setTitle(sTitle,Color3B(106,72,36));
	pl->setContentText(sContent.c_str(),Color3B(106,72,36), 24, 20, 250);
	
    //pl->setCallbackFunc(obj, callfuncND_selector(DesignLayer::MsgBoxCallback),NULL);
    // 添加按钮，设置图片，文字，tag 信息
	pl->addButton("button_normal.png", "button_dis.png", "OK",Color3B(255,255,255), 1);
    // 添加到指定层
	baseLayer->addChild(pl);
}

void DesignLayer::MsgBoxCallback(cocos2d::Node *pNode,void *data)
{
//log("button call back. tag: %d", pNode->getTag());
	
}

bool DesignLayer::LoadDraft(const char* filename)
{
	char sSQL[512], sUrl[256],filenamebuffer[128];

	sprintf(sSQL,"select rowid from DraftList where filename='%s'",filename);
	char **re;
	int r,c;
	auto pDB = CollageTemplateList::initDB(DB_File);
	if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK) 
	{
		sqlite3_close(pDB);
        log("load rowid from table[draftlist] failure.");
		return false;
    }

	if(r==0)
	{
		sqlite3_free_table(re);
		sqlite3_close(pDB);
        log("no record match %s.",filename);
		return false;
    }
	unsigned long long RecNo = atoi(re[1]);
	sprintf(sSQL,"select * from DraftInfo where id=%lld",RecNo);
	if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK) 
	{
		sqlite3_close(pDB);
        log("load elements from table[draftinfo] failure.");
		return false;
    }

	sqlite3_close(pDB);

	if(r==0)
	{
		sqlite3_free_table(re);
		log("no element for %s.",filename);
		return false;
	}

	sp_vec.clear();
	lb_vec.clear();
	sp_info_vec.clear();
	selSprite=nullptr;
	selLabel=nullptr;

	SPRITE_INFO	item_info;
	for(int i=1;i<r+1;i++)
	{
		if(re[i*c])
			strcpy(item_info.uid,re[i*c]);
		auto nType = atoi(re[i*c+3]);
		item_info._position.x = atoi(re[i*c+5]);
		item_info._position.y = atoi(re[i*c+6]);
		item_info.nZorder = atoi(re[i*c+7]);
		float scale = atof(re[i*c+8]);
		float angle = atof(re[i*c+9]);
		item_info._Cat = (PicType)atoi(re[i*c+13]);
		
		if(nType==kETypeLabel)
		{
			item_info._type = kETypeLabel;
			string sLabel = re[i*c+4];
			auto color = I2RGB(atoi(re[i*c+11]));
			string sFontpath = re[i*c+12];

			Label* ltf = Label::createWithTTF(sLabel, sFontpath, Default_FontSize);
			
			ltf->setPosition(item_info._position);
			ltf->setScale(scale);
			ltf->setRotation(angle);
			ltf->setZOrder(item_info.nZorder);
			ltf->setColor(color);

			item_info._seat = lb_vec.size();
			lb_vec.pushBack(ltf);
			sp_info_vec.push_back(item_info);
		}
		else
		{
			item_info._type = kETypeSprite;
			item_info._filename = re[i*c+1];
			auto flip_flag = atoi(re[i*c+10]);
			auto cPos = strrchr(re[i*c+1],'/');
			if(!cPos)
				sprintf(filenamebuffer,"$P_%s",re[i*c+1]);
			else
				sprintf(filenamebuffer,"$P_%s",cPos+1);
			string cache_file = TEMP_filePath + Cache_Path + filenamebuffer;
			
			ifstream ifs(cache_file);
			if(!ifs)
			{	
				if(item_info._Cat == PicType::kPTypeDecoration)
				{
					strcpy(sUrl,Decoration_URL);
				}
				else if(item_info._Cat == PicType::kPTypeArticle)
				{
					strcpy(sUrl,Article_URL);
				}
				else
				{
					strcpy(sUrl,Product_URL);
				}
				strcat(sUrl,item_info._filename.c_str());
			
				auto http_connect = new HttpConnection();
				if(http_connect->CurlReq(sUrl,cache_file.c_str())!=CURLE_OK)
				{
					//download failed 
					//HttpConnection::numberOfLoadSprites++;
					log("%s download error",sUrl);
					delete http_connect;
					continue;
				}
				delete http_connect;

			}
			auto sprite = Sprite::create(cache_file);
			
			if(flip_flag==1)
				sprite->setFlippedX(true);
			else if(flip_flag==2)
				sprite->setFlippedY(true);
			else if(flip_flag==3)
			{
				sprite->setFlippedX(true);
				sprite->setFlippedY(true);
			}
			sprite->setPosition(item_info._position);
			sprite->setScale(scale);
			sprite->setRotation(angle);
			sprite->setZOrder(item_info.nZorder);

			item_info._seat = sp_vec.size();
			sp_vec.pushBack(sprite);
			sp_info_vec.push_back(item_info);

		}//kETypeSprite
	}
	sqlite3_free_table(re);
	return true;
}


/*std::vector<std::string> DesignLayer::getFilePathAtVec(std::string filePath)
{
    std::vector<std::string> path_vec;
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    int i=0;
    
    if((dp=opendir(filePath.c_str()))==NULL)
    {
        fprintf(stderr,cannot open %s,filePath.c_str());
        exit(1);
    }
    chdir(filePath.c_str());
     
    while((entry=readdir(dp))!=NULL&&i<255)
    {
        stat(entry->d_name,&statbuf);
        if(!S_ISREG(statbuf.st_mode))
            continue;
        path_vec.push_back(StringUtils::format(%s,entry->d_name));
    }
    return path_vec;
}//</std::string></std::string>*/