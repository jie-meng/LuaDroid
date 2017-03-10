package com.jmeng.luadroid;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.jmeng.lualib.Lua;

import butterknife.BindView;
import butterknife.ButterKnife;

public class MainActivity extends AppCompatActivity {

    @BindView(R.id.text)
    TextView textView;

    private Lua lua;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
        lua = new Lua();
        lua.setString("s", "test");
        lua.parseLine("result = string.find(s, 'a')");
        textView.setText(Boolean.toString(lua.getBoolean("result")));
    }

    @Override
    protected void onDestroy() {
        if (lua != null) {
            lua.close();
        }
        super.onDestroy();
    }
}
