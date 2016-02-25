#include "SlidingMenuLayer.h"
#include "WelcomeScene.h"
#include "global_const.h"
#include "PictureGameScene.h"

/*
public class SlidingMenuLayer extends CCLayer {
	private static final int SCROLLVIEW_TAG = 300;
	private static CGSize screenSize;
	CCScrollView scrollView;
	CCBitmapFontAtlas statusLabel ;
	private CGPoint startlocation; //keep track of touch starting point
	private CGPoint endlocation;   ////keep track of touch ending point
	float tilescale ;

	float generalscalefactor = 0.0f ;

*/

char jigSawFile[100];
SlidingMenuLayer::SlidingMenuLayer (){};
SlidingMenuLayer::~SlidingMenuLayer (){};	
bool SlidingMenuLayer::init(){
		if(!Layer::init()){
			return false;
		}
	//ge{

		generalscalefactor = 0.0f ;

		this->setTouchEnabled(true);// setIsTouchEnabled(true);
		this->setKeyboardEnabled(true); //setIsKeyEnabled(true) ; 
		//this->isTouchEnabled_ = true ;

		Size screenSize = Director::getInstance()->getVisibleSize();
		Point origin = Director::getInstance()->getVisibleOrigin();


		::ui::Layout* layout = ::ui::Layout::create();
		layout->setSize(Size(screenSize.width, screenSize.height));
		layout->setBackGroundColorType(::ui::LAYOUT_COLOR_SOLID);
		layout->setBackGroundColor(ccc3(4,123,182));
		layout->setPosition(Vec2(0, 0));
		addChild(layout,-5);

		//screenSize = CCDirectorsharedDirector().winSize();
		/*
		generalscalefactor  = screenSize.height / 500 ;

		Sprite* background = Sprite::create("background.jpg");
		
		background->setScale(screenSize.width / background->getContentSize().width);
		background->setAnchorPoint(Vec2(0.0f,1.0f)) ;
		background->setPosition(Vec2(0, screenSize.height));
		
		addChild(background,-5);

		//Some instruction for the user to process

		 auto label = Label::createWithTTF("Select an option", "fonts/Marker Felt.ttf", 24);
		 label->setPosition(Vec2(screenSize.width/2 + origin.x, screenSize.height/2 + origin.y+200));
		addChild(label,1);

	Sprite *game7 = Sprite::create("Back_60.png");
	Sprite *game7_a = Sprite::create("Back_60.png");//Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
	game7_a->setPositionY(5);
	auto menuItem_7  = MenuItemSprite::create(game7,game7_a,NULL,CC_CALLBACK_1(SlidingMenuLayer::menuCallback, this));
	menuItem_7->setPosition(Vec2(screenSize.width - game7->getContentSize().width * 1.5f *generalscalefactor , game7->getContentSize().height * 1.5f *generalscalefactor));

	auto menu7 = Menu::create(menuItem_7,NULL);
	menu7->setPosition(Point(origin.x ,origin.y));
	this->addChild(menu7,1);


	Size scollFrameSize = Size(screenSize.width, screenSize.height/4);
	auto scrollView = cocos2d::ui::ScrollView::create();
	scrollView->setContentSize(scollFrameSize);
	scrollView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	scrollView->setBackGroundColor(Color3B(200, 200, 200));
	scrollView->setPosition(Point(0, screenSize.height/2 ));
	scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
	scrollView->setBounceEnabled(true);
	scrollView->setTouchEnabled(true);
	auto containerSize = Size(scollFrameSize.width*2, scollFrameSize.height*2);
	 scrollView->setInnerContainerSize(containerSize);

	this->addChild(scrollView);

	char file[100];
	for(int i = 1; i <= 9; i++) 
	{
		
		sprintf(file, "match_game_img_%d.jpg", i);
		file[20]='\0';

		auto button1 = ui::Button::create();
		button1->setTouchEnabled(true);
		button1->ignoreContentAdaptWithSize(false);
		button1->setContentSize(Size(200, containerSize.height));
		button1->loadTextures(file, file);
		button1->setPosition(Point(200*(i-1)+30 , containerSize.height ));
		button1->addTouchEventListener(this,::ui::SEL_TouchEvent(&SlidingMenuLayer::touchButton));
		scrollView->addChild(button1,1,i);
	}
	
*/

		Size scollFrameSize = Size(screenSize.width, screenSize.height -140);
	auto scrollView = cocos2d::ui::ScrollView::create();
	scrollView->setContentSize(scollFrameSize);
	scrollView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	scrollView->setBackGroundColor(Color3B(200, 200, 200));
	scrollView->setPosition(Point(0, 100 ));
	scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
	scrollView->setBounceEnabled(true);
	scrollView->setTouchEnabled(true);
	auto containerSize = Size(470*5, 360);
	 scrollView->setInnerContainerSize(containerSize);

	this->addChild(scrollView);

	
	char file[100];
	for(int i = 1; i <= 9; i++) 
	{
		
		sprintf(file, "match_game_img_%d.jpg", i);
		file[20]='\0';
		auto button1 = ui::Button::create();
		button1->setTouchEnabled(true);
		button1->ignoreContentAdaptWithSize(false);
		//button1->setScale(containerSize.height / 353);
		button1->setContentSize(Size(440, containerSize.height));
		button1->loadTextures(file, file);
		button1->setPosition(Point(440*(i-1)+ 240 , containerSize.height -80));
		button1->addTouchEventListener(this,::ui::SEL_TouchEvent(&SlidingMenuLayer::touchButton));
		scrollView->addChild(button1,1,1000+i);
	}

	Sprite *game7 = Sprite::create("game_menu/back_btn.png");
	Sprite *game7_a = Sprite::create("game_menu/back_btn.png");//Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
	game7_a->setPositionY(5);
	auto menuItem_7  = MenuItemSprite::create(game7,game7_a,NULL,CC_CALLBACK_1(SlidingMenuLayer::menuCallback, this));
	menuItem_7->setPosition(Point(origin.x + screenSize.width/2 ,60));

    auto menu7 = Menu::create(menuItem_7,NULL);
	menu7->setPosition(Point(origin.x ,origin.y));
	this->addChild(menu7,1);

	return true;
}

void SlidingMenuLayer::touchButton(Object* obj,::ui::Widget::TouchEventType eventType)  
{  
    auto button = dynamic_cast<::ui::Button*>(obj);  
    int tag = button->getTag();  
    switch(eventType)  
    {  
	case ::ui::Widget::TouchEventType::ENDED:  

		//char file[100];
		sprintf(jigSawFile, "match_game_img_%d.jpg", tag-1000);
		jigSawFile[20]='\0';


		//jigSawFile=file;
		//load Picture Game Layer
		//PictureGameLayer::create();
		auto scene = PictureGameScene::create();
		//scene->jigSawFile=file;
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	
		break;
    }  
}  

void SlidingMenuLayer::LevelChoose(){


}

	//Controls the events launched when a menu item is clicked
	void SlidingMenuLayer::launchmenu( int i){
		//Load Product image
		/*
		if (i == 0){
			CCScene scene =  GameLayer.scene(); //  
			CCDirector.sharedDirector().runWithScene(scene); 
		}else if (i == 1){
			try {
				PictureGameLayer.getBitmapFromAsset("benin.jpg");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			CCScene scene =  PictureGameLayer.scene(); //  
			CCDirector.sharedDirector().runWithScene(scene); 
		}
		else if (i == 2){
			PictureGameLayer.getBitMapfromCamera(); 
		}
		else if (i == 3){
			PictureGameLayer.getBitMapFromGallery();; 
		}
		else if (i == 4){
			CCScene scene =  MenuLayer.scene(); //  
			CCDirector.sharedDirector().runWithScene(scene); 
		}
		*/
	}

	bool SlidingMenuLayer::TouchesBegan(Touch *pTouch, Event *pEvent) {

		//startlocation = CCDirector.sharedDirector().convertToGL(CGPoint.Vec2(event.getX(), event.getY()));

		//ccMacros.CCLOG("Dist Bdgan ","------------------ " );

		return true;
	}

	bool SlidingMenuLayer::TouchesEnded(Touch *pTouch, Event *pEvent)
	{
		/*
		CGRect spritePos ; 

		endlocation = CCDirector.sharedDirector().convertToGL(CGPoint.Vec2(event.getX(), event.getY()));

		if (Math.abs(startlocation.x - endlocation.x) < 5){

			//Obtain a reference to the scrollview control
			CCScrollView tilesNode = (CCScrollView)getChildByTag(SCROLLVIEW_TAG) ;

			//Get scrollview children .. its inside container variable
			CCClipNode container = (CCClipNode)(tilesNode.getChildren().get(0)) ;  

			//Iterate through its children ... nlabel and tilebox sprite			
			// we divide by two here because we have two items added for each menu item
			for (int i = 0 ; i < ((container.getChildren().size())/2)  ; i++){ 

				// In order to get the position of the item menu that has been touched we note the following
				// We need the current position of the touch event ... which is endlocation .. check
				// We need to know where it falls into the scrollview content
				// Since we are scrolling horizontally we add the content current x position to our touch x position
				//and construct a rectangle in that area . 
				// You can inspect these values in logcat (x, y, endlocation ) using  ccMacros.CCLOG.
				Sprite backsprite = (Sprite)container.getChildByTag(i);
				spritePos = CGRect.make(
						(backsprite.getPosition().x + container.getPosition().x ) ,
						backsprite.getPosition().y ,
						backsprite.getContentSize().width*tilescale  ,
						backsprite.getContentSize().height*tilescale  );
				if(spritePos.contains(endlocation.x, endlocation.y)){

					//Use cccMacro to log information to logcat to to LogCat
					ccMacros.CCLOG("Tile " + i + "  has been touched : ", "Began touched : " + i);
					//Launch the appropriate menu when its toucheds
					launchmenu(i);
				}
			}

		} 
		*/
		return true ;
	}

	void SlidingMenuLayer::menuCallback(Ref *sender) {

		//CCDirector.sharedDirector().replaceScene(SlidingMenuLayer.scene());
		auto scene = WelcomeScene::create();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
