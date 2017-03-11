package com.jmeng.luadroid;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
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
        lua.setInteger("i", 33);
        lua.setDouble("d", 44.5);
        lua.parseLine("result = string.find(s, 'a')");
        textView.setText(String.format("%s %s %d %f\n%s %s %d %f",
                lua.getString("s", ""),
                Boolean.toString(lua.getBoolean("result", true)),
                lua.getInteger("i", -1),
                lua.getDouble("d", -0.1),
                lua.getString("s1", ""),
                lua.getBoolean("b1", true),
                lua.getInteger("i1", -1),
                lua.getDouble("d1", -0.1)));
    }

    @Override
    protected void onDestroy() {
        if (lua != null) {
            lua.close();
        }
        super.onDestroy();
    }
}
