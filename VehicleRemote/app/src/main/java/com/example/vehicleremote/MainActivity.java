package com.example.vehicleremote;

import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.google.android.material.bottomnavigation.BottomNavigationView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

import org.videolan.libvlc.LibVLC;
import org.videolan.libvlc.Media;
import org.videolan.libvlc.MediaPlayer;
import org.videolan.libvlc.util.VLCVideoLayout;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;

public class MainActivity extends AppCompatActivity implements View.OnClickListener, View.OnTouchListener, CompoundButton.OnCheckedChangeListener {

    /* sending msg protocol values */
    static final String ManualMode = "0";
    static final String AutoMode = "1";
    static final String StartEngine = "2";
    static final String StopEngine = "3";
    static final String Forward = "4";
    static final String ForwardLeft = "5";
    static final String ForwardRight = "6";
    static final String Reverse = "7";
    static final String ReverseRight = "8";
    static final String ReverseLeft = "9";
    static final String Stop = "10";
    static final String StartRecord = "11";
    static final String StopRecord = "12";
    static final String GotoBack = "13";
    static final String Playback = "13";

    /* receiving msg protocol values */
    static final String Success = "0";
    static final String Playmodeactive = "1";
    static final String Playmodeerror = "2";
    static final String Recordmodeactive = "3";
    static final String Startengineactive = "4";
    static final String Stopengineactive = "5";
    static final String Automaticmodeactive = "6";
    static final String Manualmodeactive = "7";
    static final String Failure = "8";
    static final String Recordmodeinactive = "9";
    static final String Playbackinactive = "10";
    static final String NoConnection = "No Connection";
    static final String ConnectionComplete = "Connection successful";


    private ToggleButton modeButton;
    private ToggleButton engineButton;
    private ToggleButton recButton;
    private Button gotostartButton;
    private ImageButton fwdleftButton;
    private ImageButton fwdButton;
    private ImageButton fwdrightButton;
    private ImageButton revleftButton;
    private ImageButton revButton;
   // private Button stopButton;
    private ImageButton revrightButton;


    //vlc options
    private static final boolean USE_TEXTURE_VIEW =  false;
    private static final boolean ENABLE_SUBTITLES = false;
   // private static final String URL = "rtsp://10.0.1.36:8554/";
    private static final String URL = "rtsp://192.168.0.101:8554/";
    private VLCVideoLayout mVideoLayout = null;
    private LibVLC mLibVLC = null;
    private MediaPlayer mMediaplayer = null;

    SocketConnector mTcpClient;

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (mTcpClient == null) {
            return false;

        }
        switch (v.getId()) {
            case R.id.fwdleft:
                if (event.getAction() == MotionEvent.ACTION_DOWN)
                    mTcpClient.sendMessage(ForwardLeft);
                else if (event.getAction() == MotionEvent.ACTION_UP)
                    mTcpClient.sendMessage(Stop);
                break;
            case R.id.fwd:
                if (event.getAction() == MotionEvent.ACTION_DOWN)
                    mTcpClient.sendMessage(Forward);
                else if (event.getAction() == MotionEvent.ACTION_UP)
                    mTcpClient.sendMessage(Stop);
                break;
            case R.id.fwdright:
                if (event.getAction() == MotionEvent.ACTION_DOWN)
                    mTcpClient.sendMessage(ForwardRight);
                else if (event.getAction() == MotionEvent.ACTION_UP)
                    mTcpClient.sendMessage(Stop);
                break;
            case R.id.revleft:
                if (event.getAction() == MotionEvent.ACTION_DOWN)
                    mTcpClient.sendMessage(ReverseLeft);
                else if (event.getAction() == MotionEvent.ACTION_UP)
                    mTcpClient.sendMessage(Stop);
                break;

            case R.id.reverse:
                if (event.getAction() == MotionEvent.ACTION_DOWN)
                    mTcpClient.sendMessage(Reverse);
                else if (event.getAction() == MotionEvent.ACTION_UP)
                    mTcpClient.sendMessage(Stop);
                break;
           /* case R.id.stop:
                mTcpClient.sendMessage("10");
                break;*/
            case R.id.revright:
                if (event.getAction() == MotionEvent.ACTION_DOWN)
                    mTcpClient.sendMessage(ReverseRight);
                else if (event.getAction() == MotionEvent.ACTION_UP)
                    mTcpClient.sendMessage(Stop);
                break;
        }

        return true;
    }

    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        if (mTcpClient == null) {
            return;

        }
        switch (buttonView.getId()) {
            case R.id.modeButton:
                if (isChecked) {
                    mTcpClient.sendMessage(AutoMode); //automatic
                    automodeaction();

                } else {
                    mTcpClient.sendMessage(ManualMode); //manual
                    manualmodeaction();
                }
                break;

            case R.id.engineButton:
                if (isChecked)
                    mTcpClient.sendMessage(StartEngine); //on engine
                else
                    mTcpClient.sendMessage(StopEngine); //off engine
                break;
            case R.id.recordButton2:
                if (isChecked)
                    mTcpClient.sendMessage(StartRecord); //on record
                else
                    mTcpClient.sendMessage(StopRecord); //off record

                break;

        }

    }

    public class ConnectTask extends AsyncTask<String, String, SocketConnector> {

        @Override
        protected SocketConnector doInBackground(String... message) {

            //we create a TCPClient object
            mTcpClient = new SocketConnector(new SocketConnector.OnMessageReceived() {
                @Override
                //here the messageReceived method is implemented
                public void messageReceived(String message) {
                    //this method calls the onProgressUpdate
                    publishProgress(message);
                }
            });
            mTcpClient.run();

            return null;
        }

        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);
            //response received from server
            Log.d("test", "response " + values[0]);
            //process server response here....
            String[] arr = values[0].split(";");

            for(String a : arr)
            {
                switch(a)
                {
                    case Automaticmodeactive:
                    modeButton.setChecked(true);
                    automodeaction();
                    break;
                    case Manualmodeactive:
                    modeButton.setChecked(false);
                    manualmodeaction();
                    break;
                    case Playmodeactive:
                    case Playmodeerror:
                        Toast.makeText(getApplicationContext(),"Vehicle in playback",Toast.LENGTH_LONG).show();
                    break;
                    case Playbackinactive:
                        Toast.makeText(getApplicationContext(),"Playback finished",Toast.LENGTH_LONG).show();
                        break;

                    case Recordmodeactive: Toast.makeText(getApplicationContext(),"Record active",Toast.LENGTH_LONG).show();
                        recButton.setChecked(true);

                    break;
                    case Recordmodeinactive:Toast.makeText(getApplicationContext(),"Record active",Toast.LENGTH_LONG).show();
                        recButton.setChecked(false);

                        break;
                    case NoConnection: Toast.makeText(getApplicationContext(),"No Connection",Toast.LENGTH_LONG).show();
                        break;
                    case ConnectionComplete:Toast.makeText(getApplicationContext(),"Connection Success",Toast.LENGTH_LONG).show();
                    onStartStreaming();
                        break;
                    case Startengineactive:
                        engineButton.setChecked(true);
                        break;
                    case Stopengineactive:
                        engineButton.setChecked(false);
                        break;


                }
            }

        }
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
       // BottomNavigationView navView = findViewById(R.id.nav_view);
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        /*AppBarConfiguration appBarConfiguration = new AppBarConfiguration.Builder(
                R.id.navigation_home, R.id.navigation_dashboard, R.id.navigation_notifications)
                .build();*/
        //NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment);
        //NavigationUI.setupActionBarWithNavController(this, navController, appBarConfiguration);
        //NavigationUI.setupWithNavController(navView, navController);

        //listen to all onclick events
      //  s.run();
        new ConnectTask().execute("");

        //sending message to server
     /*   if (mTcpClient != null) {
            mTcpClient.sendMessage(ManualMode); //set to manual mode
            mTcpClient.sendMessage(StopEngine); //set to engine off
            mTcpClient.sendMessage(StopRecord); //set to stop record

        }
*/
        modeButton = findViewById(R.id.modeButton);
        modeButton.setOnCheckedChangeListener(this);
        engineButton = findViewById(R.id.engineButton);
        engineButton.setOnCheckedChangeListener(this);
        recButton = findViewById(R.id.recordButton2);
        recButton.setOnCheckedChangeListener(this);

        gotostartButton = findViewById(R.id.GoBack);
        gotostartButton.setOnClickListener(this);

        fwdleftButton = findViewById(R.id.fwdleft);
        fwdleftButton.setOnTouchListener(this);
        fwdButton = findViewById(R.id.fwd);
        fwdButton.setOnTouchListener(this);
        fwdrightButton = findViewById(R.id.fwdright);
        fwdrightButton.setOnTouchListener(this);
        revleftButton = findViewById(R.id.revleft);
        revleftButton.setOnTouchListener(this);
        revButton = findViewById(R.id.reverse);
        revButton.setOnTouchListener(this);
       // stopButton = findViewById(R.id.stop);
        //stopButton.setOnClickListener(this);
        revrightButton = findViewById(R.id.revright);
        revrightButton.setOnTouchListener(this);

        final ArrayList<String> args= new ArrayList<>();
        args.add("-vvv");

        mLibVLC = new LibVLC(this,args);
        mMediaplayer = new MediaPlayer(mLibVLC);
        mVideoLayout = findViewById(R.id.video_layout);
    }

    @Override
    protected  void onDestroy() {

        super.onDestroy();
        onStopStreaming();
        mMediaplayer.release();
        mLibVLC.release();
    }
    private void onStartStreaming()
    {
        mMediaplayer.attachViews(mVideoLayout,null,ENABLE_SUBTITLES,USE_TEXTURE_VIEW);

        final Media media = new Media(mLibVLC, Uri.parse(URL));
        media.addOption(":network-caching=100");
        media.addOption(":clock-jitter=0");
        media.addOption(":clock-synchro=0");
        //media.addOption(":fullscreen");

        mMediaplayer.setMedia(media);
        mMediaplayer.setAspectRatio(null);
        mMediaplayer.setScale(1);

        media.release();
        mMediaplayer.play();
    }

    private void onStopStreaming()
    {
        mMediaplayer.stop();
        mMediaplayer.detachViews();
    }
    @Override
    public void onClick(View view)
    {
        if (mTcpClient == null) {
            return;

        }
        switch (view.getId()) {
            case R.id.GoBack:
                mTcpClient.sendMessage(GotoBack);
                break;
        }


    }
    private void automodeaction()
    {
        //disable the direction buttons
        fwdButton.setEnabled(false);
        fwdleftButton.setEnabled(false);
        fwdrightButton.setEnabled(false);
        revButton.setEnabled(false);
        revleftButton.setEnabled(false);
        revrightButton.setEnabled(false);
    }
    private void manualmodeaction()
    {
        //enable the direction buttons
        fwdButton.setEnabled(true);
        fwdleftButton.setEnabled(true);
        fwdrightButton.setEnabled(true);
        revButton.setEnabled(true);
        revleftButton.setEnabled(true);
        revrightButton.setEnabled(true);

    }

}
