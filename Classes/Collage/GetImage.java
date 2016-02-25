package org.cocos2dx.cpp;

import java.io.File;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.ByteArrayOutputStream;
import java.util.UUID;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Context;
import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.provider.MediaStore;
import android.net.Uri;
import android.graphics.Bitmap;

public class GetImage{
	
	public static final int 	NONE = 0;
    public static final int 	PHOTOHRAPH = 1;		// ����
    public static final int 	PHOTOZOOM = 2; 		// ����
    public static final int 	PHOTORESOULT = 3;	// ���
    public static final String  IMAGE_UNSPECIFIED = "image/*";
    
    private static GetImage instance = null;
    private static Activity    activity = null;
    
    public static native void onImageSaved(String path);
    
    public static GetImage getInstance(){
    	if(instance == null){
    		instance = new GetImage();
    	}
    	return instance;
    }
    
    // ��ʼ��
    public void init(Activity activity){
    	GetImage.activity = activity;
    }
    
    // �����
	static public void openPhoto(){
		Intent intent = new Intent(Intent.ACTION_PICK, null);
        intent.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, IMAGE_UNSPECIFIED);
        activity.startActivityForResult(intent, PHOTOZOOM);
	}
	 
	// �����
	static public void openCamera(){
    	Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(new File(activity.getFilesDir(), "@cc_cameraCache.jpg")));
        activity.startActivityForResult(intent, PHOTOHRAPH);
    }
	
	// �ص�
	public void onActivityResult(int requestCode, int resultCode, Intent data){
		if (resultCode == NONE)
            return;
        
        // ����
        if (requestCode == PHOTOHRAPH) {
            File picture = new File(activity.getFilesDir() + "/@cc_cameraCache.jpg");
            startPhotoZoom(Uri.fromFile(picture));
        }
 
        if (data == null)
            return;
 
        // ��ȡ�������ͼƬ
        if (requestCode == PHOTOZOOM) {
            startPhotoZoom(data.getData());
        }
        
        // ������
        if (requestCode == PHOTORESOULT) {
            Bundle extras = data.getExtras();
            if (extras != null) {
                Bitmap photo = extras.getParcelable("data");
                ByteArrayOutputStream stream = new ByteArrayOutputStream();
                photo.compress(Bitmap.CompressFormat.JPEG, 75, stream);
                
                // XXX/@ci_8888-8888-8888-8888.jpg
                String path = activity.getFilesDir() + "/@ci_" + UUID.randomUUID().toString() + ".jpg";
                saveMyBitmap(path, photo);
                
                // ֪ͨC++���ѱ���ͼƬ ������·��
                onImageSaved(path);
            }
        }
	}
	
	public void startPhotoZoom(Uri uri) {
        Intent intent = new Intent("com.android.camera.action.CROP");
        intent.setDataAndType(uri, IMAGE_UNSPECIFIED);
        intent.putExtra("crop", "true");
        intent.putExtra("aspectX", 1);
        intent.putExtra("aspectY", 1);
        intent.putExtra("outputX", 100);
        intent.putExtra("outputY", 100);
        intent.putExtra("return-data", true);
        activity.startActivityForResult(intent, PHOTORESOULT);
    }
	
	public void saveMyBitmap(String filePath, Bitmap mBitmap){
    	File f = new File(filePath);
    	try {
    		f.createNewFile();
    	} catch (IOException e) {
    	}
    	FileOutputStream fOut = null;
    	try {
    		fOut = new FileOutputStream(f);
    	} catch (FileNotFoundException e) {
    		e.printStackTrace();
    	}
    	mBitmap.compress(Bitmap.CompressFormat.JPEG, 70, fOut);
    	try {
    		fOut.flush();
    	} catch (IOException e) {
    		e.printStackTrace();
    	}
    	try {
    		fOut.close();
    	} catch (IOException e) {
    		e.printStackTrace();
    	}
    }
}