package com.jmengxy.luadroid;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

import com.jmengxy.luadroid.R;
import com.jmengxy.lualib.Lua;

import butterknife.BindView;
import butterknife.ButterKnife;

import static com.jmengxy.lualib.Lua.*;

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
//        lua.setString("s", "test");
//        lua.setInteger("i", 33);
//        lua.setDouble("d", 44.5);
//        lua.parseLine("result = string.find(s, 'a')");
//        textView.setText(String.format("%s %s %d %f\n%s %s %d %f",
//                lua.getString("s", ""),
//                Boolean.toString(lua.getBoolean("result", true)),
//                lua.getInteger("i", -1),
//                lua.getDouble("d", -0.1),
//                lua.getString("s1", "[]"),
//                lua.getBoolean("b1", true),
//                lua.getInteger("i1", -1),
//                lua.getDouble("d1", -0.1)));
        lua.setString("s", "greeting");
        lua.parseLine("r = true");
        if (lua.getType("r") == Lua.LUA_TYPE_BOOLEAN) {
            boolean b = lua.getBoolean("r", false);
            textView.setText(Boolean.toString(b));
        } else if (lua.getType("r") == Lua.LUA_TYPE_NUMBER) {
            int b = lua.getInteger("r", 3);
            textView.setText(Integer.toString(b));
        }
    }

    @Override
    protected void onDestroy() {
        if (lua != null) {
            lua.close();
        }
        super.onDestroy();
    }
}
