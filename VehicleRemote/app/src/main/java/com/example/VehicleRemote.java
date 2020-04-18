package com.example;

import android.app.Application;
import android.content.Context;

public class VehicleRemote extends Application {
    private static Context context;

    public void onCreate() {
        super.onCreate();
        VehicleRemote.context = getApplicationContext();
    }

    public static Context getAppContext() {
        return VehicleRemote.context;
    }
}
