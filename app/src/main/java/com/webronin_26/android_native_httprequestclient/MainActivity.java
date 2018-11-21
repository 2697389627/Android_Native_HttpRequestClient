package com.webronin_26.android_native_httprequestclient;

import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.lang.ref.WeakReference;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private TextView text_view;

    private testHandler handler = new testHandler( new WeakReference<MainActivity>( MainActivity.this ) );

    private static class testHandler extends Handler {

        private WeakReference<MainActivity> mMainActivityWeakReference = null;

        public testHandler( WeakReference<MainActivity> testActivityWeakReference ){

            mMainActivityWeakReference = testActivityWeakReference;

        }

        @Override
        public void handleMessage(Message msg) {

            super.handleMessage(msg);

            MainActivity mMainActivity = mMainActivityWeakReference.get();

            if( mMainActivity != null ){

                String str = ( String ) msg.obj;

                mMainActivity.text_view.setText( str );

            }

        }

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        text_view = findViewById(R.id.text_view);

        Button button = findViewById( R.id.button );

        button.setOnClickListener( new View.OnClickListener() {

            @Override
            public void onClick(View view) {

                new Thread(new Runnable() {

                    @Override
                    public void run() {

                        String str = NativeSendRequest();

                        Message msg = handler.obtainMessage();

                        msg.obj = str;

                        handler.sendMessage( msg );

                    }

                }).start();

            }
        });

    }

    public native String NativeSendRequest();

}
