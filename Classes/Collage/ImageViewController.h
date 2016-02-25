#import <UIKit/UIKit.h>

@interface ImageViewController : UIViewController<UINavigationControllerDelegate, UIImageControllerDelegate>
{
    NSString* filePath;
}

// 打开本地相册
- (void)localPhoto;

// 打开相机
- (void)takePhoto;


@end