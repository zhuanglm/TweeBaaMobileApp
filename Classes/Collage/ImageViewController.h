#import <UIKit/UIKit.h>

@interface ImageViewController : UIViewController<UINavigationControllerDelegate, UIImageControllerDelegate>
{
    NSString* filePath;
}

// �򿪱������
- (void)localPhoto;

// �����
- (void)takePhoto;


@end