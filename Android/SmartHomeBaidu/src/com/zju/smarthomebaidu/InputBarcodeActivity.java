package com.zju.smarthomebaidu;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

public class InputBarcodeActivity extends Activity {

	public InputBarcodeActivity() {
		// TODO Auto-generated constructor stub
	}
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
        ///////////////full screen & landscape/////////begin/////////
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);//���ó�ȫ��ģʽ
		//setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);//ǿ��Ϊ����
		requestWindowFeature(Window.FEATURE_NO_TITLE);//�ޱ���
        ///////////////full screen & landscape/////////end/////////
		setContentView(R.layout.input_barcode);
		
	}
}
