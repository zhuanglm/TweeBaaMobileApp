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
        //�������պ��ͼ��ɱ༭
        picker.allowsEditing = YES;
        picker.sourceType = sourceType;
        [picker release];
        [self presentModalViewController:picker animated:YES];
    }
    else{
        NSLog(@"ģ�������޷������������������е���");
    }
}

- (void)imageController:(UIImageController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info{
    NSString *type = [info objectForKey:UIImageControllerMediaType];
    
    //��ѡ���������ͼƬ
    if ([type isEqualToString:@"public.image"])
    {
        //�Ȱ�ͼƬת��NSData
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
        
        //ͼƬ�����·��
        //���ｫͼƬ����ɳ�е�documents�ļ�����
        NSString * DocumentsPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
        
        //�ļ�������
        NSFileManager *fileManager = [NSFileManager defaultManager];
        
        //����Ψһ�ַ���
        NSString* uuid = [[NSUUID UUID] UUIDString];
        
        //�ļ���
        NSString* fileName = [NSString stringWithFormat:@"/%@.png", uuid];
        
        //�Ѹո�ͼƬת����data���󿽱���ɳ���� ������ΪXXXXXXXX-XXXX-XXXX....XXXX.png
        [fileManager createDirectoryAtPath:DocumentsPath withIntermediateDirectories:YES attributes:nil error:nil];
        [fileManager createFileAtPath:[DocumentsPath stringByAppendingString:fileName] contents:data attributes:nil];
        
        
        //�õ�ѡ���ɳ����ͼƬ������·��
        filePath = [[NSString alloc]initWithFormat:@"%@%@", DocumentsPath, fileName];
        
        //�ر�������
        [picker dismissModalViewControllerAnimated:YES];
        
        std::string strFilePath = [filePath UTF8String];
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ImagePickerEvent", &strFilePath);
    }
    
}

- (void)imageControllerDidCancel:(UIImagePickerController *)picker{
    NSLog(@"��ȡ����ѡ��ͼƬ");
    [picker dismissModalViewControllerAnimated:YES];
}

@end

