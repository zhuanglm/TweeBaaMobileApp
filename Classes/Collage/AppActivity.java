/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.plugin.PluginWrapper;

import android.os.Bundle;
import cn.sharesdk.ShareSDKUtils;

import android.content.Intent;

public class AppActivity extends Cocos2dxActivity {
	 protected void onCreate(Bundle savedInstanceState){
			super.onCreate(savedInstanceState);
			ShareSDKUtils.prepare();
			
			//在这里为应用设置异常处理程序，然后我们的程序才能捕获未处理的异常

			CrashHandler crashHandler = CrashHandler.getInstance();

			crashHandler.init(this);
			
			GetImage.getInstance().init(this);
			
		}
		
		protected void onActivityResult(int requestCode, int resultCode, Intent data) {
			// TODO Auto-generated method stub
			super.onActivityResult(requestCode, resultCode, data);
			
			GetImage.getInstance().onActivityResult(requestCode, resultCode, data);
		}

	    public Cocos2dxGLSurfaceView onCreateView() {
	    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
	    	// HelloCpp should create stencil buffer
	    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
			PluginWrapper.init(this);
			PluginWrapper.setGLSurfaceView(glSurfaceView);
	    	return glSurfaceView;
	    }

	    static {
	        System.loadLibrary("cocos2dcpp");
	    }
}
