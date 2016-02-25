/*



public class MainActivity extends Activity {

	public static MainActivity  app ;
	public static final int CAMERA_REQUEST_CODE = 1;
	public static final int GALLERY_REQUEST_CODE = 3;
	public static Bitmap bitmap =  null ;
	protected CCGLSurfaceView _glSurfaceView; 
	
	
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		app =  this ;
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		_glSurfaceView = new CCGLSurfaceView(this);
		setContentView( _glSurfaceView); 

		CCDirector director = CCDirector.sharedDirector();
		director.attachInView(_glSurfaceView);
		director.setDeviceOrientation(CCDirector.kCCDeviceOrientationLandscapeLeft);
		CCDirector.sharedDirector().setAnimationInterval(1.0f / 60.0f);
		CCDirector.sharedDirector().setDisplayFPS(true);
		
		CCScene scene =  SlidingMenuLayer.scene(); //  
		CCDirector.sharedDirector().runWithScene(scene); 

	}
	
	@Override
	public void onStart()
	{
		super.onStart();
	}
	
	@Override
	public bool onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public void onPause()
	{
		super.onPause();
		CCDirector.sharedDirector().pause();
	}

	@Override
	public void onResume()
	{
		super.onResume();
		CCDirector.sharedDirector().resume();
	}

	@Override
	public void onStop()
	{
		super.onStop();
		CCDirector.sharedDirector().end();
	} 

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// For Handling Camera  Intent  
		if (requestCode == CAMERA_REQUEST_CODE && resultCode == Activity.RESULT_OK) {

			MainActivity.bitmap = (Bitmap) data.getExtras().get("data");
			CCScene scene =  PictureGameLayer.scene(); //  
			CCDirector.sharedDirector().runWithScene(scene);
		}

		// For Handling Picture Gallery Intent  
		if (requestCode == GALLERY_REQUEST_CODE && resultCode == Activity.RESULT_OK) {
			// We need to recyle unused bitmaps
			if (MainActivity.bitmap != null) {
				MainActivity.bitmap.recycle();
			}

			try {
				InputStream stream;
				stream = getContentResolver().openInputStream(
						data.getData());
				MainActivity.bitmap = Bitmap.createBitmap(BitmapFactory.decodeStream(stream));
				stream.close(); 

			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}


			CCScene scene =  PictureGameLayer.scene(); //  
			CCDirector.sharedDirector().runWithScene(scene);
		}
		super.onActivityResult(requestCode, resultCode, data);
	}
}
*/