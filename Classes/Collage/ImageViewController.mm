#import "ImageViewController.h"
#import "cocos2d.h"

@interface ImageViewController ()

@end

@implementation ImageViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    //[self localPhoto];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)localPhoto{
    UIImageController *picker = [[UIImageController alloc] init];
    picker.delegate      = self;
    picker.sourceType    = UIImageControllerSourceTypePhotoLibrary;
    picker.allowsEditing = YES;
    //[self presentModalViewController:picker animated:YES];
    [self presentViewController:picker animated:YES completion:^(void){
        NSLog(@"Imageviewcontroller is presented");
    }];
    [picker release];
    
    NSLog(@"-(void)localPhoto();");
}

- (void)takePhoto{
    UIImageControllerSourceType sourceType = UIImageControllerSourceTypeCamera;
    if ([UIImageController isSourceTypeAvailable:UIImageControllerSourceTypeCamera]) {
        UIImageController* picker = [[UIImageController alloc] init];
        picker.delegate = self;
        //设置拍照后的图像可编辑
        picker.allowsEditing = YES;
        picker.sourceType = sourceType;
        [picker release];
        [self presentModalViewController:picker animated:YES];
    }
    else{
        NSLog(@"模拟器中无法打开照相机，请在真机中调试");
    }
}

- (void)imageController:(UIImageController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info{
    NSString *type = [info objectForKey:UIImageControllerMediaType];
    
    //当选择的类型是图片
    if ([type isEqualToString:@"public.image"])
    {
        //先把图片转成NSData
//        UIImage* image = [info objectForKey:@"UIImageControllerOriginalImage"];
        UIImage* image = [info objectForKey:@"UIImageControllerEditedImage"];
        NSData *data;
        if (UIImagePNGRepresentation(image) == nil)
        {
            data = UIImageJPEGRepresentation(image, 1.0);
        }
        else
        {
            data = UIImagePNGRepresentation(image);
        }
        
        //图片保存的路径
        //这里将图片放在沙盒的documents文件夹中
        NSString * DocumentsPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
        
        //文件管理器
        NSFileManager *fileManager = [NSFileManager defaultManager];
        
        //生成唯一字符串
        NSString* uuid = [[NSUUID UUID] UUIDString];
        
        //文件名
        NSString* fileName = [NSString stringWithFormat:@"/%@.png", uuid];
        
        //把刚刚图片转换的data对象拷贝至沙盒中 并保存为XXXXXXXX-XXXX-XXXX....XXXX.png
        [fileManager createDirectoryAtPath:DocumentsPath withIntermediateDirectories:YES attributes:nil error:nil];
        [fileManager createFileAtPath:[DocumentsPath stringByAppendingString:fileName] contents:data attributes:nil];
        
        
        //得到选择后沙盒中图片的完整路径
        filePath = [[NSString alloc]initWithFormat:@"%@%@", DocumentsPath, fileName];
        
        //关闭相册界面
        [picker dismissModalViewControllerAnimated:YES];
        
        std::string strFilePath = [filePath UTF8String];
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ImagePickerEvent", &strFilePath);
    }
    
}

- (void)imageControllerDidCancel:(UIImagePickerController *)picker{
    NSLog(@"您取消了选择图片");
    [picker dismissModalViewControllerAnimated:YES];
}

@end

